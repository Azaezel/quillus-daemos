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
#include "GUIManager.hpp"
#include "GameClient.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Math/Radian.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Core/I_ActionMap.hpp>

#include <Zen/Engine/Input/I_MouseMoveEvent.hpp>
#include <Zen/Engine/Input/I_MouseClickEvent.hpp>

#include <Zen/Engine/Widgets/I_WidgetService.hpp>
#include <Zen/Engine/Widgets/I_Widget.hpp>
#include <Zen/Engine/Widgets/I_WidgetEvent.hpp>
#include <Zen/Engine/Widgets/I_FontService.hpp>

#include <Zen/plugins/ZCrazyEddie/I_WidgetService.hpp>
#include <Zen/plugins/ZCrazyEddie/I_ImageSetService.hpp>
#include <Zen/plugins/ZCrazyEddie/I_ImageSet.hpp>
#include <Zen/plugins/ZCrazyEddie/I_SchemeService.hpp>

#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_Camera.hpp>

// For non-Win32 platforms, use SDL for mouse stuff
#ifndef WIN32
#include "SDL/SDL.h"
#endif

#include <boost/bind.hpp>

#include <iostream>



//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
GUIManager::GUIManager(GameClient& _gameClient, Ogre::RenderWindow* _pRenderWindow, Zen::Engine::Input::I_InputService& _inputService)
:   m_gameClient(_gameClient)
,   m_pRenderWindow(_pRenderWindow)
,   m_mouseX(0)
,   m_mouseY(0)
#endif  // deprecated
GUIManager::GUIManager(GameClient& _gameClient)
:   m_gameClient(_gameClient)
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GUIManager::~GUIManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::init()
{
    // Hook up the mouse click event
    m_gameClient.base().getWidgetService().onMouseMoveEvent.connect(boost::bind(&GUIManager::handleMouseMoveEvent, this, _1));
    m_gameClient.base().getWidgetService().onMouseClickEvent.connect(boost::bind(&GUIManager::handleMouseClickEvent, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::initMainMenu()
{
    Zen::Engine::Widgets::I_WidgetService& widgetService(
        m_gameClient.base().getWidgetService()
    );

    Zen::ZCrazyEddie::I_WidgetService* pCEWidgetService =
        dynamic_cast<Zen::ZCrazyEddie::I_WidgetService*>(&widgetService);

    pCEWidgetService->setMouseCursor(
        pCEWidgetService->getImageSetService().createImageSet("CyberLook.imageset"),
        "MouseArrow"
    );

    // Hide the operating system's mouse cursor.
    pCEWidgetService->hideMouseCursor();

    // Subscribe to click events for every window in the GUI so that
    // the script can hook up actions to them.
    subscribeClickEvents(
        widgetService.getWidget("root")
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::subscribeClickEvents(Zen::Engine::Widgets::I_Widget& _widget)
{
    std::string type = _widget.getType();

    class ClickEventSubscriptionVisitor
    :   public Zen::Engine::Widgets::I_Widget::I_ChildVisitor
    {

    public:

        virtual void begin() {}

        virtual void visit(Zen::Engine::Widgets::I_Widget& _child)
        {
            m_manager.subscribeClickEvents(_child);
        }

        virtual void end() {}

        ClickEventSubscriptionVisitor(GUIManager& _manager)
        :   m_manager(_manager)
        {}

    private:
        GUIManager&     m_manager;

    };  // class ClickEventSubscriptionVisitor

    /// TODO Determine if the action needs to be
    /// created here or within I_WidgetService et al
    m_gameClient.game().getActionMap().createAction(
        _widget.getName() + "::ButtonClick",
        boost::bind(
            &Zen::Engine::Widgets::I_Widget::handleButtonClick,
            &_widget,
            _1
        )
    );

    ClickEventSubscriptionVisitor visitor(*this);
    _widget.getChildren(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::getMousePosition(int& _x, int& _y)
{
    Zen::Engine::Widgets::I_WidgetService& widgetService(
        m_gameClient.base().getWidgetService()
    );

    Zen::ZCrazyEddie::I_WidgetService* pCEWidgetService =
        dynamic_cast<Zen::ZCrazyEddie::I_WidgetService*>(&widgetService);

    pCEWidgetService->getMousePosition(_x,_y);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void script_mapClickEvent(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    typedef GUIManager                              Object_type;
    typedef Object_type::ScriptObjectReference_type ObjectRef_type;
    GUIManager* const pGUIManager = (dynamic_cast<ObjectRef_type*>(_pObject))->getRawObject();
    assert(pGUIManager != NULL);

    // The first parameter is the click event name
    std::string key = boost::any_cast<std::string>(_parms[0]);

    // The second parameter is an action
    Zen::Scripting::I_ObjectReference* pActionObj = boost::any_cast<Scripting::I_ObjectReference*>(_parms[1]);

    Zen::Engine::Core::I_Action::ScriptObjectReference_type* pAction = dynamic_cast<Zen::Engine::Core::I_Action::ScriptObjectReference_type*>(pActionObj);


    pGUIManager->mapClickEvent(key, pAction->getObject());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::initScriptType()
{
    m_pScriptType = m_gameClient.base().getScriptModule()->createScriptType(getScriptTypeName(), "Widget Service", 0);

    m_pScriptType->addMethod("mapClickEvent", "Map a click event", script_mapClickEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::createScriptObject()
{
    m_pScriptObject = new ScriptObjectReference_type
        (
            m_gameClient.base().getScriptModule(),
            m_gameClient.base().getScriptModule()->getScriptType(getScriptTypeName()),
            getSelfReference().lock(),
            "widgetService"
        );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
GUIManager::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_scriptTypeName("WidgetService");
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GUIManager::getScriptTypeName()
{
    return sm_scriptTypeName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::handleMouseMoveEvent(Zen::Engine::Input::I_MouseMoveEvent& _event)
{
    // If the shift key is down, the mouse moves the camera, otherwise the mouse moves
    // the direction that the player is facting.
    Zen::Engine::Rendering::I_Camera& camera = m_gameClient.base().getRenderingCanvas().getCurrentCamera();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::handleMouseClickEvent(Zen::Engine::Input::I_MouseClickEvent& _event)
{
    m_gameClient.handleMouseClickEvent(_event);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GUIManager::mapClickEvent(const std::string& _eventName, pAction_type _pAction)
{
#if 0   // deprecated
    // TODO This should be moved to widget service implementation and should be part of
    // I_WidgetService::getEvent()->attachToAction()
    m_clickEvents[_eventName] = _pAction;
#endif  // deprecated

    Zen::Engine::Widgets::I_WidgetService& widgetService(
        m_gameClient.base().getWidgetService()
    );

    widgetService.getWidget(_eventName).getEvent("Clicked").attachToAction(_pAction);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
