//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Worlds Game Client
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
#ifndef ZEN_WORLDS_GUI_MANAGER_HPP_INCLUDED
#define ZEN_WORLDS_GUI_MANAGER_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Core/I_Action.hpp>

//#include <CEGUI/CEGUI.h>
//#include <OgreCEGUIRenderer.h>
//#include <OgreCEGUIResourceProvider.h>
//#include <Ogre.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Engine {
        namespace Widgets {
            class I_Widget;
        }   // namespace Widgets
    }   // namespace Engine
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameClient;

class GUIManager
:   public virtual Zen::Scripting::I_ScriptableType
,   public Zen::Memory::managed_self_ref<GUIManager>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<GUIManager>                pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<GUIManager>         ScriptObjectReference_type;
    typedef Zen::Engine::Core::I_Action::pScriptObject_type     pAction_type;
    typedef std::map<std::string, pAction_type>                 EventMap_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name GUIManager implementation
    /// @{
public:
    void init();

    void initMainMenu();

    /// Subscribe to all of the click events for this window
    /// and all child windows.
#if 0   // deprecated
    void subscribeClickEvents(CEGUI::Window* _pParent);
#endif  // deprecated
    void subscribeClickEvents(Zen::Engine::Widgets::I_Widget& _widget);

    void getMousePosition(int &_x, int& _y);
    /// @}

    /// @name Script support
    /// @{
public:
    /// Initialize the script type
    void initScriptType();

    /// Create the script object
    void createScriptObject();

    void mapClickEvent(const std::string& _eventName, pAction_type _pAction);
    /// @}

    /// @name Event Handlers
    /// @{
public:
    void handleMouseMoveEvent(Zen::Engine::Input::I_MouseMoveEvent& _event);
    void handleMouseClickEvent(Zen::Engine::Input::I_MouseClickEvent& _event);
    /// @}

    /// @name Class public implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit GUIManager(GameClient& _gameClient);
    virtual ~GUIManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameClient&                         m_gameClient;

    typedef Zen::Scripting::I_ScriptModule::pScriptType_type    pScriptType_type;
    pScriptType_type                    m_pScriptType;
    ScriptObjectReference_type*         m_pScriptObject;
    /// @}


};  // class GUIManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_WORLDS_GUI_MANAGER_HPP_INCLUDED
