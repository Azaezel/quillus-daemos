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
#ifndef ZEN_ENGINE_RULES_I_STATE_HPP_INCLUDED
#define ZEN_ENGINE_RULES_I_STATE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Core {
        class I_Action;
    }
namespace Rules {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_GameRule;
class I_StateMachine;
class I_Transition;

class RULES_DLL_LINK I_State
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_GameRule>     pGameRule_type;
    typedef Memory::managed_ptr<Core::I_Action> pAction_type;
    typedef Event::Event<I_State&>              StateEvent_type;
    /// @}

    /// @name I_State interface
    /// @{
public:
    /// Get the state machine to which this state belongs
    virtual I_StateMachine& getParent() const = 0;

    virtual const std::string& getName() const = 0;

    /// Adds an input action associated with this state
    virtual void addAction(pAction_type _pAction) = 0;

    /// Adds a game rule associated with this state.
    /// @todo Is this even used?
    virtual void addRule(pGameRule_type _pGameRule) = 0;

    /// Create a Transition that transitions from one state to another.
    virtual I_Transition& createTransition(const std::string& _transitionName, const I_State& _destination) = 0;

    /// Get a transition by name
    virtual I_Transition& getTransition(const std::string& _transitionName) const = 0;

    /// @}

    /// @name Events
    /// @{
public:
    StateEvent_type onEnter;
    StateEvent_type onExit;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_State();
    virtual ~I_State();
    /// @}

};  // interface I_State

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rules
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RULES_I_STATE_HPP_INCLUDED
