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
#ifndef ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_VIEWABLE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_VIEWABLE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Viewable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Spaces {
        namespace ObjectModel {
            class I_Filter;
            class I_Subscription;
        }   // namespace ObjectModel
    }   // namespace Spaces
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_SpreadSheetView;
class I_SpreadSheetCell;
class I_SpreadSheetAction;
class I_SpreadSheetController;
class I_SpreadSheetProperties;
class I_SpreadSheetCellProperties;

/// Spread Sheet Viewable interface.
/// Implement this interface if you have a document or model that is viewable
/// by a I_SpreadSheetView.
class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetViewable
:   public virtual I_Viewable
{
    /// @name Types
    /// @{
public:
    typedef I_SpreadSheetView*                                          pSpreadSheetView_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Filter>          pFilter_type;
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;

    /// Defined below.
    class I_SpreadSheetViewEvent;

    typedef Event::Event<I_SpreadSheetViewEvent&>                       SpreadSheetViewEvent_type;
    /// @}

    /// @name I_SpreadSheetViewable interface
    /// @{
public:
    /// Subscribe a view to this viewable.
    /// The subscription should be stored in the view so that when
    /// the view goes out of scope the subscription also goes out
    /// of scope.  The view should also retain a strong reference
    /// to the document.
    /// @param _pView - Unmanaged pointer to the view.  The view MUST remain
    ///     alive for at least as long as the viewable object (which it will if it keeps
    ///     a strong reference to the viewable object).  The view MUST dereference the
    ///     subscription before the view is destroyed.
    virtual pSubscription_type subscribe(pSpreadSheetView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Get the controller to control this viewable.
    /// I_SpreadSheetController is the write access to the document.
    /// Use it to modify the contents of the document.
    /// @return A controller that can be used to modify this document.
    ///     Do not retain a reference to this controller.
    virtual I_SpreadSheetController& getSpreadSheetController() = 0;

    /// Get the properties of this spread sheet.
    virtual I_SpreadSheetProperties& getProperties() = 0;

    /// Get the properties of the specified spread sheet cell.
    virtual I_SpreadSheetCellProperties& getCellProperties(const int _column, const int _row) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    SpreadSheetViewEvent_type   onRightClickLabel;
    SpreadSheetViewEvent_type   onCellEndEdit;
    /// @}

    /// @name Inner Types
    /// @{
public:
    class WORKBENCHCOMMON_DLL_LINK I_SpreadSheetViewEvent
    {
    public:
        /// Get the view asociated with this event.
        virtual I_SpreadSheetView& getView() = 0;

        /// Get the cell associated with this event.
        /// @return NULL if this event is not associated with a cell, 
        ///     otherwise it returns the cell that was highlighted when
        ///     the event was fired.
        virtual I_SpreadSheetCell* getCell() = 0;

        /// Get the column that is selected for the event.
        /// If a cell is selected, this is the column number of that cell.
        /// If no column is selected (or a row label is selected) then 
        /// -1 is returned.
        virtual int getColumn() = 0;

        /// Get the row that is selected for the event.
        /// If a cell is selected, this is the row number of that cell.
        /// If no column is selected (or a column label is selected) then 
        /// -1 is returned.
        virtual int getRow() = 0;

        /// Call this method to add an action handler for this event.
        virtual void addAction(I_SpreadSheetAction& _action) = 0;

        /// Call this method to reject the event.
        /// If the event is rejected, no actions are displayed or invoked.
        virtual void rejectEvent() = 0;

    };  // interface I_SpreadSheetViewEvent
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SpreadSheetViewable();
    virtual ~I_SpreadSheetViewable();
    /// @}


};  // interface I_SpreadSheetViewable

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_SpreadSheetDocument is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_SpreadSheetViewable> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_SPREAD_SHEET_VIEWABLE_HPP_INCLUDED
