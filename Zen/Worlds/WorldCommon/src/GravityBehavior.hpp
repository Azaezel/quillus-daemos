//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Worlds
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not use or redistribute this source code without explicit permission 
// from Tony Richards.
//
// Tony Richards sgtflame@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_WORLDS_GRAVITY_BEHAVIOR_HPP_INCLUDED
#define ZEN_WORLDS_GRAVITY_BEHAVIOR_HPP_INCLUDED

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class GravityBehavior
:   public Zen::Engine::Core::I_GameObjectBehaviors::I_PhysicsForceBehavior
{
    /// @name I_PhysicsTransformBehavior implementation
    /// @{
public:
    virtual bool handlePhysicsForce(Zen::Engine::Core::I_GameObject& _gameObject, Zen::Engine::Physics::I_PhysicsShape::I_ApplyForcesEventData& _eventData);
    /// @}

    /// @name GravityBehavior implementation
    /// @{
public:
    /// Set a constant velocity in a given direction
    void setGravity(const Zen::Math::Vector3& _vector);
    /// @}

    /// @name 'Structors
    /// @{
public:
             GravityBehavior();
    virtual ~GravityBehavior();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Math::Vector3   m_gravity;
    /// @}

};  // class GravityBehavior

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_WORLDS_GRAVITY_BEHAVIOR_HPP_INCLUDED
