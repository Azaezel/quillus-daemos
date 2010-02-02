//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_ENGINE_RESOURCE_MANAGER_I_RESOURCE_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_RESOURCE_MANAGER_I_RESOURCE_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptEngine;
        class I_ScriptModule;
    }   // namespace Scripting
namespace Engine {
namespace Resource {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ResourceService;
class I_CompoundResourceService;

class RESOURCE_DLL_LINK I_ResourceManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine> pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;

    typedef Zen::Memory::managed_ptr<I_ResourceService>         pResourceService_type;
    typedef Zen::Memory::managed_weak_ptr<I_ResourceService>    wpResourceService_type;

    typedef Zen::Memory::managed_ptr<I_CompoundResourceService>         pCompoundResourceService_type;
    typedef Zen::Memory::managed_weak_ptr<I_CompoundResourceService>    wpCompoundResourceService_type;
    
    typedef std::map< std::string, std::string >                        config_type;
    /// @}

    /// @name I_ResourceManager interface
    /// @{
public:
    /// Create an empty compound resource
    /// @return Pointer to new I_CompoundResource instance
    virtual pCompoundResourceService_type createCompoundResource() = 0;

    /// Create a resource service of the specified type.
    virtual pResourceService_type create(const std::string& _type, config_type& _config) = 0;

    /// Register the default script engine for all resource services.
    /// Every resource service that has been created or is created in the future
    /// will use this script engine.
    /// @param _pEngine NULL to set the default engine to none, but doing so will
    ///             not unregister the script engine to services that have already
    ///             been created.  It will only prevent subsequent services from
    ///             using this script engine.
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine) = 0;

    /// Get the default Resource script module.
    virtual pScriptModule_type getDefaultScriptModule() = 0;
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get the singleton
    static I_ResourceManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ResourceManager();
    virtual ~I_ResourceManager();
    /// @}

};  // interface I_ResourceManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RESOURCE_MANAGER_I_RESOURCE_MANAGER_HPP_INCLUDED
