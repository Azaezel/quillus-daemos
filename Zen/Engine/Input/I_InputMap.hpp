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
#ifndef ZEN_ENGINE_INPUT_MANAGER_I_INPUT_MAP_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_MANAGER_I_INPUT_MAP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Core/I_Action.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Plugins/I_Service.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class INPUTMANAGER_DLL_LINK I_InputMap
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_InputMap*                                         pScriptObject_type;
    typedef Scripting::ObjectReference<I_InputMap>              ScriptObjectReference_type;

    typedef Zen::Memory::managed_ptr<I_InputEvent>              pInputEventPayload_type;
	typedef Zen::Memory::managed_ptr<Core::I_Action>            pAction_type;
    typedef Zen::Event::Event<pInputEventPayload_type>          InputEvent_type;
    typedef Zen::Event::Event<Input::I_InputMap*>               InputMap_event;
    /// @}

    /// @name I_InputMap interface
    /// @{
public:
    /// Map a keystroke to an action by name
    /// @param _name - Textual representation of the key, such as ALT-Q
    /// @param _pAction - Action to take when the key is pressed.
    virtual void mapKeyInput(const std::string& _name, const pAction_type _pAction) = 0;

    /// Set the focus on this input map
    virtual void focus() = 0;

    /// Disable the focus from this input map
    virtual void unfocus() = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "InputMap"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    InputMap_event onDestroyEvent;
    InputEvent_type onInputEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_InputMap();
    virtual ~I_InputMap();
    /// @}

};  // interface I_InputMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
namespace Memory 
{
    /// I_InputMap is managed by a factory
    template<>
	struct is_managed_by_factory<Zen::Engine::Input::I_InputMap> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_MANAGER_I_INPUT_MAP_HPP_INCLUDED
