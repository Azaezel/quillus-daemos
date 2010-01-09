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
#ifndef ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_ACTION_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_ACTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SpreadSheetView;

class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetAction
{
    /// @name I_ExplorerNodeAction interface
    /// @{
public:
    /// Get the name of this action.
    /// The name is generally used as the display name.
    virtual const std::string& getName() const = 0;

    /// Get the type of this action.
    /// Known types are:
    ///     "insertrow" - This is what happens when a row needs to be inserted.
    virtual const std::string& getType() const = 0;

    /// Invoke this action on a particular view.
    /// If both _column and _row are -1, the upper-left header cell is highlighted.
    /// 
    /// @param _column Column number of the cell that's highlighted, or -1 if no 
    ///     column is highlighted or if a row header is highlighted.
    /// @param _row Row number of the cell that's highlighted, or -1 if no row is 
    ///     highlighted or if a column header is highlighted.
    virtual void invoke(I_SpreadSheetView& _view, const int _column, const int _row) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetAction();
    virtual ~I_SpreadSheetAction();
    /// @}

};  // interface I_SpreadSheetAction

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCHCOMMON_I_SPREAD_SHEET_ACTION_HPP_INCLUDED
