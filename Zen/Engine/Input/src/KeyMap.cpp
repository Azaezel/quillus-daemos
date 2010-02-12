//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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

#include "KeyMap.hpp"

#include "KeyState.hpp"
#include "KeyModifierState.hpp"
#include "../KeyCodes.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/MutexFactory.hpp>

#include <Zen/Core/Scripting/ObjectReference.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_InputServiceManager.hpp>
#include <Zen/Engine/Input/KeyCodes.hpp>
#include <Zen/Engine/Input/I_KeyPublisher.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Table of mappable key inputs.
/// @todo Completely flesh out the table of mappable key inputs.
const KeyMap::KeyMapTable_type KeyMap::sm_keyMapTable = boost::assign::map_list_of
//   Name                                   Code            State
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    ("esc",                 KeyState(   Engine::Input::KC_ESCAPE,       &NullKeyModifierState::instance()))
    ("left-shift-esc",      KeyState(   Engine::Input::KC_ESCAPE,       &LeftShiftKeyModifierState::instance()))
    ("right-shift-esc",     KeyState(   Engine::Input::KC_ESCAPE,       &RightShiftKeyModifierState::instance()))
    ("shift-esc",           KeyState(   Engine::Input::KC_ESCAPE,       &EitherShiftKeyModifierState::instance()))
    ("1",                   KeyState(   Engine::Input::KC_1,            &NullKeyModifierState::instance()))
    ("left-shift-1",        KeyState(   Engine::Input::KC_1,            &LeftShiftKeyModifierState::instance()))
    ("right-shift-1",       KeyState(   Engine::Input::KC_1,            &RightShiftKeyModifierState::instance()))
    ("shift-1",             KeyState(   Engine::Input::KC_1,            &EitherShiftKeyModifierState::instance()))
    ("2",                   KeyState(   Engine::Input::KC_2,            &NullKeyModifierState::instance()))
    ("3",                   KeyState(   Engine::Input::KC_3,            &NullKeyModifierState::instance()))
    ("4",                   KeyState(   Engine::Input::KC_4,            &NullKeyModifierState::instance()))
    ("5",                   KeyState(   Engine::Input::KC_5,            &NullKeyModifierState::instance()))
    ("6",                   KeyState(   Engine::Input::KC_6,            &NullKeyModifierState::instance()))
    ("7",                   KeyState(   Engine::Input::KC_7,            &NullKeyModifierState::instance()))
    ("8",                   KeyState(   Engine::Input::KC_8,            &NullKeyModifierState::instance()))
    ("9",                   KeyState(   Engine::Input::KC_9,            &NullKeyModifierState::instance()))
    ("0",                   KeyState(   Engine::Input::KC_0,            &NullKeyModifierState::instance()))
    ("-",                   KeyState(   Engine::Input::KC_MINUS,        &NullKeyModifierState::instance()))
    ("=",                   KeyState(   Engine::Input::KC_EQUALS,       &NullKeyModifierState::instance()))
    ("backspace",           KeyState(   Engine::Input::KC_BACK,         &NullKeyModifierState::instance()))
    ("tab",                 KeyState(   Engine::Input::KC_TAB,          &NullKeyModifierState::instance()))
    ("q",                   KeyState(   Engine::Input::KC_Q,            &NullKeyModifierState::instance()))
    ("w",                   KeyState(   Engine::Input::KC_W,            &NullKeyModifierState::instance()))
    ("e",                   KeyState(   Engine::Input::KC_E,            &NullKeyModifierState::instance()))
    ("r",                   KeyState(   Engine::Input::KC_R,            &NullKeyModifierState::instance()))
    ("t",                   KeyState(   Engine::Input::KC_T,            &NullKeyModifierState::instance()))
    ("y",                   KeyState(   Engine::Input::KC_Y,            &NullKeyModifierState::instance()))
    ("u",                   KeyState(   Engine::Input::KC_U,            &NullKeyModifierState::instance()))
    ("i",                   KeyState(   Engine::Input::KC_I,            &NullKeyModifierState::instance()))
    ("o",                   KeyState(   Engine::Input::KC_O,            &NullKeyModifierState::instance()))
    ("p",                   KeyState(   Engine::Input::KC_P,            &NullKeyModifierState::instance()))
    ("[",                   KeyState(   Engine::Input::KC_LBRACKET,     &NullKeyModifierState::instance()))
    ("]",                   KeyState(   Engine::Input::KC_RBRACKET,     &NullKeyModifierState::instance()))
    ("enter",               KeyState(   Engine::Input::KC_RETURN,       &NullKeyModifierState::instance()))
    ("left-ctrl",           KeyState(   Engine::Input::KC_LCONTROL,     &NullKeyModifierState::instance()))
    ("a",                   KeyState(   Engine::Input::KC_A,            &NullKeyModifierState::instance()))
    ("s",                   KeyState(   Engine::Input::KC_S,            &NullKeyModifierState::instance()))
    ("d",                   KeyState(   Engine::Input::KC_D,            &NullKeyModifierState::instance()))
    ("f",                   KeyState(   Engine::Input::KC_F,            &NullKeyModifierState::instance()))
    ("g",                   KeyState(   Engine::Input::KC_G,            &NullKeyModifierState::instance()))
    ("h",                   KeyState(   Engine::Input::KC_H,            &NullKeyModifierState::instance()))
    ("j",                   KeyState(   Engine::Input::KC_J,            &NullKeyModifierState::instance()))
    ("k",                   KeyState(   Engine::Input::KC_K,            &NullKeyModifierState::instance()))
    ("l",                   KeyState(   Engine::Input::KC_L,            &NullKeyModifierState::instance()))
    (";",                   KeyState(   Engine::Input::KC_SEMICOLON,    &NullKeyModifierState::instance()))
    ("'",                   KeyState(   Engine::Input::KC_APOSTROPHE,   &NullKeyModifierState::instance()))
    ("`",                   KeyState(   Engine::Input::KC_GRAVE,        &NullKeyModifierState::instance()))
    ("left-shift",          KeyState(   Engine::Input::KC_LSHIFT,       &NullKeyModifierState::instance()))
    ("\\",                  KeyState(   Engine::Input::KC_BACKSLASH,    &NullKeyModifierState::instance()))
    ("z",                   KeyState(   Engine::Input::KC_Z,            &NullKeyModifierState::instance()))
    ("x",                   KeyState(   Engine::Input::KC_X,            &NullKeyModifierState::instance()))
    ("c",                   KeyState(   Engine::Input::KC_C,            &NullKeyModifierState::instance()))
    ("v",                   KeyState(   Engine::Input::KC_V,            &NullKeyModifierState::instance()))
    ("b",                   KeyState(   Engine::Input::KC_B,            &NullKeyModifierState::instance()))
    ("n",                   KeyState(   Engine::Input::KC_N,            &NullKeyModifierState::instance()))
    ("m",                   KeyState(   Engine::Input::KC_M,            &NullKeyModifierState::instance()))
    (",",                   KeyState(   Engine::Input::KC_COMMA,        &NullKeyModifierState::instance()))
    (".",                   KeyState(   Engine::Input::KC_PERIOD,       &NullKeyModifierState::instance()))
    ("/",                   KeyState(   Engine::Input::KC_SLASH,        &NullKeyModifierState::instance()))
    ("right-shift",         KeyState(   Engine::Input::KC_RSHIFT,       &NullKeyModifierState::instance()))
    ("*",                   KeyState(   Engine::Input::KC_MULTIPLY,     &NullKeyModifierState::instance()))
    ("left-alt",            KeyState(   Engine::Input::KC_LMENU,        &NullKeyModifierState::instance()))
    ("space",               KeyState(   Engine::Input::KC_SPACE,        &NullKeyModifierState::instance()))
    ("capslock",            KeyState(   Engine::Input::KC_CAPITAL,      &NullKeyModifierState::instance()))
    ("f1",                  KeyState(   Engine::Input::KC_F1,           &NullKeyModifierState::instance()))
    ("f2",                  KeyState(   Engine::Input::KC_F2,           &NullKeyModifierState::instance()))
    ("f3",                  KeyState(   Engine::Input::KC_F3,           &NullKeyModifierState::instance()))
    ("f4",                  KeyState(   Engine::Input::KC_F4,           &NullKeyModifierState::instance()))
    ("f5",                  KeyState(   Engine::Input::KC_F5,           &NullKeyModifierState::instance()))
    ("f6",                  KeyState(   Engine::Input::KC_F6,           &NullKeyModifierState::instance()))
    ("f7",                  KeyState(   Engine::Input::KC_F7,           &NullKeyModifierState::instance()))
    ("f8",                  KeyState(   Engine::Input::KC_F8,           &NullKeyModifierState::instance()))
    ("f9",                  KeyState(   Engine::Input::KC_F9,           &NullKeyModifierState::instance()))
    ("f10",                 KeyState(   Engine::Input::KC_F10,          &NullKeyModifierState::instance()))
    ("numlock",             KeyState(   Engine::Input::KC_NUMLOCK,      &NullKeyModifierState::instance()))
    ("scrolllock",          KeyState(   Engine::Input::KC_SCROLL,       &NullKeyModifierState::instance()))
    ("numpad7",             KeyState(   Engine::Input::KC_NUMPAD7,      &NullKeyModifierState::instance()))
    ("numpad8",             KeyState(   Engine::Input::KC_NUMPAD8,      &NullKeyModifierState::instance()))
    ("numpad9",             KeyState(   Engine::Input::KC_NUMPAD9,      &NullKeyModifierState::instance()))
    ("numpad-",             KeyState(   Engine::Input::KC_SUBTRACT,     &NullKeyModifierState::instance()))
    ("numpad4",             KeyState(   Engine::Input::KC_NUMPAD4,      &NullKeyModifierState::instance()))
    ("numpad5",             KeyState(   Engine::Input::KC_NUMPAD5,      &NullKeyModifierState::instance()))
    ("numpad6",             KeyState(   Engine::Input::KC_NUMPAD6,      &NullKeyModifierState::instance()))
    ("+",                   KeyState(   Engine::Input::KC_ADD,          &NullKeyModifierState::instance()))
    ("numpad1",             KeyState(   Engine::Input::KC_NUMPAD1,      &NullKeyModifierState::instance()))
    ("numpad2",             KeyState(   Engine::Input::KC_NUMPAD2,      &NullKeyModifierState::instance()))
    ("numpad3",             KeyState(   Engine::Input::KC_NUMPAD3,      &NullKeyModifierState::instance()))
    ("numpad0",             KeyState(   Engine::Input::KC_NUMPAD0,      &NullKeyModifierState::instance()))
    ("numpad.",             KeyState(   Engine::Input::KC_DECIMAL,      &NullKeyModifierState::instance()))
    ("f11",                 KeyState(   Engine::Input::KC_F11,          &NullKeyModifierState::instance()))
    ("f12",                 KeyState(   Engine::Input::KC_F12,          &NullKeyModifierState::instance()))
    ("numpadenter",         KeyState(   Engine::Input::KC_NUMPADENTER,  &NullKeyModifierState::instance()))
    ("right-ctrl",          KeyState(   Engine::Input::KC_RCONTROL,     &NullKeyModifierState::instance()))
    ("numpad/",             KeyState(   Engine::Input::KC_DIVIDE,       &NullKeyModifierState::instance()))
    ("right-alt",           KeyState(   Engine::Input::KC_RMENU,        &NullKeyModifierState::instance()))
    ("home",                KeyState(   Engine::Input::KC_HOME,         &NullKeyModifierState::instance()))
    ("up",                  KeyState(   Engine::Input::KC_UP,           &NullKeyModifierState::instance()))
    ("pageup",              KeyState(   Engine::Input::KC_PGUP,         &NullKeyModifierState::instance()))
    ("left",                KeyState(   Engine::Input::KC_LEFT,         &NullKeyModifierState::instance()))
    ("right",               KeyState(   Engine::Input::KC_RIGHT,        &NullKeyModifierState::instance()))
    ("end",                 KeyState(   Engine::Input::KC_END,          &NullKeyModifierState::instance()))
    ("down",                KeyState(   Engine::Input::KC_DOWN,         &NullKeyModifierState::instance()))
    ("insert",              KeyState(   Engine::Input::KC_INSERT,       &NullKeyModifierState::instance()))
    ("delete",              KeyState(   Engine::Input::KC_DELETE,       &NullKeyModifierState::instance()))
    ("left-win",            KeyState(   Engine::Input::KC_LWIN,         &NullKeyModifierState::instance()))
    ("right-win",           KeyState(   Engine::Input::KC_RWIN,         &NullKeyModifierState::instance()));
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
KeyMap::KeyMap()
:   m_pScriptObject(NULL)
,   m_pEventConnection(NULL)
,   m_isEnabled(false)
,   m_pPublisher(NULL)
{
    // What is the purpose of this mutex?
    m_pMutex = Threading::MutexFactory::create();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
KeyMap::~KeyMap()
{
    Threading::MutexFactory::destroy(m_pMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
KeyMap::mapKeyInput(const std::string& _name, const pAction_type _pAction)
{
    KeyMapTable_type::const_iterator iter = sm_keyMapTable.find(_name);
    if( iter != sm_keyMapTable.end() )
    {
        m_keyMap.insert(std::pair<KeyState,pAction_type>(iter->second,_pAction));
    }
    else
    {
        // TODO Probably should only log an error and not throw an exception since generally this
        // is done via script and should be considered a "soft" error.
        //throw Zen::Utility::runtime_exception("Zen::ZInput::KeyMap::mapKeyInput() : Invalid key name.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
KeyMap::connect(I_KeyPublisher& _keyPublisher)
{
    if (m_pEventConnection != NULL)
    {
        m_pEventConnection->disconnect();
    }

    m_pPublisher = &_keyPublisher;

    m_pEventConnection = _keyPublisher.onKeyEvent.connect(boost::bind(&KeyMap::handleKeyEvent, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
KeyMap::enable(bool _enable)
{
    m_isEnabled = _enable;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
KeyMap::handleKeyEvent(pKeyEvent_type _pEvent)
{
    if(m_isEnabled)
    {
        KeyState state(_pEvent->getKeyState());

        KeyMap_type::iterator iter = m_keyMap.find(state);

        while(iter != m_keyMap.end() && iter->first == state)
        {
            boost::any any(_pEvent);
            // Dispatch the event
            iter->second->dispatch(any);

            iter++;
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
KeyMap::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        Zen::Engine::Input::I_InputServiceManager::pScriptModule_type pModule =
            Zen::Engine::Input::I_InputServiceManager::getSingleton().getDefaultScriptModule();

        if(pModule.isValid())
        {
            m_pScriptObject = new Zen::Engine::Input::I_KeyMap::ScriptObjectReference_type
                (pModule, pModule->getScriptType(getScriptTypeName()), this);
        }
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

