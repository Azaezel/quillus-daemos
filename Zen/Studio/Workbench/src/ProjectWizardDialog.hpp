//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_DIALOG_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_DIALOG_HPP_INCLUDED

//#include <wx/wxprec.h>

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/treectrl.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Workbench;
class I_ProjectType;
class I_DocumentType;
class I_ProjectWizardType;
class StudioTopFrame;

class ProjectWizardDialog
:   public wxDialog
{
    /// @name Types
    /// @{
public:
    enum 
    {
        ID_STATIC_WIZARDS,
        ID_SEARCH_TEXT,
        ID_WIZARD_TREE
    };
    /// @}

    /// @name wxDialog overrides
    /// @{
public:
    /// @}

    /// @name ProjectWizardDialog implementation
    /// @{
public:
    /// Start the wizard
    void startWizard();

private:
    /// Initialize the first panel of the wizard
    void initFirstPanel();
    /// @}
    
    /// @name Event Handlers
    /// @{
private:
	DECLARE_EVENT_TABLE()
public:
    void onClose(wxCloseEvent& _event);

    void onBack(wxCommandEvent& _event);
    void onNext(wxCommandEvent& _event);
    void onFinish(wxCommandEvent& _event);
    void onCancel(wxCommandEvent& _event);

    void onKeyDown(wxTreeEvent& _event);
    void onWizardSelected(wxTreeEvent& _event);
    /// @}

    /// @name 'Structors
    /// @{
public:
    /// @param _open True if this object should open instead of create
             ProjectWizardDialog(StudioTopFrame* _pParent);
    virtual ~ProjectWizardDialog();
    /// @}

    /// @name Inner Classes
    /// @{
private:
    struct ProjectTypeTreeItem 
    :   public wxTreeItemData
    {
        ProjectTypeTreeItem(const I_ProjectType& _projectType) : m_projectType(_projectType) {}
        virtual ~ProjectTypeTreeItem() {}

        const I_ProjectType& getProjectType()
        {
            return m_projectType;
        }

    private:
        const I_ProjectType& m_projectType;
    };  // struct ProjectTypeTreeItem

    struct ProjectWizardTreeItem 
    :   public wxTreeItemData
    {
        ProjectWizardTreeItem(const I_ProjectWizardType& _projectWizardType) : m_projectWizardType(_projectWizardType) {}
        virtual ~ProjectWizardTreeItem() {}

        const I_ProjectWizardType& getProjectWizardType()
        {
            return m_projectWizardType;
        }

    private:
        const I_ProjectWizardType& m_projectWizardType;
    };  // struct ProjectWizardTreeItem

    /// @}

    /// @name Member Variables
    /// @{
private:
    StudioTopFrame* m_pParent;

    wxBoxSizer*     m_topSizer;
    wxPanel*        m_topPanel;
    wxPanel*        m_firstPanel;
    wxPanel*        m_nextPanel;

    wxButton*       m_back;
    wxButton*       m_next;
    wxButton*       m_finish;
    wxButton*       m_cancel;
    wxTreeCtrl*     m_treeControl;

    /// Currently selected project type, or NULL
    ProjectTypeTreeItem*    m_pProjectType;

    /// HACK 
    int             m_wizardPage;
    /// @}

};  // class ProjectWizardDialog

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_PROJECT_WIZARD_DIALOG_HPP_INCLUDED
