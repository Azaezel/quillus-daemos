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

#include "WidgetManager.hpp"

#include <Zen/Engine/Widgets/I_WidgetServiceFactory.hpp>
#include <Zen/Engine/Widgets/I_WidgetService.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/I_ScriptObject.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Widgets {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetManager::WidgetManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetManager::~WidgetManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WidgetManager::pService_type
WidgetManager::create(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_serviceCache.getLock());

    pService_type pService(m_serviceCache.getCachedService(_type));

    if( pService.isValid() )
    {
        return pService;
    }

    I_WidgetServiceFactory* pFactory = m_serviceCache.getFactory(_type);

    if( pFactory == NULL )
    {
        std::stringstream errorMessage;
        errorMessage << "Zen::Widgets::WidgetManager::create() : Error: could not create service factory for type " << _type;

        throw Zen::Utility::runtime_exception(errorMessage.str());
    }

    pService = m_serviceCache.cacheService(_type, pFactory->create(_type, _config));

    if( m_pDefaultScriptEngine.isValid() )
    {
        registerScriptEngine(m_pDefaultScriptEngine, pService);
    }

    return pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if(!m_pDefaultScriptEngine.isValid())
    {
        Threading::CriticalSection guard(m_serviceCache.getLock());

        for(ServiceCache_type::iterator iter = m_serviceCache.begin(); iter != m_serviceCache.end(); iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetManager::pScriptModule_type
WidgetManager::getDefaultScriptModule()
{
    return m_pWidgetsModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetManager::registerScriptEngine(pScriptEngine_type _pEngine, pService_type _pService)
{
    new I_WidgetService::ScriptObjectReference_type(m_pWidgetsModule, m_pWidgetServiceType, _pService, "widgetService");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetManager::registerScriptTypes(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    // Create a Widgets module
    m_pWidgetsModule = _pEngine->createScriptModule("Widgets", "Zen Widgets Module");

    // Expose I_WidgetService to the Script Engine
    m_pWidgetServiceType = m_pWidgetsModule->createScriptType("WidgetService", "Widget Service", 0);

    m_pWidgetsModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Widgets
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
