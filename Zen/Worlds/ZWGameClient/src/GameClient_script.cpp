//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Worlds Game Client
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
/// This file is in addition to GameClient.cpp and adds all of the code that
/// exposes GameClient to script.
#include "GameClient.hpp"
#include "GUIManager.hpp"
#include "GameObject.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>

#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// HACK This is because I_BaseGameClient has no way to get us to GameClient*
static GameClient* sm_pTheOnlyGameClient;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_init(Zen::Scripting::I_ObjectReference* _pObject)
{
    sm_pTheOnlyGameClient->init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static bool
script_isDebug(Zen::Scripting::I_ObjectReference* _pObject)
{
#ifdef _DEBUG
    return true;
#else
    return false;
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_setGameName(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    sm_pTheOnlyGameClient->setGameName(boost::any_cast<std::string>(_parms[0]));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string
script_getGameName(Zen::Scripting::I_ObjectReference* _pObject)
{
    return sm_pTheOnlyGameClient->getGameName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_initRenderingService(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // TODO Validate the parm types

    std::string type = boost::any_cast<std::string>(_parms[0]);
    std::string windowTitle = boost::any_cast<std::string>(_parms[1]);
    Zen::Math::Real x = boost::any_cast<Math::Real>(_parms[2]);
    unsigned windowWidth = (unsigned)x;
    x = boost::any_cast<Math::Real>(_parms[3]);
    unsigned windowHeight = (unsigned)x;

    // Init the rendering service.
    sm_pTheOnlyGameClient->base().initRenderingService(type,
        windowTitle, windowWidth, windowHeight);

    // For now just assume the scene service and resource service are
    // the same type as the rendering service.
    sm_pTheOnlyGameClient->base().initSceneService(type);
    sm_pTheOnlyGameClient->base().initRenderingResourceService(type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_initPhysicsService(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Init the physics service
    std::string type = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->game().initPhysicsService(type);

    // For now assume the physics resource service is the same type as
    // the physics service
    sm_pTheOnlyGameClient->game().initPhysicsResourceService(type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_initTerrainService(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Init the terrain service
    std::string type = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->base().initTerrainService(type);
 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_createTerrain(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Create a terrain object
    std::string resource = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->createTerrain(resource);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_initSkyService(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Init the sky service
    std::string type = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->base().initSkyService(type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_createSkybox(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Create a sky object
    std::string resource = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->createSkybox(resource);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_initInputService(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Init the input service
    std::string type = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->base().initInputService(type);

    // Set the window size of the input service so that mouse events are scaled properly.
    sm_pTheOnlyGameClient->base().getInputService().setWindowSize(sm_pTheOnlyGameClient->base().getRenderingCanvas().getWidth(), sm_pTheOnlyGameClient->base().getRenderingCanvas().getHeight());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_initWidgetService(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Init the input service
    std::string type = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->base().initWidgetService(type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_initNavigationService(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    // Init the input service
    std::string type = boost::any_cast<std::string>(_parms[0]);

    sm_pTheOnlyGameClient->game().initNavigationService(type);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::activateGameClientScriptModule()
{
    std::cout << "GameClient::activateGameClientScriptModule()" << std::endl;

    sm_pTheOnlyGameClient = this;

    // Initialize the widget service
    m_pGUIManager.reset(new GUIManager(*this));
    m_pGUIManager->initScriptType();

    // Register more methods to the gameClient script type
    pScriptType_type pGameClientScriptType = m_base.getGameClientScriptType();

    pGameClientScriptType->addMethod("init", "Initialize the game client", &script_init);
    pGameClientScriptType->addMethod("isDebug", "Returns true if the C++ code is DEBUG", &script_isDebug);
    pGameClientScriptType->addMethod("setGameName", "Set the game name", &script_setGameName);
    pGameClientScriptType->addMethod("getGameName", "Get the game name", &script_getGameName);

    pGameClientScriptType->addMethod("initRenderingService", "Initialize the rendering service", &script_initRenderingService);

    pGameClientScriptType->addMethod("initInputService", "Initialize the input service", &script_initInputService);
    pGameClientScriptType->addMethod("initWidgetService", "Initialize the widget  service", &script_initWidgetService);

    // TODO Move these to game, not game client
    pGameClientScriptType->addMethod("initPhysicsService", "Initialize the physics service", &script_initPhysicsService);
    pGameClientScriptType->addMethod("initNavigationService", "Initialize the navigation service", &script_initNavigationService);
    pGameClientScriptType->addMethod("initTerrainService", "Initialize the terrain service", &script_initTerrainService);
    pGameClientScriptType->addMethod("initSkyService", "Initialize the sky service", &script_initSkyService);
    pGameClientScriptType->addMethod("createSkybox", "Create a skybox", &script_createSkybox);
    pGameClientScriptType->addMethod("createTerrain", "Create a terrain", &script_createTerrain);

    createScriptTypes();

    // Tell m_base we're done.  After the modules are activated then you cannot
    // modify any script types.
    m_base.activateScriptModules();

    // Create the script object for this game client
    m_pScriptObject = new ScriptObjectReference_type
        (base().getScriptModule(), base().getScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());

    // Create the other script objects
    m_pGUIManager->createScriptObject();

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
GameClient::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createScriptTypes()
{
    GameObject::createScriptTypes(m_base.getScriptEnginePtr());
    // This is where you'd create script types that are outside of the base
    // script module.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
