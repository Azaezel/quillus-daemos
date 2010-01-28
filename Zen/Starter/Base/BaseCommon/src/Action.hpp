//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
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
#ifndef ZEN_ENGINE_BASE_ACTION_HPP_INCLUDED
#define ZEN_ENGINE_BASE_ACTION_HPP_INCLUDED

#include "../I_BaseAction.hpp"

#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class Action
:   public I_BaseAction
,   public Memory::managed_self_ref<Zen::Engine::Core::I_Action>
{
    /// @name Types
    /// @{
public:
    typedef Core::I_ActionMap::ActionFunction_type          ActionFunction_type;
    /// @}

    /// @name I_Action implementation
    /// @{
public:
    virtual void dispatch(boost::any& _parameter);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name Action implementation
    /// @{
public:
    /// Get the name of the action
    const std::string& getName() const;

    /// Get the description of the action
    const std::string& getDescription() const;

    void setDescription(const std::string& _description);
    /// @}

    /// @name 'Structors
    /// @{
protected:
     /// If you use this constructor, you must also override dispatch()
     Action(const std::string& _name);

public:
             Action(const std::string& _name, ActionFunction_type _function);
    virtual ~Action();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Scripting::I_ObjectReference*           m_pScriptObject;

    std::string         m_name;
    ActionFunction_type m_function;
    std::string         m_description;
    /// @}

};  // class Action

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_BASE_ACTION_HPP_INCLUDED
