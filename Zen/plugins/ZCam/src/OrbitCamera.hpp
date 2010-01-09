//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
// Copyright (C)        2008 Walt Collins
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
#ifndef ZEN_ZCAM_ORBIT_CAMERA_HPP_INCLUDED
#define ZEN_ZCAM_ORBIT_CAMERA_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Camera/I_Camera.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCam {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class CameraService;
/// Basic Orbit Camera
///
/// Although this is a plugin, feel free to use it as a library and
/// extend this class as you see fit.
class OrbitCamera
:   public Engine::Camera::I_Camera
,	public Memory::managed_self_ref<Engine::Camera::I_Camera>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Engine::Camera::I_CameraService>    wpCameraService_type;
    typedef Memory::managed_weak_ptr<Engine::Camera::I_Camera>           wpCamera_type;
    /// @}

    /// @name I_Camera implementation
    /// @{
public:
    virtual void setOrigin(const Math::Matrix4& _position);
	virtual void rotate(float _theta, RotateAxis _axis);
    virtual void setRotation(float _degrees, RotateAxis _axis);
    virtual void setOffset(const Math::Matrix4& _offset);
    virtual void move(float _theta, RotateAxis _axis);
    virtual void move(const Math::Vector3& _offset);
    virtual const Math::Matrix4& getTransform();
    virtual const Math::Matrix4& getRotation() const;
    virtual void setRotation(const Math::Matrix4& _rotation);
    /// @}

    /// @name 'Structors
    /// @{
public:
             OrbitCamera(wpCameraService_type _wpService);
    virtual ~OrbitCamera();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wpCameraService_type  m_pCameraService;
    Math::Matrix4	      m_origin;
    Math::Matrix4         m_rotate;
    Math::Matrix4		  m_offset;
    Math::Matrix4		  m_transform;
    /// @}

};  // class OrbitCamera

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCam
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZCAM_ORBIT_CAMERA_HPP_INCLUDED

