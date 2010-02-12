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
#include "CameraService.hpp"
#include "OrbitCamera.hpp"

#include <Zen/Core/Math/Vector3.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCam {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

OrbitCamera::OrbitCamera(wpCameraService_type _wpService)
:   m_pCameraService(_wpService)
,	m_origin(Math::Matrix4::INIT_IDENTITY)
,	m_rotate(Math::Matrix4::INIT_IDENTITY)
,	m_offset(Math::Matrix4::INIT_IDENTITY)
,	m_transform(Math::Matrix4::INIT_IDENTITY)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
OrbitCamera::~OrbitCamera()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OrbitCamera::setOrigin(const Math::Matrix4& _position)
{
    m_origin = _position;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OrbitCamera::rotate(float _theta, RotateAxis _axis)
{
    Math::Degree d(_theta);
    Math::Radian r(d);

    Math::Matrix4 rotate;

	switch(_axis)
	{
	case X_AXIS:
        rotate.setXRotation(r);
		break;
	case Y_AXIS:
		rotate.setYRotation(r);
		break;
	case Z_AXIS:
		rotate.setZRotation(r);
		break;
    default:
        rotate.identity();
	}

    m_rotate *= rotate;
}

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OrbitCamera::setRotation(float _degrees, RotateAxis _axis)
{
    Math::Degree d(_degrees);
    Math::Radian r(d);

	switch(_axis)
	{
	case X_AXIS:
        //m_rotateX = r;
        break;
	case Y_AXIS:
        //m_rotateY = r;
		break;
	case Z_AXIS:
        //m_rotateZ = r;
		break;
	}

    // TODO Finish
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OrbitCamera::setOffset(const Math::Matrix4& _offset)
{
	m_offset = _offset;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Matrix4&
OrbitCamera::getTransform()
{
    m_transform.identity();

    m_transform *= m_origin; // Center of rotation
    m_transform *= m_rotate; // rotation
    m_transform *= m_offset; // world offset

	return m_transform;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Matrix4&
OrbitCamera::getRotation() const
{
    return m_rotate;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OrbitCamera::setRotation(const Math::Matrix4& _rotation)
{
    m_rotate = _rotation;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OrbitCamera::move(float _theta, Engine::Camera::I_Camera::RotateAxis _axis)
{
    Math::Vector3 position;
    m_offset.getPosition(position);
    switch(_axis)
    {
	case X_AXIS:
        position.m_x += _theta;
        break;
	case Y_AXIS:
        position.m_y += _theta;
		break;
	case Z_AXIS:
        position.m_z += _theta;
		break;
    }
    m_offset.setPosition(position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
OrbitCamera::move(const Math::Vector3& _offset)
{
    m_offset += _offset;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCam
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
