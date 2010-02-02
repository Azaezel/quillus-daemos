//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
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
#ifndef GAMEBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED
#define GAMEBUILDER_CONTRIBUTOR_SERVICE_HPP_INCLUDED

#include <Zen/Studio/Workbench/I_ContributorService.hpp>

#include "FolderContributor.hpp"
#include "GameObjectTypeContributor.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
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
    FolderContributor                       m_folderContributor;
    GameObjectTypeContributor               m_gameObjectTypeContributor;
    /// @}

};  // class ContributorService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMEBUILDER_EXPLORER_NODE_ACTION_SERVICE_HPP_INCLUDED

