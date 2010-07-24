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
#ifndef ZEN_ENGINE_RENDERING_I_CAMERA_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_CAMERA_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Radian.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Ray.hpp>

#include <Zen/Engine/Rendering/I_AttachableObject.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Camera;

/// Rendering Canvas Interface
class RENDERING_DLL_LINK I_Camera
:   public I_AttachableObject
{
    /// @name Types
    /// @{
public:
    typedef I_Camera*                               pScriptObject_type;
    typedef Scripting::ObjectReference<I_Camera>    ScriptObjectReference_type;
    typedef ScriptObjectReference_type              ScriptWrapper_type;
    typedef ScriptWrapper_type*                     pScriptWrapper_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_Camera interface
    /// @{
public:
    /// Get the name of this camera.
    virtual const std::string& getName() const = 0;

    /// Set the aspect ratio of this camera
    virtual void setAspectRatio(const Math::Real& _ratio) = 0;

    /// Set the horizontal field of view.
    /// The vertical field of view is automagically calculated based on the 
    /// dimensions of the view.
    virtual void setHorizontalFOV(const Math::Radian& _fov) = 0;

    virtual void setNearClipDistance(const Math::Real _nearDistance) = 0;

    virtual void setFarClipDistance(const Math::Real _farDistance) = 0;

    /// Set the position of this camera.
    virtual void setPosition(const Math::Real _x, const Math::Real _y, const Math::Real _z) = 0;

    /// Get the camera position.
    /// If the camera is attached to a scene node, this position is relative to the parent
    /// scene node.
    virtual Math::Point3 getPosition() const = 0;

    /// Set the orientation of this camera using a quaternion.
    virtual void setOrientation(const Math::Real _w, const Math::Real _x, const Math::Real _y, const Math::Real _z) = 0;

    /// Get the orientation of this camera.
    virtual Math::Quaternion4 getOrientation() const = 0;

    /// Set the direction of this camera (in the World coordinate frame).
    /// It is important to note that this direction is not related to any attached scene node.
    virtual void setDirection(const Math::Real _x, const Math::Real _y, const Math::Real _z) = 0;

    /// Get the direction of this camera (in the World coordinate frame).
    /// It is important to note that this direction is not related to any attached scene node.
    virtual Math::Vector3 getDirection() const = 0;

	/// Get the direction to the right and perpendicular to the facing direction of the camera 
	/// (in the World coordinate frame). It is important to note that this direction is not 
	/// related to any attached scene node.
	virtual Math::Vector3 getRight() const = 0;

    /// Points this camera at a position (in the World coordinate frame).
    /// It is important to note that the resulting direction is not related to any attached scene node.
    virtual void lookAt(const Math::Real _x, const Math::Real _y, const Math::Real _z) = 0;

    /// Get the view matrix of this camera (World to Eye coordinate frame transformation).
    virtual Math::Matrix4 getViewMatrix() const = 0;

    /// Get the projection matrix of this camera (Eye to Frustum frame transformation).
    virtual Math::Matrix4 getProjectionMatrix() const = 0;

    /// Set the pitch of this camera.
    virtual void pitch(const Math::Radian& _pitch, bool _parent = false, bool _local = false) = 0;

    /// Set the yaw of this camera.
    virtual void yaw(const Math::Radian& _yaw, bool _parent = false, bool _local = false) = 0;

    /// Update the camera transformations
    virtual void update() = 0;

    /// Get the viewport ray.
    /// @see http://www.indiezen.org/wiki/irclogs/2009/08/12/#UTC2009-08-12T17:28:49
	virtual Math::Ray getViewportRay(int _x, int _y) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class I_Canvas;
             I_Camera();
    virtual ~I_Camera();
    /// @}

};	// interface I_Camera

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_CAMERA_HPP_INCLUDED
