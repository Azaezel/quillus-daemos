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
#ifndef GAME_BUILDER_GAME_OBJECT_TYPE_DOCUMENT_HPP_INCLUDED
#define GAME_BUILDER_GAME_OBJECT_TYPE_DOCUMENT_HPP_INCLUDED

#include "GameObjectTypeController.hpp"
#include "DocumentProperties.hpp"

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeDocument.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeView.hpp>

#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectElementDataCollection.hpp>
#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectElementDomainObject.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameObjectElement;
class GameObjectType;

/// Game Object Type Document.
class GameObjectTypeDocument
:   public I_GameObjectTypeDocument
,   public I_GameObjectTypeView
,   public Zen::Studio::Workbench::I_Document
,   public Zen::Studio::Workbench::I_PropertiesListener
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SpreadSheetController     I_SpreadSheetController;
    typedef Zen::Studio::Workbench::I_SpreadSheetDocumentType   I_SpreadSheetDocumentType;
    typedef Zen::Studio::Workbench::I_SpreadSheetCell           I_SpreadSheetCell;
    typedef Zen::Studio::Workbench::I_SpreadSheetProperties     I_SpreadSheetProperties;
    typedef Zen::Studio::Workbench::I_SpreadSheetCellProperties I_SpreadSheetCellProperties;

    typedef std::set<pGameObjectTypeView_type>                  GameObjectTypeSubscribers_type;
    typedef std::set<pSpreadSheetView_type>                     SpreadSheetSubscribers_type;

    typedef Zen::Studio::Workbench::I_ExplorerNode              I_ExplorerNode;
    typedef I_ExplorerNode::ExplorerNodeEvent_type::pConnection_type    pNodeEventConnection_type;

    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type  pDatabaseConnection_type;
    /// @}

    /// @name I_Document interface
    /// @{
public:
    virtual const std::string& getShortName() const;
    virtual const std::string& getLongName() const;
    virtual Zen::Studio::Workbench::I_Project* getProject() const;
    /// @}

    /// @name I_SpreadSheetViewable implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual I_SpreadSheetController& getSpreadSheetController();
    virtual I_SpreadSheetProperties& getProperties();
    virtual I_SpreadSheetCellProperties& getCellProperties(const int _column, const int _row);
    /// @}

    /// @name I_GameObjectTypeDocument implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pGameObjectTypeView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual I_GameObjectTypeController& getGameObjectTypeController();
    virtual pExplorerNode_type getNode();
    /// @}

    /// @name I_GameObjectTypeView implementation
    /// @{
public:
    virtual void onDocumentModified(I_GameObjectTypeDocument& _gameObjectTypeDocument);
    virtual void onNewElement(I_GameObjectElement& _element, int _position);
    virtual void onElementRemoved(int _position);
    virtual void onElementModified(I_GameObjectElement& _element, int _row);
    /// @}

    /// @name I_PropertiesListener imlementation for listening to document properties
    /// @{
public:
    virtual void onAddProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    virtual void onValueModified(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    virtual void onRemoveProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property);
    /// @}

    /// @name GameObjectTypeDocument implementation
    /// @{
public:
    /// Unsubscribe a view.
    /// This method is called automatically when the pSubscription_type is destroyed.
    void unSubscribe(pSpreadSheetView_type _pView);
    void unSubscribe(pGameObjectTypeView_type _pView);

    /// Notify a spreadsheet view of the elements in this document.
    /// This is called by a child document.
    /// @return number elements passed to the view.
    int notifyView(pSpreadSheetView_type _pView);

    /// Notify a game object type view of the elements in this document.
    /// This is called by a child document.
    /// @return number elements passed to the view.
    int notifyView(pGameObjectTypeView_type _pView);

    inline
    GameObjectTypeDocument* getParent()
    { return dynamic_cast<GameObjectTypeDocument*>(m_ppParent->get()); }

    /// Add an element to this document.
    /// This is called by GameObjectTypeDocument::load() when
    /// loading elements for this document from the database.
    void addElement(pDatabaseConnection_type _pDbConn, I_GameObjectElementDataCollection::pGameObjectElementDomainObject_type _pDomainObject);

    /// Save this document.
    void save();

    boost::uint64_t getGameObjectTypeId() const;
protected:
    friend class GameObjectType;
    /// Load the document.
    /// Eventually this should become asynchronous.
    /// @see GameObjectType::getDocument()
    void load(GameObjectType& _node);

    /// Notify all of the GOT views that this document has been modified.
    void notifyViewsOfModification();
    /// @}

    /// @name Event handlers
    /// @{
public:
    /// Invoked when the explorer node for this document has been modified.
    /// Generally this modification is the name being modified.
    void handleExplorerNodeModified(I_ExplorerNode* _pNode);
    /// @}

    /// @name Controller methods.
    /// @{
protected:
    friend class GameObjectTypeController;
    void insertElement(int _position, const std::string& _elementName);

    /// Get the element at the given row.
    /// @param _row The row of the element, but this row includes the parent rows.
    GameObjectElement& getElement(int _row);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             GameObjectTypeDocument(Zen::Studio::Workbench::I_Project& _project, Zen::Studio::Workbench::I_Document::pDocument_type* _ppParent);
    virtual ~GameObjectTypeDocument();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Project&                  m_project;
    /// Pointer to a managed pointer to the parent document.
    /// This document doesn't have any parents if this pointer is NULL.
    Zen::Studio::Workbench::I_Document::pDocument_type* m_ppParent;
    pSubscription_type                                  m_pParentSubscription;

    /// Explorer node to which this document belongs.
    pExplorerNode_type                                  m_pNode;
    pNodeEventConnection_type                           m_pNodeConnection;

    GameObjectTypeController                            m_controller;

    /// Guard for m_childSubscribers and m_viewSubscribers.
    /// @note Should this be a shared mutex or two separate one's?  For now I
    ///     am keeping it shared so as to reduce the risk of deadlocks.
    Zen::Threading::I_Mutex*                            m_pSubscriptionsGuard;
    GameObjectTypeSubscribers_type                      m_subscribersGOT;
    SpreadSheetSubscribers_type                         m_subscribersSS;

    /// Ordered elements.
    typedef std::vector<GameObjectElement*>             Elements_type;
    int                                                 m_parentElementCount;
    Elements_type                                       m_parentElements;
    Elements_type                                       m_elements;

    /// true if this document has been loaded from the database
    bool                                                m_loaded;

    /// Subscription to the document properties
    DocumentProperties::pSubscription_type              m_pDocumentPropertySubscription;
    /// @}

};  // class GameObjectTypeDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_GAME_OBJECT_TYPE_DOCUMENT_HPP_INCLUDED
