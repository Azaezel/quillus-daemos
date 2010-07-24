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
#ifndef ZEN_ENGINE_STARTER_BASE_I_BASE_GAME_CLIENT_HPP_INCLUDED
#define ZEN_ENGINE_STARTER_BASE_I_BASE_GAME_CLIENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Client/I_GameClient.hpp>

#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Rendering {
        class I_Context;
        class I_View;
        class I_RenderingCanvas;
        class I_RenderingService;
        class I_SceneService;
    }   // namespace Rendering
    namespace Camera {
        class I_CameraService;
        class I_Camera;
    }   // namespace Camera
    namespace World {
        class I_WaterService;
    }   // namespace World
    namespace Resource {
        class I_ResourceService;
    }   // namespace Resource
    namespace Input {
        class I_InputService;
        class I_KeyEvent;
        class I_KeyMap;
    }   // namespace Input
    namespace Core {
        class I_Action;
        class I_ActionMap;
        class I_GameGroup;
        class I_BehaviorService;
    }   // namespace Core
    namespace Move {
        class I_MoveService;
    }   // namespace Move
    namespace Navigation {
        class I_NavigationService;
    }   // namespace Navigation
    namespace Widgets {
        class I_WidgetService;
    }   // namespace Widgets
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Base Game Engine Client
class BASECLIENT_DLL_LINK I_BaseGameClient
:   public Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef double                                              FrameDelta_type;
    typedef Event::Event<FrameDelta_type>                       FrameEvent_type;

    typedef Memory::managed_ptr<Scripting::I_ScriptEngine>      pScriptEngine_type;
    typedef Memory::managed_ptr<Scripting::I_ScriptType>        pScriptType_type;
    /// @}

    /// @name I_BaseGameClient Initializers
    /// You must call each of these methods once and only once in the order outlined
    /// unless otherwise specified.
    /// @{
public:
    /// @brief Set the window handle of the parent window that will contain this client.
    virtual void setWindowHandle(Client::I_GameClient::WindowHandle_type _pParent) = 0;

    /// @brief Initialize the script engine
    ///
    /// This is optional and is only used by game engines that utilize a script engine.
    virtual bool initScriptEngine(const std::string& _type) = 0;

    /// @brief Initialize the rendering service
    virtual bool initRenderingService(const std::string& _type, const std::string& _windowName = std::string("Zen Engine - Base Client"), unsigned _width = 800, unsigned _height = 600) = 0;

    /// @brief Initialize the scene service
    virtual bool initSceneService(const std::string& _type) = 0;

private:
    /// @brief Initialize the camera service
    /// Deprecated.  See I_RenderingCanvas::getCurrentCamera()
    virtual bool initCameraService(const std::string& _type) { return false; }

public:
    /// @brief Initialize the rendering resource service
    virtual bool initRenderingResourceService(const std::string& _type) = 0;

    /// @brief Initialize one or more input services
    ///
    /// Typically more than one input service is initialized.  "keyboard" "mouse" and
    /// "joystick" are typical types.
    virtual bool initInputService(const std::string& _type) = 0;

    /// @brief Initialize the water service
    virtual bool initWaterService(const std::string& _type) = 0;

    /// @brief Initialize the widgets service
    virtual bool initWidgetService(const std::string& _type) = 0;

    /// @breif Initialize the sound service
    virtual bool initSoundService(const std::string& _type) = 0;

    /// @brief Activate the script modules.
    ///
    /// Do this after all services have been initialized and your game client has
    /// completed adding new methods to the registered script types.
    virtual void activateScriptModules() = 0;
    /// @}

    /// @name I_BaseGameClient Getter methods
    ///
    /// When a reference is returned, do not use it outside of the known lifetime
    /// of this I_BaseGameClient object.  Normally you won't have to worry about this
    /// if you're utilizing these references within your game engine code that holds
    /// a reference to this I_BaseGameClient.
    /// @{
public:
    /// @brief Get the main window handle
    virtual Client::I_GameClient::WindowHandle_type getWindowHandle() = 0;

    /// @brief Get the script engine
    ///
    /// @note Since this is a reference but the underlying object is a managed
    ///         pointer, please do not hold onto it outside of the lifetime of this
    ///         I_BaseGameClient object.
    virtual Scripting::I_ScriptEngine& getScriptEngine() = 0;

    /// Get the script engine by pointer.
    virtual pScriptEngine_type getScriptEnginePtr() = 0;

    /// @brief Get the rendering resource service
    ///
    /// Use this to set additional directories for the rendering resource service.
    ///
    /// @note Since this is a reference but the underlying object is a managed
    ///         pointer, please do not hold onto it outside of the lifetime of this
    ///         I_BaseGameClient object.
    virtual Resource::I_ResourceService& getRenderingResourceService() = 0;

    typedef Memory::managed_ptr<Rendering::I_SceneService>      pSceneService_type;
    virtual pSceneService_type getSceneService() = 0;

    virtual Input::I_InputService& getInputService() = 0;

    virtual Rendering::I_RenderingService& getRenderingService() = 0;
    virtual Rendering::I_RenderingCanvas& getRenderingCanvas() = 0;

    virtual World::I_TerrainService& getTerrainService() = 0;
    virtual World::I_SkyService& getSkyService() = 0;
    virtual World::I_WaterService& getWaterService() = 0;

    virtual Widgets::I_WidgetService& getWidgetService() = 0;

    /// Get the primary KeyMap
    virtual Input::I_KeyMap& getKeyMap() = 0;

    /// Get the GameClient script module
    virtual pScriptModule_type getScriptModule() = 0;

    /// Get the primary GameClient script type
    virtual pScriptType_type getGameClientScriptType() = 0;

    /// @}

    /// @name I_BaseGameClient
    /// @{
public:
    virtual void run() = 0;
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Call this method to shut everything down.
    virtual void quit(boost::any& _parameter) = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    static I_BaseGameClient& getSingleton();
    /// @}

    /// @name Events
    /// @{
public:
    /// This event is fired before every frame is rendered.
    /// The payload is the number of elapsed seconds since
    /// the previous frame event.
    FrameEvent_type     onBeforeFrameRenderedEvent;

    /// This event is fired after every frame is rendered.
    /// The payload is the number of elapsed seconds since
    /// the previous frame event.
    FrameEvent_type     onAfterFrameRenderedEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BaseGameClient();
    virtual ~I_BaseGameClient();
    /// @}

};  // interface I_BaseGameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_STARTER_BASE_I_BASE_GAME_CLIENT_HPP_INCLUDED
