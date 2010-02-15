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
#ifndef ARTLIBRARY_I_ART_ASSET_DOCUMENT_HPP_INCLUDED
#define ARTLIBRARY_I_ART_ASSET_DOCUMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetViewable.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocumentType.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ArtAssetView;
class I_ArtAssetController;
class I_ArtAssetExplorerNode;

/// Art Asset Document.
/// This document can be viewed by a spread sheet view, but is not a spread sheet
/// document.  It can also be viewed using a I_ArtAssetView.
class ARTLIBRARYCOMMON_DLL_LINK I_ArtAssetDocument
:   public Zen::Studio::Workbench::I_SpreadSheetViewable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SpreadSheetView*                          pSpreadSheetView_type;
    typedef I_ArtAssetView*                                                     pArtAssetView_type;

    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Filter>        pFilter_type;
    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Subscription>  pSubscription_type;

    typedef I_SpreadSheetViewable::SpreadSheetViewEvent_type                    SpreadSheetViewEvent_type;
    typedef I_ArtAssetExplorerNode*                                             pExplorerNode_type;
    /// @}

    /// @name I_ArtAssetDocument interface
    /// @{
public:
    /// The subscription should be stored in the listener so that when
    /// the listener goes out of scope, the subscription also goes out
    /// of scope.  The listener should also retain a strong reference
    /// to the document.
    /// @param _pListener - Unmanaged pointer to the listener.  The
    ///                     listener MUST remain alive for at least as
    ///                     long as the document (which it will if it
    ///                     keeps a strong reference to the document).
    ///                     The listener MUST dereference the
    ///                     subscription before the listener is destroyed.
    virtual pSubscription_type subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Subscribe using a I_ArtAssetView.
    /// The rules given for the spreadsheet view version of subscribe()
    /// apply here as well.
    virtual pSubscription_type subscribe(pArtAssetView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    virtual I_ArtAssetController& getArtAssetController() = 0;

    /// Get the node associated with this document.
    /// This node is a I_ArtAsset.
    virtual pExplorerNode_type getNode() = 0;
    /// @}

    /// @name Static events
    /// @{
public:
    static SpreadSheetViewEvent_type        onRightClickArtAssetLabel;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ArtAssetDocument();
    virtual ~I_ArtAssetDocument();
    /// @}

};  // interface I_ArtAssetDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_I_ART_ASSET_DOCUMENT_HPP_INCLUDED
