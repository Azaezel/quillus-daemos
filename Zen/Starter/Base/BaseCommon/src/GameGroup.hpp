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
#ifndef ZEN_ENGINE_BASE_GAME_GROUP_HPP_INCLUDED
#define ZEN_ENGINE_BASE_GAME_GROUP_HPP_INCLUDED

#include "GameObjectBehaviors.hpp"

#include <Zen/Engine/Core/I_GameGroup.hpp>
#include <Zen/Engine/Core/I_GameObject.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class BaseGame;
class BehavioredGameObject;

/// @brief Base Game Group
///
/// Collection of Game Objects
class GameGroup
:   public Zen::Engine::Core::I_GameGroup
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_GameGroup implementation
    /// @{
public:
    virtual Zen::Engine::Core::I_BaseGameObject& createObject(const std::string& _name);
    virtual void addObject(Zen::Engine::Core::I_BaseGameObject& _gameObject);
    virtual Zen::Engine::Core::I_BaseGameObject& getObject(const std::string& _name);
    virtual void destroyObject(const std::string& _name);
    virtual void getObjects(struct I_GameObjectVisitor& _visitor);

    virtual Core::I_GameGroup& createGroup(const std::string& _name);
    virtual Core::I_GameGroup& getGroup(const std::string& _name);
    virtual void destroyGroup(const std::string& _name);

    virtual void setDefaultBehaviors(Core::I_BehaviorService::pGameObjectBehaviors_type _pBehaviors);
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:

    /// @}

    /// @name Default Behaviors
    /// @{
public:

    /// @}

    /// @name 'Structors
    /// @{
public:
             GameGroup(BaseGame& _client);
    virtual ~GameGroup();
    /// @}

    /// @name Member Variables
    /// @{
private:
    BaseGame&     m_game;

    typedef std::map<std::string, BehavioredGameObject*>  GameMap_type;
    GameMap_type    m_gameObjects;

    typedef std::map<std::string, GameGroup*>   GameGroup_type;
    GameGroup_type  m_gameGroups;

    GameObjectBehaviors*                    m_pDefaultBehaviors;

    Scripting::I_ObjectReference*           m_pScriptObject;
    /// @}

};  // class GameGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_BASE_GAME_GROUP_HPP_INCLUDED
