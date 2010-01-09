//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef INDIEZEN_THREESPACE_I_MESH_HPP_INCLUDED
#define INDIEZEN_THREESPACE_I_MESH_HPP_INCLUDED

#include "Configuration.hpp"

#include <IndieZen/framework/ResourceManager/I_Resource.hpp>

#include <boost/noncopyable.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
    namespace Math {
        class Point3;
    }   // namespace Math
namespace ThreeSpace {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// @author Tony Richards
/// @since 1.0
class THREESPACE_DLL_LINK I_Mesh
:   virtual public ResourceManager::I_Resource
{
    /// @name Types
    /// @{
public:
    typedef std::vector<Math::Point3*>              vertex_container_type;
    /// @}

    /// @name I_Mesh interface
    /// @{ 
public:
    /// Get the number of faces
    virtual unsigned int getFaceCount() = 0;

    /// Get the vertex count for an indexed face
    virtual unsigned int getVertexCount(unsigned int _face) = 0;

    /// Get the vertex for an indexed face
    virtual Math::Point3& getVertexForFace(unsigned int _face, unsigned int _vertex) = 0;

    /// Get the vertex index for a face
    virtual unsigned int getVertexIndexForFace(unsigned int _face, unsigned int _vertex) = 0;

    /// Returns true if the mesh is made up completely of triangles
    virtual bool isAllTriangles() = 0;

    /// Returns true if the mesh is made up completely of quads
    virtual bool isAllQuads() = 0;

    /// Indicates that the mesh has finished loading
    virtual void setLoaded(bool _loaded) = 0;

    /// Returns true if the mesh is finished loading
    virtual bool isLoaded() const = 0;

    virtual vertex_container_type* getVertices() = 0;
    /// @}

    /// @{ 'Structors
protected:
             I_Mesh();
    virtual ~I_Mesh();
    /// @}

};  // interface I_Mesh

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ThreeSpace
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_THREESPACE_I_MESH_HPP_INCLUDED
