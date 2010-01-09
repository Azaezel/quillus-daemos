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
#ifndef ZENSTUDIO_WORKBENCH_NOTEBOOK_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_NOTEBOOK_HPP_INCLUDED

#include "../I_Notebook.hpp"

#include <string>

#include <wx/aui/aui.h>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class View;

/// Base Notebook class
class Notebook
:   public wxAuiNotebook
,   public I_Notebook
{
protected:
    DECLARE_EVENT_TABLE()

    /// @name Types
    /// @{
public:
    typedef std::map<wxWindow*, View*>      Views_type;
    /// @}

    /// @name I_Notebook interface
    /// @{
public:
    virtual wxWindow* addPage(wxWindow* _pWindow, const std::string& _title);
    virtual void showPage(wxWindow* _pWindow);
    virtual void hidePage(wxWindow* _pWindow);
    virtual wxWindow* addPage(I_View* _pWindow, const std::string& _title);
    virtual void showPage(I_View& _view);
    virtual wxWindow* getPage(const std::string& _title);
    virtual wxWindow* getWindow();
    /// @}

    /// @name Notebook implementation
    /// @{
public:
    void onPageChanged(wxAuiNotebookEvent& _event);
    void onPageClose(wxAuiNotebookEvent& _event);

    /// @param _pOldView The old view that was active before this call.
    ///     If the value is NULL, m_pActiveView is used.
    void notifyPageChange(I_View* _pNewView, I_View* _pOldView = NULL);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Notebook(wxWindow* _pParent);
    virtual ~Notebook();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Views_type          m_views;

    ///
    /// This is maintained in onPageChanged().
    I_View*             m_pActiveView;
    /// @}

};  // class Notebook

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_NOTEBOOK_HPP_INCLUDED
