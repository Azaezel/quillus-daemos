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
#include "ContributorService.hpp"

#include <Zen/StudioPlugins/GameBuilderCommon/I_Project.hpp>

#include <Zen/StudioPlugins/ArtLibraryCommon/I_Folder.hpp>
#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetExplorerNode.hpp>
#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetDocument.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ContributorService::ContributorService(Zen::Studio::Workbench::I_Workbench& _workbench)
:   m_workbench(_workbench)
,   m_projectContributor(*this)
,   m_folderContributor(*this)
,   m_artAssetContributor(*this)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ContributorService::~ContributorService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ContributorService::enableActions()
{
    // Project contributor
    GameBuilder::I_Project::getNodeType().addContributor(m_projectContributor);

    // Folder contributor
    I_Folder::getNodeType().addContributor(m_folderContributor);

    // Art Asset contributor
    I_ArtAssetExplorerNode::getNodeType().addContributor(m_artAssetContributor);

    // Art Asset document contributors
    I_ArtAssetDocument::onRightClickArtAssetLabel.connect(
        boost::bind(
            &ArtAssetContributor::getActions,
            m_artAssetContributor, 
            _1
        )
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Workbench&
ContributorService::getWorkbench() const
{
    return m_workbench;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

