//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
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

#include "ProjectContributor.hpp"
#include "ContributorService.hpp"

#include <Zen/Studio/Workbench/I_Workbench.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_Project.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectContributor::ProjectContributor(ContributorService& _service)
:   m_service(_service)
,   m_addArtLibraryAspect("Add Art Library Aspect", "aspect", boost::bind(&ProjectContributor::addArtLibraryAspect, this, _1))
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectContributor::~ProjectContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectContributor::getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor)
{
    _visitor.begin();

    // TODO Make sure the project doesn't already have this aspect.
    // If it does, maybe have a "remove aspect" action?
    _visitor.visit(m_addArtLibraryAspect);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectContributor::getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor)
{
    _visitor.begin();

    // TODO Implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectContributor::addArtLibraryAspect(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    GameBuilder::I_Project* pProject = dynamic_cast<GameBuilder::I_Project*>(_selectedNode.getUserData().get());

    if (pProject)
    {
        // TODO Make sure the project doesn't already have this aspect.
        // TODO What else to do to add an aspect to a project?

        Zen::Studio::Workbench::I_ExplorerNode::pUserData_type 
            pData = m_service.getWorkbench().getWorkbenchService().
            createNodeUserData(0, "ArtLibrary::ArtFolder", _selectedNode);

        pProject->getController().createChildNode(*pProject->getNode(), pData);
    }
    else
    {
        // Wrong project type, ignore it or throw an exception or what?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
