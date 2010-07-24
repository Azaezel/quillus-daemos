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
#ifndef ZENSTUDIO_WORKBENCH_SPREAD_SHEET_CONTROLLER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_SPREAD_SHEET_CONTROLLER_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetController.hpp>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class SpreadSheetDocument;

/// @todo TR - Instead of this, why not just make SpreadSheetDocument implement
///     I_SpreadSheetController?  Does it need to be kept separate?
class SpreadSheetController
:   public I_SpreadSheetController
{
    /// @name Types
    /// @{
public:
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
    friend class SpreadSheetDocument;
             SpreadSheetController(SpreadSheetDocument& _document);
    virtual ~SpreadSheetController();
    /// @}

    /// @name Member Variables
    /// @{
private:
    SpreadSheetDocument&           m_document;
    /// @}

};  // class SpreadSheetController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_SPREAD_SHEET_CONTROLLER_HPP_INCLUDED
