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
#include "GridCellEditor.hpp"
#include "SpreadSheetView.hpp"

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetController.hpp>

#include "../wxUtilities.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GridCellEditor::GridCellEditor(SpreadSheetView& _view)
:   m_view(_view)
,   m_pImpl(NULL)
{
    // HACK; for now (for debugging) just default to a wxGridCellTextEditor
    m_pImpl = new wxGridCellTextEditor();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GridCellEditor::~GridCellEditor()
{
    if (m_pImpl)
    {
        /// HACK to allow editing of GameObjectElement spreadsheet view grid cell.
        /// If we don't set m_control to NULL here, wxWidgets will try
        /// to deallocate it twice (once for m_pImpl, and again for this).
        m_control = NULL;
        m_pImpl->DecRef();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::Create(wxWindow* _pParent, wxWindowID _id, wxEvtHandler* _pEvtHandler)
{
    if (m_pImpl)
    {
        m_pImpl->Create(_pParent, _id, _pEvtHandler);

        /// HACK to allow editing of GameObjectElement spreadsheet view grid cell.
        /// This is a weak reference, although wxWidgets doesn't know any better :P.
        m_control = m_pImpl->GetControl();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::SetSize(const wxRect& _rect)
{
    if (m_pImpl)
    {
        m_pImpl->SetSize(_rect);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::Show(bool _show, wxGridCellAttr* _pAttr)
{
    if (m_pImpl)
    {
        m_pImpl->Show(_show, _pAttr);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::PaintBackground(const wxRect& _rectCell, wxGridCellAttr* _pAttr)
{
    if (m_pImpl)
    {
        m_pImpl->PaintBackground(_rectCell, _pAttr);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::BeginEdit(int _row, int _col, wxGrid* _pGrid)
{
    if (m_pImpl)
    {
        m_pImpl->BeginEdit(_row, _col, _pGrid);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GridCellEditor::EndEdit(int _row, int _col, const wxGrid* _pGrid, const wxString& oldval, wxString* newval)
{
    if (m_pImpl)
    {
        bool changed = m_pImpl->EndEdit(_row, _col, _pGrid, oldval, newval);

        // If the value changed, set the underlying cell value.
        if (changed)
        {
            I_SpreadSheetViewable* pViewable = dynamic_cast<I_SpreadSheetViewable*>(m_view.getViewable());

            // Double-check it's not NULL
            if (pViewable)
            {
                pViewable->getSpreadSheetController().setCellTextValue(_col, _row, wx2std(m_pImpl->GetValue()));
            }
            else
            {
                // TODO Throw?
            }
        }

        return changed;
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::ApplyEdit(int row, int col, wxGrid* grid)
{
    // TODO Implement?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::Reset()
{
    if (m_pImpl)
    {
        m_pImpl->Reset();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
GridCellEditor::IsAcceptedKey(wxKeyEvent& _event)
{
    if (m_pImpl)
    {
        return m_pImpl->IsAcceptedKey(_event);
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::StartingKey(wxKeyEvent& _event)
{
    if (m_pImpl)
    {
        m_pImpl->StartingKey(_event);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::StartingClick()
{
    if (m_pImpl)
    {
        m_pImpl->StartingClick();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::HandleReturn(wxKeyEvent& _event)
{
    if (m_pImpl)
    {
        m_pImpl->HandleReturn(_event);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GridCellEditor::Destroy()
{
    if (m_pImpl)
    {
        m_pImpl->Destroy();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxGridCellEditor*
GridCellEditor::Clone() const
{
    return new GridCellEditor(m_view);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxString
GridCellEditor::GetValue() const
{
    if (m_pImpl)
    {
        return m_pImpl->GetValue();
    }

    return _T("");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
