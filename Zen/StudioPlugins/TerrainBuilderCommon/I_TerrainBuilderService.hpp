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
#ifndef TERRAINBUILDER_I_TERRAIN_BUILDER_SERVICE_HPP_INCLUDED
#define TERRAINBUILDER_I_TERRAIN_BUILDER_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ProjectService;

class TERRAINBUILDERCOMMON_DLL_LINK I_TerrainBuilderService
:   public Zen::Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_ExplorerNode::pUserData_type  pUserData_type;
    /// @}

    /// @name I_TerrainBuilderService interface
    /// @{
public:
    /// Get the project service associated with this Terrain Builder Service.
    virtual I_ProjectService& getProjectService() = 0;

    /// Create a folder.
    /// @param _pParent - Parent object. Currently only projects are supported
    ///     as a parent.
    virtual pUserData_type createFolder(pUserData_type _pParent) = 0;

    /// Create a TerrainGroup
    virtual pUserData_type createTerrainGroup(pUserData_type _pParent) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainBuilderService();
    virtual ~I_TerrainBuilderService();
    /// @}

};  // interface I_TerrainBuilderService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_I_TERRAIN_BUILDER_SERVICE_HPP_INCLUDED
