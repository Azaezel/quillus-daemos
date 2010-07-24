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
#include "SceneViewWidget.hpp"
#include "Workbench.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Engine/Rendering/I_SceneService.hpp>

#include <Zen/Studio/Workbench/I_SceneContributor.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SceneModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SceneModelType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SceneAction.hpp>

#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <Zen/Studio/Workbench/wxUtilities.hpp>

BEGIN_EVENT_TABLE(Zen::Studio::Workbench::SceneViewWidget, View)
	EVT_SIZE(Zen::Studio::Workbench::SceneViewWidget::onSize)
	EVT_PAINT(Zen::Studio::Workbench::SceneViewWidget::onPaint)
	EVT_SET_FOCUS(Zen::Studio::Workbench::SceneViewWidget::handleSetFocus)
	EVT_KILL_FOCUS(Zen::Studio::Workbench::SceneViewWidget::handleKillFocus)
	EVT_ACTIVATE(Zen::Studio::Workbench::SceneViewWidget::handleActivate)
	EVT_SET_CURSOR(Zen::Studio::Workbench::SceneViewWidget::handleSetCursor)
    EVT_KEY_DOWN(Zen::Studio::Workbench::SceneViewWidget::handleKeyDown)
    EVT_KEY_UP(Zen::Studio::Workbench::SceneViewWidget::handleKeyUp)
    EVT_ENTER_WINDOW(Zen::Studio::Workbench::SceneViewWidget::handleEnterWindow)
    EVT_LEAVE_WINDOW(Zen::Studio::Workbench::SceneViewWidget::handleLeaveWindow)
    EVT_MOTION(Zen::Studio::Workbench::SceneViewWidget::handleMotion)
    EVT_LEFT_DOWN(Zen::Studio::Workbench::SceneViewWidget::handleLeftDown)
    EVT_LEFT_UP(Zen::Studio::Workbench::SceneViewWidget::handleLeftUp)
    EVT_RIGHT_DOWN(Zen::Studio::Workbench::SceneViewWidget::handleRightDown)
    EVT_RIGHT_UP(Zen::Studio::Workbench::SceneViewWidget::handleRightUp)
    EVT_MIDDLE_DOWN(Zen::Studio::Workbench::SceneViewWidget::handleMiddleDown)
    EVT_MIDDLE_UP(Zen::Studio::Workbench::SceneViewWidget::handleMiddleUp)
    EVT_MOUSEWHEEL(Zen::Studio::Workbench::SceneViewWidget::handleMouseWheel)
END_EVENT_TABLE()

#ifdef __WXGTK__
#include <gtk/gtk.h>
#include <gtk/gtkprivate.h>

#include <gdk/gdkprivate.h>
#include <gdk/gdkx.h>

#include <sstream>
#endif

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneViewWidget::SceneViewWidget(wxWindow* _pParent, Workbench* _pWorkbench, pViewable_type _pViewable, const int _instance)
:   View(_pParent, _pWorkbench, _pViewable, _instance, wxNO_BORDER | wxWANTS_CHARS)
,   m_pRenderingView(NULL)
,   m_enterWindowCount(0)
{
    // TODO Eventually _pDocument will be a future.  Set up an event to handle
    // when it becomes available.

    // Create the rendering view.
	createGUIControls();

    // TODO Subscribing may add a large number of scene nodes.
    // Make it so that this is batched and redraw the view
    I_SceneModel* pModel = m_pViewable.as<Memory::managed_ptr<I_SceneModel> >().get();
    m_pSubscription = pModel->subscribe(this);

    this->SetFocus();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneViewWidget::~SceneViewWidget()
{
    m_pWorkbench->getRenderingService()->destroyView(m_pRenderingView);
    m_pWorkbench->getRenderingService()->destroyContext(m_pContext);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Project*
SceneViewWidget::getProject() const
{
    return getDocument()->getProject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::activate()
{
    View::activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::deactivate()
{
    View::deactivate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Rendering::I_View*
SceneViewWidget::getSceneView()
{
    return m_pRenderingView;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Rendering::I_Context*
SceneViewWidget::getContext()
{
    return m_pContext;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::createGUIControls()
{
#ifdef WIN32
    m_pContext = m_pWorkbench->getRenderingService()->createContext((HWND)(this->GetHandle()));
#else
    // Ogre 1.7 + Linux + GLX platform wants a string of the format display:screen:window,
    // which has variable types ulong:uint:ulong.
	GtkWidget* pWidget = GetHandle();
	gtk_widget_realize(pWidget);

    std::stringstream handleStream;

    // Get the Display
	Display* pDisplay = GDK_WINDOW_XDISPLAY(pWidget->window );

    // Window is a typedef for XID, which is a typedef for unsigned int
	Window wid = GDK_WINDOW_XWINDOW(pWidget->window);

	// Get the right display (DisplayString() returns ":display.screen")
    std::string displayStr = DisplayString(pDisplay);

    // TODO Need to use stl / boost intead of OGRE for substr
    // see http://www.boost.org/doc/libs/1_39_0/doc/html/string_algo/usage.html
	displayStr = displayStr.substr(1, (displayStr.find( ".", 0 ) - 1 ));

	// Put all together
	handleStream << displayStr << ':' << DefaultScreen(pDisplay) << ':' << wid;
    const std::string handle = handleStream.str();

    std::cout << "GTK2+ using handle " << handle << " as the parent for creating the OGRE window." << std::endl;

    m_pContext = m_pWorkbench->getRenderingService()->createContext(handle.c_str());
#endif
    // TODO Get the size from the parent window?
    m_pRenderingView = m_pWorkbench->getRenderingService()->createView(*m_pContext, "Test", 100, 100);

    // TODO Do these next two steps need to be done elsewhere?

    // Create a scene service.
    // Name the Ogre scene manager "default" and use the ST_EXTERIOR_CLOSE scene manager type.
    m_pSceneService = Engine::Rendering::I_SceneManager::getSingleton().create("ogre", "default", 2);

    Zen::Engine::Resource::I_ResourceManager::config_type config;

    // Create the service
    m_pRenderingResourceService =
        Zen::Engine::Resource::I_ResourceManager::getSingleton().create("ogre", config);

    std::cout << "Initializing the canvas" << std::endl;

    // Initialize the canvas.
    m_pRenderingView->initCanvas();

    std::cout << "initCanvas done" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_RenderingCanvas&
SceneViewWidget::getRenderingCanvas()
{
    Engine::Rendering::I_RenderingCanvas* pCanvas = dynamic_cast<Engine::Rendering::I_RenderingCanvas*>(&m_pRenderingView->getCanvas());

    if (pCanvas != NULL)
    {
        return *pCanvas;
    }
    else
    {
        throw Utility::runtime_exception("SceneViewWidget::getRenderingCanvas(): Error, wrong canvas type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::onSize(wxSizeEvent& _event)
{
    std::cout << "SceneViewWidget::onSize()" << std::endl;

	wxSize sz = GetClientSize();

    bool manualRepaint = false;

    if (!(sz.x > (int)getRenderingCanvas().getWidth() || sz.y > (int)getRenderingCanvas().getHeight()))
    {
        // Need to repaint manually if the window got smaller.
        manualRepaint = true;
    }

    getRenderingCanvas().resize(0, 0, sz.x, sz.y);

    if (manualRepaint)
    {
        updateView();
    }

    std::cout << "SceneViewWidget::onSize() done" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::onPaint(wxPaintEvent& _event)
{
    wxPaintDC dc(this);

    updateView();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleSetFocus(wxFocusEvent& _event)
{
    std::cout << "SceneViewWidget::handleSetFocus()" << std::endl;
    //CaptureMouse();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleKillFocus(wxFocusEvent& _event)
{
    std::cout << "SceneViewWidget::handleKillFocus()" << std::endl;
    //ReleaseMouse();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleActivate(wxActivateEvent& _event)
{
    std::cout << "SceneViewWidget::handleActivate(): "
        << (_event.GetActive() ? "Activate" : "Deactivate")
        << std::endl;
    //ReleaseMouse();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleSetCursor(wxSetCursorEvent& _event)
{
    std::cout << "SceneViewWidget::handleSetCursor()" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleKeyDown(wxKeyEvent& _event)
{
    std::cout << "SceneViewWidget::handleKeyDown(): " << _event.GetRawKeyCode() << std::endl;
    onKeyDown(_event.GetRawKeyCode());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleKeyUp(wxKeyEvent& _event)
{
    std::cout << "SceneViewWidget::handleKeyUp(): " << _event.GetRawKeyCode() << std::endl;
    onKeyUp(_event.GetRawKeyCode());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleEnterWindow(wxMouseEvent& _event)
{
    std::cout << "SceneViewWidget::handleEnterWindow(): " << m_enterWindowCount << std::endl;
    // See if the mouse is entering this window.
    if (_event.Entering())
    {

        // If this window doesn't have the mouse capture, get it.
        if (m_enterWindowCount == 0)
        {
            std::cout << "Capture Mouse" << std::endl;
            //CaptureMouse();
        }

        m_enterWindowCount++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleLeaveWindow(wxMouseEvent& _event)
{
    std::cout << "SceneViewWidget::handleLeaveWindow(): " << m_enterWindowCount << std::endl;
    // See if the mouse is leaving this window.
    if (_event.Leaving())
    {
        // TODO Also need to decrement this when EVT_MOUSE_CAPTURE_LOST is fired.

        // If this window has the mouse capture, release it.
        --m_enterWindowCount;

        if (m_enterWindowCount == 0)
        {
            std::cout << "Release Mouse" << std::endl;
            //ReleaseMouse();
        }

        MouseEventPayload_type mouseEvent = {
            _event.GetX(), _event.GetY(), getMouseButtons(_event)
        };

        onLeaveWindow(mouseEvent);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleMotion(wxMouseEvent& _event)
{
    std::cout << "." << std::flush;

    this->SetFocus();

    if (!HasCapture())
    {
        //CaptureMouse();
    }

    MouseEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event)
    };

    onMouseMove(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleLeftDown(wxMouseEvent& _event)
{
    MouseEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event)
    };

    onLeftDown(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleLeftUp(wxMouseEvent& _event)
{
    MouseEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event)
    };

    onLeftUp(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleRightDown(wxMouseEvent& _event)
{
    MouseEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event)
    };

    onRightDown(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleRightUp(wxMouseEvent& _event)
{
    MouseEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event)
    };

    onRightUp(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleMiddleDown(wxMouseEvent& _event)
{
    MouseEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event)
    };

    onMiddleDown(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleMiddleUp(wxMouseEvent& _event)
{
    MouseEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event)
    };

    onMiddleUp(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::handleMouseWheel(wxMouseEvent& _event)
{
    MouseWheelEventPayload_type mouseEvent = {
        _event.GetX(), _event.GetY(), getMouseButtons(_event), _event.GetWheelDelta()
    };

    onMouseWheel(mouseEvent);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
unsigned int
SceneViewWidget::getMouseButtons(wxMouseEvent& _event)
{
    return (_event.LeftIsDown() ? ZMB_LEFT : 0) |
        (_event.RightIsDown() ? ZMB_RIGHT : 0) |
        (_event.MiddleIsDown() ? ZMB_MIDDLE : 0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::updateView()
{
    // TODO Possibly need to get the scene manager and add a render queue listener.
    // See ViewOgreWindow::updateOgreView in wxOgreMVC.
    getRenderingCanvas().renderScene();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
