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
#ifndef TERRAIN_BUILDER_TERRAIN_GROUP_DOCUMENT_HPP_INCLUDED
#define TERRAIN_BUILDER_TERRAIN_GROUP_DOCUMENT_HPP_INCLUDED

#include "TerrainGroupController.hpp"
#include "Properties.hpp"

#include <Zen/Engine/World/I_TerrainGroup.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>

#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainGroupDocument.hpp>
#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainGroupView.hpp>

#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainGroupDataCollection.hpp>
#include <Zen/StudioPlugins/TerrainBuilderModel/I_TerrainGroupDomainObject.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TerrainGroupNode;

/// Terrain Group Document
class TerrainGroupDocument
:   public I_TerrainGroupDocument
,   public I_TerrainGroupView
,   public Zen::Studio::Workbench::I_Document
,   public Zen::Studio::Workbench::I_PropertiesListener
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SceneController                       I_SceneController;
    typedef Zen::Studio::Workbench::I_SceneModelType                        I_SceneModelType;
    typedef Zen::Studio::Workbench::I_SceneEntity                           I_SceneEntity;
    typedef Zen::Studio::Workbench::I_PropertiesPublisher                   I_PropertiesPublisher;

    typedef std::set<pTerrainGroupView_type>                                TerrainGroupSubscribers_type;
    typedef std::set<pSceneView_type>                                       SceneSubscribers_type;

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

    /// @name I_SceneViewable implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pSceneView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual I_SceneController& getSceneController();
    virtual I_PropertiesPublisher& getProperties();
    virtual I_PropertiesPublisher& getEntityProperties(const I_SceneEntity& _entity);
    /// @}

    /// @name I_TerrainGroupDocument implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pTerrainGroupView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual I_TerrainGroupController& getTerrainGroupController();
    virtual pExplorerNode_type getNode();
    /// @}

    /// @name I_TerrainGroupView implementation
    /// @{
public:
    virtual void onDocumentModified(I_TerrainGroupDocument& _terrainGroupDocument);
    /// @}

    /// @name I_PropertiesListener imlementation for listening to document properties
    /// @{
public:
    virtual void onAddProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    virtual void onValueModified(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    virtual void onRemoveProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    /// @}

    /// @name TerrainGroupDocument implementation
    /// @{
public:
    /// Unsubscribe a view.
    /// This method is called automatically when the pSubscription_type is destroyed.
    void unSubscribe(pSceneView_type _pView);
    void unSubscribe(pTerrainGroupView_type _pView);

    /// Save this document.
    void save();

    boost::uint64_t getTerrainGroupId() const;

protected:
    friend class TerrainGroupNode;
    /// Load the document.
    /// Eventually this should become asynchronous.
    /// @see TerrainGroupNode::getDocument()
    void load(TerrainGroupNode& _node);

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
    friend class TerrainGroupController;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             TerrainGroupDocument(Zen::Studio::Workbench::I_Project& _project);
    virtual ~TerrainGroupDocument();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Project&                          m_project;

    /// Explorer node to which this document belongs
    pExplorerNode_type                                          m_pNode;
    pNodeEventConnection_type                                   m_pNodeConnection;

    TerrainGroupController                                      m_controller;

    /// Guard for m_viewSubscribers.
    Zen::Threading::I_Mutex*                                    m_pSubscriptionsGuard;
    TerrainGroupSubscribers_type                                m_subscribersTG;
    SceneSubscribers_type                                       m_subscribersScene;

    /// True if this document has been loaded from the database
    bool                                                        m_loaded;

    /// Subscription to the document properties
    Properties::pSubscription_type                              m_pPropertiesSubscription;

    typedef Zen::Memory::managed_ptr<Zen::Engine::World::I_TerrainGroup>    pTerrainGroup_type;
    pTerrainGroup_type                                          m_pTerrainGroup;
    /// @}

};  // class TerrainGroupDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_TERRAIN_GROUP_DOCUMENT_HPP_INCLUDED
