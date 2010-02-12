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
#ifndef GAMECLIENT_BEHAVIOR_GROUP_HPP_INCLUDED
#define GAMECLIENT_BEHAVIOR_GROUP_HPP_INCLUDED

#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class BehaviorManager;
class GameObject;

/// @brief Behavior Group
///
/// This class presents an easy-to-use facade for working with behavior groups
class BehaviorGroup
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Engine::Core::I_BehaviorService::pGameObjectBehaviors_type     pGameObjectBehaviors_type;
    /// @}

    /// @name BehaviorGroup implementation
    /// @{
public:
    BehaviorGroup& add(const std::string& _behaviorName);

    /// Attach this behavior group to a game object
    BehaviorGroup& attach(GameObject& _gameObject);
    /// @}

    /// @name 'Structors
    /// @{
public:
             BehaviorGroup(BehaviorManager& _manager, const std::string& _groupName);
    virtual ~BehaviorGroup();
    /// @}

    /// @name Member Variables
    /// @{
private:
    BehaviorManager&                        m_manager;
    pGameObjectBehaviors_type               m_pBehaviors;
    /// @}

};  // class BehaviorManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMECLIENT_BEHAVIOR_MANAGER_HPP_INCLUDED