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
#ifndef ZEN_ENGINE_INPUT_MANAGER_I_MOUSE_CLICK_EVENT_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_MANAGER_I_MOUSE_CLICK_EVENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Engine/Input/I_InputEvent.hpp>
#include <Zen/Engine/Input/ButtonIDs.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_KeyModifierState;

class INPUTMANAGER_DLL_LINK I_MouseClickEvent
:   public I_InputEvent
,   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_MouseClickEvent*                              pScriptObject_type;
    typedef Scripting::ObjectReference<I_MouseClickEvent>   ScriptObjectReference_type;
    /// @}

    /// @name I_MouseClickEvent interface
    /// @{
public:
    /// @return true if a mouse button was clicked, false if a mouse button was un-clicked
    virtual bool wasClicked() = 0;

    /// @return the button id of the button that was clicked or unclicked
    virtual Zen::Engine::Input::ButtonID getButton() = 0;

    virtual const I_KeyModifierState& getModifierState() const = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "MouseClickEvent"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
//protected:
public:
             I_MouseClickEvent();
    virtual ~I_MouseClickEvent();
    /// @}

};  // interface I_MouseClickEvent;


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
namespace Memory 
{
    /// I_MouseClickEvent is managed by a factory
    template<>
    struct is_managed_by_factory<Engine::Input::I_MouseClickEvent> : public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_MANAGER_I_MOUSE_CLICK_EVENT_HPP_INCLUDED
