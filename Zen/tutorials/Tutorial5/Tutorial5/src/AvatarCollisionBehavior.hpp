//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
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
// This project is part of the Zen Engine Tutorials
// 
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMECLIENT_AVATARCOLLISION_BEHAVIOR_HPP_INCLUDED
#define GAMECLIENT_AVATARCOLLISION_BEHAVIOR_HPP_INCLUDED

#include <Zen/Engine/Core/I_GameObject.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class AvatarCollisionBehavior
:   public Zen::Engine::Core::I_GameObjectBehaviors::I_CollisionBehavior
{
    /// @name I_CollisionBehavior implementation
    /// @{
public:
    virtual bool handleBoundBoxCollision(Zen::Engine::Core::I_BaseGameObject& _gameObject, Zen::Engine::Physics::I_CollisionShape::I_BeginCollisionEventData& _eventData);
    virtual bool handleCollision(Zen::Engine::Core::I_BaseGameObject& _gameObject, Zen::Engine::Physics::I_CollisionShape::I_DuringCollisionEventData& _eventData);
	virtual void handleCollisionResolution(Zen::Engine::Core::I_BaseGameObject& _gameObject, Zen::Engine::Physics::I_CollisionShape::I_EndCollisionEventData& _eventData); 
	/// @}

    /// @name AvatarCollisionBehavior implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             AvatarCollisionBehavior();
    virtual ~AvatarCollisionBehavior();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

	Zen::Math::Vector3			m_origionalAngularMomentum;

};  // class AvatarCollisionBehavior

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMECLIENT_AVATARCOLLISION_BEHAVIOR_HPP_INCLUDED