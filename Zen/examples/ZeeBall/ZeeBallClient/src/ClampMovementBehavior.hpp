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
#ifndef ZEN_CLIENT_CLAMP_MOVEMENT_BEHAVIOR_HPP_INCLUDED
#define ZEN_CLIENT_CLAMP_MOVEMENT_BEHAVIOR_HPP_INCLUDED

#include <Zen/Engine/Core/I_GameObject.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZeeBall {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ClampMovementBehavior
:   public Zen::Engine::Core::I_GameObjectBehaviors::I_PhysicsTransformBehavior
{
    /// @name I_PhysicsTransformBehavior implementation
    /// @{
public:
    virtual bool handlePhysicsTransform(Zen::Engine::Core::I_GameObject& _gameObject, Zen::Engine::Physics::I_PhysicsShape::I_TransformEventData& _eventData);
    /// @}

    /// @name ClampMovementBehavior implementation
    /// @{
public:
    void setClampX(Zen::Math::Real _clampX);
    void setClampY(Zen::Math::Real _clampY);
    void setClampZ(Zen::Math::Real _clampZ);
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
    Zen::Math::Real  m_clampX;
    Zen::Math::Real  m_clampY;
    Zen::Math::Real  m_clampZ;

    bool        m_isClampedX;
    bool        m_isClampedY;
    bool        m_isClampedZ;
    /// @}

};  // class ClampMovementBehavior

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace ZeeBall
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_CLIENT_CLAMP_MOVEMENT_BEHAVIOR_HPP_INCLUDED
