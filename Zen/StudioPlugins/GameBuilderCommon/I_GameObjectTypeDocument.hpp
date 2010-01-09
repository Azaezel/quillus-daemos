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
#ifndef GAME_BUILDER_I_GAME_OBJECT_TYPE_DOCUMENT_HPP_INCLUDED
#define GAME_BUILDER_I_GAME_OBJECT_TYPE_DOCUMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetViewable.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocumentType.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_GameObjectTypeView;
class I_GameObjectTypeController;
class I_GameObjectType;

/// Game Object Type Document.
/// This document can be viewed by a spread sheet view, but is not a spread sheet
/// document.  It can also be viewed using a I_GameObjectTypeView.
/// @note TR - Am I being stubborn about the fact that this is not a Spread Sheet Doc?
///     My concern is that eventually I'll want to have a document that is viewable
///     by multiple view types and I don't want to have to implement all of the 
///     methods for each of those document types.
class GAMEBUILDERCOMMON_DLL_LINK I_GameObjectTypeDocument
:   public Zen::Studio::Workbench::I_SpreadSheetViewable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_SpreadSheetView*                          pSpreadSheetView_type;
    typedef I_GameObjectTypeView*                                               pGameObjectTypeView_type;

    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Filter>        pFilter_type;
    typedef Zen::Memory::managed_ptr<Zen::Spaces::ObjectModel::I_Subscription>  pSubscription_type;

    typedef I_SpreadSheetViewable::SpreadSheetViewEvent_type                    SpreadSheetViewEvent_type;
    typedef I_GameObjectType*                                                   pExplorerNode_type;
    /// @}

    /// @name I_GameObjectTypeDocument interface
    /// @{
public:
    /// The subscription should be stored in the listener so that when
    /// the listener goes out of scope the subscription also goes out
    /// of scope.  The listener should also retain a strong reference
    /// to the document.
    /// @param _pListener - Unmanaged pointer to the listener.  The listener 
    ///     MUST remain alive for at least as long as the document (which it 
    ///     will if it keeps a srong reference to the document).  
    ///     The listener MUST dereference the subscription before the listener 
    ///     is destroyed.
    virtual pSubscription_type subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Subscribe using a I_GameObjectTypeView.
    /// The rules given for the spreadsheet view version of subscribe() apply here
    /// as well.
    virtual pSubscription_type subscribe(pGameObjectTypeView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    virtual I_GameObjectTypeController& getGameObjectTypeController() = 0;

    /// Get the node associated with this document.
    /// This node is a I_GameObjectType.
    virtual pExplorerNode_type getNode() = 0;
    /// @}

    /// @name Static events
    /// @{
public:
    static SpreadSheetViewEvent_type        onRightClickGameObjectTypeLabel;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameObjectTypeDocument();
    virtual ~I_GameObjectTypeDocument();
    /// @}

};  // interface I_GameObjectTypeDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_I_GAME_OBJECT_TYPE_DOCUMENT_HPP_INCLUDED
