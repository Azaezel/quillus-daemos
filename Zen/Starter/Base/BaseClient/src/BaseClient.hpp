//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
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
#ifndef ZEN_ENGINE_BASE_CLIENT_HPP_INCLUDED
#define ZEN_ENGINE_BASE_CLIENT_HPP_INCLUDED

#include "../I_BaseGameClient.hpp"

#include <Zen/Core/Utility/I_EnvironmentHandler.hpp>

#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_Application.hpp>

#include <Zen/Core/Scripting/I_ScriptingManager.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Resource/I_ResourceManager.hpp>
#include <Zen/Engine/Core/I_GameGroup.hpp>

#include <Zen/Starter/Base/BaseCommon/I_BaseGame.hpp>

#include <boost/filesystem/path.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptEngine;
    }   // namespace Scripting
namespace Engine {
    namespace Camera {
        class I_CameraService;
        class I_Camera;
    }   // namespace Camera
    namespace Rendering {
        class I_Context;
        class I_View;
        class I_RenderingCanvas;
        class I_RenderingService;
        class I_SceneService;
    }   // namespace Rendering
    namespace World {
        class I_TerrainService;
    }   // namespace World
    namespace Input {
        class I_InputService;
        class I_InputMapService;
        class I_KeyEvent;
        class I_KeyMap;
    }   // namespace Input
    namespace Core {
        class I_Action;
        class I_ActionMap;
        class I_GameGroup;
    }   // namespace Core
    namespace Move {
        class I_MoveService;
    }   // namespace Move
    namespace Sound {
        class I_SoundService;
    }   // namespace Sound
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameGroup;

/// Base Game Engine Client
class BaseClient
:   public I_BaseGameClient
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_BaseGameClient Initializers
    /// @{
public:
    virtual void setWindowHandle(Client::I_GameClient::WindowHandle_type _pParent);
    virtual bool initScriptEngine(const std::string& _type);
    virtual bool initRenderingService(const std::string& _type, const std::string& _windowName, unsigned _width, unsigned _height);
    virtual bool initSceneService(const std::string& _type);
    virtual bool initRenderingResourceService(const std::string& _type);
    virtual bool initInputService(const std::string& _type);
    virtual bool initWaterService(const std::string& _type);
    virtual bool initTerrainService(const std::string& _type);
    virtual bool initSkyService(const std::string& _type);
    virtual bool initWidgetService(const std::string& _type);
    virtual bool initSoundService(const std::string& _type);
    virtual void activateScriptModules();
    /// @}

    /// @name I_BaseGameClient Getter methods
    /// @{
public:
    virtual Scripting::I_ScriptEngine& getScriptEngine();
    virtual pScriptEngine_type getScriptEnginePtr();
    virtual Resource::I_ResourceService& getRenderingResourceService();
    virtual Input::I_InputService& getInputService();
    virtual Rendering::I_RenderingService& getRenderingService();
    virtual Client::I_GameClient::WindowHandle_type getWindowHandle();
    virtual World::I_WaterService& getWaterService();
    virtual World::I_TerrainService& getTerrainService();
    virtual World::I_SkyService& getSkyService();
    virtual Widgets::I_WidgetService& getWidgetService();
    /// @}


    /// @name I_BaseGameClient implementation
    /// @{
public:

    /// Preferably don't override this.
    /// @see The other init* methods declared below.
    //virtual bool init();
    virtual void run();
    /// @}

    /// @name Getter Methods
    /// @{
public:
    const std::string& getEnvironment(const std::string& _name) const;

    /// Get the root grame group
    virtual Core::I_GameGroup& getRootGroup();

    /// Get the primary scene service.
    /// The scene service maintains and manipulates the scene graph.
    virtual Rendering::I_SceneService&  getSceneService();

    /// Get the primary rendering canvas.
    /// The rendering canvas is the primary 3d canvas where everything
    /// is drawn.
    Rendering::I_RenderingCanvas& getRenderingCanvas();

    /// Get the primary KeyMap
    virtual Input::I_KeyMap& getKeyMap();

    virtual Core::I_BehaviorService& getBehaviorService();

    virtual pScriptModule_type getScriptModule();

    virtual pScriptType_type getGameClientScriptType();

    /// Get the script game group.
    Zen::Scripting::I_ObjectReference* getScriptGameGroup();

    /// Get the script game object.
    pScriptType_type getGameObjectScriptType();
    /// @}

    /// @name Additional implementation
    /// @{
private:
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Call this method to shut everything down.
    void quit(boost::any& _parameter);
    /// @}

    /// @name 'Structors
    /// @{
public:
             BaseClient();
    virtual ~BaseClient();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    I_BaseGame&                             m_game;

    Plugins::I_PluginManager&               m_pluginManager;
    Plugins::I_ExtensionRegistry&           m_extensionRegistry;

    Client::I_GameClient::WindowHandle_type m_pParent;

    boost::filesystem::path                 m_configPath;
    Plugins::I_Application::ptr_type        m_pApp;

    Zen::Memory::managed_ptr<Rendering::I_RenderingService> m_pRenderingService;
    Zen::Memory::managed_ptr<Resource::I_ResourceService>   m_pRenderingResourceService;
    Zen::Memory::managed_ptr<Rendering::I_SceneService>     m_pSceneService;
    Zen::Memory::managed_ptr<Camera::I_CameraService>       m_pCameraService;
    Zen::Memory::managed_ptr<Camera::I_Camera>              m_pCamera;
    Zen::Memory::managed_ptr<Input::I_InputService>         m_pInputService;
    Zen::Memory::managed_ptr<Input::I_InputMapService>      m_pInputMapService;
    Zen::Memory::managed_ptr<World::I_WaterService>         m_pWaterService;
    Zen::Memory::managed_ptr<World::I_TerrainService>       m_pTerrainService;
    Zen::Memory::managed_ptr<World::I_SkyService>           m_pSkyService;
    Zen::Memory::managed_ptr<Widgets::I_WidgetService>      m_pWidgetService;
    Zen::Memory::managed_ptr<Sound::I_SoundService>         m_pSoundService;

    /// Primary input map for the game
    Zen::Memory::managed_ptr<Input::I_KeyMap>               m_pMainKeyMap;

    Core::I_GameGroup::ScriptObjectReference_type*          m_pScriptMainGroup;

protected:
    // Probably the rest of these will become managed pointers
    Rendering::I_Context*                   m_pContext;
    Rendering::I_View*                      m_pView;
    Rendering::I_RenderingCanvas*           m_pRenderingCanvas;


    std::string                             m_empty;

    volatile bool                           m_quitting;

    pScriptEngine_type                      m_pScriptEngine;
    pScriptModule_type                      m_pModule;

    pScriptType_type                        m_pGameClientScriptType;
    /// @}

};  // class BaseClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_BASE_CLIENT_HPP_INCLUDED
