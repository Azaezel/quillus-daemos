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
#ifndef ZENSTUDIO_WORKBENCH_I_NOTEBOOK_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_NOTEBOOK_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <string>

class wxWindow;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_View;

class WORKBENCH_DLL_LINK I_Notebook
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    struct ViewEvent
    {
        I_View* m_pActivating;
        I_View* m_pDeactivating;
    };
    typedef Event::Event<ViewEvent&>               ViewEventSignature_type;
    /// @}

    /// @name I_Notebook interface
    /// @{
public:
    /// Add a page to this notebook.
    virtual wxWindow* addPage(wxWindow* _pWindow, const std::string& _title) = 0;

    /// Show a page and make it active.
    virtual void showPage(wxWindow* _pWindow) = 0;

    /// Hide a page.
    /// Do not delete the page, just hide it.
    virtual void hidePage(wxWindow* _pWindow) = 0;

    /// Add an I_View page to this notebook.
    /// @todo Should this be moved to a different type of notebook since it's only
    ///     used by some notebooks?
    virtual wxWindow* addPage(I_View* _pWindow, const std::string& _title) = 0;

    /// Show an I_View page.
    /// @todo Should this be moved to a different type of notebook since it's only
    ///     used by notebooks that support views?
    virtual void showPage(I_View& _view) = 0;

    /// Get a page by title.
    /// @return an wxWindow that is a page in this notebook, or NULL if
    ///     a page with the given title doesn't exist.
    virtual wxWindow* getPage(const std::string& _title) = 0;

    virtual wxWindow* getWindow() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// This event is fired when a view in this notebook is activated and/or deactivated.
    ViewEventSignature_type  onActivate;
    /// @}

    /// @name 'Structors
protected:
             I_Notebook();
    virtual ~I_Notebook();
    /// @}

};  // interface I_Notebook

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_NOTEBOOK_HPP_INCLUDED
