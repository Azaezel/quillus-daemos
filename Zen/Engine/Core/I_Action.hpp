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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_CORE_I_ACTION_HPP_INCLUDED
#define ZEN_ENGINE_CORE_I_ACTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Engine/Input/I_KeyEvent.hpp>
#include <Zen/Engine/Input/I_ButtonEvent.hpp>
#include <Zen/Engine/Input/I_ScalarEvent.hpp>
#include <Zen/Engine/Input/I_VectorEvent.hpp>
#include <Zen/Core/Scripting/I_ScriptableType.hpp>

#include <boost/function.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Core {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// @brief Action interface
class GAMECORE_DLL_LINK I_Action
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Action>                   pScriptObject_type;
    typedef Scripting::ObjectReference<I_Action>            ScriptObjectReference_type;
    typedef Event::Event<I_Action&>                         ActionEvent_type;
    /// @}

    /// @name I_Action interface
    /// @{
public:
    virtual void dispatch(boost::any& _parameter) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "Action"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    /// This event is fired when the action is enabled.
    ActionEvent_type    onEnable;

    /// This event is fired when the action is disabled.
    ActionEvent_type    onDisable;

    /// This event is fired when the action is destroyed.
    ActionEvent_type    onDestroy;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Action();
    virtual ~I_Action();
    /// @}

};  // interface I_Action

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Core
}   // namespace Engine
namespace Memory 
{
    /// I_Action is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Core::I_Action> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CORE_I_ACTION_HPP_INCLUDED
