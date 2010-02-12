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
#ifndef GAME_BUILDER_I_GAME_OBJECT_TYPE_CONTROLLER_HPP_INCLUDED
#define GAME_BUILDER_I_GAME_OBJECT_TYPE_CONTROLLER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetController.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Game Object Type Document Controller.
/// This is also a Spread Sheet Controller since the I_GameObjectTypeDocument
/// mimics some of the implementation of a Spread Sheet Document.
class GAMEBUILDERCOMMON_DLL_LINK I_GameObjectTypeController
:   public Zen::Studio::Workbench::I_SpreadSheetController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_GameObjectTypeController interface
    /// @{
public:
    /// Insert a new element at the specified position.
    /// @param _position 0 to insert at the head, -1 to insert at the end.  1 means
    ///         insert after the first row, etc.
    /// @param _elementName element name to use
    virtual void insertElement(int _position, const std::string& _elementName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameObjectTypeController();
    virtual ~I_GameObjectTypeController();
    /// @}

};  // interface I_GameObjectTypeController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_I_GAME_OBJECT_TYPE_CONTROLLER_HPP_INCLUDED
