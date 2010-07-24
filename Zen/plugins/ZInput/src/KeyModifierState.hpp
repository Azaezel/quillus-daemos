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
#ifndef ZEN_ZINPUT_KEY_MODIFIER_STATE_HPP_INCLUDED
#define ZEN_ZINPUT_KEY_MODIFIER_STATE_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Engine/Input/I_KeyModifierState.hpp>
#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/KeyCodes.hpp>

#include <boost/function.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZInput {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class KeyModifierState
:   public Engine::Input::I_KeyModifierState
{
    /// @name Types
    /// @{
public:
    typedef std::map< Engine::Input::KeyCode_enum, boost::function<void (Engine::Input::I_KeyModifierState*, Engine::Input::I_InputService&)> >   KeyModifierMap_type;   
    /// @}

    /// @name I_KeyModifierState implementation
    /// @{
public:
    virtual void leftShiftPressed(Engine::Input::I_InputService& _service) {}
    virtual void leftShiftReleased(Engine::Input::I_InputService& _service) {}
    virtual void rightShiftPressed(Engine::Input::I_InputService& _service) {}
    virtual void rightShiftReleased(Engine::Input::I_InputService& _service) {}
    virtual void leftCtrlPressed(Engine::Input::I_InputService& _service) {}
    virtual void leftCtrlReleased(Engine::Input::I_InputService& _service) {}
    virtual void rightCtrlPressed(Engine::Input::I_InputService& _service) {}
    virtual void rightCtrlReleased(Engine::Input::I_InputService& _service) {}
    virtual void leftAltPressed(Engine::Input::I_InputService& _service) {}
    virtual void leftAltReleased(Engine::Input::I_InputService& _service) {}
    virtual void rightAltPressed(Engine::Input::I_InputService& _service) {}
    virtual void rightAltReleased(Engine::Input::I_InputService& _service) {}
    virtual void leftWinPressed(Engine::Input::I_InputService& _service) {}
    virtual void leftWinReleased(Engine::Input::I_InputService& _service) {}
    virtual void rightWinPressed(Engine::Input::I_InputService& _service) {}
    virtual void rightWinReleased(Engine::Input::I_InputService& _service) {}
    virtual bool operator==(const Engine::Input::I_KeyModifierState& _rhs) const;
    virtual bool operator|=(const Engine::Input::I_KeyModifierState& _rhs) const;
    virtual bool operator<(const Engine::Input::I_KeyModifierState& _rhs) const;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             KeyModifierState() {};
    virtual ~KeyModifierState() {};
    /// @}

    /// @name Member variables
    /// @{
public:
    static const KeyModifierMap_type   sm_keyModifierPressedMap;
    static const KeyModifierMap_type   sm_keyModifierReleasedMap;
    /// @}

};  // class KeyModifierState

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class NullKeyModifierState
:   public KeyModifierState
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_KeyModifierState implementation
    /// @{
public:
    virtual KeyState_enum getStateEnum() const { return MKEY_NONE; }
    virtual KeySide_enum getSideEnum() const { return MSIDE_NEITHER; }
    virtual void leftShiftPressed(Engine::Input::I_InputService& _service);
    virtual void leftShiftReleased(Engine::Input::I_InputService& _service);
    virtual void rightShiftPressed(Engine::Input::I_InputService& _service);
    virtual void rightShiftReleased(Engine::Input::I_InputService& _service);
    virtual void leftCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void leftCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void rightCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void rightCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void leftAltPressed(Engine::Input::I_InputService& _service);
    virtual void leftAltReleased(Engine::Input::I_InputService& _service);
    virtual void rightAltPressed(Engine::Input::I_InputService& _service);
    virtual void rightAltReleased(Engine::Input::I_InputService& _service);
    virtual void leftWinPressed(Engine::Input::I_InputService& _service);
    virtual void leftWinReleased(Engine::Input::I_InputService& _service);
    virtual void rightWinPressed(Engine::Input::I_InputService& _service);
    virtual void rightWinReleased(Engine::Input::I_InputService& _service);
    /// @}

    /// @name Static methods
    /// @{
public:
    static I_KeyModifierState& instance();
    /// @}

    /// @name 'Structors
    /// @{
private:
             NullKeyModifierState();
    virtual ~NullKeyModifierState();
    /// @}

    /// @name Member variables
    /// @{
private:
    static NullKeyModifierState     sm_keyModifierState;
    /// @}

};  // NullKeyModifierState interface.

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class LeftShiftKeyModifierState
:   public KeyModifierState
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_KeyModifierState implementation
    /// @{
public:
    virtual KeyState_enum getStateEnum() const { return MKEY_SHIFT; }
    virtual KeySide_enum getSideEnum() const { return MSIDE_LEFT; }
    virtual void leftShiftPressed(Engine::Input::I_InputService& _service);
    virtual void leftShiftReleased(Engine::Input::I_InputService& _service);
    virtual void rightShiftPressed(Engine::Input::I_InputService& _service);
    virtual void rightShiftReleased(Engine::Input::I_InputService& _service);
    virtual void leftCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void leftCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void rightCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void rightCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void leftAltPressed(Engine::Input::I_InputService& _service);
    virtual void leftAltReleased(Engine::Input::I_InputService& _service);
    virtual void rightAltPressed(Engine::Input::I_InputService& _service);
    virtual void rightAltReleased(Engine::Input::I_InputService& _service);
    virtual void leftWinPressed(Engine::Input::I_InputService& _service);
    virtual void leftWinReleased(Engine::Input::I_InputService& _service);
    virtual void rightWinPressed(Engine::Input::I_InputService& _service);
    virtual void rightWinReleased(Engine::Input::I_InputService& _service);
    /// @}

    /// @name Static methods
    /// @{
public:
    static I_KeyModifierState& instance();
    /// @}

    /// @name 'Structors
    /// @{
private:
             LeftShiftKeyModifierState();
    virtual ~LeftShiftKeyModifierState();
    /// @}

    /// @name Member variables
    /// @{
private:
    static LeftShiftKeyModifierState     sm_keyModifierState;
    /// @}

};  // LeftShiftKeyModifierState interface.

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RightShiftKeyModifierState
:   public KeyModifierState
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_KeyModifierState implementation
    /// @{
public:
    virtual KeyState_enum getStateEnum() const { return MKEY_SHIFT; }
    virtual KeySide_enum getSideEnum() const { return MSIDE_RIGHT; }
    virtual void leftShiftPressed(Engine::Input::I_InputService& _service);
    virtual void leftShiftReleased(Engine::Input::I_InputService& _service);
    virtual void rightShiftPressed(Engine::Input::I_InputService& _service);
    virtual void rightShiftReleased(Engine::Input::I_InputService& _service);
    virtual void leftCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void leftCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void rightCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void rightCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void leftAltPressed(Engine::Input::I_InputService& _service);
    virtual void leftAltReleased(Engine::Input::I_InputService& _service);
    virtual void rightAltPressed(Engine::Input::I_InputService& _service);
    virtual void rightAltReleased(Engine::Input::I_InputService& _service);
    virtual void leftWinPressed(Engine::Input::I_InputService& _service);
    virtual void leftWinReleased(Engine::Input::I_InputService& _service);
    virtual void rightWinPressed(Engine::Input::I_InputService& _service);
    virtual void rightWinReleased(Engine::Input::I_InputService& _service);
    /// @}

    /// @name Static methods
    /// @{
public:
    static I_KeyModifierState& instance();
    /// @}

    /// @name 'Structors
    /// @{
private:
             RightShiftKeyModifierState();
    virtual ~RightShiftKeyModifierState();
    /// @}

    /// @name Member variables
    /// @{
private:
    static RightShiftKeyModifierState     sm_keyModifierState;
    /// @}

};  // RightShiftKeyModifierState interface.

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BothShiftKeyModifierState
:   public KeyModifierState
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_KeyModifierState implementation
    /// @{
public:
    virtual KeyState_enum getStateEnum() const { return MKEY_SHIFT; }
    virtual KeySide_enum getSideEnum() const { return MSIDE_RIGHT; }
    virtual void leftShiftPressed(Engine::Input::I_InputService& _service);
    virtual void leftShiftReleased(Engine::Input::I_InputService& _service);
    virtual void rightShiftPressed(Engine::Input::I_InputService& _service);
    virtual void rightShiftReleased(Engine::Input::I_InputService& _service);
    virtual void leftCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void leftCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void rightCtrlPressed(Engine::Input::I_InputService& _service);
    virtual void rightCtrlReleased(Engine::Input::I_InputService& _service);
    virtual void leftAltPressed(Engine::Input::I_InputService& _service);
    virtual void leftAltReleased(Engine::Input::I_InputService& _service);
    virtual void rightAltPressed(Engine::Input::I_InputService& _service);
    virtual void rightAltReleased(Engine::Input::I_InputService& _service);
    virtual void leftWinPressed(Engine::Input::I_InputService& _service);
    virtual void leftWinReleased(Engine::Input::I_InputService& _service);
    virtual void rightWinPressed(Engine::Input::I_InputService& _service);
    virtual void rightWinReleased(Engine::Input::I_InputService& _service);
    /// @}

    /// @name Static methods
    /// @{
public:
    static I_KeyModifierState& instance();
    /// @}

    /// @name 'Structors
    /// @{
private:
             BothShiftKeyModifierState();
    virtual ~BothShiftKeyModifierState();
    /// @}

    /// @name Member variables
    /// @{
private:
    static BothShiftKeyModifierState     sm_keyModifierState;
    /// @}

};  // BothShiftKeyModifierState interface.

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class EitherShiftKeyModifierState
:   public KeyModifierState
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_KeyModifierState implementation
    /// @{
public:
    virtual KeyState_enum getStateEnum() const { return MKEY_SHIFT; }
    virtual KeySide_enum getSideEnum() const { return MSIDE_EITHER; }
    virtual bool operator|=(const Engine::Input::I_KeyModifierState& _rhs) const;
    /// @}

    /// @name Static methods
    /// @{
public:
    static I_KeyModifierState& instance();
    /// @}

    /// @name 'Structors
    /// @{
private:
             EitherShiftKeyModifierState() {};
    virtual ~EitherShiftKeyModifierState() {};
    /// @}

    /// @name Member variables
    /// @{
private:
    static EitherShiftKeyModifierState     sm_keyModifierState;
    /// @}

};  // EitherShiftKeyModifierState interface.

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// @todo Implement the remaining KeyModifierStates

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZInput
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZINPUT_KEY_MODIFIER_STATE_HPP_INCLUDED
