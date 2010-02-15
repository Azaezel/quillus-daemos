//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ENGINE_CORE_I_GAME_OBJECT_BEHAVIORS_HPP_INCLUDED
#define ZEN_ENGINE_CORE_I_GAME_OBJECT_BEHAVIORS_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <boost/function.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Core {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_BaseGameObject;

/// @brief Game Object interface
class GAMECORE_DLL_LINK I_GameObjectBehaviors
{
    /// @name Types
    /// @{
public:
    typedef boost::function<void(I_BaseGameObject&, float, float, float)>   ScaleBehavior_type;
    // Defined below as an Inner Class
    class I_PhysicsTransformBehavior;
    class I_PhysicsForceBehavior;
    class I_BoundBoxCollisionBehavior;
    class I_CollisionBehavior;
    /// @}

    /// @name I_GameObjectBehaviors
    /// @todo Most I_BaseGameObject setXXX methods should have overridable behaviors.
    /// @{
public:
    /// Override the setScale behavior
    virtual void setScaleBehavior(ScaleBehavior_type _behavior) = 0;
    /// @}

    /// @name Physics Transform Behaviors
    /// @{
public:
    virtual void push_front(I_PhysicsTransformBehavior& _behavior) = 0;
    virtual void push_back(I_PhysicsTransformBehavior& _behavior) = 0;
    virtual void remove(I_PhysicsTransformBehavior& _behavior) = 0;
    /// @}

    /// @name Physics Force Behaviors
    /// @{
public:
    virtual void push_front(I_PhysicsForceBehavior& _behavior) = 0;
    virtual void push_back(I_PhysicsForceBehavior& _behavior) = 0;
    virtual void remove(I_PhysicsForceBehavior& _behavior) = 0;
    /// @}

    /// @name Collision Behaviors
    /// @{
public:
    virtual void push_front(I_CollisionBehavior& _behavior) = 0;
    virtual void push_back(I_CollisionBehavior& _behavior) = 0;
    virtual void remove(I_CollisionBehavior& _behavior) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameObjectBehaviors();
    virtual ~I_GameObjectBehaviors();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    /// Root I_GameObjectBehavior interface
    class I_GameObjectBehavior
    {
    public:
                 I_GameObjectBehavior();
        virtual ~I_GameObjectBehavior();
    };  // interface I_GameObjectBehavior

    /// Implement this class to override the physics transform handling of
    /// a class of game object(s) or game group(s).
    class GAMECORE_DLL_LINK I_PhysicsTransformBehavior
    {
    public:
        ///  @brief Handle the physics transform event.
        /// 
        /// This event handler is a chained event handler.  Each handler gets an
        /// opportunity to modify the physics behavior, but event handling stops
        /// for this frame as soon as a behavior returns false for this method.
        /// Normally this method should return true unless the behavior needs to abort the physics event handling.
        virtual bool handlePhysicsTransform(I_BaseGameObject& _gameObject, Physics::I_PhysicsActor::I_TransformEventData& _eventData) = 0;
    protected:
                 I_PhysicsTransformBehavior();
        virtual ~I_PhysicsTransformBehavior();
    };  // interface I_PhysicsTransformBehavior

    /// Implement this class to override the physics force handling of
    /// a class of game object(s) or game group(s).
    class GAMECORE_DLL_LINK I_PhysicsForceBehavior
    {
    public:
        ///  @brief Handle the physics force event.
        /// 
        /// This event handler is a chained event handler.  Each handler gets an
        /// opportunity to modify the physics forces, but event handling stops
        /// for this frame as soon as a behavior returns false for this method.
        /// Normally this method should return true unless the behavior should stop processing.
        virtual bool handlePhysicsForce(I_BaseGameObject& _gameObject, Physics::I_PhysicsActor::I_ApplyForcesEventData& _eventData) = 0;
    protected:
                 I_PhysicsForceBehavior();
        virtual ~I_PhysicsForceBehavior();
    };  // interface I_PhysicsForceBehavior

    /// Implement this class to override the physics transform handling of
    /// a class of game object(s) or game group(s).
    class GAMECORE_DLL_LINK I_CollisionBehavior
    {
    public:
        /// Handle the initial physics collision event.
        /// 
        /// This method is called when the AABB of two objects intersect.  Use this callback to 
        /// abort (or manually perform) collision detection.
        ///
        /// This event handler is a chained event handler.  Each handler gets an
        /// opportunity to handle collision, but event handling stops
        /// for this frame as soon as a behavior returns false for this method.
        /// Normally this method should return true unless the behavior should stop processing.
        ///
        /// @return Normally this method should return true unless the behavior needs to abort the physics event handling.
        virtual bool handleBoundBoxCollision(I_BaseGameObject& _gameObject, Physics::I_PhysicsActor::I_BeginCollisionEventData& _eventData) = 0;

        /// Handle the collision for an object.
        ///
        /// This event handler is a chained event handler.  Each handler gets an
        /// opportunity to modify the physics forces, but event handling stops
        /// for this frame as soon as a behavior returns false for this method.
        /// Normally this method should return true unless the behavior should stop processing.
        ///
        /// For every sequence of:
        ///     1) 1 handleBoundBoxCollision()
        ///     2) zero or more handleCollision()
        ///     3) 1 handleCollisionResolution()
        ///
        /// There can be more than one calls to handleCollision() because it is called once for each contact between the two physics shapes.
        ///
        /// This method is called for every collision that occurs between the two aforementioned _shape and _otherShape in
        /// handleBoundBoxCollision().  If no collisions occur, this method won't be called.  Use this method to override
        /// the results of the collision.
        virtual bool handleCollision(I_BaseGameObject& _gameObject, Physics::I_PhysicsActor::I_DuringCollisionEventData& _eventData) = 0;

        /// This is called after all of the collisions for two objects have been handled.
		virtual void handleCollisionResolution(I_BaseGameObject& _gameObject, Physics::I_PhysicsActor::I_EndCollisionEventData& _eventData) = 0; 

    protected:
                 I_CollisionBehavior();
        virtual ~I_CollisionBehavior();
    };  // interface I_BoundBoxCollisionBehavior

    /// @}

};  // interface I_GameObjectBehaviors

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Core
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CORE_I_GAME_OBJECT_BEHAVIORS_HPP_INCLUDED
