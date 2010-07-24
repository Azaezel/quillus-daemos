//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2006 Phillip Castaneda (pjcast -- www.wreckedgames.com)
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
#ifndef ZEN_ENGINE_INPUT_I_KEY_MODIFIER_STATE_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_I_KEY_MODIFIER_STATE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_InputService;

class INPUTMANAGER_DLL_LINK I_KeyModifierState
{
    /// @name Types
    /// @{
public:
    enum KeySide_enum
    {
        MSIDE_NEITHER,
        MSIDE_LEFT,
        MSIDE_RIGHT,
        MSIDE_EITHER,
        MSIDE_BOTH,
        MSIDE_COUNT
    };

    enum KeyState_enum
    {
        MKEY_NONE,
        MKEY_CTRL,
        MKEY_SHIFT,
        MKEY_ALT,
        MKEY_WIN,
        MKEY_CTRL_SHIFT,
        MKEY_CTRL_ALT,
        MKEY_CTRL_WIN,
        MKEY_SHIFT_ALT,
        MKEY_SHIFT_WIN,
        MKEY_ALT_WIN,
        MKEY_CTRL_SHIFT_ALT,
        MKEY_CTRL_SHIFT_WIN,
        MKEY_CTRL_ALT_WIN,
        MKEY_CTRL_SHIFT_ALT_WIN,
        MKEY_COUNT
    };

    typedef Zen::Event::Event<I_KeyModifierState&>      keyModifierState_event;
    /// @}

    /// @name I_KeyModifierState interface
    /// @{
public:
    virtual KeyState_enum getStateEnum() const = 0;

    virtual KeySide_enum getSideEnum() const = 0;

    virtual void leftShiftPressed(I_InputService& _service) = 0;

    virtual void leftShiftReleased(I_InputService& _service) = 0;

    virtual void rightShiftPressed(I_InputService& _service) = 0;

    virtual void rightShiftReleased(I_InputService& _service) = 0;

    virtual void leftCtrlPressed(I_InputService& _service) = 0;

    virtual void leftCtrlReleased(I_InputService& _service) = 0;

    virtual void rightCtrlPressed(I_InputService& _service) = 0;

    virtual void rightCtrlReleased(I_InputService& _service) = 0;

    virtual void leftAltPressed(I_InputService& _service) = 0;

    virtual void leftAltReleased(I_InputService& _service) = 0;

    virtual void rightAltPressed(I_InputService& _service) = 0;

    virtual void rightAltReleased(I_InputService& _service) = 0;

    virtual void leftWinPressed(I_InputService& _service) = 0;

    virtual void leftWinReleased(I_InputService& _service) = 0;

    virtual void rightWinPressed(I_InputService& _service) = 0;

    virtual void rightWinReleased(I_InputService& _service) = 0;

    virtual bool operator==(const I_KeyModifierState& _rhs) const = 0;

    virtual bool operator|=(const I_KeyModifierState& _rhs) const = 0;

    virtual bool operator<(const I_KeyModifierState& _rhs) const = 0;

protected:
    virtual void changeState(I_InputService& _inputService, I_KeyModifierState& _state) const;
    /// @}

    /// @name Events
    /// @{
public:
    keyModifierState_event      onStateEnter;
    keyModifierState_event      onStateExit;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_KeyModifierState();
    virtual ~I_KeyModifierState();
    /// @}

};  // I_KeyModifierState interface.

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_I_KEY_MODIFIER_STATE_HPP_INCLUDED
