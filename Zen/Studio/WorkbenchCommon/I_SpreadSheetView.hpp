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
#ifndef ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_VIEW_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetViewable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SpreadSheetCell;
class I_SpreadSheetContributor;
class I_SpreadSheetAction;
class I_SpreadSheetViewable;

/// Spreadsheet view.
/// A spreadsheet view subscribes to a I_SpreadSheetDocument and recieves
/// notifications when the document changes.  Use this interface to populate
/// a GUI or for anything else requiring notifications of a spreadsheet document
/// modification.
/// @see I_SpreadSheetDocument::subscribe()
class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetView
:   public virtual I_View
{
    /// @name Types
    /// @{
public:
    typedef I_SpreadSheetContributor                        Contributor_type;
    typedef I_SpreadSheetAction                             Action_type;
    /// @}

    /// @name I_SpreadSheetView interface
    /// @{
public:
    /// New Column event notification.
    /// This method is called when a new columnn is added to the model to which this
    /// view is subscribed.
    virtual void onNewColumn(const int _columnPosition, const std::string& _columnLabelText) = 0;

    /// New Row notification.
    /// This method is called when a new row is added to the model to which this view
    /// is subscribed.
    virtual void onNewRow(const int _rowPosition, const std::string& _rowLabeltext) = 0;

    /// Modified Cell notification.
    /// This method is called when a cell is modified in a model to which this view
    /// is subscribed.
    virtual void onCellModified(I_SpreadSheetCell& _cell, const int _column, const int _row) = 0;

    /// Document name modified notification.
    virtual void onNameModified(const std::string& _shortName, const std::string& _longName) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetView();
    virtual ~I_SpreadSheetView();
    /// @}

};  // interface I_SpreadSheetView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory 
{
    /// I_SpreadSheetView is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Studio::Workbench::I_SpreadSheetView> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_VIEW_HPP_INCLUDED
