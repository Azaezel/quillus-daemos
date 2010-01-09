//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2008 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ActionMap.hpp"
#include "Action.hpp"

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/assign.hpp>
#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ActionMap::ActionMap()
:   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ActionMap::~ActionMap()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ActionMap::createAction(const std::string& _name, ActionFunction_type _function)
{
    Action* pRawObject = new Action(_name, _function);

    pAction_type pAction(pRawObject, boost::bind(&ActionMap::onDestroyAction, this, _1));

    wpAction_type wpAction(pAction);
    
    pRawObject->setSelfReference(wpAction);

    m_actionMap[_name] = pAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ActionMap::createAction(const std::string& _name, pAction_type _pAction)
{
    m_actionMap[_name] = _pAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ActionMap::pAction_type
ActionMap::operator [](const std::string& _name)
{
    // Find the action
    ActionMap_type::iterator iter = m_actionMap.find(_name);
    if (iter != m_actionMap.end())
    {
        // Return the action if it was found
        return iter->second;
    }

    // Return an empty action if the action wasn't found
    pAction_type pAction;
    return pAction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ActionMap::onDestroyAction(wpAction_type _pAction)
{
    Action* pAction = dynamic_cast<Action*>(_pAction.get());
    if (pAction != NULL)
    {
        delete pAction;
    }
    else
    {
        // TODO Error
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ScriptAction;
typedef std::map<std::string, boost::function<void(ScriptAction&, boost::any&)> > ScriptActionMap_type;
static ScriptActionMap_type sm_typeMap;

class ScriptAction
:   public Action
{
public:
    ScriptAction(const std::string& _name, boost::any _function, boost::any _object, Zen::Scripting::I_ScriptEngine* _pEngine)
    :   m_object(_object)
    ,   m_function(_function)
    ,   m_pEngine(_pEngine)
    ,   Action(_name)
    {
    }

    static void dispatchInput(ScriptAction& _self, boost::any& _parameter)
    {
        std::vector<boost::any> parms;

        Engine::Input::I_InputService::pKeyEventPayload_type pEvent = boost::any_cast<Engine::Input::I_InputService::pKeyEventPayload_type>(_parameter);
        parms.push_back(pEvent->getScriptObject());
        _self.m_pEngine->executeMethod(_self.m_object, _self.m_function, parms);
    }

    static void dispatchScript(ScriptAction& _self, boost::any& _parameter)
    {
        std::vector<boost::any> parms;

        Scripting::I_ObjectReference* pObject = boost::any_cast<Scripting::I_ObjectReference*>(_parameter);
        parms.push_back(pObject);
        _self.m_pEngine->executeMethod(_self.m_object, _self.m_function, parms);
    }

    virtual void dispatch(boost::any& _parameter)
    {
        try
        {
            const std::string typeName = _parameter.type().name();

            // Set up the map of special types.  This map only needs to be special
            // things that need special handling
            // TODO Guard?
            if (sm_typeMap.empty())
            {
                sm_typeMap[typeid(Engine::Input::I_InputService::pKeyEventPayload_type).name()] = &ScriptAction::dispatchInput;
                sm_typeMap[typeid(Scripting::I_ObjectReference*).name()] = &ScriptAction::dispatchScript;
            }

            // Find any of the special types
            ScriptActionMap_type::iterator iter = sm_typeMap.find(typeName);
            if (iter != sm_typeMap.end())
            {
                // Found a special type, so handle it appropriately
                iter->second(*this, _parameter);
            }
            else
            {
                // Just dispatch it and hopefully the script engine can
                // figure out what type the parameter is.
                std::vector<boost::any> parms;

                parms.push_back(_parameter);
                m_pEngine->executeMethod(m_object, m_function, parms);
            }
        }
        catch(boost::bad_any_cast _ex)
        {
            // TODO This should go to a log of some sort
            std::cout << "Bad any_cast error while executing script action handler " << getName() << std::endl;
        }
    }

private:
    boost::any  m_object;
    boost::any  m_function;
    Zen::Scripting::I_ScriptEngine* m_pEngine;
};

//static ScriptActionMap_type sm_typeMap = boost::assign::map_list_of
//    (typeid(Engine::Input::I_InputService::pKeyEventPayload_type).name(), &ScriptAction::dispatchInput),
//    (typeid(Scripting::I_ObjectReference*).name(), &ScriptAction::dispatchScript)
//);

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void 
script_createAction(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    ActionMap::ScriptObjectReference_type* pObject = dynamic_cast<ActionMap::ScriptObjectReference_type*>(_pObject);

    std::string name = boost::any_cast<std::string>(_parms[0]);

    ScriptAction* pRawObject = new ScriptAction(name, _parms[1], _parms[2], pObject->getModule()->getScriptEngine().get());

    ActionMap* const pActionMap = dynamic_cast<ActionMap*>(pObject->getObject());

    Memory::managed_ptr<Zen::Engine::Core::I_Action> 
        pAction(pRawObject, boost::bind(&ActionMap::onDestroyAction, pActionMap, _1));

    pRawObject->setSelfReference(pAction.getWeak());

    pActionMap->createAction(name, pAction);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ObjectReference* 
script_getAction(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    ActionMap::ScriptObjectReference_type* pObject = dynamic_cast<ActionMap::ScriptObjectReference_type*>(_pObject);

    std::string name = boost::any_cast<std::string>(_parms[0]);

    return (*pObject->getObject())[name]->getScriptObject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ActionMap::registerScriptModule(pScriptModule_type _pScriptModule)
{
    sm_pScriptModule = _pScriptModule;

    sm_pScriptType = sm_pScriptModule->createScriptType("ActionMap", "Action Map to map actions to keys", 0);
    sm_pScriptType->addMethod("createAction", "Map an Action", &script_createAction);
    sm_pScriptType->addMethod("getAction", "Map an Action", &script_getAction);

    m_pActionScriptType = sm_pScriptModule->createScriptType("Action", "Action to handle game events", 0);
    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
ActionMap::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        if(sm_pScriptModule.isValid())
        {
            m_pScriptObject = new ScriptObjectReference_type(sm_pScriptModule, sm_pScriptType, this);
        }
    }

    return m_pScriptObject;
}

ActionMap::pScriptModule_type ActionMap::sm_pScriptModule;
ActionMap::pScriptType_type ActionMap::sm_pScriptType;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

