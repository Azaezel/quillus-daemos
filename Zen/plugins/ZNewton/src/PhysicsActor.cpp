//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
// Copyright (C)        2008 Walt Collins
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
namespace ZNewton {
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
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsActor::~PhysicsActor()
{
    //NewtonDestroyBody(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), m_pActor);
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
NewtonBody*
PhysicsActor::getActorPtr()
{
    return m_pActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setActorPtr(NewtonBody* _pActor)
{
    m_pActor = (NewtonBody*)_pActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PhysicsActor::attachBody(pCollisionShape_type _collision)
{
    // valid Newton collision primitives include the following:
    //   Null, Box, Sphere (ovoid), Cone, Capsule, Cylinder, ChamferCylinder,
    //   ConvexHull (not impl yet), CompoundCollision (not impl),
    //   TreeCollision (not impl yet), UserMesh (heightfield)



    m_pActor = NewtonCreateBody(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), dynamic_cast<CollisionShape*>(_collision.get())->getShapePtr());
    NewtonReleaseCollision(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), dynamic_cast<CollisionShape*>(_collision.get())->getShapePtr());

    // set the transform call back function
    NewtonBodySetTransformCallback(m_pActor, TransformCallback);
    NewtonBodySetForceAndTorqueCallback(m_pActor, ApplyForceAndTorqueCallback);
    NewtonBodySetAutoactiveCallback(m_pActor, ActivationStateCallback);

    //NewtonBodySetFreezeTreshold(m_pActor, 

    NewtonBodySetUserData(m_pActor, this);

    return true;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAdvancedCollisionPrediction(bool _mode)
{
    if (_mode)
    {
        NewtonBodySetContinuousCollisionMode(m_pActor, 1);
    }
    else
    {
        NewtonBodySetContinuousCollisionMode(m_pActor, 0);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PhysicsActor::getAdvancedCollisionPrediction() const
{
    return (NewtonBodyGetContinuousCollisionMode(m_pActor) != 0);
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
    NewtonBodySetMaterialGroupID(m_pActor, m_material->getMaterialID());
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
    return Math::Real(NewtonBodyGetLinearDamping(m_pActor));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setLinearDamping(float _damping)
{
    NewtonBodySetLinearDamping(m_pActor, _damping);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAutoFreeze(bool _bFreeze)
{
    NewtonBodySetAutoFreeze(m_pActor, _bFreeze ? 1 : 0);
    setActivationState(true);
    m_activationState = 1;
}
// http://newtondynamics.com/wiki/index.php5?title=NewtonBodySetFreezeTreshold
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setSleepingThresholds(float _minLinearMotion, float _minAngularMotion)
{
    //we'll stick with a default of 10 frames of motion since it's simpler to track distance/angle for most folks (that, and theres no equivalents in other engines I've found) -BJR
    NewtonBodySetFreezeTreshold(m_pActor,_minLinearMotion,_minAngularMotion, 10);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Real
PhysicsActor::getMass()
{
    Zen::Math::Real mass;
    Zen::Math::Real Ixx;
    Zen::Math::Real Iyy;
    Zen::Math::Real Izz;
    NewtonBodyGetMassMatrix(m_pActor, &mass, &Ixx, &Iyy, &Izz);

    return mass;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsActor::SetStatic(bool _isStatic = false)
{
	m_IsStatic = _isStatic;
	if (_isStatic)
	{
		setMass(0);
	}
	else
		setMass(m_mass);
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
    // set the mass to zero to make this a static body.
    // set the mass to any positive value to make this a dynamic body.
    Zen::Math::Real Ixx = _mass * m_scaleX * m_scaleX;
    Zen::Math::Real Iyy = _mass * m_scaleY * m_scaleY;
    Zen::Math::Real Izz = _mass * m_scaleZ * m_scaleZ;
    NewtonBodySetMassMatrix(m_pActor, _mass, Ixx, Iyy, Izz);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setScale(float _x, float _y, float _z)
{
    m_scaleX = _x;
    m_scaleY = _y;
    m_scaleZ = _z;

#if 0
    NewtonCollision* collision = createCollision();

    NewtonBodySetCollision(m_pActor, collision);
    NewtonReleaseCollision(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), collision);
#else
    std::cout << "Warning: PhysicsActor::setScale() not implemented!" << std::endl;
#endif
    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setPosition(const Math::Point3& _pos)
{
    Zen::Math::Matrix4 mat;
    //mat.identity();
    getOrientation(mat);
    mat.setPosition(_pos);
    setActivationState(true);
    m_activationState = 1;

    NewtonBodySetMatrix(m_pActor, mat.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Point3
PhysicsActor::getPosition()
{
    Math::Point3 position;
    Math::Matrix4 matrix;
    NewtonBodyGetMatrix(m_pActor, matrix.m_array);
    matrix.getPosition(position);
    return position;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getOrientation(Math::Quaternion4& _orient)
{
    Math::Matrix4 matrix;
    getOrientation(matrix);
    _orient = Math::Quaternion4(matrix);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getOrientation(Math::Matrix4& _orient)
{
    Math::Matrix4 matrix;
    NewtonBodyGetMatrix(m_pActor, matrix.m_array);

    // ensure a zero offset for the returned orientation matrix:
    matrix.setPosition(0.0f, 0.0f, 0.0f);
    
    // transfer values to the return matrix:
    for (int i = 0; i < 16; i++)
    {
        _orient.m_array[i] = matrix.m_array[i];
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setOrientation(const Math::Quaternion4& _orient)
{
    Math::Matrix4 matrix = Math::Matrix4(_orient);
    setOrientation(matrix);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setOrientation(const Math::Matrix4& _orient)
{
    // transfer values from input matrix to temporary matrix:
    Math::Matrix4 matrix;
    for (int i = 0; i < 16; i++)
    {
        matrix.m_array[i] = _orient.m_array[i];
    }

    // add offset to orientation matrix before setting body:
    Math::Point3 pos;
    pos = getPosition();
    matrix.setPosition(pos);

    setActivationState(true);
    m_activationState = 1;

    NewtonBodySetMatrix(m_pActor, matrix.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setLinearVelocity(const Math::Vector3& _velocity)
{
    setActivationState(true);
    m_activationState = 1;

    NewtonBodySetVelocity(m_pActor, _velocity.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::getLinearVelocity(Math::Vector3& _velocity)
{
    NewtonBodyGetVelocity(m_pActor, _velocity.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getTorque()
{
    Math::Vector3 velocity;
    NewtonBodyGetTorque(m_pActor, velocity.m_array);
    m_torque = velocity;
    return m_torque;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getAngularVelocity() const
{
    Math::Vector3 omega;
    NewtonBodyGetOmega(m_pActor, omega.m_array);
    return omega;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAngularVelocity(const Math::Vector3& _omega)
{
    setActivationState(true);
    m_activationState = 1;

    NewtonBodySetOmega(m_pActor, _omega.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Vector3
PhysicsActor::getAngularDamping() const
{
    Math::Vector3 damping;
    NewtonBodyGetAngularDamping(m_pActor, damping.m_array);
    return damping;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setAngularDamping(const Math::Vector3& _damping)
{
    NewtonBodySetAngularDamping(m_pActor, _damping.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// set the transformation of a rigid body
void 
PhysicsActor::TransformCallback(const NewtonBody* _body, const Zen::Math::Real* _matrix)
{
    void* pBody = NewtonBodyGetUserData(_body);
    if (pBody != NULL)
    {
        PhysicsActor* pShape = static_cast<PhysicsActor*>(pBody);

        // Only use the transform callback if the state is active
        if (pShape->m_activationState != 0)
        {
            Math::Matrix4 transform;
            for(int x = 0; x < 16; x++)
            {
                transform.m_array[x] = _matrix[x];
            }
            TransformEventData evenData(*pShape, transform);
            pShape->onTransformEvent(evenData);
        }
    }

#if 0
    // HACK Keep the object to a constant velocity
    Math::Vector3 velocity;

    NewtonBodyGetVelocity(_body, velocity.m_array);

    velocity.normalize();
    velocity = velocity * 50;

    NewtonBodySetVelocity(_body, velocity.m_array);
#endif

    //std::cout << "TransformCallback()" << std::endl;
#if 0
	RenderPrimitive* primitive;

	// get the graphic object form the rigid body
	primitive = (RenderPrimitive*) NewtonBodyGetUserData (body);

	// set the transformation matrix for this rigid body
	dMatrix& mat = *((dMatrix*)matrix);
	primitive->SetMatrix (mat);
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::ApplyForceAndTorqueCallback(const NewtonBody* _body)
{
    class ApplyForcesEventData
    :   public I_ApplyForcesEventData
    {
        /// @name I_ApplyForcesEventData implementation
        /// @{
    public:
        /// Get the physics shape associated with this event
        virtual I_PhysicsActor& getActor()
        {
            return *m_pShape;
        }

        /// Apply a force to this shape
        virtual void applyForce(const Math::Vector3& _force)
        {
            m_pShape->applyForce(_force);
        }

        /// Apply torque to this shape
        virtual void applyTorque(const Math::Vector3& _torque)
        {
            m_pShape->applyTorque(_torque);
        }

        /// @}

        /// @name 'Structors
        /// @{
    public:
        ApplyForcesEventData(PhysicsActor* _pShape)
        :   m_pShape(_pShape)
        {
        }

        virtual ~ApplyForcesEventData()
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        PhysicsActor*   m_pShape;
        /// @}

    };  // class ApplyForcesEventData;

    void* pBody = NewtonBodyGetUserData(_body);
    if (pBody != NULL)
    {
        PhysicsActor* pPhysicsActor = static_cast<PhysicsActor*>(pBody);

        // Only apply forces if the state is active
        if (pPhysicsActor->m_activationState != 0)
        {
            ApplyForcesEventData eventData(pPhysicsActor);
            eventData.getActor().onApplyForcesEvent(eventData);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::ActivationStateCallback(const NewtonBody* _body, unsigned _state)
{
    void* pBody = NewtonBodyGetUserData(_body);
    if (pBody != NULL)
    {
        static_cast<PhysicsActor*>(pBody)->setActivationState(_state);
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
    NewtonBodyAddForce(m_pActor, _force.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyImpulse(const Math::Vector3& _force, const Math::Vector3& _worldPos)
{
    //todo: bullet needs it's force fed to it in local space coords
     NewtonAddBodyImpulse(m_pActor, _force.m_array, _worldPos.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::applyTorque(const Math::Vector3& _torque)
{
    NewtonBodyAddTorque(m_pActor, _torque.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsActor::setActivationState(unsigned _state)
{
    m_activationState = _state;
    if (m_activationState)
        NewtonWorldUnfreezeBody(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), m_pActor);
    else
        NewtonWorldFreezeBody(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), m_pActor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
PhysicsActor::getActivationState()
{
    m_activationState = NewtonBodyGetSleepingState(m_pActor);
    return m_activationState;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
