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

#include "RenderingView.hpp"
#include <Zen/Engine/Rendering/I_SceneService.hpp>

#include "NullContext.hpp"

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingView::RenderingView(Zen::Engine::Rendering::I_Context& _context, const std::string& _windowName, unsigned int _width, unsigned int _height)
:   m_root(Ogre::Root::getSingleton())
,   m_context(_context)
,   m_pRenderWindow(NULL)
,   m_pCanvas(NULL)
{
    std::cout << "new RenderingView" << std::endl;

    Ogre::NameValuePairList nvpl;

#ifdef WIN32
    //nvpl["externalWindowHandle"] = Ogre::StringConverter::toString((int)m_context.getParentWindow());
    nvpl["parentWindowHandle"] = Ogre::StringConverter::toString((int)m_context.getParentWindow());
#else
#if 0 // not working yet
    // Retrieving the display, screen and window values from the Pigment window
    Display* dpy = XOpenDisplay (g_getenv ("DISPLAY"));
    gint screen = DefaultScreen (dpy);
    Window win;
    // viewport was created with pgm_viewport_factory_make ("opengl", &viewport);
    pgm_viewport_get_embedding_id(viewport, &win);

    misc["parentWindowHandle"] =  StringConverter::toString((long)dpy) + ":" +
                                        StringConverter::toString((int)screen) + ":" +
                                        StringConverter::toString((long)win);
#endif // not working on other platforms?
#endif
    nvpl["title"] = _windowName.c_str();

    m_pRenderWindow = m_root.createRenderWindow(_windowName.c_str(), _width, _height, false, &nvpl);

    Ogre::Root::getSingleton().initialise(false, "IndieZen Rendering Window");

    // Set the window value in the _context
    NullContext* pContext = dynamic_cast<NullContext*>(&_context);

    if (pContext != NULL)
    {
        m_pRenderWindow->getCustomAttribute("WINDOW", &pContext->m_pHandle);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingView::~RenderingView()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingView::setCamera(Zen::Engine::Camera::I_Camera* _pCamera)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_View*
RenderingView::createSubView(int _x, int _y, int _width, int _height)
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_Canvas&
RenderingView::getCanvas()
{
    return *m_pCanvas;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
RenderingView::initCanvas()
{
    m_pCanvas = new RenderingCanvas(this);
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::RenderWindow&
RenderingView::getRenderWindow()
{
    return *m_pRenderWindow;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
