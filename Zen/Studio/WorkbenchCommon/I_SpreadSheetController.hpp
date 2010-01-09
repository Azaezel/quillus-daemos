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
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CONTROLLER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CONTROLLER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetController
:   public boost::noncopyable
//:   public I_DocumentController?
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_SpreadSheetController interface
    /// @{
public:
    /// Insert a new column with a specified name.  If the name is empty, an
    /// appropriately named column will be created based on the column position.
    /// @param _position 0 to insert at the head, -1 to insert at the end.  1 means
    ///         insert after the first column, etc.
    virtual void insertColumn(int _position, const std::string& _columnLabel) = 0;

    /// @param _position 0 to insert at the head, -1 to insert at the end.  1 means
    ///         insert after the first row, etc.
    virtual void insertRow(int _position, const std::string& _rowLabel) = 0;

    /// Set the column label.
    virtual void setColumnLabelText(int _columnNumber, const std::string& _text) = 0;

    /// Set the row label.
    virtual void setRowLabelText(int _rowNumber, const std::string& _text) = 0;

    /// Set the string value of a cell.
    virtual void setCellTextValue(int _column, int _row, const std::string& _value) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetController();
    virtual ~I_SpreadSheetController();
    /// @}

};  // interface I_SpreadSheetController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_CONTROLLER_HPP_INCLUDED
