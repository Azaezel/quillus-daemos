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
#ifndef TERRAIN_BUILDER_I_TERRAIN_CHUNK_DOCUMENT_HPP_INCLUDED
#define TERRAIN_BUILDER_I_TERRAIN_CHUNK_DOCUMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>
#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_TerrainChunkView;
class I_TerrainChunkController;
class I_TerrainChunkNode;

/// Terrain Chunk Document.
class TERRAINBUILDERCOMMON_DLL_LINK I_TerrainChunkDocument
{
    /// @name Types
    /// @{
public:
    typedef I_TerrainChunkView*                                                 pTerrainChunkView_type;

    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Filter>        pFilter_type;
    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Subscription>  pSubscription_type;

    typedef I_TerrainChunkNode*                                                 pExplorerNode_type;
    /// @}

    /// @name I_TerrainChunkDocument interface
    /// @{
public:
    /// Subscribe using a I_TerrainChunkView.
    /// The subscription should be stored in the listener so that when
    /// the listener goes out of scope, the subscription also goes out
    /// of scope. The listener should also retain a strong reference
    /// to the document.
    /// @param _pListener - Unmanaged pointer to the listener. The listener
    ///     MUST remain alive for at least as long as the document (which it
    ///     will if it keeps a strong reference to the document).
    ///     The listener MUST dereference the subscription before it is
    ///     destroyed.
    virtual pSubscription_type subscribe(pTerrainChunkView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    virtual I_TerrainChunkController& getTerrainChunkController() = 0;

    /// Get the node associated with this document.
    /// This node is a I_TerrainChunkNode.
    virtual pExplorerNode_type getNode() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainChunkDocument();
    virtual ~I_TerrainChunkDocument();
    /// @}

};  // interface I_TerrainChunkDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_I_TERRAIN_CHUNK_DOCUMENT_HPP_INCLUDED
