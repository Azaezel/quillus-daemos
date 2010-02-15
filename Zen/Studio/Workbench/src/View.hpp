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
#ifndef ZENSTUDIO_WORKBENCH_VIEW_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_VIEW_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

#ifdef Yield
#undef Yield
#endif

#include <wx/wx.h>
#include <wx/frame.h>

class wxWindow;
class wxAuiNotebook;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Workbench;
class I_Document;
class I_Viewable;

class View
:   public wxPanel
,   public virtual I_View
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Viewable>             pViewable_type;
    /// @}

    /// @name I_View implementation
    /// @{
public:
    virtual I_Viewable* getViewable() const;
    virtual void activate();
    virtual void deactivate();
    /// @}

    /// @name View implementation
    /// @{
protected:
    friend class Notebook;

    /// Get the document associated with this view.
    virtual I_Document* getDocument() const;
public:
    /// Set the page and notebook containing this view.
    void setPage(wxAuiNotebook* _pNotebook, wxWindow* _pPage);

    /// Set the tab label text for this view.
    /// @note This is only valid if this view is associated with a valid
    ///     notebook and page.
    /// @see setPage().
    void setPageText(const std::string& _text);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             View(wxWindow* _pParent, Workbench* _pWorkbench, pViewable_type _pViewable, const int _instance);
             View(wxWindow* _pParent, Workbench* _pWorkbench, pViewable_type _pViewable, const int _instance, long _windowStyle);
    virtual ~View();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    Workbench* const            m_pWorkbench;
    pViewable_type              m_pViewable;
    const int                   m_instance;

    wxAuiNotebook*              m_pNotebook;
    wxWindow*                   m_pPage;
    /// @}

};  // class View

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_VIEW_HPP_INCLUDED
