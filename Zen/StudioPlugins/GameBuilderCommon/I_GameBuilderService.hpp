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
#ifndef GAMEBUILDER_I_GAME_BUILDER_SERVICE_HPP_INCLUDED
#define GAMEBUILDER_I_GAME_BUILDER_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ProjectService;

class GAMEBUILDERCOMMON_DLL_LINK I_GameBuilderService
:   public Zen::Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_ExplorerNode::pUserData_type  pUserData_type;
    /// @}

    /// @name I_GameBuilderService interface
    /// @{
public:
    /// Get the project service associated with this Game Builder Service.
    virtual I_ProjectService& getProjectService() = 0;

    /// Create a folder.
    /// @param _pParent - Parent object.  Currently only projects are supported
    ///         as a parent.
    virtual pUserData_type createFolder(pUserData_type _pParent) = 0;

    /// Create a GameObjectType.
    /// @param _pParent - Parent object.
    virtual pUserData_type createGameObjectType(pUserData_type _pParent) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameBuilderService();
    virtual ~I_GameBuilderService();
    /// @}

};  // interface I_GameBuilderService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMEBUILDER_I_GAME_BUILDER_SERVICE_HPP_INCLUDED
