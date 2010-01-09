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
#ifndef ZENSTUDIO_WORKBENCH_SPREAD_SHEET_VIEW_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_SPREAD_SHEET_VIEW_HPP_INCLUDED

#include "View.hpp"

#include <Zen/Studio/Workbench/I_Workbench.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>

#ifdef Yield
#undef Yield
#endif

#include <wx/grid.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GridCellEditor;

class SpreadSheetView
:   public View
,   public I_SpreadSheetView
{
private:
		DECLARE_EVENT_TABLE();

    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_View implementation
    /// @{
public:
    virtual I_Project* getProject() const;
    virtual I_Viewable* getViewable() const { return View::getViewable(); };
    virtual void activate();
    virtual void deactivate();
    /// @}

    /// @name I_SpreadSheetView implementation
    /// @{
public:
    virtual void onNewColumn(const int _columnPosition, const std::string& _columnLabelText);
    virtual void onNewRow(const int _rowPosition, const std::string& _rowLabeltext);
    virtual void onCellModified(I_SpreadSheetCell& _cell, const int _column, const int _row);
    virtual void onNameModified(const std::string& _shortName, const std::string& _longName);
    /// @}

    /// @name SpreadSheetView implementation
    /// @{
public:
    void createGUIControls();

    void handleRightClickLabel(wxGridEvent& _event);
    void handleRightClickCell(wxGridEvent& _event);
    void handleSelectCell(wxGridEvent& _event);

    /// Get the spreadsheet cell for the specified position.
    I_SpreadSheetCell& getCell(const int _column, const int _row);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class Workbench;
             SpreadSheetView(wxWindow* _pParent, Workbench* _pWorkbench, pViewable_type _pViewable, const int _instance);
    virtual ~SpreadSheetView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wxGrid*                                     m_pGrid;
    GridCellEditor*                             m_pEditor;

    I_SpreadSheetDocument::pSubscription_type   m_pSubscription;

    I_Workbench::pPropertiesHandle_type         m_pPropertiesHandle;
    I_Workbench::pPropertiesHandle_type         m_pCellPropertiesHandle;

    int                                         m_lastCellPropertyColumn;
    int                                         m_lastCellPropertyRow;
    bool                                        m_changePropertiesEachRow;
    bool                                        m_changePropertiesEachColumn;
    /// @}

};  // class SpreadSheetView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_SPREAD_SHEET_VIEW_HPP_INCLUDED
