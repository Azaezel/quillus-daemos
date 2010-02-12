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

#include "KeyModifierState.hpp"

#include <Zen/Engine/Input/I_InputService.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZInput {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const KeyModifierState::KeyModifierMap_type KeyModifierState::sm_keyModifierPressedMap = boost::assign::map_list_of
    (Engine::Input::KC_LSHIFT,     boost::bind(&Engine::Input::I_KeyModifierState::leftShiftPressed,_1,_2))
    (Engine::Input::KC_RSHIFT,     boost::bind(&Engine::Input::I_KeyModifierState::rightShiftPressed,_1,_2))
    (Engine::Input::KC_LCONTROL,   boost::bind(&Engine::Input::I_KeyModifierState::leftCtrlPressed,_1,_2))
    (Engine::Input::KC_RCONTROL,   boost::bind(&Engine::Input::I_KeyModifierState::rightCtrlPressed,_1,_2))
    (Engine::Input::KC_LMENU,      boost::bind(&Engine::Input::I_KeyModifierState::leftAltPressed,_1,_2))
    (Engine::Input::KC_RMENU,      boost::bind(&Engine::Input::I_KeyModifierState::rightAltPressed,_1,_2))
    (Engine::Input::KC_LWIN,       boost::bind(&Engine::Input::I_KeyModifierState::leftWinPressed,_1,_2))
    (Engine::Input::KC_RWIN,       boost::bind(&Engine::Input::I_KeyModifierState::rightWinPressed,_1,_2));

const KeyModifierState::KeyModifierMap_type KeyModifierState::sm_keyModifierReleasedMap = boost::assign::map_list_of
    (Engine::Input::KC_LSHIFT,     boost::bind(&Engine::Input::I_KeyModifierState::leftShiftReleased,_1,_2))
    (Engine::Input::KC_RSHIFT,     boost::bind(&Engine::Input::I_KeyModifierState::rightShiftReleased,_1,_2))
    (Engine::Input::KC_LCONTROL,   boost::bind(&Engine::Input::I_KeyModifierState::leftCtrlReleased,_1,_2))
    (Engine::Input::KC_RCONTROL,   boost::bind(&Engine::Input::I_KeyModifierState::rightCtrlReleased,_1,_2))
    (Engine::Input::KC_LMENU,      boost::bind(&Engine::Input::I_KeyModifierState::leftAltReleased,_1,_2))
    (Engine::Input::KC_RMENU,      boost::bind(&Engine::Input::I_KeyModifierState::rightAltReleased,_1,_2))
    (Engine::Input::KC_LWIN,       boost::bind(&Engine::Input::I_KeyModifierState::leftWinReleased,_1,_2))
    (Engine::Input::KC_RWIN,       boost::bind(&Engine::Input::I_KeyModifierState::rightWinReleased,_1,_2));
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
KeyModifierState::operator==(const Engine::Input::I_KeyModifierState& _rhs) const
{
    return (this == &_rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
KeyModifierState::operator|=(const Engine::Input::I_KeyModifierState& _rhs) const
{
    return (this == &_rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
KeyModifierState::operator<(const Engine::Input::I_KeyModifierState& _rhs) const
{
    return getSideEnum() < _rhs.getSideEnum();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NullKeyModifierState::NullKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NullKeyModifierState::~NullKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftShiftPressed(Engine::Input::I_InputService& _service)
{
    changeState(_service,LeftShiftKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftShiftReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightShiftPressed(Engine::Input::I_InputService& _service)
{
    changeState(_service,RightShiftKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightShiftReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service,LeftCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service,RightCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service,LeftAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service,RightAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service,LeftWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::leftWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service,RightWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NullKeyModifierState::rightWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NullKeyModifierState NullKeyModifierState::sm_keyModifierState;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Input::I_KeyModifierState&
NullKeyModifierState::instance()
{
    return sm_keyModifierState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LeftShiftKeyModifierState::LeftShiftKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LeftShiftKeyModifierState::~LeftShiftKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftShiftPressed(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftShiftReleased(Engine::Input::I_InputService& _service)
{
    changeState(_service, NullKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightShiftPressed(Engine::Input::I_InputService& _service)
{
    changeState(_service, BothShiftKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightShiftReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, LeftShiftLeftCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, LeftShiftRightCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, LeftShiftLeftAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, LeftShiftRightAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, LeftShiftLeftWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::leftWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, LeftShiftRightWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LeftShiftKeyModifierState::rightWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LeftShiftKeyModifierState LeftShiftKeyModifierState::sm_keyModifierState;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Input::I_KeyModifierState&
LeftShiftKeyModifierState::instance()
{
    return sm_keyModifierState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RightShiftKeyModifierState::RightShiftKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RightShiftKeyModifierState::~RightShiftKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftShiftPressed(Engine::Input::I_InputService& _service)
{
    changeState(_service, BothShiftKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftShiftReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightShiftPressed(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightShiftReleased(Engine::Input::I_InputService& _service)
{
    changeState(_service, NullKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, RightShiftLeftCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, RightShiftRightCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, RightShiftLeftAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, RightShiftRightAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, RightShiftLeftWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::leftWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, RightShiftRightWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RightShiftKeyModifierState::rightWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RightShiftKeyModifierState RightShiftKeyModifierState::sm_keyModifierState;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Input::I_KeyModifierState&
RightShiftKeyModifierState::instance()
{
    return sm_keyModifierState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BothShiftKeyModifierState::BothShiftKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BothShiftKeyModifierState::~BothShiftKeyModifierState()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftShiftPressed(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftShiftReleased(Engine::Input::I_InputService& _service)
{
    changeState(_service,RightShiftKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightShiftPressed(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightShiftReleased(Engine::Input::I_InputService& _service)
{
    changeState(_service, LeftShiftKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, BothShiftLeftCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightCtrlPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, BothShiftRightCtrlKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightCtrlReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, BothShiftLeftAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightAltPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, BothShiftRightAltKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightAltReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, BothShiftLeftWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::leftWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightWinPressed(Engine::Input::I_InputService& _service)
{
    //changeState(_service, BothShiftRightWinKeyModifierState::instance());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BothShiftKeyModifierState::rightWinReleased(Engine::Input::I_InputService& _service)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BothShiftKeyModifierState BothShiftKeyModifierState::sm_keyModifierState;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Input::I_KeyModifierState&
BothShiftKeyModifierState::instance()
{
    return sm_keyModifierState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
EitherShiftKeyModifierState::operator|=(const Engine::Input::I_KeyModifierState& _rhs) const
{
    return (LeftShiftKeyModifierState::instance() == _rhs ||
            RightShiftKeyModifierState::instance() == _rhs ||
            BothShiftKeyModifierState::instance() == _rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
EitherShiftKeyModifierState EitherShiftKeyModifierState::sm_keyModifierState;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Input::I_KeyModifierState&
EitherShiftKeyModifierState::instance()
{
    return sm_keyModifierState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZInput
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
