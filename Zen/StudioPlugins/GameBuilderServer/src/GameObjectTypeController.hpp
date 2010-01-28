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
#ifndef GAME_BUILDER_GAME_OBJECT_TYPE_CONTROLLER_HPP_INCLUDED
#define GAME_BUILDER_GAME_OBJECT_TYPE_CONTROLLER_HPP_INCLUDED

#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameObjectTypeDocument;

/// Game Object Type Document Controller.
class GameObjectTypeController
:   public I_GameObjectTypeController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_GameObjectTypeDocumentController implementation
    /// @{
public:
    virtual void insertElement(int _position, const std::string& _elementName);
    /// @}

    /// @name I_SpreadSheetController implementation
    /// @{
public:
    virtual void insertColumn(int _position, const std::string& _columnLabel);
    virtual void insertRow(int _position, const std::string& _rowLabel);
    virtual void setColumnLabelText(int _columnNumber, const std::string& _text);
    virtual void setRowLabelText(int _rowNumber, const std::string& _text);
    virtual void setCellTextValue(int _column, int _row, const std::string& _value);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class GameObjectTypeDocument;
             GameObjectTypeController(GameObjectTypeDocument& _document);
    virtual ~GameObjectTypeController();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameObjectTypeDocument&             m_document;
    /// @}

};  // class GameObjectTypeDocumentController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_GAME_OBJECT_TYPE_CONTROLLER_HPP_INCLUDED
