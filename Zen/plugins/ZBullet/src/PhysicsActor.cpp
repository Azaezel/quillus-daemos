//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
// Copyright (C)        2009 Brian Roberts
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
#include "PhysicsActor.hpp"
#include "CollisionShape.hpp"
#include "PhysicsZone.hpp"

#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>
#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

//#include <Zen/Core/Memory/managed_ptr.hpp>

#include <iostream>
#include <cstddef>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBullet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::PhysicsActor(wpPhysicsZone_type _zone)
:   m_pActor(NULL)
,   m_pZone(_zone)
,   m_pScriptObject(NULL)
,   m_activationState(1)
{
    // TODO impl scale
    m_scaleX = 1.0f;
    m_scaleY = 1.0f;
    m_scaleZ = 1.0f;

    m_name = "";
    m_MotionState = NULL;
    m_mass = 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::~PhysicsActor()
{
    if (m_MotionState != NULL) delete m_MotionState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::wpPhysicsZone_type
PhysicsActor::getPhysicsZone()
{
    return m_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setCollisionShape(pCollisionShape_type _pCollisionShape)
{
    m_CollisionShape = _pCollisionShape;
    m_mass = 0.0f;
    attachBody(m_CollisionShape);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::pCollisionShape_type
PhysicsActor::getCollisionShape()
{
    return m_CollisionShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setName(const std::string& _name)
{
    m_name = _name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string& 
PhysicsActor::getName()
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
btRigidBody*
PhysicsActor::getActorPtr()
{
    return m_pActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setActorPtr(btRigidBody* _pActor)
{
    m_pActor = (btRigidBody*)_pActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PhysicsActor::attachBody(pCollisionShape_type _collision)
{
    // valid Newton collision primitives include the following:
    //   Null, Box, Sphere (ovoid), Cone, Capsule, Cylinder, ChamferCylinder,
    //   ConvexHull (not impl yet), CompoundCollision (not impl),
    //   TreeCollision (not impl yet), UserMesh (heightfield)

    // set the transform call back function
    m_MotionState = new MotionState(this);
    //mass, motionstate, collisionshape, and inertia are all necessary prior to creation for bullet
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass,m_MotionState,dynamic_cast<CollisionShape*>(_collision.get())->getShapePtr(),btVector3(0.0f,0.0f,0.0f));

    m_pActor = new btRigidBody(rbInfo);
    m_pActor->setUserPointer(this);
    m_pActor->setCollisionFlags(m_pActor->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    return true;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAdvancedCollisionPrediction(bool _mode)
{
//no ccd switch available
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PhysicsActor::getAdvancedCollisionPrediction() const
{
    return 1;//no ccd switch available
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setCollisionGroup(const int _group)
{
    m_collisionGroup = _group;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const int
PhysicsActor::getCollisionGroup() const
{
    return m_collisionGroup;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setMaterial(PhysicsActor::pPhysicsMaterial_type _material)
{
    m_material = _material;
    //todo: assign actual bullet matrerial properties to the actor it'sself, leave this reference for extended
    //variables for use with zen behaviors
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::pPhysicsMaterial_type
PhysicsActor::getMaterial()
{
    return m_material;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Real
PhysicsActor::getLinearDamping() const
{
    return m_pActor->getLinearDamping();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setLinearDamping(float _damping)
{
    m_pActor->setDamping(_damping, m_pActor->getAngularDamping());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setSleepingThresholds(float _minLinearMotion, float _minAngularMotion)
{
    m_pActor->setSleepingThresholds(_minLinearMotion, _minAngularMotion);
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Real
PhysicsActor::getMass()
{
    return m_mass;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//this gets a bit arcane since we're dealing with bit-flags. see http://www.cprogramming.com/tutorial/bitwise_operators.html for reference - bjr
void 
PhysicsActor::SetStatic(bool _isStatic = false)
{
	m_IsStatic = _isStatic;
	if (_isStatic)
	{
		setMass(0);
      m_pActor->setCollisionFlags(m_pActor->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	}
	else
   {
		setMass(m_mass);
      m_pActor->setCollisionFlags(m_pActor->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
   }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsActor::setGameObject(pOwningObject_type _pOwner)
{
	m_pOwner = _pOwner;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::pOwningObject_type
PhysicsActor::getGameObject()
{
	return m_pOwner;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setMass(float _mass)
{
    m_mass = _mass;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setScale(float _x, float _y, float _z)
{
    m_scaleX = _x;
    m_scaleY = _y;
    m_scaleZ = _z;

#if 0
    btCollisionShape* collision = createCollision();

    btRigidBodySetCollision(m_pActor, collision);
    NewtonReleaseCollision(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), collision);
#else
    std::cout << "Warning: PhysicsActor::setScale() not implemented!" << std::endl;
#endif
    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setPosition(const Math::Point3& _pos)
{
    btTransform mat;

    //convert v3f to v4f (bullet pads it's btVector3s)
    btVector3 tquat = btVector3(_pos.m_x,_pos.m_y,_pos.m_z);
    mat.setOrigin(tquat);
    mat.setRotation(m_pActor->getOrientation());
    
    if (m_pActor->getMotionState() != NULL)
        m_pActor->getMotionState()->setWorldTransform(mat);
    m_pActor->setCenterOfMassTransform(mat);

    setActivationState(true);
    m_activationState = 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Point3
PhysicsActor::getPosition()
{
    Math::Point3 position;

    btVector3 tquat = m_pActor->getWorldTransform().getOrigin();

    //convert v4f to v3f (bullet pads it's btVector3s)
    position.m_x = tquat.x();
    position.m_y = tquat.y();
    position.m_z = tquat.z();
    return position;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getOrientation(Math::Quaternion4& _orient)
{
    Math::Matrix4 matrix;
    btQuaternion tquat = m_pActor->getWorldTransform().getRotation();
    _orient.m_x = tquat.x();
    _orient.m_y = tquat.y();
    _orient.m_z = tquat.z();
    _orient.m_w = tquat.w();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getOrientation(Math::Matrix4& _orient)
{
    Math::Quaternion4 tquat;
    getOrientation(tquat);
    _orient = Math::Matrix4(tquat);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setOrientation(const Math::Quaternion4& _orient)
{
    btTransform mat;

    btVector3 tquat = btVector3(_orient.m_x,_orient.m_y,_orient.m_z);
    tquat.setW(_orient.m_w);

    mat.setOrigin(m_pActor->getWorldTransform().getOrigin());
    mat.setRotation(btQuaternion(tquat,1.0f));
    
    if (m_pActor->getMotionState() != NULL)
        m_pActor->getMotionState()->setWorldTransform(mat);
    m_pActor->setCenterOfMassTransform(mat);

    setActivationState(true);
    m_activationState = 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setOrientation(const Math::Matrix4& _orient)
{
    Math::Quaternion4 tquat = Math::Quaternion4(_orient);
    setOrientation(tquat);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setLinearVelocity(const Math::Vector3& _velocity)
{
    setActivationState(true);

    btVector3 tquat = btVector3(_velocity.m_x,_velocity.m_y,_velocity.m_z);
    m_pActor->setLinearVelocity(tquat);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getLinearVelocity(Math::Vector3& _velocity)
{
	btVector3 tquat = m_pActor->getLinearVelocity();
	_velocity.m_x = tquat.x();
	_velocity.m_y = tquat.y();
	_velocity.m_z = tquat.z();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getTorque()
{
    btVector3 tquat = m_pActor->getTotalTorque();
    m_torque.m_x = tquat.x();
    m_torque.m_y = tquat.y();
    m_torque.m_z = tquat.z();
    return m_torque;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getAngularVelocity() const
{
    Math::Vector3 omega;
    btVector3 tquat = m_pActor->getAngularVelocity();
    omega.m_x = tquat.x();
    omega.m_y = tquat.y();
    omega.m_z = tquat.z();
    return omega;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAngularVelocity(const Math::Vector3& _omega)
{
    setActivationState(true);
    m_pActor->setAngularVelocity(btVector3(_omega.m_x,_omega.m_y,_omega.m_z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getAngularDamping() const
{
    Math::Vector3 damping;
    Math::Real tdamping = m_pActor->getAngularDamping();
    //angular damping in bullet is omnidirectional
    damping = Math::Vector3(tdamping,tdamping,tdamping);
    return damping;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAngularDamping(const Math::Vector3& _damping)
{
    m_pActor->setDamping(m_pActor->getLinearDamping(), _damping.magnitude());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::MotionState::getWorldTransform(btTransform& _worldTrans) const
{
//don't belive we really need anything here since were updating visuals on the set end... might need to crosscheck later -bjr
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::MotionState::setWorldTransform(const btTransform& _worldTrans)
{
    if (m_userPointer == NULL) return;
    PhysicsActor* pShape = static_cast<PhysicsActor*>(m_userPointer);

    // Only use the transform callback if the state is active
    if (pShape->getActivationState() == 1)
    {
        Math::Matrix4 transform;
        _worldTrans.getOpenGLMatrix(transform.m_array);
        TransformEventData evenData(*pShape, transform);
        pShape->onTransformEvent(evenData);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Physics::I_PhysicsActor&
PhysicsActor::TransformEventData::getActor()
{
    return m_actor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Matrix4&
PhysicsActor::TransformEventData::getTransform() const
{
    return m_transform;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Physics::I_PhysicsActor&
PhysicsActor::CollisionEventData::getActor()
{
    return m_actor;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Physics::I_PhysicsActor&
PhysicsActor::CollisionEventData::getOtherActor()
{
    return m_otherActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
PhysicsActor::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        Zen::Engine::Physics::I_PhysicsManager::pScriptModule_type pModule =
            Zen::Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule();

        if(pModule.isValid())
        {
            m_pScriptObject = new Zen::Engine::Physics::I_PhysicsActor::ScriptObjectReference_type
                (pModule, pModule->getScriptType("PhysicsActor"), this);
        }
        else
        {
            m_pScriptObject = NULL;
        }
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyForce(const Math::Vector3& _force)
{
    m_pActor->applyCentralImpulse(btVector3(_force.m_x,_force.m_y,_force.m_z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyImpulse(const Math::Vector3& _force, const Math::Vector3& _worldPos)
{
    //todo: bullet needs it's force fed to it in local space coords
     m_pActor->applyForce(btVector3(_force.m_x,_force.m_y,_force.m_z), btVector3(_worldPos.m_x,_worldPos.m_y,_worldPos.m_z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyTorque(const Math::Vector3& _torque)
{
    btVector3 tquat = btVector3(_torque.m_x,_torque.m_y,_torque.m_z);
    m_pActor->applyTorque(tquat);//todo: crosscheck consistency, and if this one acts too far differently than newton, use applyTorqueImpulse, as that accounts for an inertial tensor
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setActivationState(unsigned _state)
{
    m_activationState = _state;
    m_pActor->activate(_state);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
PhysicsActor::getActivationState()
{
    m_activationState = m_pActor->getActivationState();
    return m_activationState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
