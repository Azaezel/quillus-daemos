//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_PHYSICS_I_PHYSICS_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_I_PHYSICS_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ClassFactory.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptEngine;
        class I_ScriptModule;
    }   // namespace Scripting
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_PhysicsService;

class PHYSICS_DLL_LINK I_PhysicsManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine> pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;

    typedef Memory::managed_ptr<I_PhysicsService>      pPhysicsService_type;
    typedef Memory::managed_weak_ptr<I_PhysicsService> wpPhysicsService_type;

    typedef std::map<std::string, std::string>        config_type;

    typedef double                                      FrameDelta_type;
    typedef Event::Event<FrameDelta_type>               FrameEvent_type;
    /// @}

    /// @name I_PhysicsManager interface
    /// @{
public:
    /// Create a physics service of the specified type
    virtual pPhysicsService_type create(const std::string& _type, config_type& _config) = 0;

    /// Register the default script engine for all physics services.
    /// Every physics service that has been created or is created in the future
    /// will use this script engine.
    /// @param _pEngine NULL to set the default engine to none, but doing so will
    ///             not unregister the script engine to services that have already
    ///             been created.  It will only prevent subsequent services from
    ///             using this script engine.
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine) = 0;

    /// Get the default Rendering script module.
    virtual pScriptModule_type getDefaultScriptModule() = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    /// Get the singleton
    static I_PhysicsManager& getSingleton();
    /// @}

    /// @name Events
    /// @{
public:
    FrameEvent_type onFrameEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PhysicsManager();
    virtual ~I_PhysicsManager();
    /// @}

};  // interface I_PhysicsManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_I_PHYSICS_MANAGER_HPP_INCLUDED

