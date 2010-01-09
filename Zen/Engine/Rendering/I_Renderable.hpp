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
#ifndef ZEN_ENGINE_RENDERING_I_RENDERABLE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_RENDERABLE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Engine/Rendering/I_AttachableObject.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RENDERING_DLL_LINK I_Renderable
: public    I_AttachableObject
{
    /// @name I_Renderable interface
    /// @{
public:
    /// Set the material name
    virtual void setMaterialName(const std::string& _name) = 0;

    /// Indicate if the normals should be normalized.
    /// This is generally set to true when the mesh is scaled
#if 0       // deprecated
    virtual void setNormaliseNormals(bool _normalise) = 0;

    virtual void updateAllAnimations(double _elapsedTime) = 0;

    /// Enable or disable an animation
    virtual void enableAnimation(const std::string& _animationName, bool _enable) = 0;
#endif      // deprecated

    /// Set the visibility of this renderable
    virtual void setVisible(bool _visible) = 0;

    /// Get the visibility of this renderable
    virtual bool getVisible() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Renderable();
    virtual ~I_Renderable();
    /// @}

};  // interface I_Renderable

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_CANVAS_HPP_INCLUDED
