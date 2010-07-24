//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ZOGRE_RENDERING_VIEW_HPP_INCLUDED
#define ZEN_ZOGRE_RENDERING_VIEW_HPP_INCLUDED

#include "RenderingCanvas.hpp"

#include "../I_OgreView.hpp"

#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>

#include <Zen/Engine/Rendering/I_View.hpp>
#include <Zen/Engine/Rendering/I_Context.hpp>

#include "Ogre.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// A ZOgre::RenderingView is the same as an Ogre::RenderWindow
class RenderingView
:   public I_OgreView
,	public Ogre::WindowEventListener
{
	/// @name Types
	/// @{
public:
    typedef Zen::Event::I_EventManager::pEventService_type  pEventService_type;
	/// @}

    /// @name I_View implementation
    /// @{
public:
    virtual void setCamera(Zen::Engine::Camera::I_Camera* _pCamera);
    virtual Zen::Engine::Rendering::I_View* createSubView(int _x, int _y, int _width, int _height);
    virtual Zen::Engine::Rendering::I_Canvas& getCanvas();
    virtual bool initCanvas();
    virtual Event::I_Event& getViewMovedEvent();
    virtual Event::I_Event& getViewResizedEvent();
    virtual Event::I_Event& getViewClosedEvent();
    virtual Event::I_Event& getViewFocusChangedEvent();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_OgreView implementation
    /// @{
public:
    virtual Ogre::RenderWindow& getRenderWindow();
    /// @}

    /// @name RenderingView implementation
    /// @{
public:
    //Ogre::RenderWindow* getRenderWindow();
    /// @}

    /// @name Ogre::WindowEventListener implementation
    /// @{
public:
	virtual void windowMoved(Ogre::RenderWindow* _pRenderWindow);
	virtual void windowResized(Ogre::RenderWindow* _pRenderWindow);
	virtual bool windowClosing(Ogre::RenderWindow* _pRenderWindow);
	virtual void windowClosed(Ogre::RenderWindow* _pRenderWindow);
	virtual void windowFocusChange(Ogre::RenderWindow* _pRenderWindow);
	/// @}

    /// @name 'Structors
    /// @{
public:
             RenderingView(Zen::Scripting::script_module& _module, Zen::Engine::Rendering::I_Context& _context, const std::string& _windowName, unsigned int _width, unsigned int _height);
    virtual ~RenderingView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogre::Root&                 	m_root;
    const Zen::Engine::Rendering::I_Context& m_context;
    Ogre::RenderWindow*         	m_pRenderWindow;

    RenderingCanvas*            	m_pCanvas;
    pEventService_type          	m_pEventService;

    ScriptObjectReference_type*     m_pScriptObject;
    Zen::Scripting::script_module&  m_module;
    /// @}

};  // class RenderingView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_RENDERING_VIEW_HPP_INCLUDED
