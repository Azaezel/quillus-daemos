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
#ifndef ZEN_ZODE_PHYSICS_MESH_COLLISION_SHAPE_HPP_INCLUDED
#define ZEN_ZODE_PHYSICS_MESH_COLLISION_SHAPE_HPP_INCLUDED

#include "CollisionShape.hpp"

#include <Zen/Engine/Physics/I_MeshCollisionShape.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class MeshCollisionShape
:   public Engine::Physics::I_MeshCollisionShape
,   public CollisionShape
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_CollisionShape implementation
    /// @{
public:
    virtual CollisionShapeType getType();
    /// @}

    /// @name I_MeshCollisionShape implementation
    /// @{
public:
    /// @}

    /// @name MeshCollisionShape implementation
    /// @{
public:
    void buildFromTriangles(dSpaceID _spaceId, std::vector<Zen::Math::Point3>& _vertices, std::vector<boost::array<int, 3> >& _indices);
    /// @}

    /// @name 'Structors
    /// @{
public:
             MeshCollisionShape(dGeomID _geometryId);
    virtual ~MeshCollisionShape();
    /// @}

    /// @name Member Variables
    /// @{
private:
    dTriMeshDataID  m_triMesh;
    dVector3*       m_pVertices;
    int             m_vertexCount;
    unsigned int*   m_pIndices;
    int             m_indexCount;
    /// @}

};  // class CollisionShape


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_PHYSICS_MESH_COLLISION_SHAPE_HPP_INCLUDED
