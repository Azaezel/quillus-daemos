//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen BatBall
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
#include "PongGameClient.hpp"

#include "WallObject.hpp"
#include "BallObject.hpp"
#include "BatObject.hpp"

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Core/I_GameGroup.hpp>
#include <Zen/Engine/Core/I_ActionMap.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

//#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>
#include <Zen/Engine/Physics/I_PhysicsShape.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Rendering/I_Light.hpp>
#include <Zen/Engine/Rendering/I_Camera.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputMap.hpp>

//#include <Zen/Engine/Camera/I_CameraManager.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/bind.hpp>

#include <stddef.h>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Pong {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PongGameClient::PongGameClient(WindowHandle_type _pParent)
:   m_baseClient(Engine::Base::I_BaseGameClient::getSingleton())
,   m_baseGame(Engine::Base::I_BaseGame::getSingleton())
,   m_pScriptObject(NULL)
{
    baseClient().setWindowHandle(_pParent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PongGameClient::~PongGameClient()
{
    //m_pCameraController.reset();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const PongGameClient::WindowHandle_type
PongGameClient::getHandle() const
{
    return baseClient().getWindowHandle();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ObjectReference*
script_createWall(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _args)
{
    Zen::Engine::Core::I_GameGroup* const pGameGroup = dynamic_cast<Zen::Engine::Core::I_GameGroup::ScriptObjectReference_type*>(_pObject)->getObject();

    // Parms
    std::string name = boost::any_cast<std::string>(_args[0]);
    Math::Real dx = boost::any_cast<Math::Real>(_args[1]);
    Math::Real dy = boost::any_cast<Math::Real>(_args[2]);
    Math::Real dz = boost::any_cast<Math::Real>(_args[3]);

    // Oops, this is being leaked... I'll fix it later
    WallObject* pWall = new WallObject(*pGameGroup, name, dx, dy, dz);

    pWall->base().getPhysicsShape()->initBoxShape(dx, dy, dz);
    pWall->base().getPhysicsShape()->setMass(0.0f);
    pWall->base().setScale(dx / 100.0f, dy / 100.0f, dz / 100.0f);

    return pWall->base().getScriptObject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
PongGameClient::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
PongGameClient::init()
{
    baseClient().initRenderingService("ogre");
    baseClient().initSceneService("ogre");
    baseClient().initRenderingResourceService("ogre");

    Zen::Engine::Rendering::I_RenderingCanvas& canvas = baseClient().getRenderingCanvas();
    // Select the default camera.  No need to create it since it always exists
    Zen::Engine::Rendering::I_Camera& camera = canvas.selectCamera("default");

    // TODO Need to implement something like this
   	//canvas.setBackgroundColour(Zen::Engine::Rendering::ColorValue(0, 0, 0.2f));

    camera.setNearClipDistance(0.01f);
    camera.setAspectRatio((Zen::Math::Real)canvas.getWidth() / (Zen::Math::Real)canvas.getHeight());
    camera.setHorizontalFOV(Zen::Math::Degree(60));
    camera.setPosition(0.0, 200.0, 100.0);


    baseGame().initPhysicsService("ZNewton");
    setupPhysicsMaterials();
    baseGame().initPhysicsResourceService("ZNewton");
    baseClient().initInputService("keyboard");

    // Register additional script types
    pScriptType_type pGameClientScriptType = baseClient().getGameClientScriptType();

    pScriptType_type pGameGroupScriptType = baseGame().getGameGroupScriptType();
    pGameGroupScriptType->addMethod("createWall", "createWall()", script_createWall);

    // Tell baseClient() we're done
    baseClient().activateScriptModules();

    // Create the script object
    m_pScriptObject = new ScriptObjectReference_type
        (baseClient().getScriptModule(), baseClient().getScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());

    createActions();
    createDefaultMapping();

    createBehaviors();

    createScene();


    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::run()
{
    baseClient().run();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::createScene()
{
#if 1   // This really should be implemented in script
    boost::filesystem::path rootPath = boost::filesystem::system_complete
        (
        //boost::filesystem::path("D:\\dev\\Zen\\examples\\taBBall\\resources", boost::filesystem::native)
        boost::filesystem::path("resources", boost::filesystem::native)
        ).normalize();

    baseClient().getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "Pong", true);
#endif

#if 0   // Oops, this doesn't work, but it used to work
    boost::filesystem::path rootPath = boost::filesystem::system_complete
        (
            boost::filesystem::path(getEnvironment("gameRoot").c_str(), boost::filesystem::native)
        ).normalize();

    rootPath = rootPath / "Pong" / "resources";
    getResourceService().addResourceLocation(rootPath.string(), "FileSystem", "Pong", true);
#endif

    // Create the world
    //m_pPhysicsWorld = baseClient().getPhysicsService().createWorld();

    //Zen::Engine::Physics::I_PhysicsWorld::pPhysicsShape_type pShape;

#if 0
    // re-usable variables
    WallObject* pWall;

    // Left Wall
    pWall = new WallObject(baseClient().getRootGroup(), "WallLeft", 5.0f, 145.0f, 10.0f);
    pWall->baseClient().setPosition(-95, 0, 0, true);

    // Right Wall
    pWall = new WallObject(baseClient().getRootGroup(), "WallRight", 5.0f, 145.0f, 10.0f);
    pWall->baseClient().setPosition(95, 0, 0, true);

    // Bottom Wall
    pWall = new WallObject(baseClient().getRootGroup(), "WallBottom", 195.0f, 5.0f, 10.0f);
    pWall->baseClient().setPosition(0, -70, 0, true);

    // Top Wall
    pWall = new WallObject(baseClient().getRootGroup(), "WallTop", 195.0f, 5.0f, 10.0f);
    pWall->baseClient().setPosition(0, 70, 0, true);

#endif

    // Create the light
    Zen::Engine::Rendering::I_SceneService::pLight_type pLight = baseClient().getSceneService().createLight("default", "Light");
    pLight->setPosition(0.0f, 0.0f, 50.0f);

    // Create the ball
    // TODO Why are we using a BaseObject instead of BallObject here?
    BaseObject* pObj = new BaseObject(baseGame().getRootGroup(), "ball");
    pObj->loadRenderable("sphere.mesh", "ball");

    // This must be done before any behaviors are activate (setScale in this case)
    Zen::Engine::Physics::I_PhysicsWorld::pPhysicsShape_type pShape = pObj->base().getPhysicsShape();

    pObj->base().setScale(0.05f, 0.05f, 0.05f);

    pShape->initOvoidShape(5.0f, 5.0f, 5.0f);

    pObj->base().setPosition(25.0f, 0.0f, 0, true);

    pShape->setMass(100.0f);
    pShape->setVelocity(Zen::Math::Vector3(100.0f, 80.0f, 0.0f));
    pShape->setLinearDamping(0.0f);
    pShape->setAutoFreeze(false);
    pShape->setAngularMomentum(Zen::Math::Vector3(0.0f, 0.0f, 0.0f));
    pShape->setAngularDamping(Zen::Math::Vector3(1.0f, 1.0f, 1.0f));
    pShape->setAdvancedCollisionPrediction(true);
    pObj->base().setBehaviors(m_pBallBehaviors);

    // Create the bat (for now single player; for multi player this is different)
    m_pPlayer = new BatObject(baseGame().getRootGroup(), "Player1", 30.0f, 2.0f, 10.0f);
    m_pPlayer->base().getPhysicsShape()->setMass(1000.0f);
    m_pPlayer->base().getPhysicsShape()->setAdvancedCollisionPrediction(true);
    m_pPlayer->base().setPosition(0, -63, 0, true);

    m_pPlayer->base().setBehaviors(m_pBatBehaviors);

    Zen::Engine::Rendering::I_RenderingCanvas& canvas = baseClient().getRenderingCanvas();

    // Select the default camera.  No need to create it since it always exists
    Zen::Engine::Rendering::I_Camera& camera = canvas.selectCamera("default");

    camera.setPosition(0.0, 0.0, 200.0);

    //m_pCameraController = Engine::Camera::I_CameraManager::getSingleton().create("orbit")->createCameraController("orbit");

    // Move thhe camera "up" 200 units
    //m_pCameraController->move(200, Engine::Camera::I_Camera::Z_AXIS);

    //baseClient().getRenderingCanvas().setCamera(m_pCameraController);

    baseClient().onBeforeFrameRenderedEvent.connect(boost::bind(&PongGameClient::beforeRender, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::createActions()
{
    // Create some actions
    baseGame().getActionMap().createAction("Quit", boost::bind(&Engine::Base::I_BaseGameClient::quit, &m_baseClient, _1));

    // Bat actions
    baseGame().getActionMap().createAction("Move Left", boost::bind(&PongGameClient::moveLeft, this, _1));
    baseGame().getActionMap().createAction("Move Right", boost::bind(&PongGameClient::moveRight, this, _1));

    baseGame().getActionMap().createAction("Spawn Ball", boost::bind(&PongGameClient::spawnBall, this, _1));

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::createDefaultMapping()
{
    // Map some keys to actions
    baseClient().getInputMap().mapKeyInput("q", baseGame().getActionMap()["Quit"]);

    baseClient().getInputMap().mapKeyInput("a", baseGame().getActionMap()["Move Left"]);
    baseClient().getInputMap().mapKeyInput("d", baseGame().getActionMap()["Move Right"]);

    baseClient().getInputMap().mapKeyInput("f", baseGame().getActionMap()["Spawn Ball"]);

    baseClient().getInputMap().mapKeyInput("SPACE", baseGame().getActionMap()["Continue"]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::createBehaviors()
{
    m_pDefaultBehavior = baseGame().getBehaviorService().createBehaviors("default");
    //m_pDefaultBehavior->setScaleBehavior(&BaseObject::setScale);

    baseGame().getRootGroup().setDefaultBehaviors(m_pDefaultBehavior);

    m_pBatBehaviors = baseGame().getBehaviorService().createBehaviors("bat");
    m_pBatBehaviors->push_front(m_clampRotationBehavior);
    m_pBatBehaviors->push_front(m_batClampMovement);
    m_pBatBehaviors->push_front(m_zeroTorque);
    m_batClampMovement.setClampY(-63.0f);
    m_batClampMovement.setClampZ(0.0f);

    m_pBallBehaviors = baseGame().getBehaviorService().createBehaviors("ball");
    m_pBallBehaviors->push_front(m_ballClampMovement);
    m_pBallBehaviors->push_front(m_ballVelocity);
    m_ballVelocity.setSpeed(100.0f);
    m_ballClampMovement.setClampZ(0.0f);

    m_pGravityBallBehaviors = baseGame().getBehaviorService().createBehaviors("gravball");
    m_pGravityBallBehaviors->push_front(m_ballClampMovement);
    m_pGravityBallBehaviors->push_front(m_gravity);

    Math::Vector3 gravity(0.0f, -90.8f, 0.0f);
    m_gravity.setGravity(gravity);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::setupPhysicsMaterials()
{
    // TODO - only the default material works, since the objects are made in script
    //   this code needs to be transferred to script and then applied to the
    //   physics shapes after they're created.

    m_materialDefault = baseGame().getCurrentPhysicsWorld()->createMaterial(true);
    m_materialDefault->setElasticity(0.1f);
    m_materialDefault->setSoftness(1.0f);
    m_materialDefault->setDynamicFriction(1.0f);
    m_materialDefault->setStaticFriction(1.0f);
    //m_materialDefault->setCollidable(false);

    m_materialBall = baseGame().getCurrentPhysicsWorld()->createMaterial();
    m_materialBall->setElasticity(0.1f);
    m_materialBall->setSoftness(1.0f);
    m_materialBall->setDynamicFriction(1.0f);
    m_materialBall->setStaticFriction(1.0f);
    //m_materialBall->setCollidable(false);

    m_materialBat = baseGame().getCurrentPhysicsWorld()->createMaterial();
    m_materialBat->setElasticity(0.1f);
    m_materialBat->setSoftness(1.0f);
    m_materialBat->setDynamicFriction(1.0f);
    m_materialBat->setStaticFriction(1.0f);
    //m_materialBat->setCollidable(false);

    m_materialWall = baseGame().getCurrentPhysicsWorld()->createMaterial();
    m_materialWall->setElasticity(0.1f);
    m_materialWall->setSoftness(1.0f);
    m_materialWall->setDynamicFriction(1.0f);
    m_materialWall->setStaticFriction(1.0f);
    //m_materialWall->setCollidable(false);

    m_materialBottomWall = baseGame().getCurrentPhysicsWorld()->createMaterial();
    m_materialBottomWall->setElasticity(0.1f);
    m_materialBottomWall->setSoftness(1.0f);
    m_materialBottomWall->setDynamicFriction(1.0f);
    m_materialBottomWall->setStaticFriction(1.0f);
    //m_materialBottomWall->setCollidable(false);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Base::I_BaseGameClient&
PongGameClient::baseClient() const
{
    return m_baseClient;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Base::I_BaseGame&
PongGameClient::baseGame() const
{
    return m_baseGame;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::beforeRender(double _elapsedTime)
{
    // this updates all physics worlds we've created:
    baseGame().getPhysicsService()->stepSimulation(_elapsedTime);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::spawnBall(boost::any& _parameter)
{
    Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if(pEvent->getPressedState())
    {
        // Create a ball
        static int num = 0;
        std::stringstream ballname;
        ballname << "ball" << num++;
        BaseObject* pObj = new BaseObject(baseGame().getRootGroup(), ballname.str());
        pObj->loadRenderable("sphere.mesh", "ball");

        // This must be done before any behaviors are activate (setScale in this case)
        Zen::Engine::Physics::I_PhysicsWorld::pPhysicsShape_type pShape = pObj->base().getPhysicsShape();

        pObj->base().setScale(0.05f, 0.05f, 0.05f);

        pShape->initOvoidShape(5.0f, 5.0f, 5.0f);

        pObj->base().setPosition(25.0f, 0.0f, 0, true);

        pShape->setMass(100.0f);
        pShape->setVelocity(Zen::Math::Vector3(100.0f, 80.0f, 0.0f));
        pShape->setLinearDamping(0.0f);
        pShape->setAutoFreeze(false);
        pShape->setAngularMomentum(Zen::Math::Vector3(0.0f, 0.0f, 0.0f));
        pShape->setAngularDamping(Zen::Math::Vector3(1.0f, 1.0f, 1.0f));
        pShape->setAdvancedCollisionPrediction(true);
        pObj->base().setBehaviors(m_pGravityBallBehaviors);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::moveLeft(boost::any& _parameter)
{
    Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if(pEvent->getPressedState())
    {
        m_pPlayer->base().getPhysicsShape()->setVelocity(Zen::Math::Vector3(-150.0f, 0.0f, 0.0f));
    }
    else
    {
        m_pPlayer->base().getPhysicsShape()->setVelocity(Zen::Math::Vector3(0.0f, 0.0f, 0.0f));
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PongGameClient::moveRight(boost::any& _parameter)
{
    Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if(pEvent->getPressedState())
    {
        m_pPlayer->base().getPhysicsShape()->setVelocity(Zen::Math::Vector3(150.0f, 0.0f, 0.0f));
    }
    else
    {
        m_pPlayer->base().getPhysicsShape()->setVelocity(Zen::Math::Vector3(0.0f, 0.0f, 0.0f));
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Pong
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
