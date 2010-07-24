//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ENGINE_RENDERING_I_VIEW_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Event/I_Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
	namespace Camera {
		class I_Camera;
	}
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Canvas;
class I_SceneService;

/// Outer OS Window that contains a canvas.
/// @todo Implement the getViewClosingEvent.  Need to implement a thread-safe
/// 	asynchronous mechanism for allowing the application to prevent
///		the view from closing.
class RENDERING_DLL_LINK I_View
:   public Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_View*                                 pScriptObject_type;
    typedef Scripting::ObjectReference<I_View>      ScriptObjectReference_type;
    typedef ScriptObjectReference_type              ScriptWrapper_type;
    typedef ScriptWrapper_type*                     pScriptWrapper_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_View interface
    /// @{
public:
	/// Set the camera
    /// @todo Remove.  The Canvas should have the camera.
	virtual void setCamera(Camera::I_Camera* _pCamera) = 0;

    /// Create a sub-view
    /// If the render engine does not support sub-views then NULL is returned
    virtual I_View* createSubView(int _x, int _y, int _width, int _height) = 0;

    /// Get the canvas associated with this view.
    virtual I_Canvas& getCanvas() = 0;

    /// Initialize / create the canvas.
    virtual bool initCanvas() = 0;

    /// Get the viewMoved event.
    /// This event is fired when this view moves.
    /// The payload is the I_View*.
    virtual Event::I_Event& getViewMovedEvent() = 0;

    /// Get the view resized event.
    /// This event is fired when the view is resized.
    /// The payload is the I_View*.
    virtual Event::I_Event& getViewResizedEvent() = 0;

    /// Get the view closed event.
    /// This event is fired when the view has closed.
    /// The payload is the I_View*.
    virtual Event::I_Event& getViewClosedEvent() = 0;

    /// Get the view focus changed event.
    /// This event is fired when the view is gaining or
    /// losing focus.
    /// The payload is the I_View*.
    virtual Event::I_Event& getViewFocusChangedEvent() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_View();
    virtual ~I_View();
    /// @}

};	// interface I_View

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_VIEW_HPP_INCLUDED
