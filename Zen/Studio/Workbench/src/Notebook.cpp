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
#include "Notebook.hpp"
#include "View.hpp"

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <iostream>

BEGIN_EVENT_TABLE(Zen::Studio::Workbench::Notebook,wxAuiNotebook)
    EVT_AUINOTEBOOK_PAGE_CHANGED(wxID_ANY, Notebook::onPageChanged)
    EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, Notebook::onPageClose)

    EVT_KEY_DOWN(Zen::Studio::Workbench::Notebook::handleKeyDown)
END_EVENT_TABLE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Notebook::Notebook(wxWindow* _pParent)
:   wxAuiNotebook(_pParent, wxID_ANY, wxDefaultPosition, wxSize(300, 200), wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_TAB_MOVE | wxNO_BORDER)
,   m_pActiveView(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Notebook::~Notebook()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxWindow*
Notebook::getWindow()
{
    return this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxWindow*
Notebook::addPage(wxWindow* _pWindow, const std::string& _title)
{
    // Get the number of pages
    const size_t pageCount = GetPageCount();

    // Add a page to the end
    AddPage(_pWindow, std2wx(_title.c_str()), false);
    _pWindow->SetLabel(std2wx(_title.c_str()));

    // Return that page.
    return GetPage(pageCount);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::showPage(wxWindow* _pWindow)
{
    int pageIndex = GetPageIndex(_pWindow);
    if (pageIndex != wxNOT_FOUND)
    {
        SetSelection(pageIndex);
    }
    else
    {
        const size_t pageCount = GetPageCount();
        AddPage(_pWindow, _pWindow->GetLabel());
        SetSelection(pageCount);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::hidePage(wxWindow* _pWindow)
{
    int pageIndex = GetPageIndex(_pWindow);
    if (pageIndex != wxNOT_FOUND)
    {
        RemovePage(pageIndex);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxWindow*
Notebook::addPage(I_View* _pWindow, const std::string& _title)
{
    View* pWindow = dynamic_cast<View*>(_pWindow);

    // Get the number of pages
    const size_t pageCount = GetPageCount();

    // Add a page to the end
    AddPage(pWindow, std2wx(_title.c_str()), true);
    pWindow->SetLabel(std2wx(_title.c_str()));

    // Return that page.
    wxWindow* const pPage = GetPage(pageCount);

    pWindow->setPage(this, pPage);

    m_views[pPage] = pWindow;

    // Activate the page.
    SetSelection(pageCount);

    pPage->SetFocus();

    // If it's the first page, activate it. (um, didn't we just do that?)
    if (pageCount == 0)
    {
        notifyPageChange(_pWindow);
    }

    return pPage;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::showPage(I_View& _view)
{
    // TODO This should call getWindow() from I_View instead of assuming
    //      _view is a View.
    View* const pView = dynamic_cast<View*>(&_view);

    const int pageIndex = GetPageIndex(pView);
    if (pageIndex != wxNOT_FOUND)
    {
        SetSelection(pageIndex);
    }
    else
    {
        const size_t pageCount = GetPageCount();
        AddPage(pView, pView->GetLabel());
        SetSelection(pageCount);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxWindow*
Notebook::getPage(const std::string& _title)
{
    for(unsigned x = 0; x < GetPageCount(); x++)
    {
        if (wx2std(GetPageText(x)) == _title)
        {
            return GetPage(x);
        }
    }

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::handleKeyDown(wxKeyEvent& _event)
{
    std::cout << "Notebook::handleKeyDown()" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::onPageChanged(wxAuiNotebookEvent& _event)
{
    View* pNewView = NULL;
    View* pOldView = NULL;

    // Get the new view, or keep it NULL if there isn't one.
    if (_event.GetSelection() != -1)
    {
        wxWindow* pPage = GetPage(_event.GetSelection());
        Views_type::iterator newPage = m_views.find(pPage);

        if (newPage != m_views.end())
        {
            pNewView = newPage->second;
        }
    }

    // Get the old view, or keep it NULL if there isn't one.
    if (_event.GetOldSelection() != -1 && _event.GetOldSelection() < GetPageCount())
    {
        wxWindow* pOldPage = GetPage(_event.GetOldSelection());
        Views_type::iterator oldPage = m_views.find(pOldPage);
        if (oldPage != m_views.end())
        {
            pOldView = oldPage->second;
        }
    }

    notifyPageChange(pNewView, pOldView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::onPageClose(wxAuiNotebookEvent& _event)
{
    // Lets just assume that the active view is being closed.
    m_pActiveView = NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Notebook::notifyPageChange(I_View* _pNewView, I_View* _pOldView)
{
    // If there's an old view, deactivate it.
    if (_pOldView)
    {
        _pOldView->deactivate();
    }
    else
    {
        if (m_pActiveView)
        {
            m_pActiveView->deactivate();
        }
    }

    // If there's a new view, activate it.
    if (_pNewView)
    {
        _pNewView->activate();
    }

    ViewEvent viewEvent;
    viewEvent.m_pActivating = _pNewView;
    viewEvent.m_pDeactivating = _pOldView;
    onActivate(viewEvent);

    m_pActiveView = _pNewView;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
