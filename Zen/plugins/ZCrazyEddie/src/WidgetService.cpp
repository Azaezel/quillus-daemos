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

#include "WidgetService.hpp"
#include "WidgetEvent.hpp"
#include "WidgetRenderer.hpp"
#include "Widget.hpp"

#include "../I_ImageSet.hpp"

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Engine/Input/I_MouseMoveEvent.hpp>
#include <Zen/Engine/Input/I_MouseClickEvent.hpp>
#include <Zen/Engine/Input/I_KeyEvent.hpp>

#include <Zen/Engine/Rendering/I_View.hpp>

#include <Zen/Engine/Widgets/I_WidgetManager.hpp>
#include <Zen/Engine/Widgets/I_Widget.hpp>

#ifndef WIN32
#include "SDL/SDL.h"
#endif // WIN32

#include <boost/bind.hpp>
#include <boost/any.hpp>

#include <list>
#include <string>

#include <Ogre.h>

#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI/ScriptingModules/LuaScriptModule/CEGUILua.h>

#include <Zen/plugins/ZLua/I_LuaScriptEngine.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetService::WidgetService(pScriptEngine_type _pScriptEngine)
:   m_pScriptEngine(_pScriptEngine)
,   m_widgetCache()
,   m_pWidgetRenderer(NULL)
,   m_pWidgetSystem(NULL)
,   m_pWidgetWindowManager(NULL)
,   m_fontService()
,   m_imageSetService()
,   m_schemeService()
,   m_root("")
,   m_mouseX(0)
,   m_mouseY(0)
,   m_pCollectionMutex(Zen::Threading::MutexFactory::create())
,   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WidgetService::~WidgetService()
{
    if (m_pWidgetSystem)
    {
        m_pWidgetSystem->destroy();
        m_pWidgetSystem = NULL;
    }

    Zen::Threading::MutexFactory::destroy(m_pCollectionMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::initialise(Zen::Engine::Rendering::I_View& _view,
                          Zen::Engine::Input::I_InputService& _input)
{
    // Connect the input events to the handlers
    _input.onMouseMoveEvent.connect(boost::bind(&WidgetService::handleMouseMoveEvent, this, _1));
    _input.onMouseClickEvent.connect(boost::bind(&WidgetService::handleMouseClickEvent, this, _1));
    _input.onKeyEvent.connect(boost::bind(&WidgetService::handleKeyPressed, this, _1));

    m_pWidgetRenderer.reset(new WidgetRenderer(_view));

    // TODO handle the case where m_pWidgetSystem may be initialized more than once.
    std::cout << "Initialzing CEGUI Widget System" << std::endl;
    m_pWidgetSystem = &CEGUI::System::getSingleton();

    // Initialize the Script system
    // TODO This should only be done if ZCRAZYEDDIE_USE_LUA is defined
    Zen::ZLua::I_LuaScriptEngine* pScriptEngine = dynamic_cast<Zen::ZLua::I_LuaScriptEngine*>(m_pScriptEngine.get());
    if (pScriptEngine)
    {
        CEGUI::LuaScriptModule* pScriptModule = &CEGUI::LuaScriptModule::create(pScriptEngine->getState());
        m_pWidgetSystem->setScriptingModule(pScriptModule);
    }

    //std::cout << "Initialzing CEGUI Singletons" << std::endl;
    //m_pWidgetSystem->createSingletons();

    std::cout << "Initializing CEGUI Logger" << std::endl;
    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

    std::cout << "Initializing CEGUI Window Manager" << std::endl;
    m_pWidgetWindowManager = CEGUI::WindowManager::getSingletonPtr();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Widgets::I_FontService&
WidgetService::getFontService()
{
    return m_fontService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ImageSetService&
WidgetService::getImageSetService()
{
    return m_imageSetService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SchemeService&
WidgetService::getSchemeService()
{
    return m_schemeService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Widgets::I_Widget&
WidgetService::getWidget(const std::string& _path,
                          bool _absolute)
{
    // HACK: First, get the name as if it were unique.
    CEGUI::Window* pWindow = NULL;
    try
    {
        pWindow = m_pWidgetWindowManager->getWindow((CEGUI::utf8*)_path.c_str());
    }
    catch(...)
    {
    }

    std::string path = _path;
    if (pWindow == NULL)
    {
        if( !_absolute )
        {
            path = m_root + "/" + _path;
        }

        /// Guard access to m_widgetCache and m_widgetIndices
        Zen::Threading::CriticalSection guard(m_pCollectionMutex);

        /// See if the widget is already cached. If so, just return it.
        widgetCache_type::iterator iter = m_widgetCache.find(path);
        if( iter != m_widgetCache.end() )
        {
            return *iter->second;
        }

        /// Parse the widget path into tokens
        std::list<std::string> tokens = parseWidgetPath(path);

        std::list<std::string>::const_iterator tokenIter = tokens.begin();

        /// This method should result in an exception if it fails.
        pWindow = m_pWidgetWindowManager->getWindow((CEGUI::utf8*)tokenIter->c_str());
        tokenIter++;

        /// Iterate through the descendants
        while( tokenIter != tokens.end() )
        {
            /// This method should result in an exception if it fails.
            pWindow = pWindow->getChild((CEGUI::utf8*)tokenIter->c_str());
            tokenIter++;
        }
    }

    /// Create the uncached I_Widget and store it in the cache
    Widget* pWidget = new Widget(*this, *pWindow);
    pWindow->setUserData(pWidget);

    m_widgetCache[path] = pWidget;

    /// Index the widget path by CEGUI::Window pointer.
    m_widgetIndices[pWindow] = path;

    /// Subscribe to the destruction event for this widget
    pWindow->subscribeEvent(
        CEGUI::Window::EventDestructionStarted,
        CEGUI::Event::Subscriber(
            &WidgetService::handleDestroyWidget,
            this
        )
    );

    return *pWidget;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::switchRoot(const std::string& _root)
{
    Zen::Engine::Widgets::I_Widget* pWidget = &getWidget(_root, true);
    Widget* pConcreteWidget = dynamic_cast<Widget*>(pWidget);
    m_root = _root;

    m_pWidgetSystem->setGUISheet(
        pConcreteWidget->getCEGUIWindow()
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::attachRoot(wpWidget_type _pWidget)
{
    Widget* pConcreteWidget = dynamic_cast<Widget*>(_pWidget);

    pConcreteWidget->getCEGUIWindow()->addChildWindow(
        (CEGUI::utf8*)_pWidget->getName().c_str()
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::destroyRoot(const std::string& _root)
{
    m_pWidgetWindowManager->destroyWindow((CEGUI::utf8*)_root.c_str());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::setDefaultTooltip(const std::string& _tooltip)
{
    m_pWidgetSystem->setDefaultTooltip((CEGUI::utf8*)_tooltip.c_str());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::setDefaultFont(const std::string& _font)
{
    m_pWidgetSystem->setDefaultFont(
        (CEGUI::utf8*)_font.c_str()
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::setDefaultMouseCursor(const I_ImageSet& _imageSet,
                                     const std::string& _imageName)
{
    m_pWidgetSystem->setDefaultMouseCursor(
        (CEGUI::utf8*)_imageSet.getName().c_str(),
        _imageName
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::setMouseCursor(const I_ImageSet& _imageSet,
                              const std::string& _imageName)
{
    CEGUI::MouseCursor::getSingleton().setImage(
        (CEGUI::utf8*)_imageSet.getName().c_str(),
        _imageName
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::showMouseCursor()
{
#ifdef WIN32
    ::ShowCursor(true);
#else
    SDL_ShowCursor(SDL_ENABLE);
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::hideMouseCursor()
{
#ifdef WIN32
    ::ShowCursor(false);
#else
    SDL_ShowCursor(SDL_DISABLE);
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::mouseGone(bool _isGone)
{
    /// TODO Implement
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::getMousePosition(int& _x, int& _y)
{
    _x = m_mouseX;
    _y = m_mouseY;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::setEnabled(bool _isEnabled)
{
    m_pWidgetSystem->setMutedState(!_isEnabled);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::loadWindowLayout(const std::string& _layout)
{
    m_pWidgetWindowManager->loadWindowLayout(_layout);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::list<std::string>
WidgetService::parseWidgetPath(const std::string& _path)
{
    // Parse the string into tokens using "/" as the delimiter.
    std::list<std::string> tokens;

    std::string delimiters = "/";

    // Skip the optional delimiters at the start of the string.
    std::string::size_type lastPosition = _path.find_first_not_of(delimiters, 0);

    // Find the first character that's not a delimiter.
    std::string::size_type curPosition = _path.find_first_of(delimiters, lastPosition);

    // Now that we're set up, loop through and find all of the strings
    while (curPosition != std::string::npos || lastPosition != std::string::npos)
    {
        // Push back the token onto the list.
        tokens.push_back(_path.substr(lastPosition, curPosition - lastPosition));

        // Skip the delimiter(s)
        lastPosition = _path.find_first_not_of(delimiters, curPosition);

        // Find the next character that's not a delimiter
        curPosition = _path.find_first_of(delimiters, lastPosition);
    }

    return tokens;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
WidgetService::handleDestroyWidget(const CEGUI::EventArgs& _eventArgs)
{
    /// TODO Compile CEGUI with RTTI and change this to a dynamic_cast.
    const CEGUI::WindowEventArgs* pEventArgs =
        static_cast<const CEGUI::WindowEventArgs*>(&_eventArgs);

    if( pEventArgs != NULL )
    {
        Zen::Threading::CriticalSection guard(m_pCollectionMutex);

        widgetIndices_type::iterator index = m_widgetIndices.find(pEventArgs->window);
        if( index != m_widgetIndices.end() )
        {
            widgetCache_type::iterator widget = m_widgetCache.find(index->second);
            if( widget != m_widgetCache.end() )
            {
                m_widgetCache.erase(widget);
                m_widgetIndices.erase(index);
                return true;
            }
            else
            {
                /// TODO Exception?
            }
        }
        else
        {
            /// TODO Throw an exception.
        }
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::handleMouseClickEvent(Zen::Engine::Input::I_MouseClickEvent& _event)
{
    bool handled = false;

    if( _event.wasClicked() )
    {
        switch( _event.getButton() )
        {
        case Zen::Engine::Input::BUTTON_ID_LEFT:
            handled = m_pWidgetSystem->injectMouseButtonDown(CEGUI::LeftButton);
            break;
        case Zen::Engine::Input::BUTTON_ID_RIGHT:
            handled = m_pWidgetSystem->injectMouseButtonDown(CEGUI::RightButton);
            break;
        }
    }
    else
    {
        switch( _event.getButton() )
        {
        case Zen::Engine::Input::BUTTON_ID_LEFT:
            handled = m_pWidgetSystem->injectMouseButtonUp(CEGUI::LeftButton);
            break;
        case Zen::Engine::Input::BUTTON_ID_RIGHT:
            handled = m_pWidgetSystem->injectMouseButtonUp(CEGUI::RightButton);
            break;
        }
    }

    if(!handled)
    {
        // Unhandled mouse events are forwarded to the widget service events
        onMouseClickEvent(_event);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::handleMouseMoveEvent(Zen::Engine::Input::I_MouseMoveEvent& _event)
{
	static int cursorVisible = 0;
    static bool widgetMouseVisible = false;

    if( (unsigned int)_event.getX() == m_pWidgetRenderer->getScreenWidth() ||
        (unsigned int)_event.getY() == m_pWidgetRenderer->getScreenHeight() ||
        _event.getX() == 0 ||
        _event.getY() == 0)
    {
#ifdef WIN32
        // Show the OS Cursor; might need to do it more than once
		while (cursorVisible < 0)
		{
			cursorVisible = ShowCursor(true);
		}
#else
        SDL_ShowCursor(SDL_ENABLE);
#endif

        // Tell the widget service that the mouse is gone
        mouseGone(true);

        if( widgetMouseVisible )
        {
            // Hide the widget mouse and disable the widget service
            hideMouseCursor();
            setEnabled(false);
            widgetMouseVisible = false;
        }
    }
    else
    {
#ifdef WIN32
        // Hide the OS cursor;
		while (cursorVisible > -1)
		{
			cursorVisible = ShowCursor(false);
		}
#else
        SDL_ShowCursor(SDL_DISABLE);
#endif

        /// Tell the widget service that the mouse is back.
        mouseGone(false);

        // Tell the widet service to show the mouse
        if (!widgetMouseVisible)
        {
            /// Show the mouse and enable the widget service.
            showMouseCursor();
            setEnabled(true);

            widgetMouseVisible = true;
        }
    }

    // TODO If editing, this event needs to be interpreted by the editor instead
    // of forwarded to CEGUI.
    bool handled = CEGUI::System::getSingleton().injectMousePosition(_event.getX(), _event.getY());

    m_mouseX = _event.getX();
    m_mouseY = _event.getY();

    if (_event.getZDelta() != 0)
    {
        //m_pWidgetSErvice->setMouseWheelDelta(_event.getZDelta());
    }

    // If the shift key is down, the mouse moves the camera, otherwise the mouse moves
    // the direction that the player is facing.
    //Zen::Engine::Rendering::I_Camera& camera = m_gameClient.base().getRenderingCanvas().getCurrentCamera();
    
    if (!handled)
    {
        // If the event wasn't handled, forward it to the widget service event.
        onMouseMoveEvent(_event);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WidgetService::handleKeyPressed(pKeyEvent_type _pKeyEvent)
{
    // TODO Inject key down or inject chars?
    bool handled = CEGUI::System::getSingleton().injectChar(_pKeyEvent->getChar());

    if (!handled)
    {
        if (_pKeyEvent->getPressedState())
        {
            handled = CEGUI::System::getSingleton().injectKeyDown(_pKeyEvent->getKeyCode());
        }
        else
        {
            handled = CEGUI::System::getSingleton().injectKeyUp(_pKeyEvent->getKeyCode());
        }
    }

    if (!handled)
    {
        // If the event wasn't handled, forward it to the widget service event.
        onKeyEvent(_pKeyEvent);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
WidgetService::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            Zen::Engine::Widgets::I_WidgetManager::getSingleton().getDefaultScriptModule(),
            Zen::Engine::Widgets::I_WidgetManager::getSingleton().getDefaultScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
