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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_RENDERING_I_RENDERING_CANVAS_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_RENDERING_CANVAS_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Engine/Rendering/I_Canvas.hpp>

#include <boost/filesystem/path.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Camera;
class I_SceneNode;

/// Rendering Canvas Interface
/// @todo Need to add I_RenderingCanvas::setBackgroundColour(color) but 
///     there isn't a I_Color or Color interface or class.
class RENDERING_DLL_LINK I_RenderingCanvas
:   public I_Canvas
{
    /// @name Types
    /// @{
public:
    typedef Zen::Event::Event<bool>             redraw_event;

    struct I_SceneNodeVisitor;                  //< Defined below
    /// @}

    /// @name I_RenderingCanvas interface
    /// @{
public:
    /// Resize the canvas
    virtual void resize(int _x, int _y, int _width, int _height) = 0;

    /// Called by the GUI manager to render the current scene.  
    /// Double buffered canvases will render the scene to the back buffer
    /// and the GUI manager will swap the buffers as necessary.
    virtual void renderScene() = 0;

    /// Assuming at least two buffers, swap the visible buffer with the back buffer.
    virtual void swapBuffers() = 0;

    /// Assuming at least two buffers, entirely clear the back buffer.
    virtual void clearBackBuffer() = 0;

    /// Set the input device
    virtual void setInputDevice(const std::string& _deviceName) = 0;

    /// Called by the GUI manager when this canvas loses focus
    virtual void focusLost() = 0;

    /// Called by the GUI manager when this canvas regains focus
    virtual void focusGained() = 0;

    /// Get the width of this canvas
    virtual unsigned getWidth() const = 0;

    /// Get the height of this canvas
    virtual unsigned getHeight() const = 0;

    /// Create a new camera
    ///
    /// Don't keep a reference to this camera as it might change.
    virtual I_Camera& createCamera(const std::string& _name) = 0;

    /// Get the current camera.
    ///
    /// Don't keep a reference to this camera as it might change.
    virtual I_Camera& getCurrentCamera() = 0;

    /// Select a camera by name.
    ///
    /// Selecting a camera makes it the camera that is used to render
    /// the scene.
    ///
    /// @param _name The name of the camera.  This must be the name of
    ///     a camera created using createCamera()
    virtual I_Camera& selectCamera(const std::string& _name) = 0;

    /// Query for the scene nodes.
    ///
    /// Creates a ray from the camera through the _x and _y coordinates on the canvas and projects 
    /// the ray to find all scene nodes that intersect with the ray.
    ///
    /// @param _x, _y The x and y coordinates where the ray should intersect the canvas.
    /// @param _visitor The visitor object that will be called for every scene node that intersects
    ///         the given ray.
    virtual void querySceneNodes(Math::Real _x, Math::Real _y, I_SceneNodeVisitor& _visitor) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// This is fired by the rendering canvas when a redraw is required
    /// but redraws always need to occur in the same thread.  It's the
    /// implementer's responsibility for handling this event appropriately.
    redraw_event onNeedRedraw;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_RenderingCanvas();
    virtual ~I_RenderingCanvas();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    /// Scene Node Visitor
    struct I_SceneNodeVisitor
    {
        typedef Memory::managed_ptr<I_SceneNode>    pSceneNode_type;

        virtual void begin() = 0;
        virtual void visit(pSceneNode_type _pSceneNode) = 0;
        virtual void end() = 0;
    };  // struct I_SceneNodeVisitor

};	// interface I_RenderingCanvas

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_RENDERING_CANVAS_HPP_INCLUDED
