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
#ifndef ZEN_ENGINE_RESOURCE_RESOURCE_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_RESOURCE_RESOURCE_MANAGER_HPP_INCLUDED

#include "../I_ResourceManager.hpp"
#include "../I_CompoundResource.hpp"

#include "../I_ResourceServiceFactory.hpp"
#include "../I_ResourceService.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Plugins/ServiceCache.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Resource {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ResourceManager
:   public I_ResourceManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine> pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>   pScriptType_type;

    typedef Zen::Plugins::ServiceCache<I_ResourceService>       resource_service_cache_type;
    typedef std::map< std::string, std::string >                config_type;
    /// @}

    /// @ name I_ResourceManager implementation
	/// @{
public:
    virtual pCompoundResourceService_type createCompoundResource();
    virtual pResourceService_type create(const std::string& _type, config_type& _config);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultScriptModule();
    /// @}

    /// @name Additional Implementation
    /// @{
private:
    /// Register the script classes with the scripting engine
    void registerScriptTypes(pScriptEngine_type _pEngine);

    /// Register a service with a script engine
    void registerScriptEngine(pScriptEngine_type _pEngine, pResourceService_type _pService);
    /// @}

    /// @name 'Structors
    /// @{ 
public:
             ResourceManager();
    virtual ~ResourceManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    resource_service_cache_type     m_resourceServiceCache;

    pScriptEngine_type              m_pDefaultScriptEngine;

    bool                            m_scriptTypesInitialized;

    pScriptModule_type              m_pResourceModule;
    pScriptType_type                m_pResourceServiceType;
    pScriptType_type                m_pResourceType;
    /// @}

};  // class ResourceManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RESOURCE_RESOURCE_MANAGER_HPP_INCLUDED
