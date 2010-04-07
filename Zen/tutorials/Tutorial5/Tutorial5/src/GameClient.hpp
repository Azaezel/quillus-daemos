//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This project is part of the Zen Engine Tutorials
// 
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMECLIENT_GAME_CLIENT_HPP_INCLUDED
#define GAMECLIENT_GAME_CLIENT_HPP_INCLUDED

#include "BehaviorManager.hpp"

#include <Zen/Engine/Client/I_GameClient.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>

#include <Zen/Engine/World/I_SkyService.hpp>
#include <Zen/Engine/World/I_TerrainService.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>
#include <Zen/Starter/Base/BaseCommon/I_BaseGame.hpp>


#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class WidgetManager;
class AvatarGameObject;

class GameClient
:   public Zen::Engine::Client::I_GameClient
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptType>          pScriptType_type;
    typedef Zen::Engine::Physics::I_PhysicsZone::pPhysicsMaterial_type     pPhysicsMaterial_type;
    typedef Zen::Engine::World::I_SkyService::pSky_type                     pSky_type;
    typedef Zen::Engine::World::I_TerrainService::pTerrain_type             pTerrain_type;
    /// @}

    /// @name I_GameClient implementation
    /// @{
public:
    virtual const WindowHandle_type getHandle() const;
    virtual void activateGameClientScriptModule();
    virtual Zen::Engine::Widgets::I_WidgetService& getWidgetService();
    virtual Zen::Engine::Rendering::I_RenderingCanvas& getRenderingCanvas();
    virtual bool initRenderingService(const std::string& _type, const std::string& _title, int _xRes, int _yRes);
    virtual bool initRenderingResourceService(const std::string& _type);
    virtual bool initTerrainService(const std::string& _type);
    virtual bool initSkyService(const std::string& _type);
    virtual bool initInputService(const std::string& _type);
    virtual bool initWidgetService(const std::string& _type);
    virtual bool init();
    virtual void run();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ScriptableService implementation.
    /// @{
public:
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name GameClient implementation
    /// @{
protected:
    /// Initialize the rendering subsystem
    void initRendering();

    /// Initialize the resource subsystem
    void initResources();

    /// Initialize the physics subsystem
    void initPhysics();

    /// Implement this to create or extend script types and link new script methods
    void createScriptTypes();

    /// Implement this to create some actions for the default
    /// action map.
    /// @param _actionMap Default action map
    void createActions();

    /// Create the default key to action mapping
    void createDefaultMapping();

    /// Create the game behaviors.
    void createBehaviors();

    /// Implement this to create a default scene
    void createScene();

    /// Get a behavior group by name
    BehaviorGroup& behaviorGroup(const std::string& _behaviorGroupName);

public:
    Zen::Engine::Base::I_BaseGameClient& base();

    Zen::Engine::Base::I_BaseGame& game();
    /// @}


    /// @name Move Manager Handlers
    /// @todo Move this to a MoveManager implementation
    /// @{
public:
    void moveLeft(boost::any& _parameter);
    void moveRight(boost::any& _parameter);
    void moveForward(boost::any& _parameter);
    void moveBackward(boost::any& _parameter);
    /// @}

    /// @name Event Handlers
    /// @{
protected:
    /// Called every frame right before the frame is rendered.
    /// @param _elapsedTime - amount of time that has elapsed since the 
    ///             previous frame was rendered (in seconds).
    void beforeRender(double _elapsedTime);
    /// @}

    /// @name 'Structors
    /// @{
public:
             GameClient(WindowHandle_type _pParent);
    virtual ~GameClient();
    /// @}

    /// @name Member Variables 
    /// @{
private:
    Zen::Engine::Base::I_BaseGameClient&            m_baseClient;
    Zen::Engine::Base::I_BaseGame&                  m_baseGame;

    ScriptObjectReference_type*                     m_pScriptObject;
    WidgetManager*                                  m_pWidgetManager;

    pSky_type                                       m_pSky;
    pTerrain_type                                   m_pTerrain;

    pPhysicsMaterial_type                           m_pTerrainMaterial;
    pPhysicsMaterial_type                           m_pAvatarMaterial;

    int                                             m_moveZDelta;
    int                                             m_moveXDelta;

    AvatarGameObject*                               m_pPlayer;

    BehaviorManager                                 m_behaviorManager;
    /// @}

};  // class GameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMECLIENT_GAME_CLIENT_HPP_INCLUDED
