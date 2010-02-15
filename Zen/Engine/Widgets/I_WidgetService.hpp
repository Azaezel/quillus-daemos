//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ENGINE_WIDGETS_I_WIDGET_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_WIDGETS_I_WIDGET_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Rendering/I_View.hpp>

#include <Zen/Engine/Input/I_InputService.hpp>
#include <Zen/Engine/Input/I_KeyPublisher.hpp>
#include <Zen/Engine/Input/I_MousePublisher.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Widgets {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_FontService;
class I_StyleService;
class I_Widget;
class I_WidgetServiceFactory;

class WIDGETS_DLL_LINK I_WidgetService
:   public virtual Zen::Scripting::I_ScriptableType
,   public Input::I_KeyPublisher
,   public Input::I_MousePublisher
{
    /// @name Types
    /// @{
public:
    typedef std::string                                 index_type;
    typedef Memory::managed_ptr<I_WidgetService>        pService_type;
    typedef Memory::managed_weak_ptr<I_WidgetService>   wpService_type;
    typedef I_Widget*                                   pWidget_type;
    typedef I_Widget*                                   wpWidget_type;
    typedef Math::Real                                  delta_type;
	typedef char                                        key_type;
    typedef Math::Real                                  MouseMotionDelta_type;
	typedef char										MouseButton_type;

    typedef I_WidgetServiceFactory                                      factory_type;

    typedef Zen::Memory::managed_ptr<I_WidgetService>                   pScriptObject_type;
    typedef Scripting::ObjectReference<I_WidgetService>                 ScriptObjectReference_type;

    typedef Zen::Event::Event<wpService_type>                           ServiceEvent_type;

    typedef Zen::Memory::managed_ptr<Zen::Engine::Input::I_KeyEvent>    pKeyEventPayload_type;
    typedef Zen::Event::Event<pKeyEventPayload_type>                    key_event;

    typedef Zen::Event::Event<Zen::Engine::Input::I_MouseMoveEvent&>    MouseMoveEvent_type;
    typedef Zen::Event::Event<Zen::Engine::Input::I_MouseClickEvent&>   MouseClickEvent_type;
    /// @}

    /// @name I_WidgetService interface
    /// @{
public:
	/// Initialise and attach the service to a view and an input service.
    virtual void initialise(Rendering::I_View & _view, Input::I_InputService& _input) = 0;

    // These methods are deprecated.  I_View (or I_RenderingCanvas?) or something along
    // those lines need to indicate when it's time for the I_WidgetService to
    // render and update by firing some sort of generic event.  The widget service
    // implementation should subscribe to those events and handle them appropriately.
#if 0 // deprecated
	/// Draw a frame
	virtual void drawFrame() = 0;
	/// Update the widget service
	virtual void update(delta_type _delta) = 0;
#endif // deprecated

    /// Get a font service
    virtual I_FontService& getFontService() = 0;
	/// Find a widget within the scene graph and return it as a reference
	/// Absolute paths mean relative to true root node or to current visible root
	virtual I_Widget& getWidget(const std::string& _path, bool _absolute = false) = 0;
	/// Switch root widget node
	virtual void switchRoot(const std::string& _root) = 0;
	/// Attach screen size widget to the service (needs unique name attribute)
	virtual void attachRoot(wpWidget_type _pWidget) = 0;
	/// Destroy a Root Widget
	virtual void destroyRoot(const std::string& _root) = 0;
    /// Set the default tooltip
    virtual void setDefaultTooltip(const std::string& _tooltip) = 0;
    /// Set the default font
    virtual void setDefaultFont(const std::string& _font) = 0;

    // These methods are deprecated.  See the events below for the
    // reason.
#if 0	// deprecated
	/// Inject a key press into the WidgetService
	virtual void onKeyPress(key_type _key) = 0;
	/// Inject a key release into the WidgetService
	virtual void onKeyRelease(key_type _key) = 0;
	/// Inject mouse motion into the WidgetService
	virtual void onMouseMotion(MouseMotionDelta_type _x, MouseMotionDelta_type _y) = 0;
	/// Inject mouse click into the WidgetService
	virtual void onMouseClick(MouseButton_type _button) = 0;
	/// Inject mouse release into the WidgetService
	virtual	void onMouseRelease(MouseButton_type _button) = 0;
#endif // deprecated

    /// Show the operating system mouse cursor.
    /// @note The logic for this and hideMouseCursor() needs to be improved.
    ///         Likely need to distinguish between hiding and showing the game's
    ///         mouse cursor vs hiding and showing the OS mouse cursor.
    virtual void showMouseCursor() = 0;

    /// Hide the operating system mouse cursor.
    virtual void hideMouseCursor() = 0;

    /// Set the mouse to a 'gone' state.
    /// @note What does "gone" mean?
    virtual void mouseGone(bool _isGone) = 0;

    /// Get mouse position
    virtual void getMousePosition(int& _x, int& _y) = 0;

    /// Set the enabled state of the widget service.
    virtual void setEnabled(bool _isEnabled) = 0;
	/// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "WidgetService"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    ServiceEvent_type       onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WidgetService();
    virtual ~I_WidgetService();
    /// @}

};	// interface I_WidgetService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Widgets
}   // namespace Engine
namespace Memory {
    /// I_WidgetService is managed by I_WidgetServiceFactory
    template<>
    struct is_managed_by_factory<Engine::Widgets::I_WidgetService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WIDGETS_I_WIDGET_SERVICE_HPP_INCLUDED
