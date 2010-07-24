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
//  Tony Richards trichards@indiezen.org
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "WidgetRenderer.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/plugins/ZOgre/I_OgreView.hpp>

#include <Ogre.h>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetRenderer::WidgetRenderer(Zen::Engine::Rendering::I_View& _view)
:   m_pWidgetRenderer(NULL)
{
    /// Provided an I_OgreView interface in ZOgre
    /// in order to initialize the m_pWidgetRenderer member
    /// variable, as a CEGUI::OgreCEGUIRenderer object
    /// requires an Ogre::RenderWindow* on initialization.
    ///
    /// We can always refactor this later and implement a
    /// custom renderer that derives from CEGUI::Renderer,
    /// thus decoupling Ogre from CEGUI in this plugin.
    ///
    /// For now, we'll just deal with ZCrazyEddie's dependence
    /// on Ogre (and thus ZOgre).
    Zen::ZOgre::I_OgreView* pView =
        dynamic_cast<Zen::ZOgre::I_OgreView*>(&_view);

    if( pView != NULL )
    {
        std::cout << "Bootstraping CEGUI OgreRenderer" << std::endl;
        m_pWidgetRenderer = &CEGUI::OgreRenderer::bootstrapSystem(pView->getRenderWindow());
    }
    else
    {
        std::cout << "Error, no OGRE renderer." << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetRenderer::~WidgetRenderer()
{
    if (m_pWidgetRenderer)
    {
        CEGUI::OgreRenderer::destroy(*m_pWidgetRenderer);
        m_pWidgetRenderer = NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetRenderer::dimension_type
WidgetRenderer::getScreenHeight()
{
    const CEGUI::Size& size = m_pWidgetRenderer->getDisplaySize();
    return (dimension_type)size.d_height;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetRenderer::dimension_type
WidgetRenderer::getScreenWidth()
{
    const CEGUI::Size& size = m_pWidgetRenderer->getDisplaySize();
    return (dimension_type)size.d_width;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CEGUI::OgreRenderer&
WidgetRenderer::getCEGUIRenderer()
{
    if (m_pWidgetRenderer == NULL)
    {
        throw Utility::runtime_exception("WidgetRenderer::getCEGUIRenderer(): Error, m_pWidgetRenderer is NULL.");
    }
    return *m_pWidgetRenderer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
