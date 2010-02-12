//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zee Ball Game Client
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
#include "GameClient.hpp"

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

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_RenderableResource.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputMap.hpp>

#include <Zen/Engine/Camera/I_CameraManager.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/bind.hpp>

#include <stddef.h>
#include <iostream>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZeeBall {
namespace Client {

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClient::GameClient(WindowHandle_type _pParent)
:   m_base(Zen::Engine::Base::I_BaseGameClient::getSingleton())
,   m_pScriptObject(NULL)
{
    m_base.setWindowHandle(_pParent);
	m_networkRole = CLIENTERVER; //we'll default to hosting for ourselves and others for simplicity for now
	m_connectionIp = boost::asio::ip::address_v4::from_string("0.0.0.0");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClient::~GameClient()
{
    m_pCameraController.reset();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const GameClient::WindowHandle_type
GameClient::getHandle() const
{
    return m_base.getWindowHandle();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ObjectReference*
script_createWall(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _args)
{
    Zen::Engine::Core::I_GameGroup* const pGameGroup = dynamic_cast<Zen::Engine::Core::I_GameGroup::ScriptObjectReference_type*>(_pObject)->getObject();

    // Parms
    std::string name = boost::any_cast<std::string>(_args[0]);
    Zen::Math::Real dx = boost::any_cast<Zen::Math::Real>(_args[1]);
    Zen::Math::Real dy = boost::any_cast<Zen::Math::Real>(_args[2]);
    Zen::Math::Real dz = boost::any_cast<Zen::Math::Real>(_args[3]);

    Zen::Engine::Base::I_BaseGameObject* pWall = dynamic_cast<Zen::Engine::Base::I_BaseGameObject*>(&pGameGroup->createObject(name));
    pWall->loadResource("cube.mesh");
    pWall->setMaterialName("wall");
    pWall->getPhysicsShape()->initBoxShape(dx, dy, dz);
    pWall->getPhysicsShape()->setMass(0.0f);
    pWall->setScale(dx / 100.0f, dy / 100.0f, dz / 100.0f);

    return pWall->getScriptObject();
}
/*
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ObjectReference*
script_setNetworkingRole(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _args)
{
    Zen::Engine::Core::I_GameGroup* const pGameGroup = dynamic_cast<Zen::Engine::Core::I_GameGroup::ScriptObjectReference_type*>(_pObject)->getScriptObject();

    // Parms
    std::string role = boost::any_cast<std::string>(_args[0]);

	//probably needs a second pass at a minimum later
	if (role == "hosting") m_networkRole = SERVER;
	else if (role == "connect_to")
	{
		m_networkRole = CLIENT;
		m_connectionIp = boost::asio::ip::address_v4::from_string(boost::any_cast<std::string>(_args[1]));
		m_connectionPort = getZonePort();  //returns hardcoded info for now. later used for per-map, or whatever breakdown.
	}
}
*/

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
GameClient::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GameClient::init()
{
    // TODO Is this necessary anymore?
    // Set Pong as the name of the game in the environment.
    //environment_type env;
    //env["gameName"] = "Pong";
    //env["starterRoot"] = "C:\\dev\\Zen\\examples\\taBBall";
    //appendEnvironment(env);

#if 0   // Lua was already initialized by ZGameLoader
    if (!m_base.initScriptEngine("python"))
        std::cout << "Error: ScriptEngine not initialized in GameClient::init()" << std::endl;
#endif

    m_base.initRenderingService("ogre");
    m_base.initSceneService("ogre");
    m_base.initRenderingResourceService("ogre");
    m_base.initPhysicsService("ZNewton");
    setupPhysicsMaterials();
    m_base.initPhysicsResourceService("ZNewton");
    m_base.initInputService("keyboard");

    // Register additional script types
    pScriptType_type pGameClientScriptType = m_base.getGameClientScriptType();
    
    //really need to break this off into a proper scriptwrapper subsystem... BJR
    pScriptType_type pGameGroupScriptType = m_base.getGameGroupScriptType();
    pGameGroupScriptType->addMethod("createWall", "createWall()", script_createWall);

//    pGameGroupScriptType->addMethod("setNetworkingRole", "setNetworkingRole()", script_setNetworkingRole);
 
    // Tell m_base we're done
    m_base.activateScriptModules();

    // Create the script object
    m_pScriptObject = new ScriptObjectReference_type
        (base().getScriptModule(), base().getScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());

    createActions();
    createDefaultMapping();

    createBehaviors();

    createScene();

	unsigned short port = getZonePort();
	
	//m_baseServer->initialiseHosting(port);

    return true;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
unsigned short
GameClient::getZonePort()
{
	//shove this in a gui func later
	hosting = true;
	currentZone = 13;

	if (hosting == true)
		return currentZone;
	else
		return 0;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::run()
{
    m_base.run();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createScene()
{
    // Create the camera
    //m_pCamera = m_pSceneService->createCamera("", "defaultCamera");

    // TODO Get the camera and modify the location.  For now the camera defaults are hacked in ZOgre

#if 1   // This really should be implemented in script
    boost::filesystem::path rootPath = boost::filesystem::system_complete
        (
        boost::filesystem::path("resources", boost::filesystem::native)
        ).normalize();

    m_base.getRenderingResourceService().addResourceLocation(rootPath.string(), "FileSystem", "Pong", true);
#endif

    // Create the world
    //m_pPhysicsWorld = m_base.getPhysicsService().createWorld();

    //Zen::Engine::Physics::I_PhysicsWorld::pPhysicsShape_type pShape;


    // Create the light
    Zen::Engine::Rendering::I_SceneService::pLight_type pLight = m_base.getSceneService().createLight("default", "Light");
    pLight->setPosition(0.0f, 0.0f, 50.0f);

    // Create the ball
    BaseObject* pObj = new BaseObject(m_base.getRootGroup(), "ball");
    pObj->base().loadResource("sphere.mesh");
    pObj->base().setMaterialName("ball");

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
    m_pPlayer = new BatObject(m_base.getRootGroup(), "Player1", 30.0f, 2.0f, 10.0f);
    m_pPlayer->base().getPhysicsShape()->setMass(1000.0f);
    m_pPlayer->base().getPhysicsShape()->setAdvancedCollisionPrediction(true);
    m_pPlayer->base().setPosition(0, -63, 0, true);

    m_pPlayer->base().setBehaviors(m_pBatBehaviors);

    m_pCameraController = Zen::Engine::Camera::I_CameraManager::getSingleton().create("orbit")->createCameraController("orbit");

    // Move thhe camera "up" 200 units
    m_pCameraController->move(200, Zen::Engine::Camera::I_Camera::Z_AXIS);

    m_base.getRenderingCanvas().setCamera(m_pCameraController);

    m_base.onBeforeFrameRenderedEvent.connect(boost::bind(&GameClient::beforeRender, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createActions()
{
    // Create some actions
    m_base.getActionMap().createAction("Quit", boost::bind(&Zen::Engine::Base::I_BaseGameClient::quit, &m_base, _1));

    // Bat actions
    m_base.getActionMap().createAction("Move Left", boost::bind(&GameClient::moveLeft, this, _1));
    m_base.getActionMap().createAction("Move Right", boost::bind(&GameClient::moveRight, this, _1));
    m_base.getActionMap().createAction("Spawn Ball", boost::bind(&GameClient::spawnBall, this, _1));

    /*additional leftovers
    m_base.getActionMap().createAction("Move Up", boost::bind(&GameClient::moveUp, this, _1));
    m_base.getActionMap().createAction("Move Down", boost::bind(&GameClient::moveDown, this, _1));
    m_base.getActionMap().createAction("Move Forward", boost::bind(&GameClient::moveForward, this, _1));
    m_base.getActionMap().createAction("Move Backward", boost::bind(&GameClient::moveBackward, this, _1));

    m_base.getActionMap().createAction("Rotate Left", boost::bind(&GameClient::rotateLeft, this, _1));
    m_base.getActionMap().createAction("Rotate Right", boost::bind(&GameClient::rotateRight, this, _1));
    m_base.getActionMap().createAction("Rotate Up", boost::bind(&GameClient::rotateUp, this, _1));
    m_base.getActionMap().createAction("Rotate Down", boost::bind(&GameClient::rotateDown, this, _1));
	*/

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createDefaultMapping()
{
    // Map some keys to actions
    m_base.getInputMap().mapKeyInput("q", m_base.getActionMap()["Quit"]);
    m_base.getInputMap().mapKeyInput("a", m_base.getActionMap()["Move Left"]);
    m_base.getInputMap().mapKeyInput("d", m_base.getActionMap()["Move Right"]);
    m_base.getInputMap().mapKeyInput("f", m_base.getActionMap()["Spawn Ball"]);
    m_base.getInputMap().mapKeyInput("SPACE", m_base.getActionMap()["Continue"]);
    /*additional leftovers
    m_base.getInputMap().mapKeyInput("2", m_base.getActionMap()["Move Up"]);
    m_base.getInputMap().mapKeyInput("1", m_base.getActionMap()["Move Down"]);
    m_base.getInputMap().mapKeyInput("w", m_base.getActionMap()["Move Forward"]);
    m_base.getInputMap().mapKeyInput("s", m_base.getActionMap()["Move Backward"]);
    m_base.getInputMap().mapKeyInput("a", m_base.getActionMap()["Rotate Left"]);
    m_base.getInputMap().mapKeyInput("d", m_base.getActionMap()["Rotate Right"]);
    m_base.getInputMap().mapKeyInput("3", m_base.getActionMap()["Rotate Up"]);
    m_base.getInputMap().mapKeyInput("4", m_base.getActionMap()["Rotate Down"]);
	*/
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createBehaviors()
{
    m_pDefaultBehavior = m_base.getBehaviorService().createBehaviors("default");
    //m_pDefaultBehavior->setScaleBehavior(&BaseObject::setScale);

    base().getRootGroup().setDefaultBehaviors(m_pDefaultBehavior);

    m_pBatBehaviors = m_base.getBehaviorService().createBehaviors("bat");
    m_pBatBehaviors->push_front(m_clampRotationBehavior);
    m_pBatBehaviors->push_front(m_batClampMovement);
    m_pBatBehaviors->push_front(m_zeroTorque);
    m_batClampMovement.setClampY(-63.0f);
    m_batClampMovement.setClampZ(0.0f);

    m_pBallBehaviors = m_base.getBehaviorService().createBehaviors("ball");
    m_pBallBehaviors->push_front(m_ballClampMovement);
    m_pBallBehaviors->push_front(m_ballVelocity);
    m_ballVelocity.setSpeed(100.0f);
    m_ballClampMovement.setClampZ(0.0f);

    m_pGravityBallBehaviors = m_base.getBehaviorService().createBehaviors("gravball");
    m_pGravityBallBehaviors->push_front(m_ballClampMovement);
    m_pGravityBallBehaviors->push_front(m_gravity);

    Zen::Math::Vector3 gravity(0.0f, -90.8f, 0.0f);
    m_gravity.setGravity(gravity);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::setupPhysicsMaterials()
{
    // TODO - only the default material works, since the objects are made in script
    //   this code needs to be transferred to script and then applied to the
    //   physics shapes after they're created.

    m_materialDefault = m_base.getCurrentPhysicsWorld()->createMaterial(true);
    m_materialDefault->setElasticity(0.1f);
    m_materialDefault->setSoftness(1.0f);
    m_materialDefault->setDynamicFriction(1.0f);
    m_materialDefault->setStaticFriction(1.0f);
    //m_materialDefault->setCollidable(false);

    m_materialBall = m_base.getCurrentPhysicsWorld()->createMaterial();
    m_materialBall->setElasticity(0.1f);
    m_materialBall->setSoftness(1.0f);
    m_materialBall->setDynamicFriction(1.0f);
    m_materialBall->setStaticFriction(1.0f);
    //m_materialBall->setCollidable(false);

    m_materialBat = m_base.getCurrentPhysicsWorld()->createMaterial();
    m_materialBat->setElasticity(0.1f);
    m_materialBat->setSoftness(1.0f);
    m_materialBat->setDynamicFriction(1.0f);
    m_materialBat->setStaticFriction(1.0f);
    //m_materialBat->setCollidable(false);

    m_materialWall = m_base.getCurrentPhysicsWorld()->createMaterial();
    m_materialWall->setElasticity(0.1f);
    m_materialWall->setSoftness(1.0f);
    m_materialWall->setDynamicFriction(1.0f);
    m_materialWall->setStaticFriction(1.0f);
    //m_materialWall->setCollidable(false);

    m_materialBottomWall = m_base.getCurrentPhysicsWorld()->createMaterial();
    m_materialBottomWall->setElasticity(0.1f);
    m_materialBottomWall->setSoftness(1.0f);
    m_materialBottomWall->setDynamicFriction(1.0f);
    m_materialBottomWall->setStaticFriction(1.0f);
    //m_materialBottomWall->setCollidable(false);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::spawnBall(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if(pEvent->getKeyState())
	{
        // Create a ball
        static int num = 0;
        std::stringstream ballname;
        ballname << "ball" << num++;
        BaseObject* pObj = new BaseObject(m_base.getRootGroup(), ballname.str());
        pObj->base().loadResource("sphere.mesh");
        pObj->base().setMaterialName("ball");

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
GameClient::moveLeft(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if(pEvent->getKeyState())
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
GameClient::moveRight(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if(pEvent->getKeyState())
    {
        m_pPlayer->base().getPhysicsShape()->setVelocity(Zen::Math::Vector3(150.0f, 0.0f, 0.0f));
    }
    else
    {
        m_pPlayer->base().getPhysicsShape()->setVelocity(Zen::Math::Vector3(0.0f, 0.0f, 0.0f));
    }
}

/*additional leftovers
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveUp(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camYMove = 1.0f;
    }
    else
    {
        m_camYMove = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveDown(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camYMove = -1.0f;
    }
    else
    {
        m_camYMove = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveForward(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camZMove = 1.0f;
    }
    else
    {
        m_camZMove = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::moveBackward(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camZMove = -1.0f;
    }
    else
    {
        m_camZMove = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::rotateLeft(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camYRotation = -1.0f;
    }
    else
    {
        m_camYRotation = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::rotateRight(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camYRotation = 1.0f;
    }
    else
    {
        m_camYRotation = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::rotateUp(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camXRotation = -1.0f;
    }
    else
    {
        m_camXRotation = 0.0f;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::rotateDown(boost::any& _parameter)
{
    Zen::Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Zen::Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);

    if (pEvent->getKeyState())
    {
        m_camXRotation = 1.0f;
    }
    else
    {
        m_camXRotation = 0.0f;
    }
}
end additional leftovers */

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
    // this updates all physics worlds we've created:
    base().getPhysicsService().stepSimulation(_elapsedTime);
}

//For initial networking example baseline, refference: http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/tutorial.html
//and http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/examples.html

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace ZeeBall
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~