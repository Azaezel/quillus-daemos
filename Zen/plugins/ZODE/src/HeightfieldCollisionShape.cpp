//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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

#include "HeightfieldCollisionShape.hpp"

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>

#include <Zen/Engine/World/I_TerrainChunk.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
dReal
getHeightCallback(void* _userdata, int _x, int _z)
{
    HeightfieldCollisionShape* pShape = 
        static_cast<HeightfieldCollisionShape*>(_userdata);

    return pShape->getHeight(_x, _z);
}
    
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HeightfieldCollisionShape::HeightfieldCollisionShape(dSpaceID _spaceId, pTerrainChunk_type _pTerrainChunk)
:   m_pTerrainChunk(_pTerrainChunk)
,   CollisionShape(NULL)
{
    m_heightfieldDataId = dGeomHeightfieldDataCreate();

    dGeomHeightfieldDataBuildCallback(
        m_heightfieldDataId,
        this,
        getHeightCallback,
        m_pTerrainChunk->getWorldWidth(),
        m_pTerrainChunk->getWorldWidth(),
        m_pTerrainChunk->getWidth(),
        m_pTerrainChunk->getWidth(),
        1.0f,
        m_pTerrainChunk->getOffset(),
        m_pTerrainChunk->getSkirtSize(),
        0
    );

    dGeomHeightfieldDataSetBounds(
        m_heightfieldDataId,
        m_pTerrainChunk->getMinHeight(),
        m_pTerrainChunk->getMaxHeight()
    );

    dGeomID geometryId = dCreateHeightfield(
        _spaceId,
        m_heightfieldDataId,
        1
    );

    setGeometryId(geometryId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HeightfieldCollisionShape::~HeightfieldCollisionShape()
{
    dGeomHeightfieldDataDestroy(m_heightfieldDataId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
HeightfieldCollisionShape::getHeight(int _x, int _y)
{
    return m_pTerrainChunk->getHeight(_x, _y);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
