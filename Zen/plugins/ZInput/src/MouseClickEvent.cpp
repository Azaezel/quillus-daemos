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

#include "MouseClickEvent.hpp"

#include <Zen/Core/Scripting/ObjectReference.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputServiceManager.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZInput {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
MouseClickEvent::MouseClickEvent(const OIS::MouseEvent& _event, 
                                 const bool _clicked, 
                                 OIS::MouseButtonID _button, 
                                 const Engine::Input::I_KeyModifierState& _state)
:   m_event(_event)
,   m_clicked(_clicked)
,   m_button(_button)
,   m_modifierState(_state)
,   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
MouseClickEvent::~MouseClickEvent()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
MouseClickEvent::wasClicked()
{
    return m_clicked;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Input::ButtonID
MouseClickEvent::getButton()
{
    switch(m_button)
    {
    case OIS::MB_Left:
        return Zen::Engine::Input::BUTTON_ID_LEFT;
    case OIS::MB_Right:
        return Zen::Engine::Input::BUTTON_ID_RIGHT;
    case OIS::MB_Middle:
        return Zen::Engine::Input::BUTTON_ID_MIDDLE;
    case OIS::MB_Button3:
        return Zen::Engine::Input::BUTTON_ID_BUTTON4;
    case OIS::MB_Button4:
        return Zen::Engine::Input::BUTTON_ID_BUTTON5;
    case OIS::MB_Button5:
        return Zen::Engine::Input::BUTTON_ID_BUTTON6;
    case OIS::MB_Button6:
        return Zen::Engine::Input::BUTTON_ID_BUTTON7;
    case OIS::MB_Button7:
        return Zen::Engine::Input::BUTTON_ID_BUTTON8;
    default:
        return Zen::Engine::Input::BUTTON_ID_NONE;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Engine::Input::I_KeyModifierState&
MouseClickEvent::getModifierState() const
{
    return m_modifierState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
MouseClickEvent::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        Zen::Engine::Input::I_InputServiceManager::pScriptModule_type pModule =
            Zen::Engine::Input::I_InputServiceManager::getSingleton().getDefaultScriptModule();

        if(pModule.isValid())
        {
            m_pScriptObject = new ScriptObjectReference_type(
                pModule, 
                pModule->getScriptType(getScriptTypeName()), 
                getSelfReference().lock()
            );
        }
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::script_module* MouseClickEvent::sm_pModule = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
MouseClickEvent::registerScriptModule(Zen::Scripting::script_module& _module)
{
    sm_pModule = &_module;

    Zen::Scripting::script_type<MouseClickEvent>(_module.getScriptModule()->getScriptType("MouseClickEvent"))
        .addMethod("getButton", &MouseClickEvent::scriptGetButton)
        .activate()
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZInput
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

