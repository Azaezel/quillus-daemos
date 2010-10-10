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
#ifndef TERRAIN_BUILDER_TERRAIN_CHUNK_DOCUMENT_HPP_INCLUDED
#define TERRAIN_BUILDER_TERRAIN_CHUNK_DOCUMENT_HPP_INCLUDED

#include "TerrainChunkController.hpp"
#include "Properties.hpp"

#include <Zen/Engine/World/I_TerrainChunk.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>

#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainChunkDocument.hpp>
#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainChunkView.hpp>

#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainChunkDataCollection.hpp>
#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainChunkDomainObject.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TerrainChunkNode;

/// Terrain Group Document
class TerrainChunkDocument
:   public I_TerrainChunkDocument
,   public I_TerrainChunkView
,   public Zen::Studio::Workbench::I_Document
,   public Zen::Studio::Workbench::I_PropertiesListener
{
    /// @name Types
    /// @{
public:
    typedef std::set<pTerrainChunkView_type>                                TerrainChunkSubscribers_type;

    typedef Zen::Studio::Workbench::I_ExplorerNode                          I_ExplorerNode;
    typedef I_ExplorerNode::ExplorerNodeEvent_type::pConnection_type        pNodeEventConnection_type;

    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type      pDatabaseConnection_type;
    /// @}

    /// @name I_Document implementation
    /// @{
public:
    virtual const std::string& getShortName() const;
    virtual const std::string& getLongName() const;
    virtual Zen::Studio::Workbench::I_Project* getProject() const;
    /// @}

    /// @name I_TerrainChunkDocument implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pTerrainChunkView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual I_TerrainChunkController& getTerrainChunkController();
    virtual pExplorerNode_type getNode();
    /// @}

    /// @name I_TerrainChunkView implementation
    /// @{
public:
    virtual void onDocumentModified(I_TerrainChunkDocument& _terrainGroupDocument);
    /// @}

    /// @name I_PropertiesListener imlementation for listening to document properties
    /// @{
public:
    virtual void onAddProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    virtual void onValueModified(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    virtual void onRemoveProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    /// @}

    /// @name TerrainChunkDocument implementation
    /// @{
public:
    /// Unsubscribe a view.
    /// This method is called automatically when the pSubscription_type is destroyed.
    void unSubscribe(pTerrainChunkView_type _pView);

    /// Save this document.
    void save();

    boost::uint64_t getTerrainChunkId() const;

protected:
    friend class TerrainChunkNode;
    /// Load the document.
    /// Eventually this should become asynchronous.
    /// @see TerrainChunkNode::getDocument()
    void load(TerrainChunkNode& _node);

    /// Notify all the terrain group views that this document has been modified.
    void notifyViewsOfModification();
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Invoked when the explorer node for this document has been modified.
    /// Generally this modification is the name being modified.
    void handleExplorerNodeModified(I_ExplorerNode* _pNode);
    /// @}

    /// @name Controller Methods.
    /// @{
protected:
    friend class TerrainChunkController;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             TerrainChunkDocument(Zen::Studio::Workbench::I_Project& _project);
    virtual ~TerrainChunkDocument();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Project&                          m_project;

    /// Explorer node to which this document belongs
    pExplorerNode_type                                          m_pNode;
    pNodeEventConnection_type                                   m_pNodeConnection;

    TerrainChunkController                                      m_controller;

    /// Guard for m_viewSubscribers.
    Zen::Threading::I_Mutex*                                    m_pSubscriptionsGuard;
    TerrainChunkSubscribers_type                                m_subscribers;

    /// True if this document has been loaded from the database
    bool                                                        m_loaded;

    /// Subscription to the document properties
    Properties::pSubscription_type                              m_pPropertiesSubscription;

    typedef Zen::Memory::managed_ptr<Zen::Engine::World::I_TerrainChunk>    pTerrainChunk_type;
    pTerrainChunk_type                                          m_pTerrainChunk;
    /// @}

};  // class TerrainChunkDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_TERRAIN_CHUNK_DOCUMENT_HPP_INCLUDED
