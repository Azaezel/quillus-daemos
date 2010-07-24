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
#ifndef ZEN_ZINPUT_MOUSE_CLICK_EVENT_HPP_INCLUDED
#define ZEN_ZINPUT_MOUSE_CLICK_EVENT_HPP_INCLUDED

#include <Zen/Engine/Input/I_MouseClickEvent.hpp>

#include <OISMouse.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZInput {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class MouseClickEvent
:   public Engine::Input::I_MouseClickEvent
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_MouseClickEvent implementation
    /// @{
public:
    virtual bool wasClicked();
    virtual Zen::Engine::Input::ButtonID getButton();
    virtual const Engine::Input::I_KeyModifierState& getModifierState() const;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name MouseClickEvent implementation
    /// @{
public:
    static void registerScriptModule(Zen::Scripting::script_module& _module);

    inline
    int scriptGetButton()
    {
        return static_cast<int>(getButton());
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    MouseClickEvent(const OIS::MouseEvent& _event, bool _clicked, OIS::MouseButtonID _button, const Engine::Input::I_KeyModifierState& _state);
    virtual ~MouseClickEvent();
    /// @}

    /// @name Member Variables
    /// @{
private:
    const OIS::MouseEvent&                      m_event;
    bool                                        m_clicked;
    OIS::MouseButtonID                          m_button;
    const Engine::Input::I_KeyModifierState&    m_modifierState;
    Scripting::I_ObjectReference*               m_pScriptObject;

    static Zen::Scripting::script_module*       sm_pModule;
    /// @}

};  // class MouseClickEvent

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZInput
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZINPUT_MOUSE_CLICK_EVENT_HPP_INCLUDED
