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
#ifndef ZEN_ZCRAZYEDDIE_WIDGET_SERVICE_HPP_INCLUDED
#define ZEN_ZCRAZYEDDIE_WIDGET_SERVICE_HPP_INCLUDED

#include "FontService.hpp"
#include "ImageSetService.hpp"
#include "SchemeService.hpp"

#include "../I_WidgetService.hpp"

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#include <Ogre.h>

#include <boost/any.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
    namespace Engine {
        namespace Input {
            class I_MouseClickEvent;
            class I_MouseMoveEvent;
        }   // namespace Input
        namespace Widgets {
            class I_FontService;
        }   // namespace Widgets
    }   // namespace Engine
namespace ZCrazyEddie {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class WidgetRenderer;

class WidgetService
:   public I_WidgetService
,   public Zen::Memory::managed_self_ref<Zen::Engine::Widgets::I_WidgetService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine>             pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Zen::Engine::Input::I_KeyEvent>        pKeyEvent_type;
    typedef Zen::Memory::managed_ptr<Zen::Engine::Input::I_MouseClickEvent> pMouseClickEvent_type;
    typedef Zen::Memory::managed_ptr<Zen::Engine::Input::I_MouseMoveEvent>  pMouseMoveEvent_type;
    /// @}

    /// @name I_WidgetService implementation
    /// @{
public:
    virtual void initialise(Zen::Engine::Rendering::I_View& _view,
            Zen::Engine::Input::I_MousePublisher* _pMousePublisher,
            Zen::Engine::Input::I_KeyPublisher* _pKeyPublisher);
    virtual Zen::Engine::Widgets::I_FontService& getFontService();
    virtual Zen::Engine::Widgets::I_Widget& getWidget(const std::string& _path, bool _absolute = false);
    virtual void switchRoot(const std::string& _root);
    virtual void attachRoot(wpWidget_type _pWidget);
    virtual void destroyRoot(const std::string& _root);
    virtual void setDefaultTooltip(const std::string& _tooltip);
    virtual void setDefaultFont(const std::string& _font);
    virtual void showMouseCursor();
    virtual void hideMouseCursor();
    virtual void mouseGone(bool _isGone);
    virtual void getMousePosition(int& _x, int& _y);
    virtual void setEnabled(bool _isEnabled);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name ZCrazyEddie::I_WidgetService implementation
    /// @{
public:
    virtual I_ImageSetService& getImageSetService();
    virtual I_SchemeService& getSchemeService();
    virtual void loadWindowLayout(const std::string& _layout);
    virtual void setDefaultMouseCursor(const I_ImageSet& _imageSet, const std::string& _imageName);
    virtual void setMouseCursor(const I_ImageSet& _imageSet, const std::string& _imageName);
    /// @}

    /// @name WidgetService implementation
    /// @{
protected:
    std::list<std::string> parseWidgetPath(const std::string& _path);
    bool handleDestroyWidget(const CEGUI::EventArgs& _eventArgs);
    void handleMouseClickEvent(pMouseClickEvent_type _pEvent);
    void handleMouseMoveEvent(pMouseMoveEvent_type _pEvent);
    void handleKeyPressed(pKeyEvent_type _pKeyEvent);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class WidgetServiceFactory;
             WidgetService(pScriptEngine_type _pScriptEngine);
    virtual ~WidgetService();
    /// @}

    /// @name Member variables
    /// @{
private:
    pScriptEngine_type                                              m_pScriptEngine;
    Zen::Threading::I_Mutex*                                        m_pCollectionMutex;

    typedef std::map<std::string, Zen::Engine::Widgets::I_Widget* > widgetCache_type;
    widgetCache_type                                                m_widgetCache;

    typedef std::map<CEGUI::Window*, std::string>                   widgetIndices_type;
    widgetIndices_type                                              m_widgetIndices;

    std::auto_ptr<WidgetRenderer>                                   m_pWidgetRenderer;

    /// A pointer to the underlying Widget System.
    CEGUI::System*                                                  m_pWidgetSystem;

    CEGUI::WindowManager*                                           m_pWidgetWindowManager;

    FontService                                                     m_fontService;
    ImageSetService                                                 m_imageSetService;
    SchemeService                                                   m_schemeService;

    std::string                                                     m_root;

    int                                                             m_mouseX;
    int                                                             m_mouseY;

    ScriptObjectReference_type*                                     m_pScriptObject;
    /// @}

};	// class WidgetService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCrazyEddie
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZCRAZYEDDIE_WIDGET_SERVICE_HPP_INCLUDED
