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
#include "BehaviorManager.hpp"
#include "BehaviorGroup.hpp"

#include "GameClient.hpp"
#include "GameObject.hpp"

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BehaviorManager::BehaviorManager(GameClient& _gameClient)
:   m_gameClient(_gameClient)
,   m_pGroupGuard(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BehaviorManager::~BehaviorManager()
{
    Zen::Threading::MutexFactory::destroy(m_pGroupGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehaviorManager::init()
{
    m_pDefaultBehavior = game().getBehaviorService().createBehaviors("default");
    m_pDefaultBehavior->setScaleBehavior(&GameObject::setScale);
    game().getRootGroup().setDefaultBehaviors(m_pDefaultBehavior);

    m_pAvatarBehavior = game().getBehaviorService().createBehaviors("AvatarBehavior");
    m_pAvatarBehavior->push_front(m_avatarCollision);
    m_pAvatarBehavior->push_front(m_gravity);

	Zen::Math::Vector3 gravity(0.0f, -90.8f, 0.0f);
    m_gravity.setGravity(gravity);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Base::I_BaseGameClient&
BehaviorManager::base()
{
    return m_gameClient.base();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Base::I_BaseGame&
BehaviorManager::game()
{
    return m_gameClient.game();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BehaviorGroup&
BehaviorManager::getGroup(const std::string& _groupName)
{
    Zen::Threading::CriticalSection lock(m_pGroupGuard);

    Groups_type::iterator iter = m_behaviorGroups.find(_groupName);

    if(iter == m_behaviorGroups.end())
    {
        BehaviorGroup* const pGroup = new BehaviorGroup(*this, _groupName);
        m_behaviorGroups[_groupName] = pGroup;
        return *pGroup;
    }
    else
    {
        return *(iter->second);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehaviorManager::add(pGameObjectBehaviors_type _pGroup, const std::string& _behaviorName)
{
    PhysicsForceBehaviors_type::iterator iter = m_physicsForceBehaviors.find(_behaviorName);

    if (iter == m_physicsForceBehaviors.end())
    {
        std::stringstream errorMessage;
        errorMessage << "BehaviorManager::add(): Error finding behavior: " << _behaviorName;
        throw Zen::Utility::runtime_exception(errorMessage.str());
    }

    _pGroup->push_back(*iter->second);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
