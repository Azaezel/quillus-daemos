//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
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
#include "PhysicsActor.hpp"
#include "PhysicsZone.hpp"
#include "HeightfieldResource.hpp"
#include "CollisionShape.hpp"
#include "MeshCollisionShape.hpp"

#include <Zen/Engine/Resource/I_CollisionResource.hpp>

#include <Zen/Engine/Physics/I_CollisionShape.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>
#include <Zen/Engine/Physics/I_PhysicsManager.hpp>

#include <iostream>
#include <cstddef>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::PhysicsActor(dBodyID _bodyId, wpPhysicsZone_type _wpParentZone)
:   m_bodyId(_bodyId)
,   m_pModule(Zen::Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
,   m_wpParentZone(_wpParentZone)
{
    if (m_bodyId != NULL)
    {
        dBodySetData(m_bodyId, this);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::~PhysicsActor()
{
    if (m_bodyId != NULL)
    {
        dBodyDestroy( m_bodyId );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
PhysicsActor::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_pModule,
            m_pModule->getScriptType(getScriptTypeName()),
            this->getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setGameObject(pGameObject_type _pGameObject)
{
    throw Utility::runtime_exception("PhysicsActor::setGameObject(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::pGameObject_type
PhysicsActor::getGameObject()
{
    throw Utility::runtime_exception("PhysicsActor::getGameObject(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setCollisionShape(pCollisionShape_type _pCollisionShape)
{
    switch(_pCollisionShape->getType())
    {
    case Zen::Engine::Physics::I_CollisionShape::HEIGHTFIELD_SHAPE:
        {
            Zen::Engine::Resource::I_CollisionResource* pCollisionResource = dynamic_cast<Zen::Engine::Resource::I_CollisionResource*>(_pCollisionShape.get());
            HeightfieldResource* pHeightfield = dynamic_cast<HeightfieldResource*>(pCollisionResource);

            if (pHeightfield)
            {
                pHeightfield->initialize(this);
            }

            CollisionShape* pShape = dynamic_cast<CollisionShape*>(_pCollisionShape.get());

            dGeomSetBody(pShape->getGeomId(), m_bodyId);

            return;
        }
    case Zen::Engine::Physics::I_CollisionShape::CAPSULE_SHAPE:
        {
            CollisionShape* pShape = dynamic_cast<CollisionShape*>(_pCollisionShape.get());

            dGeomSetBody(pShape->getGeomId(), m_bodyId);

            return;
        }
    case Zen::Engine::Physics::I_CollisionShape::MESH_SHAPE:
        {
            MeshCollisionShape* pShape = dynamic_cast<MeshCollisionShape*>(_pCollisionShape.get());

            dGeomSetBody(pShape->getGeomId(), m_bodyId);

            return;
        }
    default:
        throw Utility::runtime_exception("PhysicsActor::setCollisionShape(): Error, unsupported shape type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::pCollisionShape_type
PhysicsActor::getCollisionShape()
{
    throw Utility::runtime_exception("PhysicsActor::getCollisionShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setMaterial(pPhysicsMaterial_type _material)
{
    throw Utility::runtime_exception("PhysicsActor::setMaterial(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::pPhysicsMaterial_type
PhysicsActor::getMaterial()
{
    throw Utility::runtime_exception("PhysicsActor::getMaterial(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::wpPhysicsZone_type
PhysicsActor::getPhysicsZone()
{
    return m_wpParentZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Real
PhysicsActor::getMass()
{
    throw Utility::runtime_exception("PhysicsActor::getMass(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setMass(const Zen::Math::Real _mass)
{
    dMass mass;

    dBodyGetMass(m_bodyId, &mass);
    mass.mass = _mass;
    dBodySetMass(m_bodyId, &mass);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setPosition(const Math::Point3& _position)
{
    dBodySetPosition(m_bodyId, _position.m_x, _position.m_y, _position.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Point3
PhysicsActor::getPosition()
{
    throw Utility::runtime_exception("PhysicsActor::getPosition(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getOrientation(Math::Quaternion4& _orient)
{
    throw Utility::runtime_exception("PhysicsActor::getOrientation(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setOrientation(const Math::Quaternion4& _orient)
{
    throw Utility::runtime_exception("PhysicsActor::setOrientation(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getOrientation(Math::Matrix4& _orient)
{
    throw Utility::runtime_exception("PhysicsActor::getOrientation(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setOrientation(const Math::Matrix4& _orient)
{
    throw Utility::runtime_exception("PhysicsActor::setOrientation(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setLinearVelocity(const Math::Vector3& _velocity)
{
    throw Utility::runtime_exception("PhysicsActor::setLinearVelocity(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getLinearVelocity(Math::Vector3& _velocity)
{
    throw Utility::runtime_exception("PhysicsActor::getLinearVelocity(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getAngularVelocity() const
{
    throw Utility::runtime_exception("PhysicsActor::getAngularVelocity(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAngularVelocity(const Math::Vector3& _omega)
{
    throw Utility::runtime_exception("PhysicsActor::setAngularVelocity(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getTorque()
{
    throw Utility::runtime_exception("PhysicsActor::getTorque(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyTorque(const Math::Vector3& _torque)
{
    throw Utility::runtime_exception("PhysicsActor::applyTorque(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyForce(const Math::Vector3& _force)
{
    throw Utility::runtime_exception("PhysicsActor::applyForce(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyImpulse(const Math::Vector3& _force, const Math::Vector3& _worldPos)
{
    throw Utility::runtime_exception("PhysicsActor::applyImpulse(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3 
PhysicsActor::getAngularDamping() const
{
    throw Utility::runtime_exception("PhysicsActor::getAngularDamping(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsActor::setAngularDamping(const Math::Vector3& _damping)
{
    throw Utility::runtime_exception("PhysicsActor::setAngularDamping(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Real 
PhysicsActor::getLinearDamping() const
{
    throw Utility::runtime_exception("PhysicsActor::getLinearDamping(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsActor::setLinearDamping(float _damping)
{
    throw Utility::runtime_exception("PhysicsActor::setLinearDamping(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsActor::setSleepingThresholds(float _minLinearMotion, float _minAngularMotion)
{
    throw Utility::runtime_exception("PhysicsActor::setSleepingThresholds(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsActor::setActivationState(unsigned _state)
{
    throw Utility::runtime_exception("PhysicsActor::setActivationState(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
PhysicsActor::getActivationState()
{
    throw Utility::runtime_exception("PhysicsActor::getActivationState(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setCollisionGroup(const int _group)
{
    throw Utility::runtime_exception("PhysicsActor::setCollisionGroup(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const int
PhysicsActor::getCollisionGroup() const
{
    throw Utility::runtime_exception("PhysicsActor::getCollisionGroup(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone&
PhysicsActor::getParentZone()
{
    assert(m_wpParentZone.lock().isValid());
    return *dynamic_cast<PhysicsZone*>(m_wpParentZone.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
