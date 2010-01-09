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

#include "Widget.hpp"
#include "WidgetService.hpp"
#include "WidgetEvent.hpp"
#include "WidgetProperty.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Widget::Widget(WidgetService& _service, CEGUI::Window& _window)
:   m_service(_service)
,   m_window(_window)
,   m_name(_window.getName().c_str())
,   m_type(_window.getType().c_str())
,   m_style(_window.getLookNFeel().c_str())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Widget::~Widget()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Widget::getName()
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Widget::getType()
{
    return m_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Widget::getStyle()
{
    return m_style;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Widget::pWidget_type
Widget::getParent()
{
    Widget* pWidget =
        static_cast<Widget*>(m_window.getParent()->getUserData());

    return pWidget;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Widget::pWidget_type
Widget::getChild(const std::string& _name)
{
    try
    {
        CEGUI::Window* pWindow =
            m_window.getChild((CEGUI::utf8*)_name.c_str());

        Widget* pWidget =
            static_cast<Widget*>(pWindow->getUserData());

        return pWidget;
    }
    catch(CEGUI::UnknownObjectException& _e)
    {
        return NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Widget::getChildren(Zen::Engine::Widgets::I_Widget::I_ChildVisitor& _visitor)
{
    _visitor.begin();

    for( unsigned int i = 0; i < m_window.getChildCount(); i++ )
    {
        _visitor.visit(
            m_service.getWidget(
                m_window.getChildAtIdx(i)->getName().c_str()
            )
        );
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Widget::wpWidget_type
Widget::getAsWeakReference()
{
    return this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Widget::addChild(pWidget_type _pWidget)
{
    Widget* pWidget =
        dynamic_cast<Widget*>(_pWidget);

    if( pWidget != NULL )
    {
        m_window.addChildWindow(
            pWidget->getCEGUIWindow()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Widget::destroyChild(const std::string& _name)
{
    try
    {
        CEGUI::Window* pWindow = m_window.getChild((CEGUI::utf8*)_name.c_str());

        pWindow->destroy();
    }
    catch(CEGUI::UnknownObjectException& _e)
    {
        throw Zen::Utility::runtime_exception(_e.getMessage().c_str());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Widgets::I_WidgetEvent&
Widget::getEvent(const std::string& _name)
{
    eventCache_type::iterator iter = m_eventCache.find(_name);
    if( iter != m_eventCache.end() )
    {
        return *iter->second;
    }

    WidgetEvent* pWidgetEvent = new WidgetEvent(*this, _name);

    m_eventCache[_name] = pWidgetEvent;

    return *pWidgetEvent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Widget::show()
{
    m_window.show();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Widget::hide()
{
    m_window.hide();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Widget::setText(const std::string& _text)
{
    m_window.setText((CEGUI::utf8*)_text.c_str());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Widgets::I_WidgetProperty&
Widget::getProperty(const std::string& _name)
{
    propertyCache_type::iterator iter = m_propertyCache.find(_name);
    if( iter != m_propertyCache.end() )
    {
        return *iter->second;
    }

    CEGUI::String propertyValue =
        m_window.getProperty((CEGUI::utf8*)_name.c_str());

    WidgetProperty* pWidgetProperty = new WidgetProperty(*this,propertyValue.c_str());

    m_propertyCache[_name] = pWidgetProperty;

    return *pWidgetProperty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CEGUI::Window*
Widget::getCEGUIWindow()
{
    return &m_window;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Widget::handleButtonClick(boost::any& _parameter)
{
    CEGUI::EventArgs eventArgs = boost::any_cast<CEGUI::EventArgs>(_parameter);

    const CEGUI::MouseEventArgs& mouseEventArgs =
        *static_cast<const CEGUI::MouseEventArgs*>(&eventArgs);

    Zen::Engine::Widgets::I_WidgetEvent& widgetEvent(getEvent("ButtonClick"));


    WidgetEvent* pWidgetEvent = dynamic_cast<WidgetEvent*>(&widgetEvent);

    pWidgetEvent->handleEvent(eventArgs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
