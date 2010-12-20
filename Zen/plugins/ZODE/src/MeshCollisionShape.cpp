//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#include "MeshCollisionShape.hpp"

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>

#include <ode/ode.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
MeshCollisionShape::MeshCollisionShape(dGeomID _geometryId)
:   CollisionShape(_geometryId)
,   m_pVertices(NULL)
,   m_vertexCount(0)
,   m_pIndices(NULL)
,   m_indexCount(0)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
MeshCollisionShape::~MeshCollisionShape()
{
    if (m_pVertices)
    {
        delete m_pVertices;
    }

    if (m_pIndices)
    {
        delete m_pIndices;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CollisionShape::CollisionShapeType
MeshCollisionShape::getType()
{
    return MESH_SHAPE;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
MeshCollisionShape::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_pModule,
            m_pModule->getScriptType(getScriptTypeName()),
            this
        );
    }
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
MeshCollisionShape::buildFromTriangles(dSpaceID _spaceId, std::vector<Zen::Math::Point3>& _vertices, std::vector<boost::array<int, 3> >& _indices)
{
    // Make sure this hasn't already been called
    assert(m_pVertices == NULL);

    // Create the buffers to store the values.
    m_pVertices = new dVector3[_vertices.size()];
    m_vertexCount = _vertices.size();
    m_pIndices = new unsigned int[_indices.size()];
    m_indexCount = _indices.size();

    // Copy the values
    dVector3* pVertex = m_pVertices;
    for(std::vector<Zen::Math::Point3>::iterator iter = _vertices.begin(); iter != _vertices.end(); iter++)
    {
        (*pVertex)[0] = iter->m_array[0];
    }

    m_triMesh = dGeomTriMeshDataCreate();
    dGeomTriMeshDataBuildSimple(m_triMesh, (dReal*)m_pVertices, m_vertexCount, (const dTriIndex*)m_pIndices, m_indexCount);

    setGeometryId(dCreateTriMesh(_spaceId, m_triMesh, NULL, NULL, NULL));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
