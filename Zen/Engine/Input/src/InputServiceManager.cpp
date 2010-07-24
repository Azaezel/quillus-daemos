//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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

#include "InputServiceManager.hpp"
#include "InputMapService.hpp"

#include "../I_InputServiceFactory.hpp"
#include "../I_InputService.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Core/I_Action.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputMapService.hpp>
#include <Zen/Engine/Input/I_KeyMap.hpp>
#include <Zen/Engine/Input/I_KeyEvent.hpp>
#include <Zen/Engine/Input/I_MouseMoveEvent.hpp>
#include <Zen/Engine/Input/I_MouseClickEvent.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputServiceManager::InputServiceManager()
:   m_pInputModule(NULL)
,   m_scriptTypesInitialized(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputServiceManager::~InputServiceManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputServiceManager::pService_type
InputServiceManager::create(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_inputServiceCache.getLock());

    pInputService_type pService(m_inputServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_InputServiceFactory* pFactory = m_inputServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO Error
        return pService;
    }

    pService = pFactory->create(_type, _config);

    if (m_pInputModule != NULL)
    {
        pService->registerScriptModule(*m_pInputModule);
    }

    return m_inputServiceCache.cacheService(_type, pService);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputServiceManager::pInputMapService_type
InputServiceManager::createInputMapService()
{
    // TODO Should only one of these be created?

    I_InputMapService* const pRawInputMapService = new InputMapService;
    pInputMapService_type pInputServiceMap(pRawInputMapService,
        boost::bind(&InputServiceManager::destroyInputMapService, this, _1));

    pRawInputMapService->registerScriptModule(*m_pInputModule);

    return pInputServiceMap;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputServiceManager::destroyInputMapService(wpInputMapService_type _wpInputMapService)
{
    InputMapService* pService = dynamic_cast<InputMapService*>(_wpInputMapService.get());

    if (pService)
    {
        delete pService;
    }
    else
    {
        throw Zen::Utility::runtime_exception("InputServiceManager::destroyInputMapService(): Error, wrong type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputServiceManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || _pEngine == NULL)
        return;

    // Create an InputManager module
    m_pInputModule = new Zen::Scripting::script_module(_pEngine, "Input", "Zen Input Services Module");

    // Expose I_InputService to the Script Engine
    m_pInputModule->addType<I_InputService>("InputService", "Input Service")
	;

    m_pInputModule->addType<I_KeyMap>("KeyMap", "Key Map")
		.addMethod("mapKeyInput", &I_KeyMap::mapKeyInput)
	;

    m_pInputModule->addType<I_InputMapService>("InputMapService", "Input Map Service")
		.addMethod("createKeyMap", &I_InputMapService::createKeyMap)
	;

    m_pInputModule->addType<I_KeyEvent>("KeyEvent", "Key Event")
		.addMethod("getPressedState", &I_KeyEvent::getPressedState)
		.addMethod("getKeyCode", &I_KeyEvent::getKeyCode)
		.addMethod("getChar", &I_KeyEvent::getChar)
	;

    m_pInputModule->addType<I_MouseMoveEvent>("MouseMoveEvent", "Mouse Move Event")
        .addMethod("getXDelta", &I_MouseMoveEvent::getXDelta)
        .addMethod("getYDelta", &I_MouseMoveEvent::getYDelta)
        .addMethod("getZDelta", &I_MouseMoveEvent::getZDelta)
        .addMethod("getX", &I_MouseMoveEvent::getX)
        .addMethod("getY", &I_MouseMoveEvent::getY)
        .addMethod("getZ", &I_MouseMoveEvent::getZ)
        //.addMethod("getModifierState", &I_MouseClickEvent::getModifierState)
    ;

    m_pInputModule->addType<I_MouseClickEvent>("MouseClickEvent", "Mouse Click Event")
        .addMethod("wasClicked", &I_MouseClickEvent::wasClicked)
        // TODO This is going to need a custom script_getButton() method
        // implemented at the concrete service level.
        //.addMethod("getButton", &I_MouseClickEvent::getButton)  
        //.addMethod("getModifierState", &I_MouseClickEvent::getModifierState)
    ;


    m_pInputModule->activate();

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = true;

    /// Register all of the existing services
    registerScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputServiceManager::pScriptModule_type
InputServiceManager::getDefaultScriptModule()
{
    return m_pInputModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputServiceManager::registerScriptModule()
{
	Threading::CriticalSection guard(m_inputServiceCache.getLock());

	for(input_service_cache_type::iterator iter = m_inputServiceCache.begin(); iter != m_inputServiceCache.end(); iter++)
	{
		iter->second->registerScriptModule(*m_pInputModule);
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
