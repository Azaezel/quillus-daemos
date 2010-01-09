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

#include "GameObjectBehaviors.hpp"

#include <Zen/Engine/Core/I_GameObject.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectBehaviors::GameObjectBehaviors()
{
    m_scaleBehavior = &GameObjectBehaviors::defaultSetScale;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectBehaviors::GameObjectBehaviors(const GameObjectBehaviors* _pTemplate)
{
    // Copy the scale behavior to this object
    m_scaleBehavior = _pTemplate->m_scaleBehavior;

    // Shallow copy the physics transform behaviors
    for(PhysicsTransformBehaviors_type::const_iterator iter = _pTemplate->m_physicsTransformBehaviors.begin(); iter != _pTemplate->m_physicsTransformBehaviors.end(); iter++)
    {
        m_physicsTransformBehaviors.push_back(*iter);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectBehaviors::~GameObjectBehaviors()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::setScaleBehavior(ScaleBehavior_type _behavior)
{
    m_scaleBehavior = _behavior;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::push_front(I_PhysicsTransformBehavior& _behavior)
{
    m_physicsTransformBehaviors.push_front(&_behavior);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::push_back(I_PhysicsTransformBehavior& _behavior)
{
    m_physicsTransformBehaviors.push_back(&_behavior);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::remove(I_PhysicsTransformBehavior& _behavior)
{
    for(PhysicsTransformBehaviors_type::iterator iter = m_physicsTransformBehaviors.begin(); iter != m_physicsTransformBehaviors.end(); iter++)
    {
        if (*iter == &_behavior)
        {
            m_physicsTransformBehaviors.erase(iter);
            return;
        }
    }

    // TODO Error?  Didn't find the behavior.
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::push_front(I_PhysicsForceBehavior& _behavior)
{
    m_physicsForceBehaviors.push_front(&_behavior);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::push_back(I_PhysicsForceBehavior& _behavior)
{
    m_physicsForceBehaviors.push_back(&_behavior);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::remove(I_PhysicsForceBehavior& _behavior)
{
    for(PhysicsForceBehaviors_type::iterator iter = m_physicsForceBehaviors.begin(); iter != m_physicsForceBehaviors.end(); iter++)
    {
        if (*iter == &_behavior)
        {
            m_physicsForceBehaviors.erase(iter);
            return;
        }
    }

    // TODO Error?  Didn't find the behavior.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::push_front(I_CollisionBehavior& _behavior)
{
    m_collisionBehaviors.push_front(&_behavior);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::push_back(I_CollisionBehavior& _behavior)
{
    m_collisionBehaviors.push_back(&_behavior);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::remove(I_CollisionBehavior& _behavior)
{
    for(CollisionBehavior_type::iterator iter = m_collisionBehaviors.begin(); iter != m_collisionBehaviors.end(); iter++)
    {
        if (*iter == &_behavior)
        {
            m_collisionBehaviors.erase(iter);
            return;
        }
    }

    // TODO Error?  Didn't find the behavior.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::setScale(Core::I_BaseGameObject& _object, float _x, float _y, float _z)
{
    m_scaleBehavior(_object, _x, _y, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::handlePhysicsTransform(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_TransformEventData& _eventData)
{
    for(PhysicsTransformBehaviors_type::iterator iter = m_physicsTransformBehaviors.begin(); iter != m_physicsTransformBehaviors.end(); iter++)
    {
        if ((*iter)->handlePhysicsTransform(_gameObject, _eventData) == false)
            return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::handlePhysicsForce(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_ApplyForcesEventData& _eventData)
{
    for(PhysicsForceBehaviors_type::iterator iter = m_physicsForceBehaviors.begin(); iter != m_physicsForceBehaviors.end(); iter++)
    {
        if ((*iter)->handlePhysicsForce(_gameObject, _eventData) == false)
            return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameObjectBehaviors::handleBoundBoxCollision(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_BeginCollisionEventData& _eventData)
{
    for(CollisionBehavior_type::iterator iter = m_collisionBehaviors.begin(); iter != m_collisionBehaviors.end(); iter++)
    {
        if ((*iter)->handleBoundBoxCollision(_gameObject, _eventData) == false)
            return false;
    }
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::handleCollision(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_DuringCollisionEventData& _eventData)
{
    for(CollisionBehavior_type::iterator iter = m_collisionBehaviors.begin(); iter != m_collisionBehaviors.end(); iter++)
    {
        if ((*iter)->handleCollision(_gameObject, _eventData) == false)
            return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::handleCollisionResolution(Core::I_BaseGameObject& _gameObject, Physics::I_CollisionShape::I_EndCollisionEventData& _eventData)
{
    for(CollisionBehavior_type::iterator iter = m_collisionBehaviors.begin(); iter != m_collisionBehaviors.end(); iter++)
    {
        (*iter)->handleCollisionResolution(_gameObject, _eventData);
    }
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectBehaviors::defaultSetScale(Core::I_BaseGameObject& _object, float _x, float _y, float _z)
{
    _object.setScale(_x, _y, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
