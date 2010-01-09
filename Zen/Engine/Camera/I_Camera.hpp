//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_CAMERA_I_CAMERA_HPP_INCLUDED
#define ZEN_ENGINE_CAMERA_I_CAMERA_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Camera {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Basic camera interface
class CAMERA_DLL_LINK I_Camera
{
	/// @name Types
	/// @{
public:
	enum RotateAxis
	{
		X_AXIS, 
		Y_AXIS, 
		Z_AXIS
	};

	typedef Memory::managed_weak_ptr<I_Camera>		wpCamera_type;
	typedef Event::Event<wpCamera_type>				cameraEvent_type;
	/// @}

    /// @name I_Camera interface
    /// @{
public:
    /// Set the origin of the camera
    /// The camera rotates about this point
    virtual void setOrigin(const Math::Matrix4& _position) = 0;

    /// Rotate the camera on a particular axis by a detlta amount
    virtual void rotate(float _theta, RotateAxis _axis) = 0;

    virtual void setRotation(float _degrees, RotateAxis _axis) = 0;

    /// Set the offset of the camera from the origin.
    /// This offset is applied after the rotation and origin is applied
    virtual void setOffset(const Math::Matrix4& _offset) = 0;

    /// Move the camera along a particular axis by the delta amount
    virtual void move(float _theta, RotateAxis _axis) = 0;

    /// Move the camera by the specified vector amount.
    virtual void move(const Math::Vector3& _offset) = 0;

    /// Get the transformation matrix
    /// To apply this to a scene, don't forget to call .inverse()
    virtual const Math::Matrix4& getTransform() = 0;

    /// Get the rotation matrix (no offset, no origin)
    virtual const Math::Matrix4& getRotation() const = 0;

    /// Set the rotation matrix (no offset, no origin)
    virtual void setRotation(const Math::Matrix4& _rotation) = 0;

    /// Update the camera transformations
    virtual void update() = 0;
    /// @}

	/// @name Events
	/// @{
public:
	cameraEvent_type	onDestroyEvent;
	/// @}

    /// @name 'Structors
    /// @{
protected:
             I_Camera();
    virtual ~I_Camera();
    /// @}

};	// interface I_Camera

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Camera
}   // namespace Engine
namespace Memory {
	// I_Camera is managed by I_CameraFactory
	template<>
	struct is_managed_by_factory<Zen::Engine::Camera::I_Camera> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENGINE_CAMERA_I_CAMERA_HPP_INCLUDED
