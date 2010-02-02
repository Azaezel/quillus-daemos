//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_ZOGRE_NULL_CAMERA_HPP_INCLUDED
#define ZEN_ZOGRE_NULL_CAMERA_HPP_INCLUDED

#if 0 // deprecated 

#include <Zen/Engine/Camera/I_Camera.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Default NULL camera for manually manipulating the camera
class NullCamera
:   public Engine::Camera::I_Camera
{
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
    virtual void update();
    /// @}

    /// @name 'Structors
    /// @{
public:
             NullCamera(Ogre::Camera* _pCamera);
    virtual ~NullCamera();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogre::Camera*       m_pCamera;

    Math::Matrix4       m_rotation;
    Math::Matrix4       m_transform;
    /// @}

};  // class NullCamera

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // deprecated 

#endif // ZEN_ZOGRE_NULL_CAMERA_HPP_INCLUDED
