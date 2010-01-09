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
#ifndef INDIEZEN_THREESPACE_I_EDITABLE_MESH_HPP_INCLUDED
#define INDIEZEN_THREESPACE_I_EDITABLE_MESH_HPP_INCLUDED

#include <IndieZen/framework/ThreeSpace/I_Mesh.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ThreeSpace {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class VertexIndex;
class I_Face;

/// Mesh optimized for editing
/// @author Tony Richards
/// @since 1.0
class THREESPACE_DLL_LINK I_EditableMesh
:   public virtual I_Mesh
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_EditableMesh interface
    /// @{ 
public:
    virtual void setName(const std::string& _name) = 0;

    virtual const std::string& getName() const = 0;

	/// Set a vertex
	/// The called method obtains ownership of the vertex.
	/// @return The index for this vertex
	virtual unsigned int addVertex(Math::Point3* _vertex) = 0;

	/// Use the vertex list from another mesh
	virtual void shareVertexList(I_Mesh* _pMesh) = 0;

	/// Add a triangle face to the mesh using indices.
	virtual void addFace(VertexIndex* _pX, VertexIndex* _pY, VertexIndex* _pZ) = 0;

	/// Add a quad face to the mesh using indices.
	virtual void addFace(VertexIndex* _p1, VertexIndex* _p2, VertexIndex* _p3, VertexIndex* _p4) = 0;

    virtual I_Face* createFace() = 0;
    /// @}

    /// @{ 'Structors
protected:
             I_EditableMesh();
    virtual ~I_EditableMesh();
    /// @}

};  // interface I_EditableMesh

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ThreeSpace
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_THREESPACE_I_EDITABLE_MESH_HPP_INCLUDED

