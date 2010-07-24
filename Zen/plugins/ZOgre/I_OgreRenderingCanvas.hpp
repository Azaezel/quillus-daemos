//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_ZOGRE_I_OGRE_RENDERING_CANVAS_HPP_INCLUDED
#define ZEN_ZOGRE_I_OGRE_RENDERING_CANVAS_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>

namespace Ogre {
    class Camera;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class RenderingView;

/// A I_OgreRenderingCanvas is the same as an Ogre::ViewPort
/// Use this to get access to the raw Ogre viewport and sub objects
class ZOGRE_DLL_LINK I_OgreRenderingCanvas
:   public Engine::Rendering::I_RenderingCanvas
{
    /// @name I_OgreRenderingCanvas I_OgreRenderingCanvas
    /// @{
public:
    /// Get the current Ogre camera
    /// @note DO NOT keep a copy of this.  If you need to use it to
    ///     manually control it, get it every frame.  You cannot control
    ///     it manually if you use I_Canvas::setCamera() to set the camera.
    //virtual Ogre::Camera& getCurrentCamera() = 0;

    /// Initialize a compositor to be used with the associated viewport
    virtual void initializeCompositor(const std::string& _name) = 0;

    /// Enable a compositor for the associated viewport
    virtual void enableCompositor(const std::string& _name) = 0;

    /// Disable a compositor for the associated viewport
    virtual void disableCompositor(const std::string& _name) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_OgreRenderingCanvas();
    virtual ~I_OgreRenderingCanvas();
    /// @}

};  // class RenderingCanvas

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_I_OGRE_RENDERING_CANVAS_HPP_INCLUDED
