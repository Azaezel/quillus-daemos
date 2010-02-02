//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ENGINE_INPUT_MANAGER_INPUT_SERVICE_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_MANAGER_INPUT_SERVICE_MANAGER_HPP_INCLUDED

#include "../I_InputServiceManager.hpp"

#include "../I_InputServiceFactory.hpp"
#include "../I_InputService.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Plugins/ServiceCache.hpp>

#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_InputService;

class InputServiceManager
:   public I_InputServiceManager
{
    /// @name Types
    /// @{
public:
    typedef Scripting::ObjectReference<I_InputService>                      InputServiceObjectReference_type;
    typedef Zen::Memory::managed_ptr<InputServiceObjectReference_type>      pInputServiceObjectReference_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine> pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>   pScriptType_type;

    typedef Zen::Plugins::ServiceCache<I_InputService>      input_service_cache_type;

    typedef std::map<std::string, pService_type>            services_type;
    typedef std::map<I_InputService*, std::string>          serviceIndex_type;

    typedef Zen::Memory::managed_ptr<I_InputService>        pInputService_type;
    typedef Zen::Memory::managed_weak_ptr<I_InputService>   wpInputService_type;
    typedef Zen::Memory::managed_weak_ptr<I_InputMapService> wpInputMapService_type;
    /// @}

    /// @name I_InputServiceManager implementation
    /// @{
public:
    virtual pService_type create(const std::string& _type, config_type& _config);
    virtual pInputMapService_type createInputMapService();
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultScriptModule();
    /// @}

    /// @name InputServiceManager implementation
    /// @{
public:
    /// Service destruction event handler.
    /// @param _pService Weak pointer to service being destroyed.
    void onDestroy(wpInputService_type _pService);

    void destroyInputMapService(wpInputMapService_type _wpInputMapService);

private:
    /// Register the script classes with the scripting engine
    void registerScriptTypes(pScriptEngine_type _pEngine);

    /// Register a service with a script engine
    void registerScriptEngine(pScriptEngine_type _pEngine, pInputService_type _pService);
    /// @}

    /// @name 'Structors
    /// @{
public:
             InputServiceManager();
    virtual ~InputServiceManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    input_service_cache_type    m_inputServiceCache;

    pScriptEngine_type          m_pDefaultScriptEngine;

    bool                        m_scriptTypesInitialized;

    pScriptModule_type          m_pInputModule;
    pScriptType_type            m_pInputServiceType;
    pScriptType_type            m_pKeyMapType;
    pScriptType_type            m_pKeyEventType;
    pScriptType_type            m_pInput;

    pScriptType_type            m_pSceneServiceType;
    pScriptType_type            m_pSceneNodeType;
    pInputServiceObjectReference_type   m_pInputServiceObjectReference;
    /// @}

};  // class InputServiceManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_MANAGER_INPUT_SERVICE_MANAGER_HPP_INCLUDED
