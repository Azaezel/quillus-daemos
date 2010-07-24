//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_ZBULLET_PHYSICS_ACTOR_HPP_INCLUDED
#define ZEN_ZBULLET_PHYSICS_ACTOR_HPP_INCLUDED

// Reference Note: http://www.bulletphysics.com/Bullet/BulletFull/classbtRigidBody.html
// TR - Newton.h should include stdlib.h but it doesn't.
#include <stdlib.h>
#include <btBulletDynamicsCommon.h>

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
namespace ZBullet {
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
    virtual void setSleepingThresholds(float _minLinearMotion, float _minAngularMotion);
    virtual void setActivationState(unsigned _state);
    virtual int getActivationState();

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
    void applyForce(const Math::Vector3& _force);
    void applyImpulse(const Math::Vector3& _force, const Math::Vector3& _worldPos);

    void applyTorque(const Math::Vector3& _torque);
    /// @}
public:
    /// @ For Internal Bullet usage
    /// @{
    btRigidBody* getActorPtr();
    void setActorPtr(btRigidBody* _pActor);
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
    btRigidBody*                            m_pActor;
    wpPhysicsZone_type                      m_pZone;
    pPhysicsMaterial_type                   m_material;
    Zen::Math::Real                         m_scaleX, m_scaleY, m_scaleZ;
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

        CollisionEventData(Engine::Physics::I_PhysicsActor& _shape, Engine::Physics::I_PhysicsActor& _otherShape);

    protected:
        Engine::Physics::I_PhysicsMaterial*     m_pMaterial;
        Engine::Physics::I_PhysicsActor&        m_actor;
        Engine::Physics::I_PhysicsActor&        m_otherActor;

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
        BeginCollisionEventData(Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor);

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
            //TODO - Re-evaluate the need for this at all, since pre-collsion returns a boolean for just this purpose
        }

        virtual Math::Real getContactSpeed() const
        {
            return m_pContact->getAppliedImpulse();
        }

        virtual void getContactForce(Math::Vector3& _forceVector) const
        {
            btVector3 tquat = m_pContact->m_normalWorldOnB;
            tquat *= m_pContact->getAppliedImpulse();
            _forceVector.m_x = tquat.x();
            _forceVector.m_y = tquat.y();
            _forceVector.m_z = tquat.z();
        }

        virtual void getContactPosition(Math::Point3& _contactPosition) const
        {
            btVector3 tquat = m_pContact->getPositionWorldOnB();
            _contactPosition.m_x = tquat.x();
            _contactPosition.m_y = tquat.y();
            _contactPosition.m_z = tquat.z();
        }

        virtual void getContactNormal(Math::Vector3& _contactNormal) const
        {
            btVector3 tquat = m_pContact->m_normalWorldOnB;
            _contactNormal.m_x = tquat.x();
            _contactNormal.m_y = tquat.y();
            _contactNormal.m_z = tquat.z();
        }

        virtual void getContactPositionAndNormal(Math::Point3& _contactPosition, Math::Vector3& _contactNormal) const
        {
            getContactPosition(_contactPosition);
            getContactNormal(_contactNormal);
        }

        //from conversations, the later intended usage of this will be to temporarily set a given restitution just during collisions
        virtual void setContactRestitution(Math::Real _restitution)
        {
            //TODO: resolve - illegal call of non-static member function
            //getActorPtr()->setRestitution(_restitution);
        }

        virtual void resetContactRestitution()
        {
            //for clearing modifications made to actor restitutions at collision-time
            //mebbey we should auto-run this after the callbacks fired?
            //TODO: resolve - illegal call of non-static member function
            //getActorPtr()->setRestitution(getActor().getMaterial()->getRestitution()); 
        }

        virtual void setContactFrictionState(int _state, bool _primary = true)
        {
            //TODO: resolve -
            //if getActor().getMaterial()->getStaticFriction() > 0;
            //getActorPtr()->setFriction(getActor().getMaterial()->getStaticFriction());
        }

        virtual void setStaticFrictionCoefficient(Math::Real _coefficent, bool _primary = true)
        {
            //TODO: resolve -
            //if getActor().getMaterial()->getStaticFriction() > 0;
            //getActorPtr()->setFriction(getActor().getMaterial()->getStaticFriction());
        }

        virtual void setKineticFrictionCoefficient(Math::Real _coefficent, bool _primary = true)
        {
            //TODO: resolve -
            //if getActor().getMaterial()->getStaticFriction() > 0;
            //getActorPtr()->setFriction(getActor().getMaterial()->getDynamicFriction());
        }

        virtual void getContactTangentDirections(Math::Vector3& _direction0, Math::Vector3& _direction1) const
        {
            //TODO: calculate tangents as a function of the contact normal as it relates to the baseline orientation, perhapse?
            //in the newton plugin, this seems to be intended for use with fluid dynamics.
            _direction0.ones();
            _direction1.ones();
        }

        virtual Math::Real getContactTangentSpeed(bool _primary = true) const
        {
            //TODO: calculate tangents as a function of the contact normal as it relates to the baseline orientation, perhapse?
            //in the newton plugin, this seems to be intended for use with fluid dynamics.
            return 1.0f;
        }

        virtual void setContactTangentAcceleration(Math::Real _acceleration, bool _primary = true)
        {
            //TODO: calculate tangents as a function of the contact normal as it relates to the baseline orientation, perhapse?
            //in the newton plugin, this seems to be intended for use with fluid dynamics.
            _acceleration = 1.0f;
            //NewtonMaterialSetContactTangentAcceleration(m_pMaterial, _acceleration, _primary ? 0 : 1);
        }

        virtual void setContactTangentVector(Math::Vector3 _tangent)
        {
            //TODO: calculate tangents as a function of the contact normal as it relates to the baseline orientation, perhapse?
            //in the newton plugin, this seems to be intended for use with fluid dynamics. (specificly orienting the colliding body along the tangent vector, like being swept downriver)
            _tangent = _tangent.ones();
        }

    public:

        DuringCollisionEventData(const btManifoldPoint* _contact, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor);

    private:
        const btManifoldPoint*        m_pContact;
    };  // class DuringCollisionEventData


    class EndCollisionEventData
    :   public CollisionEventData
    ,   public I_EndCollisionEventData
    {
    public:
        virtual I_PhysicsActor& getActor() { return CollisionEventData::getActor(); }
        virtual I_PhysicsActor& getOtherActor() { return CollisionEventData::getOtherActor(); }

    public:

        EndCollisionEventData(Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor);

    private:

    };  // class EndCollisionEventData
    public:
        //copied from the bullet headers:
        //The btMotionState interface class allows the dynamics world to synchronize and interpolate the updated world transforms with graphics
        //For optimizations, potentially only moving objects get synchronized (using setWorldPosition/setWorldOrientation)
        class MotionState : public btMotionState
        {
        public:
            MotionState(void* _userData);
            virtual ~MotionState();
            virtual void getWorldTransform(btTransform& _worldTrans) const;
            //Bullet only calls the update of worldtransform for active objects
            virtual void setWorldTransform(const btTransform& _worldTrans);
            void*		m_userPointer;
        };
    /// @name Extended Member Variables
    /// @{
    MotionState * m_MotionState;
    /// @}
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
PhysicsActor::MotionState::MotionState(void* _userData)
: m_userPointer(_userData)
{

}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::MotionState::~MotionState()
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::CollisionEventData::CollisionEventData(Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   m_actor(_actor)
,   m_otherActor(_otherActor)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::BeginCollisionEventData::BeginCollisionEventData(Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   CollisionEventData(_actor, _otherActor)
,   m_ignoreCollision(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::DuringCollisionEventData::DuringCollisionEventData(const btManifoldPoint* _pContact, Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   CollisionEventData(_actor, _otherActor)
,   m_pContact(_pContact)
{
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
PhysicsActor::EndCollisionEventData::EndCollisionEventData(Engine::Physics::I_PhysicsActor& _actor, Engine::Physics::I_PhysicsActor& _otherActor)
:   CollisionEventData(_actor, _otherActor)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZBULLET_PHYSICS_ACTOR_HPP_INCLUDED
