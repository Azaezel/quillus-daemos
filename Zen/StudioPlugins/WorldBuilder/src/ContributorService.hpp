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
#ifndef WORLDBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED
#define WORLDBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED

#include <Zen/Studio/Workbench/I_ContributorService.hpp>

#include "ZoneFolderContributor.hpp"
#include "ZoneContributor.hpp"
#include "ProjectContributor.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
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
    ZoneFolderContributor                   m_zoneFolderContributor;
    ZoneContributor                         m_zoneContributor;
    ProjectContributor                      m_projectContributor;
    /// @}

};  // class ContributorService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED

