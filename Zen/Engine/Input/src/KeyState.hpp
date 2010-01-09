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
#ifndef ZEN_ENGINE_KEY_STATE_HPP_INCLUDED
#define ZEN_ENGINE_KEY_STATE_HPP_INCLUDED

#include <Zen/Engine/Input/I_KeyState.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class KeyState
:   public I_KeyState
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_KeyState implementation
    /// @{
public:
    virtual Engine::Input::KeyCode_enum getKeyCode() const;
    virtual const Engine::Input::I_KeyModifierState& getModifierState() const;
    virtual bool operator==(Engine::Input::KeyCode_enum _rhs) const;
    virtual bool operator==(const Engine::Input::I_KeyModifierState& _rhs) const;
    virtual bool operator|=(const Engine::Input::I_KeyModifierState& _rhs) const;
    virtual bool operator==(const Engine::Input::I_KeyState& _rhs) const;
    virtual bool operator<(const Engine::Input::I_KeyState& _rhs) const;
    /// @}

    /// @name KeyState implementation
    /// @{
public:
    KeyState& operator=(const KeyState& _keyState);
    /// @}

    /// @name 'Structors
    /// @{
public:
             KeyState(Engine::Input::KeyCode_enum _code, const Engine::Input::I_KeyModifierState* _pState);
             KeyState(const KeyState& _keyState);
             KeyState(const I_KeyState& _keyState);
    virtual ~KeyState();
    /// @}

    /// @name Member variables
    /// @{
private:
    Engine::Input::KeyCode_enum                 m_code;
    const Engine::Input::I_KeyModifierState*    m_pModifierState;
    /// @}

};  // class KeyState

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_KEY_STATE_HPP_INCLUDED
