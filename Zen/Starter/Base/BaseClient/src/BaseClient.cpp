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
#include "BaseClient.hpp"

//#include "Action.hpp"
//#include "GameGroup.hpp"
//#include "BehavioredGameObject.hpp"
//#include "GameObject.hpp"

#include <Zen/Engine/Rendering/I_RenderingService.hpp>
#include <Zen/Engine/Rendering/I_RenderingServiceFactory.hpp>

#include <Zen/Engine/Rendering/I_RenderingManager.hpp>
#include <Zen/Engine/Rendering/I_View.hpp>
#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_SceneManager.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneServiceFactory.hpp>
#include <Zen/Engine/Rendering/I_Context.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <Zen/Engine/Widgets/I_WidgetManager.hpp>
#include <Zen/Engine/Widgets/I_WidgetService.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>
#include <Zen/Engine/World/I_WaterService.hpp>
#include <Zen/Engine/World/I_TerrainService.hpp>
#include <Zen/Engine/World/I_SkyService.hpp>

#include <Zen/Engine/Camera/I_CameraManager.hpp>
#include <Zen/Engine/Camera/I_CameraService.hpp>
#include <Zen/Engine/Camera/I_Camera.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>
#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>
#include <Zen/Engine/Physics/I_PhysicsShape.hpp>

#include <Zen/Engine/Sound/I_SoundService.hpp>
#include <Zen/Engine/Sound/I_SoundManager.hpp>

#include <Zen/Engine/Input/I_InputServiceManager.hpp>
#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputMap.hpp>
#include <Zen/Engine/Input/I_KeyEvent.hpp>

#include <Zen/Engine/Navigation/I_NavigationManager.hpp>
#include <Zen/Engine/Navigation/I_NavigationService.hpp>

#include <Zen/Engine/Client/I_GameClient.hpp>

#include <Zen/Engine/Core/I_ActionMap.hpp>
#include <Zen/Starter/Base/BaseCommon/I_BaseGameObject.hpp>

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
#else
// For setting the processor affinity
#include <sched.h>

// For getpid();
#include <unistd.h>

#include <SDL/SDL.h>
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
BaseClient::BaseClient()
:   m_pluginManager(Zen::Plugins::I_PluginManager::getSingleton())
,   m_extensionRegistry(Zen::Plugins::I_ExtensionRegistry::getSingleton())
,   m_pParent(NULL)
,   m_pContext(NULL)
,   m_pView(NULL)
,   m_pRenderingCanvas(NULL)
,   m_quitting(false)
,   m_game(I_BaseGame::getSingleton())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseClient::~BaseClient()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseClient::setWindowHandle(Client::I_GameClient::WindowHandle_type _pParent)
{
    m_pParent = _pParent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
BaseClient::getEnvironment(const std::string& _name) const
{
    return Utility::I_EnvironmentHandler::getDefaultHandler().getEnvironment(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Core::I_GameGroup&
BaseClient::getRootGroup()
{
    return m_game.getRootGroup();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
Client::I_GameClient::WindowHandle_type
BaseClient::getHandle()
{
    return m_pParent;
}
#endif


#ifdef _WIN32
static LARGE_INTEGER sm_frequency;
static LARGE_INTEGER sm_lastTick;
static void initTimer()
{
    // Due to issues with high resolution timer, run this thread on a single processor
    //DWORD_PTR PROCESSOR_ZERO = 1;
    //::SetThreadAffinityMask(::GetCurrentThread(), PROCESSOR_ZERO); // << Uncomment this line for a nasty hacky fix for amd processors
    ::QueryPerformanceFrequency(&sm_frequency);
    ::QueryPerformanceCounter(&sm_lastTick);
}

static void forceProcessorAffinity()
{
    DWORD_PTR PROCESSOR_ZERO = 1;
    ::SetThreadAffinityMask(::GetCurrentThread(), PROCESSOR_ZERO);
}

// Increment _now by the number of ticks passed and return the difference (in seconds)
static double incrementTimer()
{
    const LARGE_INTEGER lastTick = sm_lastTick;
    ::QueryPerformanceCounter(&sm_lastTick);

    return double(sm_lastTick.QuadPart - lastTick.QuadPart) / (double)(sm_frequency.QuadPart);
}

#else
static unsigned int sm_lastTick;
static void initTimer()
{
    sm_lastTick = SDL_GetTicks();
}

static double incrementTimer()
{
    unsigned int lastTick = sm_lastTick;
    sm_lastTick = SDL_GetTicks();

    return double(sm_lastTick - lastTick) / (double)1000.0;
}

static void forceProcessorAffinity()
{
    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);

    pid_t pid = getpid();
    CPU_SET(0, &cpuSet);

    sched_setaffinity(pid, sizeof(cpuSet), &cpuSet);
}

#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseClient::run()
{
    getInputService().resumeEvents();

    FrameDelta_type delta = 0.0;

    // Get the current time
    initTimer();

    while(!m_quitting)
    {

#ifdef WIN32
        MSG message;
        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
#else
        // Lets just use SDL to pump the messages.
        // Is this legal?  Seems way too easy.
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            // Quit?
            if (event.type == SDL_QUIT)
            {
                m_quitting = true;
                continue;
            }
        }
#endif // WIN32
        // Increment the timer here; this gives us the least amount of time after
        // handling the input
#define CLAMP

#ifdef CLAMP
        double elapsed = incrementTimer();

        // HACK Come up with a better way to determine if forceProcessorAffinity()
        // is required.  Possibly check the timer for a few seconds during initialization
        // and make sure it never goes < 0.0f.
        if(elapsed < 0.0f)
        {
            forceProcessorAffinity();
            elapsed = 0.08f;
        }

        // Clamp to 200 fps
        if(elapsed < (1.0 / 60.0))
        {
            int sleepTime = static_cast<int>(1.0 / 60.0 - elapsed);
            if (sleepTime >= 0)
            {
                Threading::I_Thread::sleepForMilliseconds(sleepTime / 2);
                elapsed += incrementTimer();
            }
        }
#else
        double elapsed = incrementTimer();
#endif

        // HACK Continued from above.
        if(elapsed < 0.0f)
        {
            forceProcessorAffinity();
            elapsed = 0.08f;
        }

        /// Give the input service a chance to process events
        m_pInputService->processEvents();

        onBeforeFrameRenderedEvent(elapsed);
        m_pRenderingCanvas->renderScene();
        onAfterFrameRenderedEvent(elapsed);

        Threading::I_Thread::sleepForMilliseconds(0);
    }

    // TODO Shut down properly

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ObjectReference*
script_getInputMap(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    //Zen::Scripting::ObjectReference<Client::I_GameClient, Client::I_GameClient*>* pObject = dynamic_cast<Zen::Scripting::ObjectReference<Client::I_GameClient, Client::I_GameClient*>*>(_pObject);

    return I_BaseGameClient::getSingleton().getInputMap().getScriptObject();
}

static Zen::Scripting::I_ObjectReference*
script_getGame(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    return I_BaseGame::getSingleton().getScriptObject();
}

#if 0   // Implementation deferred
static Zen::Scripting::I_ObjectReference*
script_getPhysicsZone(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Zen::Scripting::ObjectReference<BaseClient>* pObject = dynamic_cast<Zen::Scripting::ObjectReference<BaseClient>*>(_pObject);
}
#endif


#if 0 // Deprecated hack.  Use a I_GameObject to implement this.
// HACK!
typedef Zen::Memory::managed_ptr<Rendering::I_RenderableResource>   pRenderableResource_type;
static std::list<pRenderableResource_type> todoRemoveMeList;

static void
script_loadResource(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Engine::Core::I_BaseGameObject::ScriptObjectReference_type* pObject = dynamic_cast<Engine::Core::I_BaseGameObject::ScriptObjectReference_type*>(_pObject);

    std::string resource = boost::any_cast<std::string>(_parms[0]);

    BehavioredGameObject* pGameObject = dynamic_cast<BehavioredGameObject*>(pObject->getObject());


    Zen::Engine::Resource::I_ResourceManager::config_type config;

    config["fileName"] = resource;

    // two meshes of the same name?  This scheme won't work :(
    std::stringstream name;
    name << pGameObject->getName() << ":" << resource;
    config["label"] = name.str();

    config["type"] = "entity";

    pRenderableResource_type pRenderable =
        pGameObject->getClient().getRenderingResourceService().loadResource(config).as<pRenderableResource_type>();

    pGameObject->getSceneNode()->attachObject(*pRenderable.get());

    // HACK This goes out of scope and nothing retains a reference to it. :(
    todoRemoveMeList.push_back(pRenderable);
}

static void
script_setMaterialName(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Engine::Core::I_BaseGameObject::ScriptObjectReference_type* pObject = dynamic_cast<Engine::Core::I_BaseGameObject::ScriptObjectReference_type*>(_pObject);

    std::string materialName = boost::any_cast<std::string>(_parms[0]);

    BehavioredGameObject* pGameObject = dynamic_cast<BehavioredGameObject*>(pObject->getObject());

    const int nCount = pGameObject->getSceneNode()->getNumAttachedObjects();
    Rendering::I_Renderable* pRenderable = dynamic_cast<Rendering::I_Renderable*>(pGameObject->getSceneNode()->getAttachedObject(nCount - 1));

    if (pRenderable)
    {
        pRenderable->setMaterialName(materialName);
    }

}


#endif // deprecated

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initScriptEngine(const std::string& _type)
{
    // was "python"

    m_pScriptEngine = Scripting::I_ScriptingManager::getSingleton().createScriptEngine(_type);

    if (m_pScriptEngine == NULL)
    {
        std::cout << "Error loading script engine: " << std::endl;
        return false;
    }

    // Iniitalize the BaseGame script
    m_game.initScriptModule(m_pScriptEngine);
    m_game.initScriptTypes();

    m_pModule = m_pScriptEngine->createScriptModule("BaseClient", "Game Client");

    m_pGameClientScriptType = m_pModule->createScriptType("GameClient", "Game Client", 0);
    m_pGameClientScriptType->addMethod("getInputMap", "Get the InputMap",
        &script_getInputMap);
    m_pGameClientScriptType->addMethod("getGame", "Get the main Game object",
        &script_getGame);

    // Register the script engine with the Rendering Manager
    if (m_pScriptEngine.isValid())
    {
        Rendering::I_RenderingManager::getSingleton().registerDefaultScriptEngine(m_pScriptEngine);
        Input::I_InputServiceManager::getSingleton().registerDefaultScriptEngine(m_pScriptEngine);
    }


    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initSceneService(const std::string& _type)
{
    // Was "ogre"

    m_pSceneService = Rendering::I_SceneManager::getSingleton().create(_type);

    pScriptModule_type const pModule = Rendering::I_RenderingManager::getSingleton().getDefaultScriptModule();

    if (pModule.isValid())
    {
        new Scripting::ObjectReference<Rendering::I_SceneService>(pModule,
            pModule->getScriptType(m_pSceneService->getScriptTypeName()),
            m_pSceneService, "sceneService");
    }

    // create the canvas
    std::cout << "creating Canvas" << std::endl;
    m_pView->initCanvas();
    Rendering::I_Canvas* pCanvas = &m_pView->getCanvas();

    if (pCanvas != NULL)
    {
        m_pRenderingCanvas = dynamic_cast<Rendering::I_RenderingCanvas*>(pCanvas);
        if (m_pRenderingCanvas)
        {
            // TODO Is onNeedRedraw no longer necessary?
            // It's normally used when the canvas is resized in a tool that isn't always
            // redrawing the screen, but since our game client redraws the screen constantly
            // then probably this isn't needed.
            //m_pRenderingCanvas->onNeedRedraw.connect(boost::bind(&BaseClient::onNeedRedraw, this, _1));

            return true;
        }
        else
        {
            std::cout << "dynamic cast failed." << std::endl;
            return false;
        }
    }
    else
    {
        std::cout << "Error creating canvas." << std::endl;
    }

    return m_pSceneService != NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
bool
BaseClient::initCameraService(const std::string& _type)
{
    // Was "ogre"

    m_pCameraService = Camera::I_CameraManager::getSingleton().create(_type);

#if 0
    pScriptModule_type const pModule = Camera::I_CameraManager::getSingleton().getDefaultScriptModule();

    if (pModule.isValid())
    {
        new Scripting::ObjectReference<Camera::I_CameraService>(pModule,
            pModule->getScriptType(m_pCameraService->getScriptTypeName()),
            m_pCameraService, "cameraService");
    }
#endif

    if (!m_pCameraService.isValid())
        return false;

    m_pCamera = m_pCameraService->createCameraController("orbit");
    m_pRenderingCanvas->setCamera(m_pCamera);

    return true;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initRenderingService(const std::string& _type, const std::string& _windowName, unsigned _width, unsigned _height)
{
    // Was "ogre"

    m_pRenderingService = Rendering::I_RenderingManager::getSingleton().create(_type);

    if (m_pRenderingService.isValid())
    {
        // Create the context
        // TODO Eventually we may need to pass in an already created context.

        std::cout << "Creating Context" << std::endl;
        m_pContext = m_pRenderingService->createContext(m_pParent);

        // Create the view
        std::cout << "Creating View" << std::endl;
        m_pView = m_pRenderingService->createView(*m_pContext, _windowName, _width, _height);

        return true;
    }

    // TODO Log Error
    std::cout << "Error creating rendering service" << std:: endl;

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initRenderingResourceService(const std::string& _type)
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
    m_pRenderingResourceService =
        Resource::I_ResourceManager::getSingleton().create(_type, config);

    pScriptModule_type const pModule = Resource::I_ResourceManager::getSingleton().getDefaultScriptModule();

    if (pModule.isValid())
    {
        new Scripting::ObjectReference<Resource::I_ResourceService>(pModule,
            pModule->getScriptType(m_pRenderingResourceService->getScriptTypeName()),
            m_pRenderingResourceService, "renderingResourceService");
    }

    return true;
}



//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initWaterService(const std::string& _type)
{
    World::I_WorldManager::config_type config;

    // Create the service
    m_pWaterService =
        Zen::Engine::World::I_WorldManager::getSingleton().createWaterService(_type, config);

    if (m_pWaterService == NULL)
    {
        std::cout << "Error: couldn't create WaterService" << std::endl;
        return false;
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initTerrainService(const std::string& _type)
{
    World::I_WorldManager::config_type config;

    // Create the service
    m_pTerrainService =
        Zen::Engine::World::I_WorldManager::getSingleton().createTerrainService(_type, config);

    if (m_pTerrainService == NULL)
    {
        std::cout << "Error: couldn't create TerrainService" << std::endl;
        return false;
    }

    // rendering service is required
    assert(m_pRenderingService.isValid());
    m_pTerrainService.get()->setRenderingService(m_pRenderingService);
    // TODO verify that rendering resource service is valid
    m_pTerrainService.get()->setRenderingResourceService(m_pRenderingResourceService);

    // physics service is not required
    if (m_game.getPhysicsService().isValid())
    {
        m_pTerrainService.get()->setPhysicsService(m_game.getPhysicsService());
        // TODO verify that physics resource service is valid
        m_pTerrainService.get()->setPhysicsResourceService(m_game.getPhysicsResourceService());
    }
    else
    {
        std::cout << "Warning: initTerrainService() has no Physics Service setup, proceeding anyway." << std::endl;
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initSkyService(const std::string& _type)
{
    World::I_WorldManager::config_type config;

    // Create the service
    m_pSkyService =
        Zen::Engine::World::I_WorldManager::getSingleton().createSkyService(_type, config);

    if ( !m_pSkyService.isValid() )
    {
        std::cout << "Error: couldn't create SkyService" << std::endl;
        return false;
    }

    // rendering service is required
    assert(m_pRenderingService.isValid());
    m_pSkyService.get()->setRenderingService(m_pRenderingService);
    // TODO verify that rendering resource service is valid
    m_pSkyService.get()->setRenderingResourceService(m_pRenderingResourceService);

    // physics service is not required
    if (m_game.getPhysicsService().isValid())
    {
        m_pSkyService.get()->setPhysicsService(m_game.getPhysicsService());
        // TODO verify that physics resource service is valid
        m_pSkyService.get()->setPhysicsResourceService(m_game.getPhysicsResourceService());
    }
    else
    {
        std::cout << "Warning: initSkyService() has no Physics Service setup, proceeding anyway." << std::endl;
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initWidgetService(const std::string& _type)
{
    Widgets::I_WidgetManager::config_type config;

    // Create the service
    m_pWidgetService =
        Zen::Engine::Widgets::I_WidgetManager::getSingleton().create(_type, config);

    if ( !m_pWidgetService.isValid() )
    {
        std::cout << "Error: couldn't create WidgetService" << std::endl;
        return false;
    }

    // rendering service is required
    assert(m_pView != NULL && m_pInputService.isValid());

    m_pWidgetService->initialise(*m_pView, *m_pInputService);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initSoundService(const std::string& _type)
{
    Sound::I_SoundManager::config_type config;

    // Create the service
    m_pSoundService =
        Zen::Engine::Sound::I_SoundManager::getSingleton().create(_type, config);

    if (m_pSoundService == NULL)
    {
        std::cout << "Error: couldn't create SoundService" << std::endl;
        return false;
    }
    else std::cout << "Sound Service Initialized" << std::endl;

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
BaseClient::initInputService(const std::string& _type)
{
    // TODO Handle multiple types
    // was "keyboard"

    Input::I_InputServiceManager::config_type config;

    std::ostringstream window;

    // TODO This is not 64 bit portable
    window << (unsigned long)m_pContext->getWindow();

    config["WINDOW"] = window.str();

    m_pInputService =
        Input::I_InputServiceManager::getSingleton().create(_type, config);

    m_pMainInputMap = m_pInputService->createInputMap("main");

    // Register the script engine with the Rendering Manager
    if (m_pScriptEngine.isValid())
    {
        Input::I_InputServiceManager::getSingleton().registerDefaultScriptEngine(m_pScriptEngine);
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ScriptEngine&
BaseClient::getScriptEngine()
{
    return *(m_pScriptEngine.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Client::I_GameClient::WindowHandle_type
BaseClient::getWindowHandle()
{
    return m_pParent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
void
BaseClient::onNeedRedraw(bool _unused)
{

}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Rendering::I_SceneService&
BaseClient::getSceneService()
{
    return *m_pSceneService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Rendering::I_RenderingCanvas&
BaseClient::getRenderingCanvas()
{
    return *m_pRenderingCanvas;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Rendering::I_RenderingService&
BaseClient::getRenderingService()
{
    return *m_pRenderingService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
Camera::I_Camera&
BaseClient::getCamera()
{
    return *m_pCamera;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
Camera::I_CameraService&
BaseClient::getCameraService()
{
    return *m_pCameraService;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Resource::I_ResourceService&
BaseClient::getRenderingResourceService()
{
    return *m_pRenderingResourceService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
World::I_WaterService&
BaseClient::getWaterService()
{
    assert(m_pWaterService.isValid());
    return *m_pWaterService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
World::I_TerrainService&
BaseClient::getTerrainService()
{
    assert(m_pTerrainService.isValid());
    return *m_pTerrainService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
World::I_SkyService&
BaseClient::getSkyService()
{
    assert(m_pSkyService.isValid());
    return *m_pSkyService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Widgets::I_WidgetService&
BaseClient::getWidgetService()
{
    assert(m_pWidgetService.isValid());
    return *m_pWidgetService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Input::I_InputService&
BaseClient::getInputService()
{
    return *m_pInputService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Input::I_InputMap&
BaseClient::getInputMap()
{
    return *(m_pMainInputMap.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseClient::pScriptModule_type
BaseClient::getScriptModule()
{
    return m_pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BaseClient::pScriptType_type
BaseClient::getGameClientScriptType()
{
    return m_pGameClientScriptType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
BaseClient::getScriptGameGroup()
{
    return m_pScriptMainGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Core::I_BehaviorService&
BaseClient::getBehaviorService()
{
    return m_game.getBehaviorService();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseClient::quit(boost::any& _parameter)
{
    m_quitting = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
bool
BaseClient::init()
{
    // Do this first
    initScriptEngine();

    initSceneService();
    initRenderingService();
    initResourceService();
    initPhysicsService();
    initInputService();
    initMoveService();

    // Execute the default scripts
    boost::filesystem::path scriptPath;
    boost::filesystem::path gameRoot(getEnvironment("gameRoot"));
    boost::filesystem::path starterRoot(getEnvironment("starterRoot"));

    // Execute the BaseClient script
    scriptPath = starterRoot / "Scripts" / "initClient.py";
    scriptPath.normalize();
    m_pScriptEngine->executeScript(scriptPath.string());

    // Execute the game specific script
    scriptPath = gameRoot / getEnvironment("gameName") / "scripts" / "initClient.py";
    scriptPath.normalize();
    m_pScriptEngine->executeScript(scriptPath.string());

    createActions(m_actionMap);
    createDefaultMapping();
    createScene();

    return true;
}
#endif // 0

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
void
BaseClient::initMoveService()
{
    //
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BaseClient::activateScriptModules()
{
    m_pModule->activate();

    m_game.createScriptObjects();
}

#if 0
void
BaseClient::beforeRender(double _elapsedTime)
{
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
void
BaseClient::afterRender()
{
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
