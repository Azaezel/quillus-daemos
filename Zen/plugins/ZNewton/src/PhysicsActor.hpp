//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_ZNEWTON_PHYSICS_ACTOR_HPP_INCLUDED
#define ZEN_ZNEWTON_PHYSICS_ACTOR_HPP_INCLUDED

// TR - Newton.h should include stdlib.h but it doesn't.
#include <stdlib.h>
#include <Newton.h>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Matrix4.hpp>

#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Engine/Core/I_GameObject.hpp>

#include <map>

#include "CollisionShape.hpp"
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZNewton {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsZone;
class PhysicsActor
:   public Engine::Physics::I_PhysicsActor
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsActor>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsZone> wpPhysicsZone_type;
    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsZone>      pPhysicsZone_type;
	typedef Memory::managed_ptr<Zen::Engine::Core::I_GameObject>          pOwningObject_type;
    typedef Memory::managed_ptr<Zen::Engine::Physics::I_CollisionShape>   pCollisionShape_type;
    
    /// @}

    /// @name I_PhysicsActor implementation
    /// @{
public:
    virtual wpPhysicsZone_type getPhysicsZone();
    virtual void setCollisionShape(pCollisionShape_type _pCollisionShape);
    virtual pCollisionShape_type getCollisionShape();
    virtual void setMaterial(pPhysicsMaterial_type _material);
    virtual pPhysicsMaterial_type getMaterial();
    virtual const std::string& getName();
    virtual void setName(const std::string& _name);

    virtual const Math::Real getMass();
    virtual void setMass(const Math::Real _mass);
    virtual void setScale(const float _x,const float _y,const float _z);
    virtual void setPosition(const Math::Point3& _pos);
    virtual const Math::Point3 getPosition();
    virtual void getOrientation(Math::Quaternion4& _orient);
    virtual void setOrientation(const Math::Quaternion4& _orient);
    virtual void getOrientation(Math::Matrix4& _orient);
    virtual void setOrientation(const Math::Matrix4& _orient);
    virtual void setLinearVelocity(const Math::Vector3& _velocity);
    virtual void getLinearVelocity(Math::Vector3& _velocity);
    virtual const Math::Vector3 getTorque();
    virtual const Math::Vector3 getAngularVelocity() const;
    virtual void setAngularVelocity(const Math::Vector3& _omega);
    virtual const Math::Vector3 getAngularDamping() const;
    virtual void setAngularDamping(const Math::Vector3& _damping);
    virtual const Math::Real getLinearDamping() const;
    virtual void setLinearDamping(float _damping);
    virtual void setAutoFreeze(bool _bFreeze);

    virtual void setAdvancedCollisionPrediction(bool _mode);
    virtual bool getAdvancedCollisionPrediction() const;
    
	virtual void setCollisionGroup(const int _group);
	virtual const int getCollisionGroup() const;

	virtual void SetStatic(bool _isStatic);
	virtual Scripting::I_ObjectReference* getScriptObject();

	virtual void setGameObject(pOwningObject_type _pOwner);
	virtual pOwningObject_type getGameObject();
    bool attachBody(pCollisionShape_type _collision);

    /// @}

    /// @name PhysicsActor implementation
    /// @{
private:
    static void TransformCallback(const NewtonBody* _body, const dFloat* _matrix);
    static void ApplyForceAndTorqueCallback(const NewtonBody* _pBody);
    static void ActivationStateCallback(const NewtonBody* body, unsigned state);

    void applyForce(const Math::Vector3& _force);
    void applyTorque(const Math::Vector3& _torque);

    void setActivationState(unsigned _state);
    /// @}
public:
    /// @ For Internal Newton usage
    /// @{
    NewtonBody* getActorPtr();
    void setActorPtr(NewtonBody* _pActor);
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsActor(wpPhysicsZone_type _zone);
    virtual ~PhysicsActor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    NewtonBody*                             m_pActor;
    wpPhysicsZone_type                      m_pZone;
    pPhysicsMaterial_type                   m_material;
    dFloat                                  m_scaleX, m_scaleY, m_scaleZ;
    Scripting::I_ObjectReference*           m_pScriptObject;
    std::string                             m_name;
    Zen::Math::Real							     m_minStrikeLen;
    unsigned                                m_activationState;
    int										        m_collisionGroup;
    Math::Vector3                           m_torque;
	 pOwningObject_type                      m_pOwner;
    pCollisionShape_type                    m_CollisionShape;

    /// @}

    /// @name Inner Classes
    /// @{
public:
    class TransformEventData
    :   public I_TransformEventData
    {
    public:
        virtual I_PhysicsActor& getActor();
        virtual const Math::Matrix4& getTransform() const;

    public:
        TransformEventData(Engine::Physics::I_PhysicsActor& _shape, Math::Matrix4& _transform);

    private:
        Engine::Physics::I_PhysicsActor&        m_actor;
        Math::Matrix4&                          m_transform;

    };  // interface I_TransformEventData

    class CollisionEventData
    :   public I_CollisionEventData
    {
    public:
        virtual I_PhysicsActor& getActor();
        virtual I_PhysicsActor& getOtherActor();

    public:

        CollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_PhysicsActor& _shape, Engine::Physics::I_PhysicsActor& _otherShape);

    protected:
        const NewtonMaterial*                   m_pMaterial;
        Engine::Physics::I_PhysicsActor&        m_actor;
        Engine::Physics::I_PhysicsActor&		m_otherActor;

    };  // class CollisionEventData

    class BeginCollisionEventData
    :   public CollisionEventData
    ,   public I_BeginCollisionEventData
    {
    public:
        virtual I_PhysicsActor& getActor() { return CollisionEventData::getActor(); }
        virtual I_PhysicsActor& getOtherActor() { return CollisionEventData::getOtherActor(); }
        virtual void ignoreCollision() { m_ignoreCollision = true; }

    public:
        bool getIgnoreCollision() const { return m_ignoreCollision; }

    public:
        BeginCollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor);

    private:
        bool        m_ignoreCollision;

    };  // class BeginCollisionEventData

    class DuringCollisionEventData
    :   public CollisionEventData
    ,   public I_DuringCollisionEventData
    {
    public:
        virtual I_PhysicsActor& getActor() { return CollisionEventData::getActor(); }
        virtual I_PhysicsActor& getOtherActor() { return CollisionEventData::getOtherActor(); }

        virtual void disableContact() const
        {
            NewtonMaterialDisableContact(m_pMaterial);
        }

        virtual Math::Real getContactSpeed() const
        {
            return NewtonMaterialGetContactNormalSpeed(m_pMaterial, m_pContact);
        }

        virtual void getContactForce(Math::Vector3& _forceVector) const
        {
            NewtonMaterialGetContactForce(m_pMaterial, _forceVector.m_array);
        }

        virtual void getContactPosition(Math::Point3& _contactPosition) const
        {
            Math::Vector3 contactNormal;
            NewtonMaterialGetContactPositionAndNormal(m_pMaterial, _contactPosition.m_array, contactNormal.m_array);
        }

        virtual void getContactNormal(Math::Vector3& _contactNormal) const
        {
            Math::Point3 contactPosition;
            NewtonMaterialGetContactPositionAndNormal(m_pMaterial, contactPosition.m_array, _contactNormal.m_array);
        }

        virtual void getContactPositionAndNormal(Math::Point3& _contactPosition, Math::Vector3& _contactNormal) const
        {
            NewtonMaterialGetContactPositionAndNormal(m_pMaterial, _contactPosition.m_array, _contactNormal.m_array);
        }

        virtual void getContactTangentDirections(Math::Vector3& _direction0, Math::Vector3& _direction1) const
        {
            NewtonMaterialGetContactTangentDirections(m_pMaterial, _direction0.m_array, _direction1.m_array);
        }

        virtual Math::Real getContactTangentSpeed(bool _primary = true) const
        {
            return NewtonMaterialGetContactTangentSpeed(m_pMaterial, m_pContact, _primary ? 0 : 1);
        }

        virtual void setContactRestitution(Math::Real _restitution)
        {
            NewtonMaterialSetContactElasticity(m_pMaterial, _restitution);
        }

        virtual void setContactFrictionState(int _state, bool _primary = true)
        {
            NewtonMaterialSetContactFrictionState(m_pMaterial, _state, _primary ? 0 : 1);
        }

        virtual void setStaticFrictionCoefficient(Math::Real _coefficent, bool _primary = true)
        {
            NewtonMaterialSetContactStaticFrictionCoef(m_pMaterial, _coefficent, _primary ? 0 : 1);
        }

        virtual void setKineticFrictionCoefficient(Math::Real _coefficent, bool _primary = true)
        {
            NewtonMaterialSetContactKineticFrictionCoef(m_pMaterial, _coefficent, _primary ? 0 : 1);
        }

        virtual void setContactTangentAcceleration(Math::Real _acceleration, bool _primary = true)
        {
            NewtonMaterialSetContactTangentAcceleration(m_pMaterial, _acceleration, _primary ? 0 : 1);
        }

        virtual void setContactTangentVector(Math::Vector3 _tangent)
        {
            NewtonMaterialContactRotateTangentDirections(m_pMaterial, _tangent.m_array);
        }

    public:

        DuringCollisionEventData(const NewtonMaterial* _pMaterial, const NewtonContact* _contact, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor);

    private:
        const NewtonContact*        m_pContact;
    };  // class DuringCollisionEventData


    class EndCollisionEventData
    :   public CollisionEventData
    ,   public I_EndCollisionEventData
    {
    public:
        virtual I_PhysicsActor& getActor() { return CollisionEventData::getActor(); }
        virtual I_PhysicsActor& getOtherActor() { return CollisionEventData::getOtherActor(); }

    public:

        EndCollisionEventData(const NewtonMaterial* _material, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor);

    private:

    };  // class EndCollisionEventData

};  // class PhysicsActor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::TransformEventData::TransformEventData(I_PhysicsActor& _actor, Math::Matrix4& _transform)
:   m_actor(_actor)
,   m_transform(_transform)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::CollisionEventData::CollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   m_pMaterial(_pMaterial)
,   m_actor(_actor)
,   m_otherActor(_otherActor)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::BeginCollisionEventData::BeginCollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   CollisionEventData(_pMaterial, _actor, _otherActor)
,   m_ignoreCollision(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::DuringCollisionEventData::DuringCollisionEventData(const NewtonMaterial* _pMaterial, const NewtonContact* _pContact, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   CollisionEventData(_pMaterial, _actor, _otherActor)
,   m_pContact(_pContact)
{
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::EndCollisionEventData::EndCollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   CollisionEventData(_pMaterial, _actor, _otherActor)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZNEWTON_PHYSICS_ACTOR_HPP_INCLUDED
