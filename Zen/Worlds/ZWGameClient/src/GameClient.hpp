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
#ifndef ZEN_WORLDS_GAME_CLIENT_HPP_INCLUDED
#define ZEN_WORLDS_GAME_CLIENT_HPP_INCLUDED

#include <Zen/Engine/Client/I_GameClient.hpp>
#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <Zen/Engine/Camera/I_CameraService.hpp>
#include <Zen/Engine/Camera/I_Camera.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>

#include <Zen/Engine/Input/I_MouseClickEvent.hpp>

#include <Zen/Engine/World/I_SkyService.hpp>
#include <Zen/Engine/World/I_TerrainService.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>
#include <Zen/Starter/Base/BaseCommon/I_BaseGame.hpp>

#include <boost/any.hpp>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Enterprise {
        namespace AppServer {
            class I_ApplicationServer;
            class I_ProtocolService;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GUIManager;
class GameObject;

class GameClient
:   public Zen::Engine::Client::I_GameClient
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Scripting::I_ScriptType>                    pScriptType_type;
    typedef Zen::Engine::Physics::I_PhysicsZone::pPhysicsMaterial_type      pPhysicsMaterial_type;
    typedef std::list<GameObject*>                                          GameObjects_type;

    typedef Memory::managed_ptr<Enterprise::AppServer::I_ProtocolService>   pProtocolService_type;

    typedef Zen::Engine::World::I_SkyService::pSky_type                     pSky_type;
    typedef Zen::Engine::World::I_TerrainService::pTerrain_type             pTerrain_type;
    /// @}

    /// @name I_BaseGameClient implementation
    /// @{
public:
    virtual void activateGameClientScriptModule();
    /// @}

    /// @name I_GameClient implementation
    /// @{
public:
    virtual const WindowHandle_type getHandle() const;
    virtual bool init();
    virtual void run();
    virtual Engine::Widgets::I_WidgetService& getWidgetService();
    virtual Engine::Rendering::I_RenderingCanvas& getRenderingCanvas();
    virtual bool initRenderingService(const std::string& _type, const std::string& _title, int _xRes, int _yRes);
    virtual bool initRenderingResourceService(const std::string& _type);
    virtual bool initTerrainService(const std::string& _type);
    virtual bool initSkyService(const std::string& _type);
    virtual bool initInputService(const std::string& _type);
    virtual bool initWidgetService(const std::string& _type);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ScriptableService implementation.
    /// @{
public:
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name Script extensions.  All of these methods are
    ///     implemented in GameCLient_script.cpp
    /// @{
public:
    /// Implement this to create or extend script types and link new script methods
    void createScriptTypes();
    /// @}

    /// @name GameClient implementation
    /// @{
public:
    /// Setup the resource paths.
    void setupResourcePaths();

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

    /// Implement this to create a skybox
    /// TODO This eventually should return an I_Sky object via script.
    void createSkybox(const std::string& _resource);

    /// Implement this to create a terrain
    /// TODO This eventually should return an I_Terrain object via script.
    void createTerrain(const std::string& _resource);

    /// Initialize the client services.
    void initServices();

    /// Setup physics materials.
    void setupPhysicsMaterials();

    Engine::Base::I_BaseGameClient& base();

    Engine::Base::I_BaseGame& game();

    void queryCursor();

    /// Set the game name
    void setGameName(const std::string& _name);

    /// Get the game name
    const std::string& getGameName() const;
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Called every frame right before the frame is rendered.
    /// @param _elapsedTime - amount of time that has elapsed since the
    ///             previous frame was rendered (in seconds).
    void beforeRender(double _elapsedTime);

    void quit(boost::any& _parameter);
    void moveLeft(boost::any& _parameter);
    void moveRight(boost::any& _parameter);
    void moveUp(boost::any& _parameter);
    void moveDown(boost::any& _parameter);
    void zoom(boost::any& _parameter);
    void print(boost::any& _parameter);

    /// Handle mouse click events.
    /// @note These mouse click events come from the GUIManager and are only events
    ///     that the GUI does not handle.
    void handleMouseClickEvent(Zen::Engine::Input::I_MouseClickEvent& _event);
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
    std::string                             m_gameName;
    bool                                    m_initialized;

    Engine::Base::I_BaseGameClient&         m_base;
    Engine::Base::I_BaseGame&               m_game;

    pSky_type                               m_pSky;
    pTerrain_type                           m_pTerrain;

    ScriptObjectReference_type*             m_pScriptObject;

    Zen::Memory::managed_ptr<GUIManager>    m_pGUIManager;

    Zen::Math::Vector3                      m_moveDirection;
    Zen::Math::Real                         m_zoomAmount;

    GameObjects_type                        m_gameObjects;

    /// Primary application server
    Enterprise::AppServer::I_ApplicationServer* m_pAppServer;

    /// Primary protocol adapter
    pProtocolService_type                       m_pProtocolService;
    /// @}

};  // class GameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_WORLDS_GAME_CLIENT_HPP_INCLUDED
