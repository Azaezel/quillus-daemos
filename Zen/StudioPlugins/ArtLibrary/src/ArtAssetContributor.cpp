//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "ArtAssetContributor.hpp"
#include "ContributorService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_Workbench.hpp>
#include <Zen/Studio/Workbench/I_ProjectExplorerWidget.hpp>

#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>

#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetExplorerNode.hpp>
#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetDocument.hpp>
#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetContributor::ArtAssetContributor(ContributorService& _service)
:   m_service(_service)
,   m_open("Edit", "open", boost::bind(&ArtAssetContributor::open, this, _1))
,   m_rename("Rename", "rename", boost::bind(&ArtAssetContributor::rename, this, _1))
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ArtAssetContributor::~ArtAssetContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetContributor::getExplorerNodeActions(I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor)
{
    _visitor.begin();

    _visitor.visit(m_open);
    _visitor.visit(m_rename);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetContributor::getExplorerNodeDecorations(I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor)
{
    _visitor.begin();

    // TODO Implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetContributor::getActions(Zen::Studio::Workbench::I_SpreadSheetViewable::I_SpreadSheetViewEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetContributor::open(I_ExplorerNode& _selectedNode)
{
    I_ArtAssetExplorerNode* pArtAssetNode = dynamic_cast<I_ArtAssetExplorerNode*>(_selectedNode.getUserData().get());

    if (pArtAssetNode)
    {
        // Create / get the view for this document.
        m_service.getWorkbench().getView("spreadsheet", pArtAssetNode->getDocument(), false);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ArtAssetContributor::rename(I_ExplorerNode& _selectedNode)
{
    I_ArtAssetExplorerNode* pArtAssetNode = dynamic_cast<I_ArtAssetExplorerNode*>(_selectedNode.getUserData().get());

    if (pArtAssetNode)
    {
        m_service.getWorkbench().getProjectExplorerWidget().beginLabelEdit(_selectedNode);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
