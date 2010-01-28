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
#ifndef GAMEBUILDER_TEXT_CELL_DATA_HPP_INCLUDED
#define GAMEBUILDER_TEXT_CELL_DATA_HPP_INCLUDED
#include "GameObjectElement.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class TextCellData
:   public Zen::Studio::Workbench::I_SpreadSheetCell::UserData
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Persistable implementation
    /// @{
public:
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name UserData overrides
    /// @{
public:
    virtual bool getMutable() const;
    virtual void setTextValue(const std::string& _value);
    virtual const std::string& getTextValue() const;
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class TextCell;
    explicit TextCellData(GameObjectElement* _pElement, GameObjectElement::GameObjectElementFunctions* _pFunctions);
    virtual ~TextCellData();
    /// @}

    /// @name Member Variables
    /// @{
private:
    GameObjectElement*                              m_pElement;
    GameObjectElement::GameObjectElementFunctions*  m_pFunctions;
    /// @}

};  // class TextCell

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // GAMEBUILDER_TEXT_CELL_DATA_HPP_INCLUDED
