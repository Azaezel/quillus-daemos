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
#ifndef ZEN_ZINPUT_INPUT_MAP_HPP_INCLUDED
#define ZEN_ZINPUT_INPUT_MAP_HPP_INCLUDED

#include "KeyState.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Engine/Input/I_InputMap.hpp>
#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_KeyModifierState.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
namespace ZInput {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class InputMap
:   public Engine::Input::I_InputMap
{
    /// @name Types
    /// @{
public:
    typedef std::map< std::string, const KeyState >   KeyMapTable_type;
    typedef Memory::managed_ptr<Engine::Input::I_InputService>          pInputService_type;
    typedef Memory::managed_weak_ptr<Engine::Input::I_InputService>     wpInputService_type;

    typedef Memory::managed_ptr<Engine::Input::I_KeyEvent>              pKeyEvent_type;
    /// @}

    /// @name I_InputMap implementation
    /// @{
public:
    virtual void mapKeyInput(const std::string& _name, const pAction_type _pAction);
    virtual void focus();
    virtual void unfocus();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name InputMap implementation
    /// @{
private:
    /// @}

    /// @name Event Handlers
    /// @{
private:
    void onKeyPressed(pKeyEvent_type _pEvent);
    /// @}

    /// @name 'Structors
    /// @{
public:
             InputMap(wpInputService_type _pInputService);
    virtual ~InputMap();
    /// @}

    /// @name Member variables
    /// @{
private:
    wpInputService_type                     m_pInputService;
    Scripting::I_ObjectReference*           m_pScriptObject;

    Threading::I_Mutex*     m_pMutex;

    /// Standard character map
    typedef Zen::Engine::Input::KeyCode_enum                            KeyCode_enum;
    typedef Zen::Engine::Input::I_KeyModifierState::KeyState_enum       KeyState_enum;
    //typedef std::map<KeyCode_enum, pAction_type>                        CharacterMap_type;
    typedef std::multimap<KeyState, pAction_type>                       KeyMap_type;

    //CharacterMap_type                                                   m_characterMap[Zen::Engine::Input::I_KeyModifierState::MKEY_COUNT];
    KeyMap_type                                                         m_keyMap;

    Zen::Engine::Input::I_InputService::key_event::pConnection_type     m_eventConnection;

    static const KeyMapTable_type                                       sm_keyMapTable;

    bool                                                                m_isFocused;
    /// @}

};  // class InputService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZInput
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZINPUT_INPUT_MAP_HPP_INCLUDED
