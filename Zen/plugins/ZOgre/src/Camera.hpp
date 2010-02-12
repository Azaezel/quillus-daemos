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
#ifndef ZEN_ZOGRE_CAMERA_HPP_INCLUDED
#define ZEN_ZOGRE_CAMERA_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_Camera.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class RenderingCanvas;

/// Thin wrapper for an Ogre camera
class Camera
:   public Engine::Rendering::I_Camera
{
    /// @name I_Camera implementation
    /// @{
public:
    virtual const std::string& getName() const;
    virtual void setAspectRatio(const Math::Real& _ratio);
    virtual void setHorizontalFOV(const Math::Radian& _fov);
    virtual void setNearClipDistance(const Math::Real _nearDistance);
    virtual void setFarClipDistance(const Math::Real _farDistance);
    virtual void setPosition(const Math::Real _x, const Math::Real _y, const Math::Real _z);
    virtual Math::Point3 getPosition() const;
    virtual void setDirection(const Math::Real _x, const Math::Real _y, const Math::Real _z);
    virtual Math::Vector3 getDirection() const;
    virtual Math::Vector3 getRight() const;
    virtual void lookAt(const Math::Real _x, const Math::Real _y, const Math::Real _z);
    virtual Math::Matrix4 getViewMatrix() const;
    virtual Math::Matrix4 getProjectionMatrix() const;
    virtual void pitch(const Math::Radian& _pitch, bool _parent, bool _local);
    virtual void yaw(const Math::Radian& _yaw, bool _parent, bool _local);
    virtual void update();
    /// @see http://www.indiezen.org/wiki/irclogs/2009/08/12/#UTC2009-08-12T17:28:49
    virtual Math::Ray getViewportRay(int _x, int _y);
    /// @}

    /// @name Camera implementation
    /// @{
public:
    Ogre::Camera& getOgreCamera();

    /// Set the scene node.  This node MUST have a parent node.
    void setSceneNode(Ogre::SceneNode* _pParentNode);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Camera(RenderingCanvas& _canvas, Ogre::Camera& _camera, const std::string& _name);
    virtual ~Camera();
    /// @}

    /// @name Member Variables
    /// @{
private:
    RenderingCanvas&        m_canvas;
    Ogre::Camera&           m_camera;
    std::string             m_name;
    Ogre::SceneNode*        m_pSceneNode;
    /// @}

};  // class Camera

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_CAMERA_HPP_INCLUDED
