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
#ifndef TERRAIN_BUILDER_TERRAIN_CHUNK_CONTROLLER_HPP_INCLUDED
#define TERRAIN_BUILDER_TERRAIN_CHUNK_CONTROLLER_HPP_INCLUDED

#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainChunkController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TerrainChunkDocument;

/// Terrain Group Document Controller.
class TerrainChunkController
:   public I_TerrainChunkController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_TerrainChunkController implementation
    /// @{
public:
    /// @}

    /// @name TerrainChunkController implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class TerrainChunkDocument;
             TerrainChunkController(TerrainChunkDocument& _document);
    virtual ~TerrainChunkController();
    /// @}

    /// @name Member Variables
    /// @{
private:
    TerrainChunkDocument&           m_document;
    /// @}

};  // class TerrainChunkController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_TERRAIN_CHUNK_CONTROLLER_HPP_INCLUDED
