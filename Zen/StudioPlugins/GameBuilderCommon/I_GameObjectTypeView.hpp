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
#ifndef GAME_BUILDER_I_GAME_OBJECT_TYPE_VIEW_HPP_INCLUDED
#define GAME_BUILDER_I_GAME_OBJECT_TYPE_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocumentType.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_GameObjectElement;
class I_GameObjectTypeDocument;

/// Game Object Type View.
/// Implement a view and use I_GameObjectTypeDocument::subscribe() in
/// order to receive notifications of modifications to the document.
class GAMEBUILDERCOMMON_DLL_LINK I_GameObjectTypeView
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_GameObjectTypeView interface
    /// @{
public:
    /// This method is called when a game object type document is modified.
    /// Generally this modification is a property modification.
    virtual void onDocumentModified(I_GameObjectTypeDocument& _gameObjectTypeDocument) = 0;

    /// This method is called when a new element is added to the document to which this
    /// view is subscribed.
    /// The element details will be indicated via a call to onElementModified().
    /// @param _position -1 if added to the end, otherwise the element was added
    ///     before the index provided (0 is the beginning, 1 is after the first
    ///     element, etc).
    virtual void onNewElement(I_GameObjectElement& _element, int _position) = 0;

    /// This method is called when an element is removed.
    /// @param _position is the position or index of the element being removed.
    ///     0 is the first element.  Negative numbers are not used.
    virtual void onElementRemoved(int _position) = 0;

    /// This method is called when an element is modified.
    virtual void onElementModified(I_GameObjectElement& _element, int _row) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameObjectTypeView();
    virtual ~I_GameObjectTypeView();
    /// @}

};  // interface I_GameObjectTypeView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_I_GAME_OBJECT_TYPE_VIEW_HPP_INCLUDED
