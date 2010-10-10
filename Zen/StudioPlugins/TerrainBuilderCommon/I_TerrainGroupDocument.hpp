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
#ifndef TERRAIN_BUILDER_I_TERRAIN_GROUP_DOCUMENT_HPP_INCLUDED
#define TERRAIN_BUILDER_I_TERRAIN_GROUP_DOCUMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>
#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SceneViewable.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SceneModelType.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_TerrainGroupView;
class I_TerrainGroupController;
class I_TerrainGroupNode;

/// Terrain Group Document.
/// This document can be viewed by a scene view, but it is not a scene 
/// document. It can also be viewed using a I_TerrainGroupView.
class TERRAINBUILDERCOMMON_DLL_LINK I_TerrainGroupDocument
:   public Zen::Studio::Workbench::I_SceneViewable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SceneView*                                pSceneView_type;
    typedef I_TerrainGroupView*                                                 pTerrainGroupView_type;

    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Filter>        pFilter_type;
    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Subscription>  pSubscription_type;

    typedef I_SceneViewable::SceneViewEvent_type                                SceneViewEvent_type;
    typedef I_TerrainGroupNode*                                                 pExplorerNode_type;
    /// @}

    /// @name I_TerrainGroupDocument interface
    /// @{
public:
    /// Subscribe using a I_SceneView.
    /// The subscription should be stored in the listener so that when
    /// the listener goes out of scope, the subscription also goes out
    /// of scope. The listener should also retain a strong reference
    /// to the document.
    /// @param _pListener - Unmanaged pointer to the listener. The listener
    ///     MUST remain alive for at least as long as the document (which it
    ///     will if it keeps a strong reference to the document).
    ///     The listener MUST dereference the subscription before it is
    ///     destroyed.
    virtual pSubscription_type subscribe(pSceneView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Subscribe using a I_TerrainGroupView.
    /// The subscription should be stored in the listener so that when
    /// the listener goes out of scope, the subscription also goes out
    /// of scope. The listener should also retain a strong reference
    /// to the document.
    /// @param _pListener - Unmanaged pointer to the listener. The listener
    ///     MUST remain alive for at least as long as the document (which it
    ///     will if it keeps a strong reference to the document).
    ///     The listener MUST dereference the subscription before it is
    ///     destroyed.
    virtual pSubscription_type subscribe(pTerrainGroupView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    virtual I_TerrainGroupController& getTerrainGroupController() = 0;

    /// Get the node associated with this document.
    /// This node is a I_TerrainGroupNode.
    virtual pExplorerNode_type getNode() = 0;
    /// @}

    /// @name Static events
    /// @{
public:
    static SceneViewEvent_type      onLeftClickEntity;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainGroupDocument();
    virtual ~I_TerrainGroupDocument();
    /// @}

};  // interface I_TerrainGroupDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_I_TERRAIN_GROUP_DOCUMENT_HPP_INCLUDED
