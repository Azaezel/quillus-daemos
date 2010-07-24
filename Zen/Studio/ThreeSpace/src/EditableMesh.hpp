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
#ifndef INDIEZEN_THREESPACE_EDITABLE_MESH_HPP_INCLUDED
#define INDIEZEN_THREESPACE_EDITABLE_MESH_HPP_INCLUDED

#include <IndieZen/framework/ThreeSpace/I_EditableMesh.hpp>

#include "Mesh.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ThreeSpace {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class EditableMesh
:   public I_EditableMesh
,   public Mesh
{
	/// @name Types
	/// @{
public:
	/// @}

	/// @name I_Mesh implementation
	/// @{
public:
    virtual void setName(const std::string& _name);
    virtual const std::string& getName() const;
    virtual unsigned int getFaceCount() { return Mesh::getFaceCount(); }
    virtual unsigned int getVertexCount(unsigned int _face) { return Mesh::getVertexCount(_face); }
    virtual Math::Point3& getVertexForFace(unsigned int _face, unsigned int _vertex) { return Mesh::getVertexForFace(_face, _vertex); }
    virtual unsigned int getVertexIndexForFace(unsigned int _face, unsigned int _vertex) { return Mesh::getVertexIndexForFace(_face, _vertex); }
    virtual bool isAllTriangles() { return Mesh::isAllTriangles(); }
    virtual bool isAllQuads() { return Mesh::isAllQuads(); }
    virtual void setLoaded(bool _loaded) { Mesh::setLoaded(_loaded); }
    virtual bool isLoaded() const { return Mesh::isLoaded(); }
    virtual vertex_container_type* getVertices() { return Mesh::getVertices(); }
	/// @}

	/// @name I_EditableMesh implementation
	/// @{
public:
	virtual unsigned int addVertex(Math::Point3* _vertex);
	virtual void shareVertexList(I_Mesh* _pMesh);
	virtual void addFace(VertexIndex* _pX, VertexIndex* _pY, VertexIndex* _pZ);
	virtual void addFace(VertexIndex* _p1, VertexIndex* _p2, VertexIndex* _p3, VertexIndex* _p4);
    virtual I_Face* createFace();
	/// @}

	/// @name 'Structors
    /// @{
public:
             EditableMesh();
    virtual ~EditableMesh();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string     m_name;
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace IndieZen
}   // namespace ThreeSpace
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_THREESPACE_EDITABLE_MESH_HPP_INCLUDED
