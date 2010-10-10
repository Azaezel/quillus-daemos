//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef TERRAINBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED
#define TERRAINBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED

#include <Zen/Studio/Workbench/I_ContributorService.hpp>

#include "FolderContributor.hpp"
#include "TerrainGroupNodeContributor.hpp"
#include "TerrainChunkNodeContributor.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ContributorService
:   public Zen::Studio::Workbench::I_ContributorService
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ContributorService implementation
    /// @{
public:
    virtual void enableActions();
    /// @}

    /// @name ContributorService implementation
    /// @{
public:
    Zen::Studio::Workbench::I_Workbench& getWorkbench() const;
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ContributorServiceFactory;
             ContributorService(Zen::Studio::Workbench::I_Workbench& _workbench);
    virtual ~ContributorService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Workbench&    m_workbench;
    FolderContributor                       m_folderContributor;
    TerrainGroupNodeContributor             m_terrainGroupNodeContributor;
    TerrainChunkNodeContributor             m_terrainChunkNodeContributor;
    /// @}

};  // class ContributorService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED
