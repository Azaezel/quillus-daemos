//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZENSTUDIO_WORKBENCH_SCENE_VIEW_WIDGET_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_SCENE_VIEW_WIDGET_HPP_INCLUDED

#include "View.hpp"

#include <Zen/Studio/Workbench/I_SceneView.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SceneModel.hpp>

#include <Zen/Engine/Rendering/I_View.hpp>
#include <Zen/Engine/Rendering/I_Context.hpp>
#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_SceneManager.hpp>

#include <Zen/Engine/Resource/I_ResourceManager.hpp>
#include <Zen/Engine/Resource/I_ResourceService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Scene View Widget.
/// This widget is an extension to an I_SceneView.
/// Use this to directly manipulate the scene view widget.
class SceneViewWidget
:   public View
,   public Zen::Studio::Workbench::I_SceneView
{
private:
    DECLARE_EVENT_TABLE();

    /// @name Types
    /// @{
public:
    typedef Zen::Engine::Rendering::I_SceneManager::pSceneService_type  pSceneService_type;
    typedef Zen::Engine::Resource::I_ResourceManager::pResourceService_type pResourceService_type;
    /// @}

    /// @name I_View implementation
    /// @{
public:
    virtual I_Project* getProject() const;
    virtual void activate();
    virtual void deactivate();
    virtual I_Viewable* getViewable() const { return View::getViewable(); }
    /// @}

    /// @name I_SceneView implementation
    /// @{
public:
    virtual Zen::Engine::Rendering::I_View* getSceneView();
    virtual Zen::Engine::Rendering::I_Context* getContext();
    /// @}

    /// @name SceneViewWidget implementation
    /// @{
public:
    void createGUIControls();

    /// Get a reference to the rendering canvas.
    Zen::Engine::Rendering::I_RenderingCanvas& getRenderingCanvas();

	void onSize(wxSizeEvent& _event);
	void onPaint(wxPaintEvent& _event);

    void handleSetFocus(wxFocusEvent& _event);
    void handleKillFocus(wxFocusEvent& _event);
    void handleActivate(wxActivateEvent& _event);

    void handleSetCursor(wxSetCursorEvent& _event);

    void handleKeyDown(wxKeyEvent& _event);
    void handleKeyUp(wxKeyEvent& _event);

    void handleEnterWindow(wxMouseEvent& _event);
    void handleLeaveWindow(wxMouseEvent& _event);
    void handleMotion(wxMouseEvent& _event);
    void handleLeftDown(wxMouseEvent& _event);
    void handleLeftUp(wxMouseEvent& _event);
    void handleRightDown(wxMouseEvent& _event);
    void handleRightUp(wxMouseEvent& _event);
    void handleMiddleDown(wxMouseEvent& _event);
    void handleMiddleUp(wxMouseEvent& _event);
    void handleMouseWheel(wxMouseEvent& _event);

    enum ZenMouseButtons
    {
        ZMB_LEFT = 1,
        ZMB_RIGHT = 2,
        ZMB_MIDDLE = 4
    };

    /// Get a bitmap of the mouse buttons that are down.
    /// Use the ZenMouseButtons for the values.
    unsigned int getMouseButtons(wxMouseEvent& _event);

    void updateView();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class Workbench;
             SceneViewWidget(wxWindow* _pParent, Workbench* _pWorkbench, pViewable_type _pViewable, const int _instance);
    virtual ~SceneViewWidget();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Engine::Rendering::I_Context*          m_pContext;
    Zen::Engine::Rendering::I_View*             m_pRenderingView;
    I_SceneModel::pSubscription_type            m_pSubscription;
    pSceneService_type                          m_pSceneService;
    pResourceService_type                       m_pRenderingResourceService;

    int                                         m_enterWindowCount;
    /// @}

};  // class SceneViewWidget

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_SCENE_VIEW_WIDGET_HPP_INCLUDED
