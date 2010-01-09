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

#include "RenderingCanvas.hpp"
#include "RenderingView.hpp"
#include "Camera.hpp"
#include "SceneNode.hpp"

#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <stddef.h>
#include <iostream>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingCanvas::RenderingCanvas(RenderingView* _pView)
:   m_root(Ogre::Root::getSingleton())
,   m_pRenderingView(_pView)
,   m_pCurrentCamera(NULL)
,   m_isCompositorManagerInitialized(false)
,   m_compositors()
{
    // Get the default scene manager
    m_pSceneManager = Ogre::Root::getSingleton().getSceneManager("default");

    // Create the default camera
    createCamera("default");

    m_pViewPort = _pView->getRenderWindow().addViewport(&m_pCurrentCamera->getOgreCamera());

    m_pViewPort->setBackgroundColour(Ogre::ColourValue(0, 0, 0));


    m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.75f, 0.75f, 0.75f));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingCanvas::~RenderingCanvas()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void 
RenderingCanvas::setCamera(pCamera_type _camera)
{
    m_pCameraController = _camera;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void 
RenderingCanvas::setCameraType(const std::string& _cameraType)
{
    m_strCameraType = _cameraType;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::setSceneGraph(const Zen::Engine::Rendering::I_SceneGraph& _sceneGraph)
{
    throw Utility::runtime_exception("RenderingCanvas::setSceneGraph(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::resize(int _x, int _y, int _width, int _height)
{
    m_pRenderingView->getRenderWindow().reposition(_x, _y);
    m_pRenderingView->getRenderWindow().resize(_width, _height);

    m_pRenderingView->getRenderWindow().windowMovedOrResized();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::renderScene()
{
#if 0   // deprecated
    // If the camera controller is valid, copy the values from there to the
    // Ogre camera, otherwise just use the Ogre camera
    if (m_pCameraController.isValid())
    {
        // copy ZCam matrix transform every frame here
        Zen::Math::Matrix4 xfm = m_pCameraController->getTransform();
        static Zen::Math::Vector3 pos;
        xfm.getPosition(pos);
        m_pCamera->setPosition(pos.m_x, pos.m_y, pos.m_z);

        xfm = m_pCameraController->getRotation();
        const Zen::Math::Quaternion4 q(xfm);
        m_pCamera->setOrientation(Ogre::Quaternion(q.m_w, q.m_x, q.m_y, q.m_z));
    }
#endif

    if( !m_isCompositorManagerInitialized )
    {
        m_isCompositorManagerInitialized = true;

        Ogre::CompositorManager& manager(Ogre::CompositorManager::getSingleton());

        std::map< std::string, bool >::iterator iter = m_compositors.begin();
        while( iter != m_compositors.end() )
        {
            manager.addCompositor(m_pViewPort, iter->first, -1);
            manager.setCompositorEnabled(m_pViewPort, iter->first, iter->second);
            iter++;
        }
    }

    m_root.renderOneFrame();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::swapBuffers()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::clearBackBuffer()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::setInputDevice(const std::string& _deviceName)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::focusLost()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
RenderingCanvas::focusGained()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
unsigned 
RenderingCanvas::getWidth() const
{
    return m_pRenderingView->getRenderWindow().getWidth();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
unsigned 
RenderingCanvas::getHeight() const
{
    return m_pRenderingView->getRenderWindow().getHeight();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_Camera&
RenderingCanvas::createCamera(const std::string& _name)
{
    // TODO Guard, or possibly limit this call to only be used by the rendering thread

    Cameras_type::iterator iter = m_cameras.find(_name);

    if (iter == m_cameras.end())
    {
        // Camera wasn't found, so create it
        Camera* pCamera = new Camera(*this, *m_pSceneManager->createCamera(_name), _name);

        // Default the aspect ratio
        pCamera->setAspectRatio((Ogre::Real)getWidth() / (Ogre::Real)getHeight());

        // If the current camera hasn't been set, set it
        if (m_pCurrentCamera == NULL)
        {
            m_pCurrentCamera = pCamera;
        }

        m_cameras[_name] = pCamera;

        return *pCamera;
    }
    else
    {
        return *iter->second;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_Camera&
RenderingCanvas::getCurrentCamera()
{
    if (m_pCurrentCamera)
    {
        return *m_pCurrentCamera;
    }
    else
    {
        throw Utility::runtime_exception("getCurrentCamera(): Error, createCamera() must be called first!");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_Camera&
RenderingCanvas::selectCamera(const std::string& _name)
{
    // TODO Guard, or possibly limit this call to only be used by the rendering thread

    Cameras_type::iterator iter = m_cameras.find(_name);

    // Find the camera
    if (iter == m_cameras.end())
    {
        // Oops, it wasn't found... throw an error.

        std::stringstream errorMessage;
        errorMessage << "selectCamera(): Error, camera not found: " << _name;
        throw Utility::runtime_exception(errorMessage.str());
    }
    else
    {
        // Set the current camera for this viewport
        m_pViewPort->setCamera(&iter->second->getOgreCamera());

        m_pCurrentCamera = iter->second;

        return *iter->second;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingCanvas::querySceneNodes(Math::Real _x, Math::Real _y, I_SceneNodeVisitor& _visitor)
{
    Ogre::Camera& camera = m_pCurrentCamera->getOgreCamera();
    Ogre::Vector3 position;
    if( camera.getParentSceneNode() )
    {
        position = camera.getParentSceneNode()->_getDerivedPosition();
    }
    else
    {
        position = camera.getDerivedPosition();
    }

    Ogre::RaySceneQuery* pSceneQuery = 
        m_pSceneManager->createRayQuery(camera.getCameraToViewportRay(_x, _y));
    pSceneQuery->setSortByDistance(true);
    
    Ogre::RaySceneQueryResult::const_iterator iter;
    const Ogre::RaySceneQueryResult& queryResult = pSceneQuery->execute();

    _visitor.begin();
    for(iter = queryResult.begin(); iter != queryResult.end(); iter++)
    {
        const Ogre::RaySceneQueryResultEntry& result = *iter;

        // Check to see if this is a scene node.
        if (result.movable != NULL)
        {
            Ogre::SceneNode *pNode = dynamic_cast<Ogre::SceneNode*>(result.movable->getParentSceneNode());
            if (pNode)
            {
                if (!pNode->getUserAny().isEmpty())
                {
                    SceneNode* pRawSceneNode = Ogre::any_cast<SceneNode*>(pNode->getUserAny());

                    I_SceneNodeVisitor::pSceneNode_type pSceneNode(pRawSceneNode->getSelfReference().lock());

                    _visitor.visit(pSceneNode);
                }
            }
        }
    }
    _visitor.end();

    delete pSceneQuery;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::SceneManager*
RenderingCanvas::getOgreSceneManager() const
{
    return m_pSceneManager;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::Viewport*
RenderingCanvas::getOgreViewport() const
{
    return m_pViewPort;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingCanvas::initializeCompositor(const std::string& _name)
{
    std::map< std::string, bool >::iterator iter = m_compositors.find(_name);
    if( iter == m_compositors.end() )
    {
        m_compositors[_name] = false;

        if( m_isCompositorManagerInitialized )
        {
            Ogre::CompositorManager& manager(Ogre::CompositorManager::getSingleton());

            manager.addCompositor(m_pViewPort, _name, -1);
        }
    }
    else
    {
        throw Zen::Utility::runtime_exception("RenderingCanvas::initializeCompositor() : Compositor already initialized.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingCanvas::enableCompositor(const std::string& _name)
{
    std::map< std::string, bool >::iterator iter = m_compositors.find(_name);
    if( iter != m_compositors.end() )
    {
        iter->second = true;

        if( m_isCompositorManagerInitialized )
        {
            Ogre::CompositorManager& manager(Ogre::CompositorManager::getSingleton());

            manager.setCompositorEnabled(m_pViewPort, _name, true);
        }
    }
    else
    {
        throw Zen::Utility::runtime_exception("RenderingCanvas::enableCompositor() : Uninitialized compositor.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingCanvas::disableCompositor(const std::string& _name)
{
    std::map< std::string, bool >::iterator iter = m_compositors.find(_name);
    if( iter != m_compositors.end() )
    {
        iter->second = false;

        if( m_isCompositorManagerInitialized )
        {
            Ogre::CompositorManager& manager(Ogre::CompositorManager::getSingleton());

            manager.setCompositorEnabled(m_pViewPort, _name, false);
        }
    }
    else
    {
        throw Zen::Utility::runtime_exception("RenderingCanvas::disableCompositor() : Uninitialized compositor.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
