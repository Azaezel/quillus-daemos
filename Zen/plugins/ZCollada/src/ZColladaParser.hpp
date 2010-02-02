//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef INDIEZEN_ZCOLLADA_ZCOLLADA_PARSER_HPP_INCLUDED
#define INDIEZEN_ZCOLLADA_ZCOLLADA_PARSER_HPP_INCLUDED

#include <IndieZen/framework/Math/Point3.hpp>
#include <IndieZen/framework/ThreeSpace/VertexIndex.hpp>
#include <IndieZen/framework/ThreeSpace/I_EditableMesh.hpp>
#include <IndieZen/framework/ResourceManager/I_CompoundResource.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ZCollada {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ZColladaParser
{
    /// @name Types
    /// @{
public:
    typedef Math::Point3                            Point3F;
	typedef IndieZen::ThreeSpace::VertexIndex	    VertexIndex;
	typedef IndieZen::ThreeSpace::I_EditableMesh    I_Mesh;
    /// @}

    /// @name ZColladaParser Interface
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             ZColladaParser();
    virtual ~ZColladaParser();
    /// @}

    /// @name Member Variables
    /// @{
private:
	std::vector<ThreeSpace::VertexIndex*>	        m_currentIndices;
    Point3F*                                        m_pCurrentVertex;
	VertexIndex*			                        m_pCurrentIndex;
	IndieZen::ThreeSpace::I_EditableMesh*           m_pCurrentMesh;
	IndieZen::ResourceManager::I_CompoundResource*	m_pResource;
    /// @}

};  // ZColladaParser

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCollada
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_ZCOLLADA_ZCOLLADA_PARSER_HPP_INCLUDED