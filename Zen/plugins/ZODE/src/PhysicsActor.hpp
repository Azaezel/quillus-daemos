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
#ifndef ZEN_ZODE_PHYSICS_ACTOR_HPP_INCLUDED
#define ZEN_ZODE_PHYSICS_ACTOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Engine/Physics/I_PhysicsActor.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PhysicsZone;

class PhysicsActor
:   public Engine::Physics::I_PhysicsActor
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsActor>
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

    /// @name I_PhysicsActor implementation
    /// @{
public:
    virtual void setGameObject(pGameObject_type _pGameObject);
    virtual pGameObject_type getGameObject();
    virtual void setCollisionShape(pCollisionShape_type _pCollisionShape);
    virtual pCollisionShape_type getCollisionShape();
    virtual void setMaterial(pPhysicsMaterial_type _material);
    virtual pPhysicsMaterial_type getMaterial();
    virtual wpPhysicsZone_type getPhysicsZone();
    virtual const Zen::Math::Real getMass();
    virtual void setMass(const Zen::Math::Real _mass);
    virtual void setPosition(const Math::Point3& _position);
    virtual const Math::Point3 getPosition();
    virtual void getOrientation(Math::Quaternion4& _orient);
    virtual void setOrientation(const Math::Quaternion4& _orient);
    virtual void getOrientation(Math::Matrix4& _orient);
    virtual void setOrientation(const Math::Matrix4& _orient);
    virtual void setLinearVelocity(const Math::Vector3& _velocity);
    virtual void getLinearVelocity(Math::Vector3& _velocity);
    virtual const Math::Vector3 getAngularVelocity() const;
    virtual void setAngularVelocity(const Math::Vector3& _omega);
    virtual const Math::Vector3 getTorque();
    virtual void applyTorque(const Math::Vector3& _torque);
    virtual void applyForce(const Math::Vector3& _force);
    virtual void applyImpulse(const Math::Vector3& _force, const Math::Vector3& _worldPos);
    virtual const Math::Vector3 getAngularDamping() const;
    virtual void setAngularDamping(const Math::Vector3& _damping);
    virtual const Math::Real getLinearDamping() const;
    virtual void setLinearDamping(float _damping);
    virtual void setSleepingThresholds(float _minLinearMotion, float _minAngularMotion);
    virtual void setActivationState(unsigned _state);
    virtual int getActivationState();
    virtual void setCollisionGroup(const int _group);
    virtual const int getCollisionGroup() const;
    /// @}

    /// @name PhysicsActor implementation
    /// @{
public:
    PhysicsZone& getParentZone();
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsActor(PhysicsZone& _parentZone);
    virtual ~PhysicsActor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    PhysicsZone&            m_parentZone;
    /// @}

};  // class PhysicsActor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_PHYSICS_ACTOR_HPP_INCLUDED
