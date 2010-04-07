//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "HeightfieldResource.hpp"
#include "PhysicsActor.hpp"
#include "PhysicsZone.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <boost/archive/binary_iarchive.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>

#include <vector>

#include <fstream>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HeightfieldResource::HeightfieldResource(config_type& _config)
:   m_heightfieldDataId(NULL)
,   m_heightfieldGeometryId(NULL)
{
    // TODO Improve error handling for missing config entries.
    // TODO Use default values?

    m_fileName = _config["fileName"];
    m_width = boost::lexical_cast<Math::Real,std::string>(_config["width"]);
    m_depth = boost::lexical_cast<Math::Real,std::string>(_config["depth"]);
    m_widthSamples = boost::lexical_cast<int,std::string>(_config["widthSamples"]);
    m_depthSamples = boost::lexical_cast<int,std::string>(_config["depthSamples"]);
    m_scale = boost::lexical_cast<Math::Real,std::string>(_config["scale"]);
    m_thickness = boost::lexical_cast<Math::Real,std::string>(_config["thickness"]);
    m_wrap = (_config["wrap"] == "true");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HeightfieldResource::~HeightfieldResource()
{
    if( m_heightfieldGeometryId != NULL )
    {
        dGeomDestroy(m_heightfieldGeometryId);
    }

    if( m_heightfieldDataId != NULL )
    {
        dGeomHeightfieldDataDestroy(m_heightfieldDataId);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HeightfieldResource::initialize(PhysicsActor* _pActor)
{
    // TODO This should be async

    /// dHeightfieldDataID is actually a pointer to a struct.
    m_heightfieldDataId = dGeomHeightfieldDataCreate();

    /// TODO Need to open file and read height data into a buffer.
    /// (Assuming unsigned short for now).
    /// While we're reading the height data, we can get the
    /// height bounds of the heightmap as well.
    /// (These may need to be scaled versions of the raw min/max
    /// based on the scale config param).

    m_buffer.clear();
    m_buffer.reserve(m_widthSamples * m_depthSamples);

    boost::int16_t min = SHRT_MAX;
    boost::int16_t max = SHRT_MIN;

    std::ifstream stream(m_fileName.c_str(), std::ios::in|std::ios::binary);

    boost::archive::binary_iarchive inputArchive(stream, boost::archive::no_header |
        boost::archive::no_tracking);

    // Read the input stream if it exists
    if(stream.good())
    {
        for(int x = 0; x < m_widthSamples; x++)
        {
            for(int y = 0; y < m_depthSamples; y++)
            {
            	boost::uint16_t c;
                inputArchive & c;
                m_buffer[x * m_widthSamples + y] = c;
                max = max > c ? max : c;
                min = min < c ? min : c;

            }
        }
    }
    else
    {
        std::stringstream errorMessage;
        errorMessage << "Error opening " << m_fileName
            << "file for input." << std::endl;
        throw Zen::Utility::runtime_exception(errorMessage.str());
    }

    const short *pBuffer = &m_buffer[0];

    Math::Real minHeight = max * m_scale;
    Math::Real maxHeight = min * m_scale;

    /// Build heightfield data from buffer.
    dGeomHeightfieldDataBuildShort(
        m_heightfieldDataId,
        pBuffer,
        false,       /// This copies the height data local to the dHeightfieldData object.
                    /// A 0 here would make the dHeightfieldData object access the data
                    /// by reference, requiring that the data persist throughout the lifetime
                    /// of the dHeightfieldData object.
        m_width,
        m_depth,
        m_widthSamples,
        m_depthSamples,
        m_scale,
        0.0f,       /// Assuming a zero offset for now.
        m_thickness,
        m_wrap
    );

    /// Setting the heightfield height bounds to those
    /// found when reading the heightmap in from the file.
    dGeomHeightfieldDataSetBounds(
        m_heightfieldDataId,
        minHeight,
        maxHeight
    );

    // TODO I think this should be moved elsewhere
    /// Create the heightfield geometry object.
    m_heightfieldGeometryId = dCreateHeightfield(
        _pActor->getParentZone().getSpaceId(),          /// TODO Get a spaceId for the current zone.
        m_heightfieldDataId,
        true        /// Setting the geometry as placeable so that it
                    /// can be transformed with dGeomSetPosition()
                    /// and dGeomSetRotation().
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
