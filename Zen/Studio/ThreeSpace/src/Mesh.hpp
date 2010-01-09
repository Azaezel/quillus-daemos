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
#ifndef INDIEZEN_THREESPACE_MESH_HPP_INCLUDED
#define INDIEZEN_THREESPACE_MESH_HPP_INCLUDED

#include <vector>
#include <list>

#include "../I_Mesh.hpp"
#include "../I_Face.hpp"

#include "../VertexIndex.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ThreeSpace {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Mesh
:   public virtual I_Mesh
{
	/// @name Types
	/// @{
public:
	class Face
    :   public I_Face
	{
    public:
        virtual unsigned int getVertexCount()
        {
            return m_indices.size();
        }

        virtual void addIndex(unsigned _index)
        {
            m_indices.push_back(new VertexIndex(_index));
        }

    public:
		typedef std::vector<VertexIndex*>   vi_container_type;
		vi_container_type m_indices;
	};

	typedef std::vector<Face*>			face_container_type;
	/// @}

	/// @name I_Mesh implementation
	/// @{
public:
	virtual unsigned int getFaceCount();
	virtual unsigned int getVertexCount(unsigned int _face);
	virtual Math::Point3& getVertexForFace(unsigned int _face, unsigned int _vertex);
    virtual unsigned int getVertexIndexForFace(unsigned int _face, unsigned int _vertex);
	virtual bool isAllTriangles();
	virtual bool isAllQuads();
    virtual void setLoaded(bool _loaded);
    virtual bool isLoaded() const;
	virtual vertex_container_type* getVertices();
	/// @}

	/// @name 'Structors
    /// @{
public:
             Mesh();
    virtual ~Mesh();
    /// @}

	/// @name Additional implementation
	/// @{
protected:
	/// @}

	/// @name Member variables
	/// @{
protected:
	vertex_container_type	m_vertices;
	face_container_type		m_faces;

	vertex_container_type*	m_pVertices;

	bool m_quads;	///< True if this mesh contains quads
	bool m_tris;	///< True if this mesh contains triangles

    bool m_isLoaded;  ///< True if this mesh is completely loaded
	/// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace IndieZen
}   // namespace ThreeSpace
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_THREESPACE_MESH_HPP_INCLUDED
