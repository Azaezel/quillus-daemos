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
#ifndef ZEN_ZOGRE_RENDERING_VIEW_HPP_INCLUDED
#define ZEN_ZOGRE_RENDERING_VIEW_HPP_INCLUDED

#include "RenderingCanvas.hpp"

#include "../I_OgreView.hpp"

#include <Zen/Engine/Rendering/I_View.hpp>
#include <Zen/Engine/Rendering/I_Context.hpp>

#include "Ogre.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// A ZOgre::RenderingView is the same as an Ogre::RenderWindow
class RenderingView
//:   public Zen::Engine::Rendering::I_View
:   public I_OgreView
{
    /// @name I_View implementation
    /// @{
public:
    virtual void setCamera(Zen::Engine::Camera::I_Camera* _pCamera);
    virtual Zen::Engine::Rendering::I_View* createSubView(int _x, int _y, int _width, int _height);
    virtual Zen::Engine::Rendering::I_Canvas& getCanvas();
    virtual bool initCanvas();
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

    /// @name 'Structors
    /// @{
public:
             RenderingView(Zen::Scripting::script_module& _module, Zen::Engine::Rendering::I_Context& _context, const std::string& _windowName, unsigned int _width, unsigned int _height);
    virtual ~RenderingView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogre::Root&                 m_root;
    const Zen::Engine::Rendering::I_Context& m_context;
    Ogre::RenderWindow*         m_pRenderWindow;

    RenderingCanvas*            m_pCanvas;

    ScriptObjectReference_type*     m_pScriptObject;
    Zen::Scripting::script_module&  m_module;
    /// @}

};  // class RenderingView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_RENDERING_VIEW_HPP_INCLUDED
