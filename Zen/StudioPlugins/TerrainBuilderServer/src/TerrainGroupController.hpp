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
#ifndef TERRAIN_BUILDER_TERRAIN_GROUP_CONTROLLER_HPP_INCLUDED
#define TERRAIN_BUILDER_TERRAIN_GROUP_CONTROLLER_HPP_INCLUDED

#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainGroupController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TerrainGroupDocument;

/// Terrain Group Document Controller.
class TerrainGroupController
:   public I_TerrainGroupController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_TerrainGroupController implementation
    /// @{
public:
    /// @}

    /// @name TerrainGroupController implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class TerrainGroupDocument;
             TerrainGroupController(TerrainGroupDocument& _document);
    virtual ~TerrainGroupController();
    /// @}

    /// @name Member Variables
    /// @{
private:
    TerrainGroupDocument&           m_document;
    /// @}

};  // class TerrainGroupController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_TERRAIN_GROUP_CONTROLLER_HPP_INCLUDED
