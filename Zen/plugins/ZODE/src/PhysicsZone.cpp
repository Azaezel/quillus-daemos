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

#include "PhysicsZone.hpp"
#include "PhysicsActor.hpp"
#include "PhysicsMaterial.hpp"
#include "CollisionShape.hpp"
#include "HeightfieldCollisionShape.hpp"
#include "MeshCollisionShape.hpp"

#include <Zen/Core/Math/Vector3.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>

#include <Zen/Engine/World/I_TerrainHeightfield.hpp>
#include <Zen/Engine/World/I_TerrainChunk.hpp>

//#include <dVector.h> //no likey. recheck
//refnotes: http://docs.taoframework.com/Tao.Ode/Tao.Ode.Ode.dHashSpaceCreate.html
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::PhysicsZone()
:   m_pScriptModule(Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
,   m_spaceId(0)
,   m_materials()
{
    m_worldId = dWorldCreate();
    m_spaceId = dSimpleSpaceCreate(m_spaceId);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::~PhysicsZone()
{
    dWorldDestroy(m_worldId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//static const Zen::Math::Real sm_epsilon = std::numeric_limits<Zen::Math::Real>::epsilon();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::beginFrame()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::stepSimulation(double _elapsedTime)
{
    dWorldStep(m_worldId, (dReal)_elapsedTime);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::endFrame()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
PhysicsZone::getDefaultMaterialID()
{
    throw Utility::runtime_exception("PhysicsZone::getDefaultMaterialID(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setBoundary(const Math::Vector3& _min, const Math::Vector3& _max)
{
    // TODO Convert _min and _max to center and extents
    Math::Vector3 center = _min + (_max - _min) / 2;

    Math::Vector3 extents(_max - _min);

    // TODO don't hard-code the dept to be 4.
    m_spaceId = dQuadTreeSpaceCreate(m_spaceId, center.m_array, extents.m_array, 4);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setGravity(const Math::Vector3& _grav)
{
    throw Utility::runtime_exception("PhysicsZone::setGravity(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
destroyActor(Zen::Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsActor> _pActor)
{
    delete dynamic_cast<PhysicsActor*>(_pActor.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsActor_type
PhysicsZone::createActor()
{
    dBodyID bodyId = dBodyCreate(m_worldId);

    pPhysicsActor_type pActor = pPhysicsActor_type(
        new PhysicsActor(bodyId, getSelfReference()),
        destroyActor
    );

    return pActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::destroyMaterial(wpPhysicsMaterial_type _pMaterial)
{
    PhysicsMaterial* pRaw = 
        dynamic_cast<PhysicsMaterial*>(_pMaterial.get());

    if( pRaw != NULL )
    {
        delete pRaw;
    }
    else
    {
        throw Zen::Utility::runtime_exception("PhysicsZone::destroyMaterial() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsMaterial_type
PhysicsZone::createMaterial(bool _default)
{
    PhysicsMaterial* pRaw = new PhysicsMaterial(_default);

    pPhysicsMaterial_type pMaterial(
        pRaw,
        &destroyMaterial
    );

    pRaw->setSelfReference(pMaterial.getWeak());

    m_materials[pMaterial->getMaterialId()] = pMaterial;

    return pMaterial;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::vector<PhysicsZone::pPhysicsMaterial_type>
PhysicsZone::getAllMaterials()
{
    throw Utility::runtime_exception("PhysicsZone::getAllMaterials(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsJoint_type
PhysicsZone::createJoint()
{
    throw Utility::runtime_exception("PhysicsZone::createJoint(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
PhysicsZone::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type
            (m_pScriptModule, m_pScriptModule->getScriptType(getScriptTypeName()), getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor)
{
    throw Utility::runtime_exception("PhysicsZone::rayCast(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createNullShape()
{
    throw Utility::runtime_exception("PhysicsZone::createNullShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createBoxShape(float _dx, float _dy, float _dz)
{
    throw Utility::runtime_exception("PhysicsZone::createBoxShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createOvoidShape(float _radiusX, float _radiusY, float _radiusZ)
{
    throw Utility::runtime_exception("PhysicsZone::createOvoidShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::destroyCollisionShape(wpCollisionShape_type _pCollisionShape)
{
    CollisionShape* pRaw = 
        dynamic_cast<CollisionShape*>(_pCollisionShape.get());

    if( pRaw != NULL )
    {
        delete pRaw;
    }
    else
    {
        throw Zen::Utility::runtime_exception("PhysicsZone::destroyCollisionShape() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createCapsuleShape(float _radius, float _height)
{
    dGeomID capsuleShapeId = dCreateCapsule(m_spaceId, _radius, _height);

    CollisionShape* pRawCollisionShape = new CollisionShape(capsuleShapeId);

    pCollisionShape_type pCollisionShape(
        pRawCollisionShape,
        &destroyCollisionShape
    );

    return pCollisionShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::destroyHeightfieldCollisionShape(wpCollisionShape_type _pCollisionShape)
{
    HeightfieldCollisionShape* pRaw = 
        dynamic_cast<HeightfieldCollisionShape*>(_pCollisionShape.get());

    if( pRaw != NULL )
    {
        delete pRaw;
    }
    else
    {
        throw Zen::Utility::runtime_exception("PhysicsZone::destroyHeightfieldCollisionShape() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShape(pTerrainChunk_type _pTerrainChunk)
{
    HeightfieldCollisionShape* pRawCollisionShape =
        new HeightfieldCollisionShape(m_spaceId, _pTerrainChunk);

    pCollisionShape_type pCollisionShape(
        pRawCollisionShape,
        &destroyHeightfieldCollisionShape
    );

    return pCollisionShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
	throw Utility::runtime_exception("PhysicsZone::createHeightFieldShapeFromRaw(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform)
{
    throw Utility::runtime_exception("PhysicsZone::createHeightFieldShapeFromSerialization(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createConvexHullShape(std::string _filename)
{
    throw Utility::runtime_exception("PhysicsZone::createConvexHullShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createTreeCollisionShape(std::string _filename)
{
    throw Utility::runtime_exception("PhysicsZone::createTreeCollisionShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::destroyMeshCollisionShape(wpCollisionShape_type _pCollisionShape)
{
    MeshCollisionShape* pRaw =
        dynamic_cast<MeshCollisionShape*>(_pCollisionShape.get());

    if( pRaw != NULL )
    {
        delete pRaw;
    }
    else
    {
        throw Zen::Utility::runtime_exception("PhysicsZone::destroyMeshCollisionShape() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createMeshShape(std::vector<Zen::Math::Point3>& _vertices, std::vector<boost::array<int, 3> >& _indices)
{
    MeshCollisionShape* pRawShape = new MeshCollisionShape(NULL);
    pRawShape->buildFromTriangles(m_spaceId, _vertices, _indices);
    pCollisionShape_type pCollisionShape(
        pRawShape,
        &destroyMeshCollisionShape
    );
    return pCollisionShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
dSpaceID
PhysicsZone::getSpaceId()
{
    return m_spaceId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
