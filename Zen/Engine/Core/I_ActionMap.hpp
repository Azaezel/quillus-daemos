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
#ifndef ZEN_ENGINE_CORE_I_ACTION_MAP_HPP_INCLUDED
#define ZEN_ENGINE_CORE_I_ACTION_MAP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

//#include <Zen/Core/Event/Event.hpp>

//#include <Zen/Engine/Input/I_KeyEvent.hpp>
//#include <Zen/Engine/Input/I_ButtonEvent.hpp>
//#include <Zen/Engine/Input/I_ScalarEvent.hpp>
//#include <Zen/Engine/Input/I_VectorEvent.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/function.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Core {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Action;

/// @brief Action interface
class GAMECORE_DLL_LINK I_ActionMap
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef boost::function<void(boost::any&)>                          ActionFunction_type;

    typedef Zen::Memory::managed_ptr<Zen::Engine::Core::I_Action>       pAction_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Core::I_Action>  wpAction_type;

    typedef I_ActionMap*                                                pScriptObject_type;
    typedef Scripting::ObjectReference<I_ActionMap>                     ScriptObjectReference_type;
    /// @}

    /// @name I_ActionMap interface
    /// @{
public:
    /// Create an action by name and bind it to a function that gets envoked when
    /// the action occurs.
    virtual void createAction(const std::string& _name, ActionFunction_type _function) = 0;

    /// Get an action by name.
    virtual pAction_type operator[](const std::string& _name) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "ActionMap"
    /// Override this method if you create a derived type
    const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ActionMap();
    virtual ~I_ActionMap();
    /// @}

};  // interface I_ActionMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Core
}   // namespace Engine
namespace Memory 
{
    /// I_ActionMap is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Core::I_ActionMap> : public boost::false_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CORE_I_ACTION_MAP_HPP_INCLUDED
