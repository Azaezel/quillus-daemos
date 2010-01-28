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
#ifndef ZENSTUDIO_WORKBENCH_GRID_CELL_EDITOR_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_GRID_CELL_EDITOR_HPP_INCLUDED

#ifdef Yield
#undef Yield
#endif

#include <wx/grid.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class SpreadSheetView;

/// Default Grid Cell Editor for SpreadSheetView.
/// This Grid Cell Editor dispatches to the appropriate editor based on the 
/// cell type.
class GridCellEditor
:   public wxGridCellEditor
{
    /// @name wxGridCellEditor implementation
public:
    virtual void Create(wxWindow* _pParent, wxWindowID _id, wxEvtHandler* _pEvtHandler);
    virtual void SetSize(const wxRect& _rect);
    virtual void Show(bool _show, wxGridCellAttr* _pAttr = (wxGridCellAttr *)NULL);
    virtual void PaintBackground(const wxRect& _rectCell, wxGridCellAttr* _pAttr);
    virtual void BeginEdit(int row, int col, wxGrid* grid);
    virtual bool EndEdit(int _row, int _col, const wxGrid* _pGrid, const wxString& oldval, wxString* newval);
    virtual void ApplyEdit(int row, int col, wxGrid* grid);
    virtual void Reset();
    virtual bool IsAcceptedKey(wxKeyEvent& _event);
    virtual void StartingKey(wxKeyEvent& _event);
    virtual void StartingClick();
    virtual void HandleReturn(wxKeyEvent& _event);
    virtual void Destroy();
    virtual wxGridCellEditor *Clone() const;
    virtual wxString GetValue() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             GridCellEditor(SpreadSheetView& _view);
    virtual ~GridCellEditor();
    /// @}

    /// @name Member Variables
    /// @{
public:
    SpreadSheetView&        m_view;
    wxGridCellEditor*       m_pImpl;
    /// @}

};  // class GridCellEditor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_GRID_CELL_EDITOR_HPP_INCLUDED
