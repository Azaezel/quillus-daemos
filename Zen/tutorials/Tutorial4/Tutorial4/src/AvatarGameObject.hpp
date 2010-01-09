//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
//
// Copyright (C) 2001 - 2008 Tony Richards
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This project is part of the Zen Engine Tutorials
//
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMECLIENT_AVATAR_GAME_OBJECT_HPP_INCLUDED
#define GAMECLIENT_AVATAR_GAME_OBJECT_HPP_INCLUDED

#include "GameObject.hpp"

#include <Zen/Starter/Base/BaseCommon/I_BaseGameObject.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Engine/Physics/I_PhysicsJoint.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class AvatarGameObject
:   public GameObject
{
    /// @name Types
    /// @{
public:
    typedef Zen::Engine::Physics::I_PhysicsZone::pPhysicsMaterial_type     pPhysicsMaterial_type;
    /// @}

    /// @name AvatarGameObject implementation
    /// @{
public:
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             AvatarGameObject(Zen::Engine::Core::I_GameGroup& _group, const std::string& _name);
    virtual ~AvatarGameObject();
    /// @}

	/// @name Member variables
    /// @{
	Zen::Math::Real												m_moveSpeed;
	Zen::Math::Real												m_stepHeight;
	Zen::Math::Real												m_turnSpeed;
	Zen::Math::Quaternion4										m_orientation;
private:
    Zen::Engine::Physics::I_PhysicsZone::pPhysicsActor_type     m_pActor;
    Zen::Engine::Physics::I_PhysicsZone::wpPhysicsZone_type     m_pZone;
    Zen::Engine::Physics::I_PhysicsZone::pPhysicsJoint_type     m_pUpJoint;
	pPhysicsMaterial_type m_pAvatarMaterial;
    /// @}

};  // class AvatarGameObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMECLIENT_AVATAR_GAME_OBJECT_HPP_INCLUDED
