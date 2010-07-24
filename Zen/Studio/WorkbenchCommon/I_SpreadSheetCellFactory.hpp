//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
/// @author Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_CELL_FACTORY_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_CELL_FACTORY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ClassFactory.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SpreadSheetDocument;

/// Spread Sheet Cell User Data Factory.
/// This factory creates the user data for spread sheet cells.
class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetCellFactory
:   public Plugins::I_ClassFactory
{
    /// @name Types
    /// @{
public:
    typedef I_SpreadSheetCell::pUserData_type                  pUserData_type;
    /// @}

    /// @name I_SpreadSheetCellFactory interface
    /// @{
public:
    /// Create an explorer node user data.
    virtual pUserData_type createSpreadSheetCell(I_SpreadSheetDocument& _document, boost::uint64_t _spreadSheetCellId, const std::string& _cellType, int _row, int _column) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetCellFactory();
    virtual ~I_SpreadSheetCellFactory();
    /// @}

};  // interface I_SpreadSheetCellFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_CELL_FACTORY_HPP_INCLUDED
