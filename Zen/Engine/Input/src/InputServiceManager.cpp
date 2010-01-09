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

#include "InputServiceManager.hpp"

#include "../I_InputServiceFactory.hpp"
#include "../I_InputService.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting/ObjectReference.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>

#include <Zen/Engine/Core/I_Action.hpp>
#include <Zen/Engine/Input/I_InputMap.hpp>

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

    return m_inputServiceCache.cacheService(_type, pFactory->create(_type, _config));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputServiceManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if(m_pDefaultScriptEngine == NULL)
    {
        Threading::CriticalSection guard(m_inputServiceCache.getLock());

        for(services_type::iterator iter = m_inputServiceCache.begin(); 
            iter != m_inputServiceCache.end(); 
            iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
InputServiceManager::pScriptModule_type
InputServiceManager::getDefaultScriptModule()
{
    return m_pInputModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputServiceManager::registerScriptEngine(pScriptEngine_type _pEngine, pInputService_type _pService)
{
    new I_InputService::ScriptObjectReference_type(m_pInputModule, m_pInputServiceType, _pService, "resourceService");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
script_mapKeyInput(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    I_InputMap::ScriptObjectReference_type* pObject = dynamic_cast<I_InputMap::ScriptObjectReference_type*>(_pObject);

    std::string key = boost::any_cast<std::string>(_parms[0]);

    Scripting::I_ObjectReference* pActionObj = boost::any_cast<Scripting::I_ObjectReference*>(_parms[1]);

    Core::I_Action::ScriptObjectReference_type* pAction = dynamic_cast<Core::I_Action::ScriptObjectReference_type*>(pActionObj);

    pObject->getObject()->mapKeyInput(key, pAction->getObject());
}

static int
script_getKeyCode(Zen::Scripting::I_ObjectReference* _pObject)
{
    I_KeyEvent::ScriptObjectReference_type* pObject = dynamic_cast<I_KeyEvent::ScriptObjectReference_type*>(_pObject);

    return pObject->getObject()->getKeyCode();
}

static bool
script_getPressedState(Zen::Scripting::I_ObjectReference* _pObject)
{
    I_KeyEvent::ScriptObjectReference_type* pObject = dynamic_cast<I_KeyEvent::ScriptObjectReference_type*>(_pObject);

    return pObject->getObject()->getPressedState();
}

static std::string
script_getChar(Zen::Scripting::I_ObjectReference* _pObject)
{
    I_KeyEvent::ScriptObjectReference_type* pObject = dynamic_cast<I_KeyEvent::ScriptObjectReference_type*>(_pObject);

    return boost::lexical_cast<std::string>(pObject->getObject()->getChar());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
InputServiceManager::registerScriptTypes(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || _pEngine == NULL)
        return;

    // Create an InputManager module
    m_pInputModule = _pEngine->createScriptModule("InputManager", "Zen Input Manager Module");

    // Expose I_InputService to the Script Engine
    m_pInputServiceType = m_pInputModule->createScriptType("InputService", "Input Service", 0);
    //m_pInputServiceType->addMethod("createInputMap", "Create an input map", createInputMap);
    //
    m_pInputMapType = m_pInputModule->createScriptType("InputMap", "Input Map", 0);
    m_pInputMapType->addMethod("mapKeyInput", "Map a key to an action", script_mapKeyInput);

    m_pKeyEventType = m_pInputModule->createScriptType("KeyEvent", "Key Event", 0);
    m_pKeyEventType->addMethod("getPressedState", "Get the state of the key (true = down, false = up", script_getPressedState);
    m_pKeyEventType->addMethod("getKeyCode", "Get the code of the key pressed or released", script_getKeyCode);
    m_pKeyEventType->addMethod("getChar", "Convert the key into a string", script_getChar);

    m_pInputModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
