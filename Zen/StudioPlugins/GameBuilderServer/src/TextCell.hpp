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
#ifndef GAMEBUILDER_TEXT_CELL_HPP_INCLUDED
#define GAMEBUILDER_TEXT_CELL_HPP_INCLUDED
#include "GameObjectElement.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class TextCell
:   public Zen::Studio::Workbench::I_SpreadSheetCell
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_SpreadSheetCell interface
    /// @{
public:
    virtual pUserData_type getUserData();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class GameObjectElement;
    explicit TextCell(GameObjectElement* _pElement, GameObjectElement::GameObjectElementFunctions* _pFunctions);
    virtual ~TextCell();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pUserData_type      m_pUserData;
    /// @}

};  // class TextCell

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // GAMEBUILDER_TEXT_CELL_HPP_INCLUDED
