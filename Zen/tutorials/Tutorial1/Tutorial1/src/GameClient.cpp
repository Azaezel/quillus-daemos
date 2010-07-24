//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
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
// This project is part of the Zen Engine Tutorials
// 
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "GameClient.hpp"

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Core/Event/I_ActionMap.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Core/I_GameGroup.hpp>
#include <Zen/Engine/Core/I_ActionMap.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Rendering/I_Light.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_KeyMap.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/bind.hpp>

#include <stddef.h>
#include <iostream>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

GameClient::GameClient(WindowHandle_type _pParent)
:   m_base(Zen::Engine::Base::I_BaseGameClient::getSingleton())
,   m_game(Zen::Engine::Base::I_BaseGame::getSingleton())
,   m_pScriptObject(NULL)
{
    m_base.setWindowHandle(_pParent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClient::~GameClient()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const GameClient::WindowHandle_type
GameClient::getHandle() const
{
    return m_base.getWindowHandle();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
GameClient::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    // This isn't really used with the Base Starter kit since ZGameLoader 
    // registers the script engine with the BaseGameClient.

    // Just ignore this method.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::init()
{
    m_base.onBeforeFrameRenderedEvent.connect(boost::bind(&GameClient::beforeRender, this, _1));
    m_base.onBeforeFrameRenderedEvent.connect(boost::bind(&GameClient::afterRender, this, _1));

    m_base.initRenderingService("ogre");
    m_base.initSceneService("ogre");
    m_base.initRenderingResourceService("ogre");

    // This tutorial doesn't use physics.  If it did then it would initialize it here.
    //game().initPhysicsService("ZNewton");
    //setupPhysicsMaterials();
    //game().initPhysicsResourceService("ZNewton");

    // Initialize the keyboard / mouse service.  Although this only says "keyboard" in reality it 
    // also initializes the mouse.
    // TODO FIX THIS! :P
    base().initInputService("keyboard");

    // Possibly the rest of this should be done later and we should show 
    // an initial game screen or splash screens here.

    createActions();
    createDefaultMapping();
    createBehaviors();
    createScene();

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::run()
{
    m_base.run();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Widgets::I_WidgetService&
GameClient::getWidgetService()
{
    return m_base.getWidgetService();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Rendering::I_RenderingCanvas&
GameClient::getRenderingCanvas()
{
    return m_base.getRenderingCanvas();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Resource::I_ResourceService&
GameClient::getRenderingResourceService()
{
    return m_base.getRenderingResourceService();
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::World::I_TerrainService&
GameClient::getTerrainService()
{
    return m_base.getTerrainService();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::World::I_SkyService&
GameClient::getSkyService()
{
    return m_base.getSkyService();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::initRenderingService(const std::string& _type, const std::string& _title, int _xRes, int _yRes)
{
    return m_base.initRenderingService(_type, _title, _xRes, _yRes);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::initRenderingResourceService(const std::string& _type)
{
    return m_base.initRenderingResourceService(_type);
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::initTerrainService(const std::string& _type)
{
    return game().initTerrainService(_type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::initSkyService(const std::string& _type)
{
    return game().initSkyService(_type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::initInputService(const std::string& _type)
{
    return m_base.initInputService(_type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::initWidgetService(const std::string& _type)
{
    return m_base.initWidgetService(_type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::activateGameClientScriptModule()
{
    createScriptTypes();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createScriptTypes()
{
    // TODO Register additional script types here

    // Tell m_base we're done.  After the modules are activated then you cannot
    // modify any script types.
    m_base.activateScriptModules();

    // Create the script object for this game client
    m_pScriptObject = new ScriptObjectReference_type
        (base().getScriptModule(), base().getScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createScene()
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createActions()
{
    // Create some actions
    game().getActionMap().createAction("Quit", boost::bind(&Zen::Engine::Base::I_BaseGameClient::quit, &m_base, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createDefaultMapping()
{
    // Map some keys to actions
    base().getKeyMap().mapKeyInput("q", game().getActionMap()["Quit"]);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createBehaviors()
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::setupPhysicsMaterials()
{
    // TODO - only the default material works, since the objects are made in script
    //   this code needs to be transferred to script and then applied to the
    //   physics shapes after they're created.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Base::I_BaseGame&
GameClient::game()
{
    return m_game;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Base::I_BaseGameClient&
GameClient::base()
{
    return m_base;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::beforeRender(double _elapsedTime)
{
    // TODO: Do everything else that needs to be done before the screen is rendered

    // This tutorial doesn't use / initialize physics.  If it did then this is what
    // you would do:

    // This updates all physics worlds we've created:
    //game().getPhysicsService()->stepSimulation(_elapsedTime);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::afterRender(double _elapsedTime)
{

}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
