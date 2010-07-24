//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#if 0 // deprecated
#ifndef ZEN_ENGINE_BASE_ACTION_MAP_HPP_INCLUDED
#define ZEN_ENGINE_BASE_ACTION_MAP_HPP_INCLUDED

#include <Zen/Engine/Core/I_ActionMap.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptModule.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ActionMap
:   public Core::I_ActionMap
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Scripting::I_ScriptModule>      pScriptModule_type;
    typedef Scripting::I_ScriptModule::pScriptType_type         pScriptType_type;
    /// @}

    /// @name I_ActionMap implementation
    /// @{
public:
    /// Create an acion by name and bind it to a function that gets envoked when
    /// the action occurs.
    virtual void createAction(const std::string& _name, ActionFunction_type _function);

    /// Get an action by name.
    virtual pAction_type operator[](const std::string& _name);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name ActionMap implementation
    /// @{
public:
    void registerScriptModule(pScriptModule_type _pScriptModule);
    void createAction(const std::string& _name, pAction_type _pAction);

    /// @}

    /// @name Event handlers
    /// @{
public:
    void onDestroyAction(wpAction_type _pAction);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ActionMap();
    virtual ~ActionMap();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Scripting::I_ObjectReference*           m_pScriptObject;

    /// Primary action map
    typedef Zen::Memory::managed_ptr<Zen::Engine::Core::I_Action>   pAction_type;
    typedef std::map<std::string, pAction_type>             ActionMap_type;
    ActionMap_type                                          m_actionMap;

    static pScriptModule_type                               sm_pScriptModule;
    static pScriptType_type                                 sm_pScriptType;
    pScriptType_type                                        m_pActionScriptType;

    /// @}

};  // class ActionMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_BASE_ACTION_MAP_HPP_INCLUDED
#endif // deprecated
