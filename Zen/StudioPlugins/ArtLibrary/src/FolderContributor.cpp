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

#include "FolderContributor.hpp"
#include "ContributorService.hpp"

#include <Zen/Studio/Workbench/I_Workbench.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

//#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

#include <Zen/StudioPlugins/ArtLibraryCommon/I_Folder.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FolderContributor::FolderContributor(ContributorService& _service)
:   m_service(_service)
,   m_newFolder("New Folder", "new", boost::bind(&FolderContributor::newFolder, this, _1))
,   m_newArtAsset("New Asset", "new", boost::bind(&FolderContributor::newArtAsset, this, _1))
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FolderContributor::~FolderContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
FolderContributor::getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor)
{
    _visitor.begin();

    _visitor.visit(m_newFolder);
    _visitor.visit(m_newArtAsset);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
FolderContributor::getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor)
{
    _visitor.begin();

    // TODO Implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
FolderContributor::newFolder(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    I_Folder* pFolder = dynamic_cast<I_Folder*>(_selectedNode.getUserData().get());

    // Must be an I_ArtFolder user data in order to add a sub folder.
    if (pFolder)
    {
        pFolder->newFolder(_selectedNode);
    }
    else
    {
        // Wrong type, ignore it or throw an exception or what?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
FolderContributor::newArtAsset(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    I_Folder* pFolder = dynamic_cast<I_Folder*>(_selectedNode.getUserData().get());

    // Must be an I_ArtFolder user data in order to add an art asset.
    if (pFolder)
    {
        pFolder->newArtAsset(_selectedNode);
    }
    else
    {
        // Wrong type, ignore it or throw an exception or what?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
