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
#ifndef ZEN_ZOGRE_RENDERING_CANVAS_HPP_INCLUDED
#define ZEN_ZOGRE_RENDERING_CANVAS_HPP_INCLUDED

#include "../I_OgreRenderingCanvas.hpp"

#include "Ogre.hpp"

#if 0 // deprecated
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#endif // deprecated

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class RenderingView;
class Camera;

/// A RenderingCanvas is the same as an Ogre::ViewPort
class RenderingCanvas
:   public I_OgreRenderingCanvas
{
    /// @name I_Canvas implementation
    /// @{
public:
    //virtual void setCamera(pCamera_type _camera);
    //virtual void setCameraType(const std::string& _cameraType);
    virtual void setSceneGraph(const Engine::Rendering::I_SceneGraph& _sceneGraph);
    /// @}

    /// @name I_RenderingCanvas implementation
    /// @{
public:
    virtual void resize(int _x, int _y, int _width, int _height);
    virtual void renderScene();
    virtual void swapBuffers();
    virtual void clearBackBuffer();
    virtual void setInputDevice(const std::string& _deviceName);

    virtual void focusLost();
    virtual void focusGained();

    virtual unsigned getWidth() const;
    virtual unsigned getHeight() const;

    virtual Engine::Rendering::I_Camera& createCamera(const std::string& _name);
    virtual Engine::Rendering::I_Camera& getCurrentCamera();
    virtual Engine::Rendering::I_Camera& selectCamera(const std::string& _name);

    virtual void querySceneNodes(Math::Real _x, Math::Real _y, I_SceneNodeVisitor& _visitor);

    virtual void pumpSystemMessages();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_OgreRenderingCanvas implementation
    /// @{
public:
    virtual void initializeCompositor(const std::string& _name);
    virtual void enableCompositor(const std::string& _name);
    virtual void disableCompositor(const std::string& _name);
    /// @}

    /// @name RenderingCanvas implementation
    /// @{
public:
    Ogre::SceneManager* getOgreSceneManager() const;
    Ogre::Viewport* getOgreViewport() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             RenderingCanvas(Zen::Scripting::script_module& _module, RenderingView* _pView);
    virtual ~RenderingCanvas();

    /// @name Member Variables
    /// @{
private:
    Ogre::Root&         m_root;

    /// Parent view
    RenderingView*      m_pRenderingView;

    Ogre::SceneManager* m_pSceneManager;

    Ogre::Viewport*     m_pViewPort;

    typedef std::map<std::string, Camera*>  Cameras_type;
    Cameras_type        m_cameras;
    Camera*             m_pCurrentCamera;

    //pCamera_type        m_pCameraController;
    //std::string         m_strCameraType;

    std::map< std::string, bool >   m_compositors;
    bool                            m_isCompositorManagerInitialized;

    ScriptObjectReference_type*     m_pScriptObject;
    Zen::Scripting::script_module&  m_module;
    /// @}

};  // class RenderingCanvas

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_RENDERING_CANVAS_HPP_INCLUDED
