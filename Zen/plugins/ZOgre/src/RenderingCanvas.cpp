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

#include <OgreRenderSystem.h>
#include <OgreWindowEventUtilities.h>

#include <stddef.h>
#include <iostream>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingCanvas::RenderingCanvas(Zen::Scripting::script_module& _module, RenderingView* _pView)
:   m_root(Ogre::Root::getSingleton())
,   m_pRenderingView(_pView)
,   m_pCurrentCamera(NULL)
,   m_isCompositorManagerInitialized(false)
,   m_compositors()
,   m_pScriptObject(NULL)
,   m_module(_module)
{
    // Get the default scene manager
    std::cout << "OGRE: Ogre::Root::getSingleton().getSceneManager(\"default\");" << std::endl;
    m_pSceneManager = Ogre::Root::getSingleton().getSceneManager("default");

    assert(m_pSceneManager != NULL);

    std::cout << "Using scene type " << m_pSceneManager->getTypeName() << std::endl;

    // Create the default camera
    createCamera("default");

    std::cout << "adding Viewport" << std::endl;
    std::cout << "OGRE: Ogre::RenderWindow::addViewPort(&m_pCurrentCamera->getOgreCamera());" << std::endl;
    m_pViewPort = _pView->getRenderWindow().addViewport(&m_pCurrentCamera->getOgreCamera());

    //m_pSceneManager->setViewport(m_pViewPort);

    //// Select the camera, just in case somehow another camera was created.
    //std::cout << "selectCamera" << std::endl;
    //selectCamera("default");

    // HACK Why do we have to do this on Linux?
    if (Ogre::Root::getSingleton().getRenderSystem()->_getViewport() == NULL)
    {
        std::cout << "RenderingCanvas::RenderingCanvas(): Warning!  OGRE RenderSystem viewPort was not set.  Why not?" << std::endl;
        std::cout << "OGRE: Ogre::Root::getSingleton().getRenderSystem()->_setViewport(m_pViewPort);" << std::endl;
        Ogre::Root::getSingleton().getRenderSystem()->_setViewport(m_pViewPort);
        assert(m_pViewPort);
        std::cout << "Set OGRE RenderSystem viewPort." << std::endl;
    }

    // HACK This should only be done for windows apps
    Ogre::RenderSystem* const pRenderSystem = Ogre::Root::getSingleton().getRenderSystem();
    pRenderSystem->setWaitForVerticalBlank(false);
    m_pRenderingView->getRenderWindow().setActive(true);
    m_pRenderingView->getRenderWindow().setVisible(true);

    m_pViewPort->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

    m_pSceneManager->setAmbientLight(Ogre::ColourValue(0.75f, 0.75f, 0.75f));

    std::cout << "RenderingCanvas::RenderingCanvas(): Done" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingCanvas::~RenderingCanvas()
{
    std::cout << "RenderingCanvas::~RenderingCanvas()" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingCanvas::setSceneGraph(const Zen::Engine::Rendering::I_SceneGraph& _sceneGraph)
{
    std::cout << "RenderingCanvas::setSceneGraph()" << std::endl;
    throw Utility::runtime_exception("RenderingCanvas::setSceneGraph(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingCanvas::resize(int _x, int _y, int _width, int _height)
{
    std::cout << "RenderingCanvas::resize(): " << _x << " " << _y << " " << _width << " " << _height << std::endl;
    std::cout << "OGRE: Ogre::RenderWindow::reposition(_x, _y);" << std::endl;
    m_pRenderingView->getRenderWindow().reposition(_x, _y);
    std::cout << "OGRE: Ogre::RenderWindow::resize(_width, _height);" << std::endl;
    m_pRenderingView->getRenderWindow().resize(_width, _height);

    std::cout << "OGRE: Ogre::RenderWindow::windowMovedOrResized();" << std::endl;
    m_pRenderingView->getRenderWindow().windowMovedOrResized();

    std::cout << "RenderingCanvas::resize(): done" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingCanvas::renderScene()
{
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

    bool rc = m_root.renderOneFrame();
    assert(rc);

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
        std::cout << "OGRE: Ogre::SceneManager::createCamera(_name);" << std::endl;
        Ogre::Camera* pOgreCamera = m_pSceneManager->createCamera(_name);
        assert(pOgreCamera);

        Camera* pCamera = new Camera(m_module, *this, *pOgreCamera, _name);

        assert(pCamera);

        // Default the aspect ratio
        //pCamera->setAspectRatio((Ogre::Real)getWidth() / (Ogre::Real)getHeight());

        // If the current camera hasn't been set, set it
        if (m_pCurrentCamera == NULL)
        {
            m_pCurrentCamera = pCamera;
        }
        //else
        //{
            // temporary
            //assert(false);
        //}

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

    std::cout << "m_cameras.find" << std::endl;
    Cameras_type::iterator iter = m_cameras.find(_name);

    // Find the camera
    if (iter == m_cameras.end())
    {
        std::cout << "Not found" << std::endl;
        // Oops, it wasn't found... throw an error.

        std::stringstream errorMessage;
        errorMessage << "selectCamera(): Error, camera not found: " << _name;
        throw Utility::runtime_exception(errorMessage.str());
    }
    else
    {
        assert(m_pViewPort);

        std::cout << "found it, getting the ogre camera" << std::endl;
        // Set the current camera for this viewport
        std::cout << "OGRE: Ogre::Viewport::setCamera(&iter->second->getOgreCamera());" << std::endl;
        m_pViewPort->setCamera(&iter->second->getOgreCamera());

        std::cout << "setting m_pCurrentCamera" << std::endl;
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
void
RenderingCanvas::pumpSystemMessages()
{
    Ogre::WindowEventUtilities::messagePump();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
RenderingCanvas::getScriptObject()
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
