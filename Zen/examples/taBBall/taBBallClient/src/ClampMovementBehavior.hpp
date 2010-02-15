//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen taBBall
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_PONG_CLIENT_CLAMP_MOVEMENT_BEHAVIOR_HPP_INCLUDED
#define ZEN_PONG_CLIENT_CLAMP_MOVEMENT_BEHAVIOR_HPP_INCLUDED

#include <Zen/Engine/Core/I_GameObject.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Pong {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ClampMovementBehavior
:   public Engine::Core::I_GameObjectBehaviors::I_PhysicsTransformBehavior
{
    /// @name I_PhysicsTransformBehavior implementation
    /// @{
public:
    virtual bool handlePhysicsTransform(Engine::Core::I_BaseGameObject& _gameObject, Engine::Physics::I_PhysicsShape::I_TransformEventData& _eventData);
    /// @}

    /// @name ClampMovementBehavior implementation
    /// @{
public:
    void setClampX(Math::Real _clampX);
    void setClampY(Math::Real _clampY);
    void setClampZ(Math::Real _clampZ);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ClampMovementBehavior();
    virtual ~ClampMovementBehavior();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Math::Real  m_clampX;
    Math::Real  m_clampY;
    Math::Real  m_clampZ;

    bool        m_isClampedX;
    bool        m_isClampedY;
    bool        m_isClampedZ;
    /// @}

};  // class ClampMovementBehavior

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Pong
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PONG_CLIENT_CLAMP_MOVEMENT_BEHAVIOR_HPP_INCLUDED
