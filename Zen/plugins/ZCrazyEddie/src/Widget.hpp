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
#ifndef ZEN_ZCRAZYEDDIE_WIDGET_HPP_INCLUDED
#define ZEN_ZCRAZYEDDIE_WIDGET_HPP_INCLUDED

#include <Zen/Engine/Widgets/I_Widget.hpp>

#include <CEGUI/CEGUI.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class WidgetService;

class Widget
:   public Zen::Engine::Widgets::I_Widget
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Widget implementation
    /// @{
public:
    virtual const std::string& getName();
    virtual const std::string& getType();
    virtual const std::string& getStyle();
    virtual pWidget_type getParent();
    virtual pWidget_type getChild(const std::string& _name);
    virtual void getChildren(Zen::Engine::Widgets::I_Widget::I_ChildVisitor& _visitor);
    virtual wpWidget_type getAsWeakReference();
    virtual void addChild(pWidget_type _pWidget);
    virtual void destroyChild(const std::string& _name);
    virtual Zen::Engine::Widgets::I_WidgetEvent& getEvent(const std::string& _name);
    virtual void show();
    virtual void hide();
    virtual void setText(const std::string& _text);
    virtual Zen::Engine::Widgets::I_WidgetProperty& getProperty(const std::string& _name);
    virtual void handleButtonClick(boost::any& _parameter);
    /// @}

    /// @name Widget implementation
    /// @{
public:
    CEGUI::Window* getCEGUIWindow();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class WidgetService;
             Widget(WidgetService& _service, CEGUI::Window& _window);
    virtual ~Widget();
    /// @}

    /// @name Member Variables
    /// @{
private:
    WidgetService&                  m_service;
    CEGUI::Window&                  m_window;

    std::string                     m_name;
    std::string                     m_type;
    std::string                     m_style;

    typedef std::map<std::string, Zen::Engine::Widgets::I_WidgetEvent*>     eventCache_type;
    eventCache_type                 m_eventCache;

    typedef std::map<std::string, Zen::Engine::Widgets::I_WidgetProperty*>  propertyCache_type;
    propertyCache_type              m_propertyCache;
    /// @}

};  // class Widget

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZCRAZYEDDIE_WIDGET_HPP_INCLUDED
