//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Knights of Zen
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

#include "BaseGameObject.hpp"

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Engine/Core/I_GameGroup.hpp>
#include <Zen/Engine/Core/I_ActionMap.hpp>
#include <Zen/Engine/Core/I_GameObjectBehaviors.hpp>

#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>
#include <Zen/Engine/Physics/I_PhysicsShape.hpp>

#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Rendering/I_Light.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_RenderableResource.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputMap.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#include <boost/bind.hpp>

#include <stddef.h>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Knights {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClient::GameClient(WindowHandle_type _pParent)
:   m_base(Engine::Base::I_BaseGameClient::getSingleton())
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
    // TODO Is this necessary anymore?
    // Set  as the name of the game in the environment.
    //environment_type env;
    //env["gameName"] = "";
    //env["starterRoot"] = "C:\\dev\\Zen\\examples\\KnightsOfZen";
    //appendEnvironment(env);

    m_base.initScriptEngine("python");
    m_base.initSceneService("ogre");
    m_base.initResourceService("ogre");
    m_base.initRenderingService("ogre");
    m_base.initPhysicsService("ZNewton");
    m_base.initInputService("keyboard");

    createActions();
    createDefaultMapping();

    // TODO createBehaviors();
    m_pDefaultBehavior = m_base.getBehaviorService().createBehaviors("default");
    m_pDefaultBehavior->setScaleBehavior(&BaseGameObject::setScale);

    base().getRootGroup().setDefaultBehaviors(m_pDefaultBehavior);

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
void
GameClient::createScene()
{
    // Create the camera
    //m_pCamera = m_pSceneService->createCamera("", "defaultCamera");

    // TODO Get the camera and modify the location.  For now the camera defaults are hacked in ZOgre

#if 1   // This really should be implemented in script
    boost::filesystem::path rootPath = boost::filesystem::system_complete
        (
        boost::filesystem::path("C:\\dev\\Zen\\examples\\KnightsOfZen\\resources", boost::filesystem::native)
        ).normalize();

    m_base.getResourceService().addResourceLocation(rootPath.string(), "FileSystem", "", true);
#endif

#if 0   // Oops, this doesn't work, but it used to work
    boost::filesystem::path rootPath = boost::filesystem::system_complete
        (
            boost::filesystem::path(getEnvironment("gameRoot").c_str(), boost::filesystem::native)
        ).normalize();

    rootPath = rootPath / "" / "resources";
    getResourceService().addResourceLocation(rootPath.string(), "FileSystem", "", true);
#endif

    // Create the world
    m_pWorld = m_base.getPhysicsService().createWorld();

    m_base.onBeforeFrameRenderedEvent.connect(boost::bind(&GameClient::beforeRender, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createActions()
{
    // Create some actions
    m_base.getActionMap().createAction("Quit", boost::bind(&Engine::Base::I_BaseGameClient::quit, &m_base, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClient::createDefaultMapping()
{
    // Map some keys to actions
    m_base.getInputMap().mapKeyInput("q", m_base.getActionMap()["Quit"]);

    m_base.getInputMap().mapKeyInput("SPACE", m_base.getActionMap()["Continue"]);
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
    // this updates all physics worlds we've created:
    base().getPhysicsService().stepSimulation(_elapsedTime);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClient::pPhysicsWorld_type
GameClient::getPhysicsWorld()
{
    return m_pWorld;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Knights
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
