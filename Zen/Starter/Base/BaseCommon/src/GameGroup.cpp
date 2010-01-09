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
#include "GameGroup.hpp"
#include "BehavioredGameObject.hpp"
#include "BaseGame.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameGroup::GameGroup(BaseGame& _client)
:   m_game(_client)
,   m_pDefaultBehaviors(NULL)
{
    m_pScriptObject = new Core::I_GameGroup::ScriptObjectReference_type
        (_client.getScriptModule(), _client.getScriptModule()->getScriptType("GameGroup"), this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameGroup::~GameGroup()
{
    // Destroy all of the child game groups.
    for(GameGroup_type::iterator iter = m_gameGroups.begin(); iter != m_gameGroups.end(); iter++)
    {
        delete iter->second;
    }
    m_gameGroups.empty();

    // Destroy all of the child game objects
    for(GameMap_type::iterator iter = m_gameObjects.begin(); iter != m_gameObjects.end(); iter++)
    {
        delete iter->second;
    }
    m_gameObjects.empty();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Core::I_BaseGameObject&
GameGroup::createObject(const std::string& _name)
{
    BehavioredGameObject* pObject = new BehavioredGameObject(m_game, _name, m_pDefaultBehaviors);

    // TODO Double check for duplicates
    m_gameObjects[_name] = pObject;

    return *pObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameGroup::addObject(Zen::Engine::Core::I_BaseGameObject& _gameObject)
{
    BehavioredGameObject* pObject = dynamic_cast<BehavioredGameObject*>(&_gameObject);

    if (pObject)
    {
        m_gameObjects[_gameObject.getName()] = pObject;
    }
    else
    {
        // TODO Error
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Core::I_BaseGameObject&
GameGroup::getObject(const std::string& _name)
{
    // TODO Don't insert a new object if not found and... what? Throw exception or
    // return NULL which means we have to return a pointer not a reference.
    return *m_gameObjects[_name];
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameGroup::destroyObject(const std::string& _name)
{
    GameMap_type::iterator iter = m_gameObjects.find(_name);

    if (iter != m_gameObjects.end())
    {
        delete iter->second;
        m_gameObjects.erase(iter);
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameGroup::getObjects(struct I_GameObjectVisitor& _visitor)
{
    // TODO Guard or verify that this is being called by the render loop thread

    // Iterate through all of the game objects and pass them to the visitor

   _visitor.begin();

   for(GameMap_type::iterator iter = m_gameObjects.begin(); iter != m_gameObjects.end(); iter++)
   {
       _visitor.visit(*iter->second);
   }

   _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Core::I_GameGroup&
GameGroup::createGroup(const std::string& _name)
{
    // Since getGroup() checks for duplicates and also creates the group if
    // it doesn't exist, then just use that instead of re-writing the logic here.
    return getGroup(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Core::I_GameGroup&
GameGroup::getGroup(const std::string& _name)
{
    // Check to see if the group exists
    GameGroup_type::iterator iter = m_gameGroups.find(_name);

    if(iter == m_gameGroups.end())
    {
        // If not, create it and return it.
        GameGroup* pGroup = new GameGroup(m_game);

        m_gameGroups[_name] = pGroup;

        return *pGroup;
    }
    else
    {
        // If the group exists, simply return it.
        return *iter->second;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameGroup::destroyGroup(const std::string& _name)
{
    GameGroup_type::iterator iter = m_gameGroups.find(_name);
    if (iter != m_gameGroups.end())
    {
        delete iter->second;
        m_gameGroups.erase(iter);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameGroup::setDefaultBehaviors(Core::I_BehaviorService::pGameObjectBehaviors_type _pBehaviors)
{
    m_pDefaultBehaviors = dynamic_cast<GameObjectBehaviors*>(_pBehaviors);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
GameGroup::getScriptObject()
{
    return m_pScriptObject;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
