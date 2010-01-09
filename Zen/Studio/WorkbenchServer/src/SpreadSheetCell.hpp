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
#ifndef ZENSTUDIO_WORKBENCH_SPREAD_SHEET_CELL_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_SPREAD_SHEET_CELL_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class SpreadSheetDocument;

class SpreadSheetCell
:   public I_SpreadSheetCell
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_SpreadSheetCell implementation
    /// @{
public:
    virtual pUserData_type getUserData();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class SpreadSheetDocument;
             SpreadSheetCell(SpreadSheetDocument& _document, I_SpreadSheetCell::pUserData_type _pUserData);
    virtual ~SpreadSheetCell();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Parent document.
    SpreadSheetDocument&                m_document;
    /// User data.
    I_SpreadSheetCell::pUserData_type   m_pUserData;
    /// @}

};  // class SpreadSheetCell

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_SPREAD_SHEET_CELL_HPP_INCLUDED
