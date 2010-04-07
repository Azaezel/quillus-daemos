//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#include "BaseGame.hpp"

#include "Action.hpp"
#include "GameGroup.hpp"
#include "BehavioredGameObject.hpp"
#include "GameObject.hpp"

#include <Zen/Core/Scripting.hpp>
#include <Zen/Core/Event/I_ActionMap.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_ResourceManager.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>
#include <Zen/Engine/World/I_WaterService.hpp>
#include <Zen/Engine/World/I_TerrainService.hpp>
#include <Zen/Engine/World/I_SkyService.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>
#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>
#include <Zen/Engine/Physics/I_PhysicsActor.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>
#include <Zen/Engine/World/I_TerrainService.hpp>
#include <Zen/Engine/World/I_SkyService.hpp>

#include <Zen/Engine/Navigation/I_NavigationManager.hpp>
#include <Zen/Engine/Navigation/I_NavigationService.hpp>

#include <Zen/Core/Scripting/I_ScriptingManager.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Threading/I_Thread.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <sstream>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <conio.h>
#endif

#include <stddef.h>

#ifdef __GNUG__
#include <stdint.h>
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::BaseGame()
:   m_pScriptObject(NULL)
,   m_pMainGroup(NULL)
,   m_pEventService(Zen::Event::I_EventManager::getSingleton().create("eventService"))
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::~BaseGame()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Core::I_BehaviorService&
BaseGame::getBehaviorService()
{
    return m_behaviorService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Core::I_GameGroup&
BaseGame::getRootGroup()
{
    // TODO Guard?
    if (m_pMainGroup == NULL)
    {
        m_pModule->activate();
        m_pMainGroup = new GameGroup(*this);
    }

    return *m_pMainGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseGame::initScriptModule(pScriptEngine_type _pScriptEngine)
{
    m_pScriptEngine = _pScriptEngine;
    m_pModule = m_pScriptEngine->createScriptModule("BaseGame", "Base Game");

    m_pEventService->registerScriptEngine(_pScriptEngine);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pScriptModule_type
BaseGame::getScriptModule()
{
    return m_pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
BaseGame::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ObjectReference*
script_getRootGroup(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    return I_BaseGame::getSingleton().getRootGroup().getScriptObject();
}

static Zen::Scripting::I_ObjectReference*
script_createObject(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Core::I_GameGroup::ScriptObjectReference_type* pObject = dynamic_cast<Core::I_GameGroup::ScriptObjectReference_type*>(_pObject);

    std::string name = boost::any_cast<std::string>(_parms[0]);

    return pObject->getObject()->createObject(name).getScriptObject();
}

static Zen::Scripting::I_ObjectReference*
script_getGroup(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Core::I_GameGroup::ScriptObjectReference_type* pObject = dynamic_cast<Core::I_GameGroup::ScriptObjectReference_type*>(_pObject);

    std::string name = boost::any_cast<std::string>(_parms[0]);

    return pObject->getObject()->getGroup(name).getScriptObject();
}

static void
script_initBoxShape(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Physics::I_PhysicsZone::ScriptObjectReference_type* pObject = dynamic_cast<Zen::Scripting::ObjectReference<Physics::I_PhysicsZone>*>(_pObject);
}

static void
script_setMass(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Zen::Scripting::ObjectReference<Physics::I_PhysicsActor>* pObject = dynamic_cast<Zen::Scripting::ObjectReference<Physics::I_PhysicsActor>*>(_pObject);
}

static void
script_setPosition(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Engine::Core::I_BaseGameObject::ScriptObjectReference_type* pObject = dynamic_cast<Engine::Core::I_BaseGameObject::ScriptObjectReference_type*>(_pObject);

    I_BaseGameObject* pGameObject = dynamic_cast<I_BaseGameObject*>(pObject->getObject());

    // Get the parms and set the position
    Math::Real dx = boost::any_cast<Math::Real>(_parms[0]);
    Math::Real dy = boost::any_cast<Math::Real>(_parms[1]);
    Math::Real dz = boost::any_cast<Math::Real>(_parms[2]);

    pGameObject->setPosition(dx, dy, dz, true);
}

static void
script_setScale(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Engine::Core::I_BaseGameObject::ScriptObjectReference_type* pObject = dynamic_cast<Engine::Core::I_BaseGameObject::ScriptObjectReference_type*>(_pObject);

    I_BaseGameObject* pGameObject = dynamic_cast<I_BaseGameObject*>(pObject->getObject());

    // Get the parms and set the position
    Math::Real dx = boost::any_cast<Math::Real>(_parms[0]);
    Math::Real dy = boost::any_cast<Math::Real>(_parms[1]);
    Math::Real dz = boost::any_cast<Math::Real>(_parms[2]);

    pGameObject->setScale(dx, dy, dz);
}

static void
script_setOrientation(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Engine::Core::I_BaseGameObject::ScriptObjectReference_type* pObject = dynamic_cast<Engine::Core::I_BaseGameObject::ScriptObjectReference_type*>(_pObject);

    I_BaseGameObject* pGameObject = dynamic_cast<I_BaseGameObject*>(pObject->getObject());

    if (_parms.size() == 4)
    {
        // Get the parms and set the position
        Math::Real _w = boost::any_cast<Math::Real>(_parms[0]);
        Math::Real _x = boost::any_cast<Math::Real>(_parms[1]);
        Math::Real _y = boost::any_cast<Math::Real>(_parms[2]);
        Math::Real _z = boost::any_cast<Math::Real>(_parms[3]);

        pGameObject->setOrientation(_w, _x, _y, _z, true);
    }
    else
    {
        Math::Quaternion4* pOrientation = boost::any_cast<Math::Quaternion4*>(_parms[0]);
        pGameObject->setOrientation(*pOrientation);
    }
}

static Zen::Scripting::I_ObjectReference*
script_getPhysicsActor(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Engine::Core::I_BaseGameObject::ScriptObjectReference_type* pObject = dynamic_cast<Engine::Core::I_BaseGameObject::ScriptObjectReference_type*>(_pObject);

    I_BaseGameObject* pGameObject = dynamic_cast<I_BaseGameObject*>(pObject->getObject());

    Zen::Engine::Core::I_BaseGameObject::pPhysicsActor_type pPhysicsActor = pGameObject->getPhysicsActor();
    Zen::Scripting::I_ObjectReference* pScriptObject = pPhysicsActor->getScriptObject();
    return pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseGame::initScriptTypes()
{
    // Create the Game script type
    m_pGameScriptType = m_pModule->createScriptType("Game", "Game", 0);
    m_pGameScriptType->addMethod("getRootGroup", "Get the root GameGroup",
        &script_getRootGroup);

    /// TODO This is a hack
    Zen::Scripting::script_type<I_BaseGame>(m_pGameScriptType)
        .addMethod("getPhysicsService", &I_BaseGame::getPhysicsService)
        .addMethod("getTerrainService", &I_BaseGame::getTerrainService)
        .addMethod("getSkyService", &I_BaseGame::getSkyService)
        .activate()
    ;

    // Create the GameGroup type
    m_pGameGroupScriptType = m_pModule->createScriptType("GameGroup", "Game Group", 0);
    m_pGameGroupScriptType->addMethod("createObject", "Create a game object",
        &script_createObject);
    m_pGameGroupScriptType->addMethod("getGroup", "Get a sub group",
        &script_getGroup);

    // Create the GameObject script type
    m_pGameObjectScriptType = m_pModule->createScriptType("GameObject", "Game Object", 0);

#if 0 // These have been moved to individual game client implementations of I_GameObject
    m_pGameObjectScriptType->addMethod("loadResource", "Load a rendering resource associated and associate it with this object",
        &script_loadResource);
    m_pGameObjectScriptType->addMethod("setMaterialName", "Set the material name for this object",
        &script_setMaterialName);
#endif // moved

    m_pGameObjectScriptType->addMethod("setPosition", "Set the position of this object",
        &script_setPosition);
    m_pGameObjectScriptType->addMethod("setScale", "Set the scale of this object",
        &script_setScale);
    m_pGameObjectScriptType->addMethod("setOrientation", "Set the orientation of this object",
        &script_setOrientation);
    m_pGameObjectScriptType->addMethod("getPhysicsActor", "Get the Physics Shape of this object",
        &script_getPhysicsActor);

    // Create the PhysicsZone script type
    m_pPhysicsZoneScriptType = m_pModule->createScriptType("PhysicsZone", "Physics World", 0);

#if 0 // not implemented?
    m_pPhysicsZoneScriptType->addMethod("createShape", "Create a Physics Shape",
        &script_createShape);
#endif

    // Create the CollisionShape script type
    m_pCollisionShapeScriptType = m_pModule->createScriptType("CollisionShape", "Physics Shape", 0);
    m_pCollisionShapeScriptType->addMethod("initBoxShape", "",
        &script_initBoxShape);
    m_pCollisionShapeScriptType->addMethod("setMass", "",
        &script_setMass);

#if 0 // TODO This needs to be implemented to support multiple worlds.
    m_pGameClientScriptType->addMethod("getPhysicsZone", "",
        &script_getPhysicsZone);
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pScriptType_type
BaseGame::getGameGroupScriptType()
{
    return m_pGameGroupScriptType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pScriptType_type
BaseGame::getGameObjectScriptType()
{
    return m_pGameObjectScriptType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseGame::createScriptObjects()
{
    // It's not until after activate() that you can create objects of the new types
    getRootGroup().getScriptObject();

    // Create the script object for this game
    m_pScriptObject = new ScriptObjectReference_type
        (getScriptModule(), getScriptModule()->getScriptType(getScriptTypeName()), this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseGame::initPhysicsService(const std::string& _type)
{
    std::cout << "Initializing Physics service: " << _type << std::endl;

    // was "ZNewton"
    Zen::Engine::Physics::I_PhysicsManager::config_type config;

    // Create the service
    m_pPhysicsService =
        Zen::Engine::Physics::I_PhysicsManager::getSingleton().create(_type, config);

    if (!m_pPhysicsService.isValid())
    {
        std::cout << "Error: couldn't initPhysicsService()" << std::endl;
    }

    std::cout << "Physics Service created, checking for scripting support." << std::endl;

    // Register the script engine with the Physics Manager
    if (m_pScriptEngine.isValid())
    {
        Zen::Engine::Physics::I_PhysicsManager::getSingleton().registerDefaultScriptEngine(m_pScriptEngine);
    }

    std::cout << "BaseGame::initPhysicsService(): Success!" << std::endl;
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseGame::initTerrainService(const std::string& _type)
{
    std::cout << "Initializing Terrain service: " << _type << std::endl;

    Zen::Engine::World::I_WorldManager::config_type config;

    m_pTerrainService =
        Zen::Engine::World::I_WorldManager::getSingleton().createTerrainService(_type, config);

    if(!m_pTerrainService.isValid())
    {
        std::cout << "Error: couldn't init Terrain service" << std::endl;
    }

    std::cout << "Terrain Service created, checking for scripting support." << std::endl;

    // Register the script engine with the Terrain Manager
    if( m_pScriptEngine.isValid())
    {
        Zen::Engine::World::I_WorldManager::getSingleton().registerDefaultScriptEngine(m_pScriptEngine);
    }

    std::cout << "BaseGame::initTerrainService(): Success!" << std::endl;
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseGame::initSkyService(const std::string& _type)
{
    std::cout << "Initializing Sky service: " << _type << std::endl;

    Zen::Engine::World::I_WorldManager::config_type config;

    m_pSkyService =
        Zen::Engine::World::I_WorldManager::getSingleton().createSkyService(_type, config);

    if(!m_pSkyService.isValid())
    {
        std::cout << "Error: couldn't init Sky service" << std::endl;
    }

    std::cout << "Sky Service created, checking for scripting support." << std::endl;

    // Register the script engine with the Terrain Manager
    if(m_pScriptEngine.isValid())
    {
        Zen::Engine::World::I_WorldManager::getSingleton().registerDefaultScriptEngine(m_pScriptEngine);
    }

    std::cout << "BaseGame::initSkyService(): Success!" << std::endl;
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pPhysicsZone_type
BaseGame::getCurrentPhysicsZone()
{
    return m_pPhysicsZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseGame::setCurrentPhysicsZone(BaseGame::pPhysicsZone_type _pPhysicsZone)
{
    m_pPhysicsZone = _pPhysicsZone;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseGame::initPhysicsResourceService(const std::string& _type)
{
    // Was "ogre"

    // Register the script engine
    if (m_pScriptEngine.isValid())
    {
        Resource::I_ResourceManager::getSingleton().registerDefaultScriptEngine(m_pScriptEngine);
    }

    // TODO Don't hard-code this
    Zen::Engine::Resource::I_ResourceManager::config_type config;

    // Create the service
    m_pPhysicsResourceService =
        Resource::I_ResourceManager::getSingleton().create(_type, config);

    pScriptModule_type const pModule = Resource::I_ResourceManager::getSingleton().getDefaultScriptModule();

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pPhysicsService_type
BaseGame::getPhysicsService()
{
    return m_pPhysicsService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pTerrainService_type
BaseGame::getTerrainService()
{
    return m_pTerrainService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pSkyService_type
BaseGame::getSkyService()
{
    return m_pSkyService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseGame::pPhysicsResourceService_type
BaseGame::getPhysicsResourceService()
{
    return m_pPhysicsResourceService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseGame::initNavigationService(const std::string& _type)
{
    m_pNavigationService =
        Zen::Engine::Navigation::I_NavigationManager::getSingleton().create(_type);

    if (!m_pNavigationService.isValid())
    {
        std::cout << "Error: couldn't create NavigationService" << std::endl;
        return false;
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Navigation::I_NavigationService&
BaseGame::getNavigationService()
{
    return *m_pNavigationService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_ActionMap&
BaseGame::getActionMap()
{
    return m_pEventService->getActionMap("default");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
