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
#ifndef ZEN_ENGINE_RULES_STATE_MACHINE_HPP_INCLUDED
#define ZEN_ENGINE_RULES_STATE_MACHINE_HPP_INCLUDED
#if 0 // TODO Implement
#include "../I_StateMachine.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rules {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_State;
class I_Transition;

class RULES_DLL_LINK I_StateMachine
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_StateMachine interface
    /// @{
public:
    /// Create a new State
    /// @param _stateName Name of the state.
    /// @return A reference to a state.  The state lifetime is as long as
    ///     the lifetime of this object, so take care if you maintain a
    ///     reference to it.  Generally use getState() if you need it again.
    virtual I_State& createState(const std::string& _stateName) = 0;

    /// Get an existing State by name
    virtual I_State& getState(const std::string& _stateName) const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             I_StateMachine();
    virtual ~I_StateMachine();
    /// @}

};  // interface I_StateMachine

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rules
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // TODO Implement
#endif // ZEN_ENGINE_RULES_I_STATE_MACHINE_HPP_INCLUDED
