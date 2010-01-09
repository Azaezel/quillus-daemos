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
#ifndef ZEN_ENGINE_BASE_I_BASE_GAME_HPP_INCLUDED
#define ZEN_ENGINE_BASE_I_BASE_GAME_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptEngine;
        class I_ScriptModule;
        class I_ScriptType;
    }   // namespace Scripting
namespace Engine {
    namespace World {
        class I_WaterService;
        class I_TerrainService;
        class I_SkyService;
    }   // namespace World
    namespace Resource {
        class I_ResourceService;
    }   // namespace Resource
    namespace Input {
        class I_InputService;
        class I_KeyEvent;
        class I_InputMap;
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
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Base Game Engine Client
class BASECOMMON_DLL_LINK I_BaseGame
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    /// This is a raw pointer since it's a singleton and is not
    /// managed.
    typedef I_BaseGame*                                         pScriptObject_type;
    typedef Scripting::ObjectReference<I_BaseGame>              ScriptObjectReference_type;

    typedef Memory::managed_ptr<Scripting::I_ScriptModule>      pScriptModule_type;
    typedef Memory::managed_ptr<Scripting::I_ScriptType>        pScriptType_type;
    typedef Memory::managed_ptr<Scripting::I_ScriptEngine>      pScriptEngine_type;

    typedef Memory::managed_ptr<Physics::I_PhysicsService>      pPhysicsService_type;
    typedef Memory::managed_ptr<Resource::I_ResourceService>    pPhysicsResourceService_type;

    typedef Zen::Engine::Physics::I_PhysicsService::pPhysicsZone_type  pPhysicsZone_type;
    /// @}

    /// @name I_BaseGame interface
    /// @{
public:
    /// @brief Initialize the physics service
    ///
    /// This is optional and is only used by game engines that utilize a physics engine.
    virtual bool initPhysicsService(const std::string& _type) = 0;

    /// @brief Initialize the physics resource service
    virtual bool initPhysicsResourceService(const std::string& _type) = 0;

    /// Get the Physics service.
    /// Use the physics service to create a virtual copy of your 3D world
    /// which will manage your objects' collisions, friction, forces, etc.
    virtual pPhysicsService_type getPhysicsService() = 0;

    virtual pPhysicsResourceService_type getPhysicsResourceService() = 0;

    virtual pPhysicsZone_type getCurrentPhysicsZone() = 0;

    /// Get the primary action map
    virtual Core::I_ActionMap& getActionMap(const std::string& _actionMapName = "default") = 0;

    /// Get the root grame group
    virtual Core::I_GameGroup& getRootGroup() = 0;

    /// Get the behavior service
    virtual Core::I_BehaviorService& getBehaviorService() = 0;

    /// @brief Initialize the navigation service.
    virtual bool initNavigationService(const std::string& _type) = 0;

    /// Get the navigation service.
    virtual Navigation::I_NavigationService& getNavigationService() = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "GameClient"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Script support
    /// @{
public:
    /// Initialize the BaseGame script module.
    /// Do not call this directly.  Call the I_BaseGameClient::initScriptEngine()
    /// or I_BaseGameServer::initScriptEngine(), which in turn will call this
    /// method as appropriate.
    virtual void initScriptModule(pScriptEngine_type _pScriptEngine) = 0;

    /// Get the BaseGame script module.
    /// Do not call this until after initScriptEngine() has been called.
    virtual pScriptModule_type getScriptModule() = 0;

    /// Initialize the script types.
    /// Do not call this directly.  Call the I_BaseGameClient::initScriptEngine()
    /// or I_BaseGameServer::initScriptEngine(), which in turn will call this
    /// method as appropriate.
    virtual void initScriptTypes() = 0;

    /// Create the script objects.
    /// Do not call this directly.  Call the I_BaseGameClient::activateScriptModules()
    /// or I_BaseGameServer::activateScriptModules(), which in turn will call this
    /// method as appropriate.
    virtual void createScriptObjects() = 0;

    /// Gets the game object script type.
    /// You can append additional methods to this script type, but only before
    /// activateScriptModules() has been called.
    virtual pScriptType_type getGameObjectScriptType() = 0;

    /// Gets the game group script type.
    /// You can append additional methods to this script type, but only before
    /// activateScriptModules() has been called.
    virtual pScriptType_type getGameGroupScriptType() = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    /// Get the one and only game object.
    static I_BaseGame& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BaseGame();
    virtual ~I_BaseGame();
    /// @}

};  // interface I_BaseGame

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_STARTER_BASE_I_BASE_GAME_HPP_INCLUDED
