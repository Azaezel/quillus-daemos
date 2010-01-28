//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C)		2009 Denis Hilliard
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
#ifndef ZEN_ENGINE_WIDGETS_I_WIDGET_HPP_INCLUDED
#define ZEN_ENGINE_WIDGETS_I_WIDGET_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/any.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Widgets {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_WidgetProperty;
class I_WidgetEvent;
	
/// Basic GUI widget.
class WIDGETS_DLL_LINK I_Widget
{
    /// @name Forward declarations
    /// @{
public:
    struct I_ChildVisitor;
    /// @}

	/// @name Types
    /// @{
public:
    typedef std::string                          	index_type;
    /// These are raw pointers because the I_WidgetService
    /// manages the lifetime and nothing outside of I_WidgetService
    /// should retain a reference to a I_Widget.
    typedef I_Widget*        	                    pWidget_type;
    typedef I_Widget*                               wpWidget_type;
 
    /// @}
    /// @name I_Widget interface
    /// @{
public:
	/// Query the widget for its name
	virtual const std::string& getName() = 0;

	/// Query the widget for its type
	virtual const std::string& getType() = 0;

	/// Query the widget for its style
	virtual const std::string& getStyle() = 0;

	/// Get the parent for this widget
    /// @return NULL if this widget does not have a parent.
	virtual pWidget_type getParent() = 0;

	/// Find the child widget by given name
    /// @return NULL if the child isn't found.
	virtual pWidget_type getChild(const std::string & _name) = 0;

    /// Get the child widgets via a visitor
    virtual void getChildren(I_ChildVisitor& _visitor) = 0;

	/// Get the widget as a weak reference
	virtual wpWidget_type getAsWeakReference() = 0;
	
	virtual void addChild(pWidget_type _pWidget) = 0;

	virtual void destroyChild(const std::string & _name) = 0;

    /// Get a widget event for this widget by name.
	virtual I_WidgetEvent& getEvent(const std::string & _name) = 0;

    /// Handle button click
    /// TODO Not sure if this belongs in the extension point interface.
    virtual void handleButtonClick(boost::any& _parameter) = 0;

    /// Show a widget
    virtual void show() = 0;

    /// Hide a widget
    virtual void hide() = 0;

    virtual void setText(const std::string& _text) = 0;
    /// @}

    /// @name Inner classes
    /// @{
public:
    struct I_ChildVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_Widget& _child) = 0;
        virtual void end() = 0;
    };  // interface I_ChildVisitor
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Widget();
    virtual ~I_Widget();
    /// @}
	

	/// @name Properties
	/// @{
public:
	virtual I_WidgetProperty& getProperty(const std::string & _name) = 0;
	/// @}
	
};	// interface I_Widget



//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Widgets
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENGINE_WIDGETS_I_WIDGET_HPP_INCLUDED
