//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Worlds Game Client
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
#include "GameClient.hpp"
#include "GUIManager.hpp"

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Ray.hpp>

#include <Zen/Core/Plugins/I_Application.hpp>
#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Core/I_GameGroup.hpp>
#include <Zen/Engine/Core/I_ActionMap.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Rendering/I_Light.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>
#include <Zen/Engine/Rendering/I_Camera.hpp>

#include <Zen/Engine/Widgets/I_WidgetService.hpp>

#include <Zen/Engine/World/I_Sky.hpp>
#include <Zen/Engine/World/I_Terrain.hpp>

#include <Zen/plugins/ZOgre/I_OgreRenderingCanvas.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <Zen/Engine/Input/I_KeyMap.hpp>

#include <Zen/Engine/Camera/I_CameraManager.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>


#include <Zen/Starter/Base/BaseCommon/I_BaseGameObject.hpp>

#include <Zen/plugins/ZOgre/I_OgreRenderingCanvas.hpp>


#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/bind.hpp>

#include <stddef.h>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClient::GameClient(WindowHandle_type _pParent)
:   m_initialized(false)
,   m_base(Engine::Base::I_BaseGameClient::getSingleton())
,   m_game(Engine::Base::I_BaseGame::getSingleton())
,   m_pScriptObject(NULL)
,   m_pGUIManager(NULL)
,   m_moveDirection(0.0f, 0.0f, 0.0f)
,   m_zoomAmount(0.0f)
,   m_pSky()
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
bool
GameClient::init()
{
    std::cout << "GameClient::init()" << std::endl;

    base().onBeforeFrameRenderedEvent.connect(boost::bind(&GameClient::beforeRender, this, _1));

    setupResourcePaths();

    // Possibly the rest of this should be done later and we should show
    // an initial game screen or splash screens here.

    std::cout << "GUI Manager init" << std::endl;
    m_pGUIManager->init();

    createActions();
    createDefaultMapping();
    createBehaviors();
    createScene();

    initServices();

    Zen::Engine::Core::I_ActionMap& actionMap = game().getActionMap();
    if (actionMap["onInitDone"].isValid())
    {
        std::cout << "hooking up onInitDone" << std::endl;
        boost::any scriptObject(getScriptObject());
        actionMap["onInitDone"]->dispatch(scriptObject);
    }
    else
    {
        std::cout << "Script didn't register handler for onInitDone" << std::endl;
    }
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::setupResourcePaths()
{
    boost::filesystem::path rootPath;

#if 0 // Moved to script
    // GUI resources and standins
    rootPath = boost::filesystem::system_complete(boost::filesystem::path("resources/gui/imagesets", boost::filesystem::native)).normalize();
    base().getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);

    rootPath = boost::filesystem::system_complete(boost::filesystem::path("resources/gui/looknfeel", boost::filesystem::native)).normalize();
    base().getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);

    rootPath = boost::filesystem::system_complete(boost::filesystem::path("resources/gui/schemes", boost::filesystem::native)).normalize();
    base().getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);

    rootPath = boost::filesystem::system_complete(boost::filesystem::path("resources/gui/fonts", boost::filesystem::native)).normalize();
    base().getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);
#endif // deprecated

    base().getRenderingResourceService().initialiseAllResourceGroups();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::run()
{
    m_base.run();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Widgets::I_WidgetService&
GameClient::getWidgetService()
{
    return m_base.getWidgetService();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_RenderingCanvas&
GameClient::getRenderingCanvas()
{
    return m_base.getRenderingCanvas();
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
GameClient::createScene()
{
    // Create the light
    Zen::Engine::Rendering::I_SceneService::pLight_type pLight = m_base.getSceneService().createLight("default", "Light");
    pLight->setPosition(0.0f, 0.0f, 50.0f);

    // Set the camera in the correct location
    Zen::Engine::Rendering::I_RenderingCanvas& canvas = base().getRenderingCanvas();
    Zen::Engine::Rendering::I_Camera& camera = canvas.selectCamera("default");
    //camera.setPosition(7.296f, 0.0262f, 30.0f);
    camera.setPosition(146.0f, 101.0f, 221.0f);
    camera.setNearClipDistance(0.1f);
    camera.setFarClipDistance(99999.0f*6.0f);
    camera.setAspectRatio((Zen::Math::Real)canvas.getWidth() / (Zen::Math::Real)canvas.getHeight());
    camera.setHorizontalFOV(Zen::Math::Degree(60));

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createActions()
{
    // Connect the main keymap to the widget service.
    // This will make it so the key map responds to keys that the
    // GUI doesn't handle.
    base().getKeyMap().connect(base().getWidgetService());
    base().getKeyMap().enable(true);

    Zen::Engine::Core::I_ActionMap& actionMap = game().getActionMap();

    // Create some actions (this should really be done in script)
    actionMap.createAction("Quit", boost::bind(&GameClient::quit, this, _1));

    actionMap.createAction("Left", boost::bind(&GameClient::moveLeft, this, _1));
    actionMap.createAction("Right", boost::bind(&GameClient::moveRight, this, _1));
    actionMap.createAction("Up", boost::bind(&GameClient::moveUp, this, _1));
    actionMap.createAction("Down", boost::bind(&GameClient::moveDown, this, _1));

    actionMap.createAction("Zoom", boost::bind(&GameClient::zoom, this, _1));
    actionMap.createAction("Print", boost::bind(&GameClient::print, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createDefaultMapping()
{
    Zen::Engine::Input::I_KeyMap& keyMap = base().getKeyMap();
    Zen::Engine::Core::I_ActionMap& actionMap = game().getActionMap();

    // Map some keys to actions
    keyMap.mapKeyInput("q", actionMap["Quit"]);

    keyMap.mapKeyInput("a", actionMap["Left"]);
    keyMap.mapKeyInput("d", actionMap["Right"]);
    keyMap.mapKeyInput("w", actionMap["Up"]);
    keyMap.mapKeyInput("s", actionMap["Down"]);

    keyMap.mapKeyInput("x", actionMap["Zoom"]);
    keyMap.mapKeyInput("p", actionMap["Print"]);
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
void
GameClient::initServices()
{
    // Get a reference to the app server manager
    Zen::Enterprise::AppServer::I_ApplicationServerManager& manager =
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton();

    // Initialize the main application server
    m_pAppServer = &Enterprise::AppServer::I_ApplicationServer::getInstance("main");

    m_pAppServer->registerDefaultScriptEngine(base().getScriptEnginePtr());

    // Load the TCP/IP protocol adapter and install it as the "services" protocol adapter.
    m_pProtocolService = manager.createProtocolService(*m_pAppServer, "tcp-binary");
    m_pAppServer->installProtocol(m_pProtocolService, "services");

    Zen::Plugins::I_PluginManager& pluginManager = Zen::Plugins::I_PluginManager::getSingleton();
    Zen::Plugins::I_PluginManager::app_ptr_type pApp = pluginManager.getApplication();

    // Get the application configuration
    typedef Plugins::I_ConfigurationElement::const_ptr_type pConfig_type;
    pConfig_type pAppConfig = pApp->getConfiguration().getConfigurationElement("application");

    // Use the application configuration to load application services and protocol adapters.
    pConfig_type pApplications = pAppConfig->getChild("applications");
    if (pApplications)
    {
        m_pAppServer->installApplications(pApplications);
    }

    // Install the protocols
    pConfig_type pProtocols = pAppConfig->getChild("protocols");

    if (pProtocols)
    {
        m_pAppServer->installProtocols(pProtocols);
    }

    // The application server must be started before any applications are actually installed.
    // The require() makes it so that this call will block until the application server
    // is fully operational.
    m_pAppServer->start()->requireCondition();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Base::I_BaseGame&
GameClient::game()
{
    return m_game;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Base::I_BaseGameClient&
GameClient::base()
{
    return m_base;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::beforeRender(double _elapsedTime)
{
    // Do everything else that needs to be done before the screen is rendered

    // Update the physics world
    if (_elapsedTime > 0.0f)
    {
        game().getPhysicsService()->stepSimulation(_elapsedTime);
    }

    // Change the camera position
    Zen::Engine::Rendering::I_RenderingCanvas& canvas = base().getRenderingCanvas();
    Zen::Engine::Rendering::I_Camera& camera = canvas.selectCamera("default");

    Zen::Math::Point3 position;
    position = camera.getPosition();
    position += m_moveDirection * _elapsedTime * 5.0f;
    position.m_z += m_zoomAmount * _elapsedTime * 10.0f;

    camera.setPosition(position.m_x, position.m_y, position.m_z);

    if (_elapsedTime > 0.0f)
    {
        boost::any elapsedTime((Math::Real)_elapsedTime);
        game().getActionMap()["onTick"]->dispatch(elapsedTime);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::quit(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getPressedState())
    {
        base().quit(_parameter);
    }
    else
    {
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveLeft(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getPressedState())
    {
        m_moveDirection.m_x -= 1.0f;
    }
    else
    {
        m_moveDirection.m_x += 1.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveRight(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getPressedState())
    {
        m_moveDirection.m_x += 1.0f;
    }
    else
    {
        m_moveDirection.m_x -= 1.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveUp(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getPressedState())
    {
        m_moveDirection.m_y += 1.0f;
    }
    else
    {
        m_moveDirection.m_y -= 1.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveDown(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getPressedState())
    {
        m_moveDirection.m_y -= 1.0f;
    }
    else
    {
        m_moveDirection.m_y += 1.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::zoom(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getPressedState())
    {
        m_zoomAmount = -1.0f;
    }
    else
    {
        m_zoomAmount = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::print(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (!pEvent->getPressedState())
        return;

    Zen::Math::Point3 position;
    Zen::Engine::Rendering::I_RenderingCanvas& canvas = m_base.getRenderingCanvas();
    Zen::Engine::Rendering::I_Camera& camera = canvas.selectCamera("default");
    position = camera.getPosition();

    std::cout << "Camera Position: " << position.m_x << " " << position.m_y << " " << position.m_z << std::endl;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::handleMouseClickEvent(Zen::Engine::Input::I_MouseClickEvent& _event)
{
    if (_event.wasClicked())
    {
        queryCursor();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::queryCursor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::setGameName(const std::string& _name)
{
    m_gameName = _name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameClient::getGameName() const
{
    return m_gameName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createSkybox(const std::string& _resource)
{
    Zen::Engine::World::I_SkyService::config_type config;
    config["type"] = "skybox";
    config["resourceName"] = _resource;

    m_pSky = game().getSkyService()->createSky(config);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createTerrain(const std::string& _resource)
{
    Zen::Math::Matrix4 matXfm(Zen::Math::Matrix4::INIT_IDENTITY);

    m_pTerrain = game().getTerrainService()->createTerrain();
    m_pTerrain->loadVisualization(_resource + ".cfg", matXfm);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
