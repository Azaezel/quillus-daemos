//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ARTLIBRARY_ART_ASSET_DOCUMENT_HPP_INCLUDED
#define ARTLIBRARY_ART_ASSET_DOCUMENT_HPP_INCLUDED

#include "ArtAssetController.hpp"

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetDocument.hpp>
#include <Zen/StudioPlugins/ArtLibraryCommon/I_ArtAssetView.hpp>

#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtAssetDocumentDataMap.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ArtAsset;

/// @see ArtAssetDocument
class ArtAssetDocument
:   public I_ArtAssetDocument
,   public I_ArtAssetView
,   public Zen::Studio::Workbench::I_Document
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SpreadSheetController             I_SpreadSheetController;
    typedef Zen::Studio::Workbench::I_SpreadSheetDocumentType           I_SpreadSheetDocumentType;
    typedef Zen::Studio::Workbench::I_SpreadSheetCell                   I_SpreadSheetCell;
    typedef Zen::Studio::Workbench::I_SpreadSheetProperties             I_SpreadSheetProperties;
    typedef Zen::Studio::Workbench::I_SpreadSheetCellProperties         I_SpreadSheetCellProperties;

    typedef std::set<pArtAssetView_type>                                ArtAssetSubscribers_type;
    typedef std::set<pSpreadSheetView_type>                             SpreadSheetSubscribers_type;

    typedef Zen::Studio::Workbench::I_ExplorerNode                      I_ExplorerNode;
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

    /// @name I_ArtAssetDocument implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pArtAssetView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual I_ArtAssetController& getArtAssetController();
    virtual pExplorerNode_type getNode();
    /// @}

    /// @name I_ArtAssetView implementation
    /// @{
public:
    /// @}

    /// @name ArtAssetDocument implementation
    /// @{
public:
    /// Unsubscribe a view.
    /// This method is called automatically when the pSubscription_type is destroyed.
    void unSubscribe(pSpreadSheetView_type _pView);
    void unSubscribe(pArtAssetView_type _pView);

    inline
    ArtAssetDocument* getParent()
    { return dynamic_cast<ArtAssetDocument*>(m_ppParent->get()); }

    /// Save this document.
    void save();

    boost::uint64_t getArtAssetId() const;
protected:
    friend class ArtAssetExplorerNode;
    /// Load the document.
    /// Eventually this should become asynchronous.
    /// @see ArtAsset::getDocument()
    void load(ArtAssetExplorerNode& _node);
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
    friend class ArtAssetController;
    /// @}

    /// @name 'Structors
    /// @{
public:
             ArtAssetDocument(Zen::Studio::Workbench::I_Project& _project, Zen::Studio::Workbench::I_Document::pDocument_type* _ppParent);
    virtual ~ArtAssetDocument();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Project&                  m_project;
    /// Pointer to a managed pointer to the parent document.
    /// This document doesn't have any parents if this pointer is NULL.
    Zen::Studio::Workbench::I_Document::pDocument_type* m_ppParent;
    pSubscription_type                                  m_pParentSubscription;

    pExplorerNode_type                                  m_pNode;
    pNodeEventConnection_type                           m_pNodeConnection;

    ArtAssetController                                  m_controller;

    /// Guard for m_childSubscribers and m_viewSubscribers
    /// @note Should this be a shared mutex or two separate ones? For now I
    ///       am keeping it shared so as to reduce the risk of deadlocks.
    Zen::Threading::I_Mutex*                            m_pSubscriptionsGuard;
    ArtAssetSubscribers_type                            m_subscribersAA;
    SpreadSheetSubscribers_type                         m_subscribersSS;

    bool                                                m_loaded;
    /// @}

};  // class ArtAssetDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ARTLIBRARY_ART_ASSET_DOCUMENT_HPP_INCLUDED
