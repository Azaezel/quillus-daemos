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

BEGIN_EVENT_TABLE(Zen::Studio::Workbench::SceneViewWidget, wxPanel)
	EVT_SIZE(Zen::Studio::Workbench::SceneViewWidget::onSize)
	EVT_PAINT(Zen::Studio::Workbench::SceneViewWidget::onPaint)
END_EVENT_TABLE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneViewWidget::SceneViewWidget(wxWindow* _pParent, Workbench* _pWorkbench, pViewable_type _pViewable, const int _instance)
:   View(_pParent, _pWorkbench, _pViewable, _instance)
,   m_pRenderingView(NULL)
{
    // TODO Eventually _pDocument will be a future.  Set up an event to handle
    // when it becomes available.

    // Create the rendering view.
	createGUIControls();

    // TODO Subscribing may add a large number of scene nodes.
    // Make it so that this is batched and redraw the view
    I_SceneModel* pModel = m_pViewable.as<Memory::managed_ptr<I_SceneModel> >().get();
    m_pSubscription = pModel->subscribe(this);
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
void
SceneViewWidget::createGUIControls()
{
#ifdef WIN32
    m_pContext = m_pWorkbench->getRenderingService()->createContext((HWND)(this->GetHandle()));
#else
    throw Utility::runtime_exception("SceneViewWidget::createGUIControls(): Error, finish.");
    #if 0
    m_pContext = m_pWorkbench->getRenderingService()->createContext((this->GetHandle()->));
    #endif
#endif
    // TODO Get the size from the parent window?
    m_pRenderingView = m_pWorkbench->getRenderingService()->createView(*m_pContext);

    // TODO Do these next two steps need to be done elsewhere?

    // Create a scene service.
    m_pSceneService = Engine::Rendering::I_SceneManager::getSingleton().create("ogre");

    // Initialize the canvas.
    m_pRenderingView->initCanvas();
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
SceneViewWidget::onSize(wxSizeEvent& e)
{
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
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneViewWidget::onPaint(wxPaintEvent& e)
{
    wxPaintDC dc(this);

    updateView();
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
