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
#ifndef GAMECLIENT_BEHAVIOR_MANAGER_HPP_INCLUDED
#define GAMECLIENT_BEHAVIOR_MANAGER_HPP_INCLUDED

#include "GravityBehavior.hpp"
#include "AvatarCollisionBehavior.hpp"

#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>
#include <Zen/Starter/Base/BaseCommon/I_BaseGame.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameClient;
class BehaviorGroup;

/// @brief Behavior Manager
///
/// This class presents an easy-to-use facade for working with behaviors.
class BehaviorManager
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, BehaviorGroup*>                               Groups_type;
    typedef Zen::Engine::Core::I_BehaviorService::pGameObjectBehaviors_type     pGameObjectBehaviors_type;
    typedef Zen::Engine::Core::I_GameObjectBehaviors::I_PhysicsForceBehavior*   pPhysicsForceBehavior_type;
    typedef std::map<std::string, pPhysicsForceBehavior_type>                   PhysicsForceBehaviors_type;
    typedef Zen::Engine::Core::I_GameObjectBehaviors::I_CollisionBehavior*		pCollisionBehavior_type;
    typedef std::map<std::string, pCollisionBehavior_type>						CollisionBehaviors_type;

    /// @}

    /// @name BehaviorManager implementation
    /// @{
public:
    Zen::Engine::Base::I_BaseGameClient& base();

    Zen::Engine::Base::I_BaseGame& game();

    /// Initialize the Behavior Manager
    void init();

    /// Get a behavior group by name
    BehaviorGroup& getGroup(const std::string& _groupName);
	pGameObjectBehaviors_type getDefaultBehaviors(){return m_pDefaultBehavior;};
	pGameObjectBehaviors_type getAvatarBehaviors(){return m_pAvatarBehavior;};

private:
    friend class BehaviorGroup;
    void add(pGameObjectBehaviors_type _pGroup, const std::string& _behaviorName);
    /// @}

    /// @name 'Structors
    /// @{
public:
             BehaviorManager(GameClient& _gameClient);
    virtual ~BehaviorManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameClient&                             m_gameClient;

    Zen::Threading::I_Mutex*                m_pGroupGuard;
    Groups_type                             m_behaviorGroups;

    pGameObjectBehaviors_type               m_pDefaultBehavior;
    pGameObjectBehaviors_type               m_pAvatarBehavior;

    PhysicsForceBehaviors_type              m_physicsForceBehaviors;
    CollisionBehaviors_type					m_avatarCollisionBehaviors;

    GravityBehavior                         m_gravity;
    AvatarCollisionBehavior                 m_avatarCollision;

	/// @}

};  // class BehaviorManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMECLIENT_BEHAVIOR_MANAGER_HPP_INCLUDED