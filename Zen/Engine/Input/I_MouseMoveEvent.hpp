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
#ifndef ZEN_ENGINE_INPUT_MANAGER_I_MOUSE_MOVE_EVENT_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_MANAGER_I_MOUSE_MOVE_EVENT_HPP_INCLUDED

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

class INPUTMANAGER_DLL_LINK I_MouseMoveEvent
:   public I_InputEvent
,   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_MouseMoveEvent*                               pScriptObject_type;
    typedef Scripting::ObjectReference<I_MouseMoveEvent>    ScriptObjectReference_type;
    /// @}

    /// @name I_MouseMoveEvent interface
    /// @{
public:
    virtual int getXDelta() = 0;
    virtual int getYDelta() = 0;
    virtual int getZDelta() = 0;
    virtual int getX() = 0;
    virtual int getY() = 0;
    virtual int getZ() = 0;

    virtual const I_KeyModifierState& getModifierState() const = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "MouseMoveEvent"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
//protected:
public:
             I_MouseMoveEvent();
    virtual ~I_MouseMoveEvent();
    /// @}

};  // interface I_MouseMoveEvent;


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
namespace Memory 
{
    /// I_MouseMoveEvent is managed by a factory
    template<>
    struct is_managed_by_factory<Engine::Input::I_MouseMoveEvent> : public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_MANAGER_I_MOUSE_MOVE_EVENT_HPP_INCLUDED
