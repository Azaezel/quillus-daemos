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
#ifndef ARTLIBRARY_CONTRIBUTOR_SERVICE_HPP_INCLUDED
#define ARTLIBRARY_CONTRIBUTOR_SERVICE_HPP_INCLUDED

#include <Zen/Studio/Workbench/I_ContributorService.hpp>

#include "ProjectContributor.hpp"
#include "FolderContributor.hpp"
#include "ArtAssetContributor.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class ContributorService
:   public Zen::Studio::Workbench::I_ContributorService
{
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
    ProjectContributor                      m_projectContributor;
    FolderContributor                       m_folderContributor;
    ArtAssetContributor                     m_artAssetContributor;
    /// @}

};  // class ContributorService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_CONTRIBUTOR_SERVICE_HPP_INCLUDED

