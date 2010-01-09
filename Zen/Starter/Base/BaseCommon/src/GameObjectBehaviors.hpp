//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_ENGINE_BASE_GAME_OBJECT_BEHAVIORS_HPP_INCLUDED
#define ZEN_ENGINE_BASE_GAME_OBJECT_BEHAVIORS_HPP_INCLUDED

#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class GameObjectBehaviors
:   public Core::I_GameObjectBehaviors
{
    /// @name Types
    /// @{
public:
    typedef std::list<I_PhysicsTransformBehavior*>  PhysicsTransformBehaviors_type;
    typedef std::list<I_PhysicsForceBehavior*>      PhysicsForceBehaviors_type;
    typedef std::list<I_CollisionBehavior*>			CollisionBehavior_type;


    /// @}

    /// @name I_GameObjectBehaviors implementation
    /// @{
public:
    virtual void setScaleBehavior(ScaleBehavior_type _behavior);
    virtual void push_front(I_PhysicsTransformBehavior& _behavior);
    virtual void push_back(I_PhysicsTransformBehavior& _behavior);
    virtual void remove(I_PhysicsTransformBehavior& _behavior);
    virtual void push_front(I_PhysicsForceBehavior& _behavior);
    virtual void push_back(I_PhysicsForceBehavior& _behavior);
    virtual void remove(I_PhysicsForceBehavior& _behavior);
	//collisions
	virtual void push_front(I_CollisionBehavior& _behavior);
    virtual void push_back(I_CollisionBehavior& _behavior);
    virtual void remove(I_CollisionBehavior& _behavior);
    /// @}

    /// @name Behavior implementation
    /// @{
public:
    void setScale(Core::I_BaseGameObject& _object, float _x, float _y, float _z);
    void handlePhysicsTransform(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_TransformEventData& _eventData);
    void handlePhysicsForce(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_ApplyForcesEventData& _eventData);
	bool handleBoundBoxCollision(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_BeginCollisionEventData& _eventData);
    void handleCollision(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_DuringCollisionEventData& _eventData);
    void handleCollisionResolution(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_EndCollisionEventData& _eventData);

    /// @}

    /// @name Default Behavior implementation
    /// @{
public:
    static void defaultSetScale(Core::I_BaseGameObject& _object, float _x, float _y, float _z);
    /// @}

    /// @name 'Structors
    /// @{
public:
             GameObjectBehaviors(const GameObjectBehaviors* const _pTemplate);
             GameObjectBehaviors();
    virtual ~GameObjectBehaviors();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ScaleBehavior_type  m_scaleBehavior;
    PhysicsTransformBehaviors_type  m_physicsTransformBehaviors;
    PhysicsForceBehaviors_type      m_physicsForceBehaviors;
	CollisionBehavior_type      m_collisionBehaviors;

    /// @}

};  // class GameObjectBehaviors

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_BASE_GAME_OBJECT_BEHAVIORS_HPP_INCLUDED
