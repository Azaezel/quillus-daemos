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
#include "GameObjectTypeController.hpp"
#include "GameObjectTypeDocument.hpp"
#include "GameObjectElement.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeController::GameObjectTypeController(GameObjectTypeDocument& _document)
:   m_document(_document)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeController::~GameObjectTypeController()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeController::insertElement(int _position, const std::string& _elementName)
{
    m_document.insertElement(_position, _elementName);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeController::insertColumn(int _position, const std::string& _columnLabel)
{
    // TODO Implement
    throw Zen::Utility::runtime_exception("GameObjectTypeController::insertColumn(): Error, not implemented.");
    //m_document.insertColumn(_position, _columnLabel);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeController::insertRow(int _position, const std::string& _rowLabel)
{
    // TODO Implement
    throw Zen::Utility::runtime_exception("GameObjectTypeController::insertRow(): Error, not implemented.");
    //m_document.insertRow(_position, _rowLabel);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeController::setColumnLabelText(int _column, const std::string& _text)
{
    // TODO Implement
    throw Zen::Utility::runtime_exception("GameObjectTypeController::setColumnLabelText(): Error, not implemented.");
    //m_document.setColumnLabelText(_column, _text);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeController::setRowLabelText(int _column, const std::string& _text)
{
    // TODO Implement
    throw Zen::Utility::runtime_exception("GameObjectTypeController::setRowLabelText(): Error, not implemented.");
    //m_document.setColumnLabelText(_column, _text);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeController::setCellTextValue(int _column, int _row, const std::string& _value)
{
    // TODO Do not modify parent rows.
    // Need to implement a getParentElementCount() in the docoument
    // and make sure _row is >= that value.

    switch(_column)
    {
    case 0:
        m_document.getElement(_row).setElementName(_value);
        break;
    case 1:
        m_document.getElement(_row).setDataType(_value);
        break;
    case 2:
        m_document.getElement(_row).setDefaultValue(_value);
        break;
    default:
        throw Zen::Utility::runtime_exception("GameObjectTypeController::setCellTextValue(): Error, invalid column.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
