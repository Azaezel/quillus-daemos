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

#include <Zen/Core/Scripting.hpp>

#include "../I_ResourceManager.hpp"
#include "../I_CompoundResource.hpp"

#include "../I_ResourceServiceFactory.hpp"
#include "../I_ResourceService.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
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
    typedef Zen::Plugins::ServiceCache<I_ResourceService>       ResourceServiceCache_type;
    typedef std::map< std::string, std::string >                config_type;
    /// @}

    /// @ name I_ResourceManager implementation
	/// @{
public:
    virtual pCompoundResourceService_type createCompoundResource();
    virtual pResourceService_type create(const std::string& _type, config_type& _config);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultResourceScriptModule();
    /// @}

    /// @name Additional Implementation
    /// @{
private:
    /// Register the script classes with the scripting engine
    void registerResourceScriptModule();
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
    ResourceServiceCache_type       m_resourceServiceCache;

    pScriptEngine_type              m_pDefaultScriptEngine;

    bool                            m_scriptTypesInitialized;

    Zen::Scripting::script_module*  m_pResourceModule;
    /// @}

};  // class ResourceManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RESOURCE_RESOURCE_MANAGER_HPP_INCLUDED
