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
#include "GameClient.hpp"
#include "GUIManager.hpp"
#include "GameObject.hpp"
#include "GameGrid.hpp"
#include "GridObject.hpp"
#include "SimpleBrain.hpp"
#include "CreepManager.hpp"

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Ray.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Core/I_GameGroup.hpp>
#include <Zen/Engine/Core/I_ActionMap.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

#include <Zen/Engine/Physics/I_PhysicsShape.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Rendering/I_Light.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>
#include <Zen/Engine/Rendering/I_Camera.hpp>

#include <Zen/plugins/ZOgre/I_OgreRenderingCanvas.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputMap.hpp>

#include <Zen/Engine/Camera/I_CameraManager.hpp>

#include <Zen/Starter/Base/BaseCommon/I_BaseGameObject.hpp>

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
:   m_base(Engine::Base::I_BaseGameClient::getSingleton())
,   m_game(Engine::Base::I_BaseGame::getSingleton())
,   m_pScriptObject(NULL)
,   m_pRoot(NULL)
,   m_pRenderWindow(NULL)
,   m_pGUIManager(NULL)
,   m_moveDirection(0.0f, 0.0f, 0.0f)
,   m_zoomAmount(0.0f)
,   m_pGameGrid(NULL)
,   m_pCreepManager(NULL)
,   m_pBrain(NULL)
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
Scripting::I_ObjectReference*
GameClient::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::init()
{
#ifdef _DEBUG
    std::string appWindowTitle("Zen Cyber Tower Defense - DEBUG");
#else
    std::string appWindowTitle("Zen Cyber Tower Defense");
#endif
    unsigned windowWidth = 1024;
    unsigned windowHeight = 768;

    m_base.initRenderingService("ogre", appWindowTitle.c_str(), windowWidth, windowHeight);

    // Initialize the ZOgre rendering, scene,  and resource services
    // TODO Remove m_pRenderWindow.  We need it for now since the ZCrazyEddie plugin is
    //      incomplete.
    m_pRoot = Ogre::Root::getSingletonPtr();
    m_pRenderWindow = dynamic_cast<Ogre::RenderWindow*>(m_pRoot->getRenderTarget(appWindowTitle.c_str()));
    m_base.initSceneService("ogre");
    m_base.initRenderingResourceService("ogre");

    // Initialize teh ZNewton physics and resource services
    game().initPhysicsService("ZNewton");
    setupPhysicsMaterials();
    game().initPhysicsResourceService("ZNewton");

    // Initialize the keyboard (and mouse in this case since the ZInput plugin handles both)
    base().initInputService("keyboard");

    // Initialize the widget service
    base().initWidgetService("cegui");

    // Initialize the ZMicroPather A* navigation service.
    game().initNavigationService("upather");

    // Set the window size of the input service so that mouse events are scaled properly.
    base().getInputService().setWindowSize(m_base.getRenderingCanvas().getWidth(), m_base.getRenderingCanvas().getHeight());

    base().onBeforeFrameRenderedEvent.connect(boost::bind(&GameClient::beforeRender, this, _1));

    setupResourcePaths();

    // Initialize the widget service
    m_pGUIManager.reset(new GUIManager(*this));

    // Create the Game Grid
    m_pGameGrid = new GameGrid(*this, 26, 26);

    // Create the Creep Manager
    m_pCreepManager = new CreepManager(*m_pGameGrid);

    createScriptTypes();

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
GameClient::setupResourcePaths()
{
    boost::filesystem::path rootPath;

    // Mesh resources and standins
    rootPath = boost::filesystem::system_complete(boost::filesystem::path("resources/Meshes/towers", boost::filesystem::native)).normalize();
    m_base.getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);

    rootPath = boost::filesystem::system_complete(boost::filesystem::path("resources/Meshes/misc", boost::filesystem::native)).normalize();
    m_base.getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);

    rootPath = boost::filesystem::system_complete(boost::filesystem::path("standins/Meshes", boost::filesystem::native)).normalize();
    m_base.getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);

    // GUI resources and standins
    rootPath = boost::filesystem::system_complete(boost::filesystem::path("resources/GUI", boost::filesystem::native)).normalize();
    m_base.getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);

    rootPath = boost::filesystem::system_complete(boost::filesystem::path("standins/GUI", boost::filesystem::native)).normalize();
    m_base.getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "General", false);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::run()
{
    m_base.run();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// HACK This is because I_BaseGameClient has no way to get us to GameClient*
static GameClient* sm_pTheOnlyGameClient;
void
script_setDifficultyLevel(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    int difficultyLevel = (int)boost::any_cast<float>(_parms[0]);

    sm_pTheOnlyGameClient->setDifficultyLevel(difficultyLevel);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createScriptTypes()
{
    sm_pTheOnlyGameClient = this;

    // Register more methods to the gameClient script type
    pScriptType_type pGameClientScriptType = m_base.getGameClientScriptType();

    pGameClientScriptType->addMethod("setDifficultyLevel", "Set the difficulty level of this game.", script_setDifficultyLevel);

    // Register additional script types
    // TODO These should be static
    m_pGUIManager->initScriptType();
    m_pGameGrid->initScriptType();

    CreepManager::initScriptType(*this);
    GameObject::initScriptType(*this);

    // Tell m_base we're done.  After the modules are activated then you cannot
    // modify any script types.
    m_base.activateScriptModules();

    // Create the script object for this game client
    m_pScriptObject = new ScriptObjectReference_type
        (base().getScriptModule(), base().getScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());

    // Create the other script objects
    m_pGUIManager->createScriptObject();
    m_pGameGrid->createScriptObject();
    m_pCreepManager->createScriptObject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createScene()
{
    // Create the light
    Zen::Engine::Rendering::I_SceneService::pLight_type pLight = m_base.getSceneService().createLight("default", "Light");
    pLight->setPosition(0.0f, 0.0f, 50.0f);

    // Load the background scene
    GameObject* pGameObject = new GameObject(game().getRootGroup(), "walls");
    pGameObject->loadRenderable("Walls.mesh");
    pGameObject->base().setPosition(0, 0, 0, false);
    m_gameObjects.push_back(pGameObject);

    pGameObject = new GameObject(game().getRootGroup(), "walltops");
    pGameObject->loadRenderable("WallTops.mesh");
    pGameObject->base().setPosition(0, 0, 0, false);
    m_gameObjects.push_back(pGameObject);

    pGameObject = new GameObject(game().getRootGroup(), "plane");
    pGameObject->loadRenderable("Plane.mesh");
    pGameObject->base().setPosition(0, 0, 0, false);
    m_gameObjects.push_back(pGameObject);

    // Set the camera in the correct location
    Zen::Engine::Rendering::I_RenderingCanvas& canvas = base().getRenderingCanvas();
    Zen::Engine::Rendering::I_Camera& camera = canvas.selectCamera("default");
    camera.setPosition(7.296, 0.0262, 30.0);
    camera.setNearClipDistance(0.1f);
    camera.setFarClipDistance(150.0f);
    camera.setAspectRatio((Zen::Math::Real)canvas.getWidth() / (Zen::Math::Real)canvas.getHeight());
    camera.setHorizontalFOV(Zen::Math::Degree(60));

    // Probably this is not the best place to put this.
    m_pClickWorld = game().getPhysicsService()->createWorld();

    // Create a physics shape for the click plane.
    m_pClickPlane = m_pClickWorld->createShape();

    // Get the game grid extents
    Math::Point3 minPt, maxPt;
    assert(m_pGameGrid != NULL);
    m_pGameGrid->getLocalExtents(minPt, maxPt);

    // Initialize the click plane physics shape
    m_pClickPlane->initBoxShape(
        maxPt.m_x - minPt.m_x,
        maxPt.m_y - minPt.m_y,
        1.0f
    );

    // Set the click plane shape such that it's 
    // facing surface aligns with the game grid.
    m_pClickPlane->setPosition(
        Math::Point3(
            0.0f,
            0.0f,
            0.5f
        )
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createActions()
{
    // Create some actions
    game().getActionMap().createAction("Quit", boost::bind(&Engine::Base::I_BaseGameClient::quit, &m_base, _1));

    game().getActionMap().createAction("Left", boost::bind(&GameClient::moveLeft, this, _1));
    game().getActionMap().createAction("Right", boost::bind(&GameClient::moveRight, this, _1));
    game().getActionMap().createAction("Up", boost::bind(&GameClient::moveUp, this, _1));
    game().getActionMap().createAction("Down", boost::bind(&GameClient::moveDown, this, _1));

    game().getActionMap().createAction("Zoom", boost::bind(&GameClient::zoom, this, _1));
    game().getActionMap().createAction("Print", boost::bind(&GameClient::print, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createDefaultMapping()
{
    // Map some keys to actions
    base().getInputMap().mapKeyInput("q", game().getActionMap()["Quit"]);

    Zen::Engine::Input::I_InputMap& inputMap = base().getInputMap();
    Zen::Engine::Core::I_ActionMap& actionMap = game().getActionMap();

    inputMap.mapKeyInput("a", actionMap["Left"]);
    inputMap.mapKeyInput("d", actionMap["Right"]);
    inputMap.mapKeyInput("w", actionMap["Up"]);
    inputMap.mapKeyInput("s", actionMap["Down"]);

    inputMap.mapKeyInput("x", actionMap["Zoom"]);
    inputMap.mapKeyInput("p", actionMap["Print"]);
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
    game().getPhysicsService()->stepSimulation(_elapsedTime);

    // Only do this once every 30 frames.
    static int frameCount = 0;

    // Only do this if the GameGrid cursor is enabled
    if (m_pGameGrid->getCursorEnabled())
    {
        queryCursor();
    }

    // Update the creep manager
    m_pCreepManager->tick(_elapsedTime);

    //static long cash = 12345678;
    //m_pGUIManager->setCash(cash++);
    // This updates all physics worlds we've created:
    game().getPhysicsService()->stepSimulation(_elapsedTime);

    // Change the camera position
    Zen::Engine::Rendering::I_RenderingCanvas& canvas = base().getRenderingCanvas();
    Zen::Engine::Rendering::I_Camera& camera = canvas.selectCamera("default");

    Zen::Math::Point3 position;
    position = camera.getPosition();
    position += m_moveDirection * _elapsedTime * 5.0f;
    position.m_z += m_zoomAmount * _elapsedTime * 10.0f;

    camera.setPosition(position.m_x, position.m_y, position.m_z);

    boost::any elapsedTime((Math::Real)_elapsedTime);
    game().getActionMap()["onTick"]->dispatch(elapsedTime);
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

        m_pGameGrid->handleClickEvent();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::queryCursor()
{
    struct ClickCollisionVisitor
    :	public Zen::Engine::Physics::I_PhysicsWorld::I_CollisionVisitor
    {
        virtual bool visit(pPhysicsShape_type _Shape, Math::Vector3 _normal, Math::Real _distance)
        {
            // TODO Compute the position of the intersection by
            // using m_ray and _distance, and then pass the results
            // to updateGridCursor()
            Math::Point3 intersection = m_ray.getOrigin() + m_ray.getDirection() * _distance;

            m_gameClient.getGameGrid().updateGridCursor(
                intersection
            );

            // Only check one collision.
            return false;
        }

        virtual bool filter(pPhysicsShape_type _filterType)
        {
            // Check everything
            return true;
        }

        ClickCollisionVisitor(GameClient& _gameClient, Math::Ray& _ray)
        :   m_gameClient(_gameClient)
        ,   m_ray(_ray)
        {
        }

    private:
        GameClient&	m_gameClient;
        Math::Ray&	m_ray;

    };	// struct CursorCollisionVisitor

    int x, y;
    m_pGUIManager->getMousePosition(x, y);

    // Get the ray by querying the rendering service

    Zen::Engine::Rendering::I_Camera& 
        camera = base().getRenderingCanvas().getCurrentCamera();

    Math::Ray ray(
        camera.getViewportRay(x,y)
    );

    ClickCollisionVisitor visitor(*this, ray);

    // TODO Use the click world to do the ray cast
    m_pClickWorld->rayCast(
        ray,
        40.0f,	// Placeholder for now
        visitor
    );


}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameGrid&
GameClient::getGameGrid() const
{
    return *m_pGameGrid;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::setDifficultyLevel(int _difficultyLevel)
{
    // TODO Implement based on the difficulty level

    if (m_pBrain == NULL)
    {
        std::cout << "The scarecrow needs a brain. Creating one...\n";
        m_pBrain = new SimpleBrain(*m_pGameGrid);
        m_pBrain->init();

        if( m_pCreepManager != NULL )
        {
            m_pCreepManager->setBrain(*m_pBrain);
        }
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
