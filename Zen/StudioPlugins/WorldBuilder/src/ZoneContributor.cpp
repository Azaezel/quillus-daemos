//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "ZoneContributor.hpp"
#include "ContributorService.hpp"
#include "Creator.hpp"
#include "ZoneLayerView.hpp"

#include <Zen/Studio/Workbench/I_Workbench.hpp>
#include <Zen/Studio/Workbench/I_StudioTopFrame.hpp>
#include <Zen/Studio/Workbench/I_Notebook.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SceneView.hpp>

#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <Zen/StudioPlugins/WorldBuilderCommon/I_Zone.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_Project.hpp>

#include <Zen/plugins/ZOgre/I_OgreView.hpp>

#include <Ogitors.h>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZoneContributor::ZoneContributor(ContributorService& _service)
:   m_service(_service)
,   m_open("Open in View", "open", boost::bind(&ZoneContributor::open, this, _1))
,   m_setActiveZone("Set as Active Zone", "open", boost::bind(&ZoneContributor::setActiveZone, this, _1))
,   m_pCurrentCreator(NULL)
,   m_pZoneLayerView(NULL)
,   m_initialized(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZoneContributor::~ZoneContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::enableActions()
{
    // First time initialization.
    if (!m_initialized)
    {
        // Connect to the main workspace notebook onActivate event.
        m_service.getWorkbench().getMainWindow().getWorkspaceNotebook().
            onActivate.connect(boost::bind(&ZoneContributor::activate, this, _1));

        m_initialized = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor)
{
    assert(m_initialized);

    _visitor.begin();

    _visitor.visit(m_open);
    _visitor.visit(m_setActiveZone);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor)
{
    assert(m_initialized);

    _visitor.begin();

    // TODO Implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::getActions(Zen::Studio::Workbench::I_SceneModel& _model, Zen::Engine::Rendering::I_SceneNode* _pSceneNode, I_SceneActionVisitor& _visitor)
{
    assert(m_initialized);

    _visitor.begin();

    // TODO Implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::open(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    assert(m_initialized);

    I_Zone* pZone = dynamic_cast<I_Zone*>(_selectedNode.getUserData().get());

    if (pZone)
    {
        // Get or Create the view.
        Zen::Studio::Workbench::I_View& view = m_service.getWorkbench().getView("ogrescene", pZone->getDocument(), false);

        // Initialize Ogitor
        initializeOgitor(view);

        // Activate the view.
        m_service.getWorkbench().getMainWindow().getWorkspaceNotebook().showPage(view);

        // Show the ZoneLayerView if it's not already being shown.
        createZoneLayerView();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::setActiveZone(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    assert(m_initialized);

    I_Zone* pZone = dynamic_cast<I_Zone*>(_selectedNode.getUserData().get());

    if (pZone)
    {
        createZoneLayerView();
        m_pZoneLayerView->setActiveZone(pZone);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::activate(Zen::Studio::Workbench::I_Notebook::ViewEvent& _event)
{
    assert(m_initialized);

    // Get a reference to the explorerNotebook; we'll need it once or twice.
    Zen::Studio::Workbench::I_Notebook&
        explorerNotebook = m_service.getWorkbench().getMainWindow().getExplorerNotebook();

    bool activating = true;

    // Rule out some reasons why we wouldn't activate a Creator view.
    if (_event.m_pActivating == NULL)
    {
        // Make sure the activating view is not NULL
        activating = false;
    }
    else if (_event.m_pActivating->getProject() == NULL)
    {
        // Make sure the view is associated with a project
        activating = false;
    }
    else
    {
        // Make sure the view is a I_SceneView
        Zen::Studio::Workbench::I_SceneView*
            pView = dynamic_cast<Zen::Studio::Workbench::I_SceneView*>(_event.m_pActivating);

        if (pView == NULL)
        {
            activating = false;
        }
    }

    // If not activating, early out here
    if (!activating)
    {
        // If a view is deactivating and we're not activating a new view,
        // then hide the currently shown Creator window, if there is one.
        if (m_pCurrentCreator != NULL)
        {
            explorerNotebook.hidePage(m_pCurrentCreator);
        }

        m_pCurrentCreator = NULL;

        // And we're done.
        return;
    }

    // TODO Guard the view collection
    Views_type::iterator iter = m_views.find(_event.m_pActivating);

    // The Creator that needs to be activated
    Creator* pActivatingCreator = NULL;

    // Find the Creator using the I_View* as a key.
    if (iter == m_views.end())
    {
        // Make sure the I_View is associated with a GameBuilder Project.
        GameBuilder::I_Project* const pProject = dynamic_cast<GameBuilder::I_Project*>(_event.m_pActivating->getProject());
        if (pProject != NULL)
        {
            // Create a creator window.
            pActivatingCreator = new Creator(*pProject, explorerNotebook.getWindow());

            // TODO Create the page based on the project name.
            explorerNotebook.addPage(pActivatingCreator, "Creator");

            m_views[_event.m_pActivating] = pActivatingCreator;
        }
    }
    else
    {
        // Found the creator, lets activate it.
        pActivatingCreator = iter->second;
    }

    // If a creator needs to be activated, activate it.
    if (pActivatingCreator != NULL && m_pCurrentCreator != pActivatingCreator)
    {
        // If a creator is already shown, hide it.
        if (m_pCurrentCreator != NULL)
        {
            explorerNotebook.hidePage(m_pCurrentCreator);
        }

        // Show the new creator
        explorerNotebook.showPage(pActivatingCreator);
        m_pCurrentCreator = pActivatingCreator;
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::createZoneLayerView()
{
    // TODO Guard?
    if (m_pZoneLayerView == NULL)
    {
        Zen::Studio::Workbench::I_Notebook&
            toolNotebook = m_service.getWorkbench().getMainWindow().getToolNotebook();

        m_pZoneLayerView = new ZoneLayerView(toolNotebook.getWindow());

        toolNotebook.addPage(m_pZoneLayerView, "Layers");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneContributor::initializeOgitor(Zen::Studio::Workbench::I_View& _view)
{
    std::cout << "initializeOgitor" << std::endl;
    Ogitors::OgitorsRoot::getSingletonPtr()->SetRenderWindow(&dynamic_cast<Zen::ZOgre::I_OgreView*>(&_view)->getRenderWindow());
    std::cout << "initializeOgitor done" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
