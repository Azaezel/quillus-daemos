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
#ifndef WORLDBUILDER_I_WORLDBUILDER_SERVICE_HPP_INCLUDED
#define WORLDBUILDER_I_WORLDBUILDER_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ProjectService;

class WORLDBUILDERCOMMON_DLL_LINK I_WorldBuilderService
:   public Zen::Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_ExplorerNode::pUserData_type  pUserData_type;
    /// @}

    /// @name I_WorldBuilderService interface
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
             I_WorldBuilderService();
    virtual ~I_WorldBuilderService();
    /// @}

};  // interface I_WorldBuilderService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_I_WORLDBUILDER_SERVICE_HPP_INCLUDED
