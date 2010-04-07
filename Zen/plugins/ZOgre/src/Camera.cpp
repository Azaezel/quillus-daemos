//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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

#include "Camera.hpp"
#include "RenderingCanvas.hpp"

#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Vector3.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Camera::Camera(Zen::Scripting::script_module& _module, RenderingCanvas& _canvas, Ogre::Camera& _camera, const std::string& _name)
:   m_canvas(_canvas)
,   m_camera(_camera)
,   m_name(_name)
,   m_pSceneNode(NULL)
,   m_pScriptObject(NULL)
,   m_module(_module)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Camera::~Camera()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Camera::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::setAspectRatio(const Math::Real& _ratio)
{
    m_camera.setAspectRatio(_ratio);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::setHorizontalFOV(const Math::Radian& _fov)
{
    Ogre::Radian fov(_fov);
    m_camera.setFOVy(fov);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::setNearClipDistance(const Math::Real _nearDistance)
{
    m_camera.setNearClipDistance(_nearDistance);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::setFarClipDistance(const Math::Real _farDistance)
{
    m_camera.setFarClipDistance(_farDistance);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::Camera&
Camera::getOgreCamera()
{
    return m_camera;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::setPosition(const Math::Real _x, const Math::Real _y, const Math::Real _z)
{
    if (m_pSceneNode)
    {
        m_pSceneNode->setPosition(_x, _y, _z);
    }
    else
    {
        m_camera.setPosition(_x, _y, _z);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Point3
Camera::getPosition() const
{
    Ogre::Vector3 position;

    if (m_pSceneNode)
    {
        position = m_pSceneNode->getPosition();
    }
    else
    {
        position = m_camera.getPosition();
    }

    return Math::Point3(position.x, position.y, position.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::setDirection(const Math::Real _x, const Math::Real _y, const Math::Real _z)
{
    m_camera.setDirection(_x, _y, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Vector3
Camera::getDirection() const
{
    Ogre::Vector3 direction;

    direction = m_camera.getDirection();

    return Math::Vector3(direction.x, direction.y, direction.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Vector3
Camera::getRight() const
{
    Ogre::Vector3 right;

    right = m_camera.getRight();

    return Math::Vector3(right.ptr());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::lookAt(const Math::Real _x, const Math::Real _y, const Math::Real _z)
{
    m_camera.lookAt(_x, _y, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Matrix4
Camera::getViewMatrix() const
{
    Ogre::Matrix4 viewMatrix;

    viewMatrix = m_camera.getViewMatrix();

    return Math::Matrix4(viewMatrix[0][0],viewMatrix[0][1],viewMatrix[0][2],viewMatrix[0][3],
                         viewMatrix[1][0],viewMatrix[1][1],viewMatrix[1][2],viewMatrix[1][3],
                         viewMatrix[2][0],viewMatrix[2][1],viewMatrix[2][2],viewMatrix[2][3],
                         viewMatrix[3][0],viewMatrix[3][1],viewMatrix[3][2],viewMatrix[3][3]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Matrix4
Camera::getProjectionMatrix() const
{
    Ogre::Matrix4 projMatrix;

    projMatrix = m_camera.getProjectionMatrix();

    return Math::Matrix4(projMatrix[0][0],projMatrix[0][1],projMatrix[0][2],projMatrix[0][3],
                         projMatrix[1][0],projMatrix[1][1],projMatrix[1][2],projMatrix[1][3],
                         projMatrix[2][0],projMatrix[2][1],projMatrix[2][2],projMatrix[2][3],
                         projMatrix[3][0],projMatrix[3][1],projMatrix[3][2],projMatrix[3][3]);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::pitch(const Math::Radian& _pitch, bool _parent, bool _local)
{
    Ogre::Radian pitch(_pitch);

    if (m_pSceneNode)
    {
        if (_parent)
        {
            m_pSceneNode->getParentSceneNode()->pitch(pitch, _local ? Ogre::Node::TS_LOCAL : Ogre::Node::TS_WORLD);
        }
        else
        {
            m_pSceneNode->pitch(pitch, _local ? Ogre::Node::TS_LOCAL : Ogre::Node::TS_WORLD);
        }
    }
    else
    {
        m_camera.pitch(pitch);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::yaw(const Math::Radian& _yaw, bool _parent, bool _local)
{
    Ogre::Radian yaw(_yaw);

    if (m_pSceneNode)
    {
        if (_parent)
        {
            m_pSceneNode->getParentSceneNode()->yaw(yaw, _local ? Ogre::Node::TS_LOCAL : Ogre::Node::TS_WORLD);
        }
        else
        {
            m_pSceneNode->yaw(yaw, _local ? Ogre::Node::TS_LOCAL : Ogre::Node::TS_WORLD);
        }
    }
    else
    {
        m_camera.yaw(yaw);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::setSceneNode(Ogre::SceneNode* _pSceneNode)
{
    m_pSceneNode = _pSceneNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Camera::update()
{
    if(m_pSceneNode)
    {
        m_pSceneNode->getParentSceneNode()->_update(true,false);
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Ray
Camera::getViewportRay(int _x, int _y)
{
    // Convert the values to Real
    Ogre::Real x((Ogre::Real)_x);
    Ogre::Real y((Ogre::Real)_y);

    // Normalise them based on the size of the screen.
    x = x / m_canvas.getWidth();
    y = y / m_canvas.getHeight();

    const Ogre::Ray ray = m_camera.getCameraToViewportRay(x, y);

    return Math::Ray(
        Math::Point3(ray.getOrigin().ptr()),
        Math::Vector3(ray.getDirection().ptr())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
Camera::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_module.getScriptModule(),
            m_module.getScriptModule()->getScriptType(getScriptTypeName()),
            this
        );
    }
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
