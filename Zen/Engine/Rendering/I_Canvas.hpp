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
#ifndef ZEN_ENGINE_RENDERING_I_CANVAS_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_CANVAS_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Camera {
        class I_Camera;
    }
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_SceneGraph;

/// Base Canvas Interface
class RENDERING_DLL_LINK I_Canvas
:   public Zen::Scripting::I_ScriptableType
,   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef I_Canvas*                               pScriptObject_type;
    typedef Scripting::ObjectReference<I_Canvas>    ScriptObjectReference_type;
    typedef ScriptObjectReference_type              ScriptWrapper_type;
    typedef ScriptWrapper_type*                     pScriptWrapper_type;

    typedef Memory::managed_ptr<Engine::Camera::I_Camera>  pCamera_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_Canvas interface
    /// @{
public:
    /// Set the current scene graph.
    /// @note is this deprecated?
    virtual void setSceneGraph(const I_SceneGraph& _sceneGraph) = 0;
    /// @}

    /// @name I_Canvas deprecated interface
    /// @{
private:
    /// Set the current camera
    /// @deprecated Use I_RenderingCanvas::getCurrentCamera() instead
    virtual void setCamera();

    /// Set the current camera type.  The canvas is responsible
    /// for loading / caching the correct camera extension.
    /// @deprecated Use I_RenderingCanvas::getCurrentCamera() instead
    virtual void setCameraType(const std::string& _cameraType);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Canvas();
    virtual ~I_Canvas();
    /// @}

};  // interface I_Canvas

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_CANVAS_HPP_INCLUDED
