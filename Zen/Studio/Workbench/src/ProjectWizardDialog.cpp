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
#include "ProjectWizardDialog.hpp"
#include "StudioTopFrame.hpp"
#include "Workbench.hpp"

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectService.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_DocumentType.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectWizardManager.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectWizardType.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BEGIN_EVENT_TABLE(ProjectWizardDialog, wxDialog)
    EVT_CLOSE(ProjectWizardDialog::onClose)
    EVT_BUTTON(wxID_BACKWARD, ProjectWizardDialog::onBack)
    EVT_BUTTON(wxID_FORWARD, ProjectWizardDialog::onNext)
    EVT_BUTTON(wxID_OK, ProjectWizardDialog::onFinish)
    EVT_BUTTON(wxID_CANCEL, ProjectWizardDialog::onCancel)
    EVT_TREE_SEL_CHANGED(ID_WIZARD_TREE, ProjectWizardDialog::onWizardSelected)
    EVT_TREE_KEY_DOWN(ID_WIZARD_TREE, ProjectWizardDialog::onKeyDown)
END_EVENT_TABLE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectWizardDialog::ProjectWizardDialog(StudioTopFrame* _pParent)
:   wxDialog(_pParent, -1, wxT("Project Wizard"), wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
             )
,   m_pParent(_pParent)
,   m_pProjectType(NULL)
{
    m_topPanel = new wxPanel(this, -1);
    m_nextPanel = new wxPanel(m_topPanel, -1);

    // Main outside vertical layout
    m_topSizer = new wxBoxSizer(wxVERTICAL);

    initFirstPanel();

    m_topSizer->Add(m_firstPanel, 1, wxEXPAND | wxALL, 0);
    m_topSizer->Add(m_nextPanel, 1, wxEXPAND | wxALL, 0);

    // Button layout
    wxBoxSizer* buttonLayout = new wxBoxSizer(wxHORIZONTAL);
    m_back = new wxButton(m_topPanel, wxID_BACKWARD, wxT("< &Back"));
    m_next = new wxButton(m_topPanel, wxID_FORWARD, wxT("&Next >"));
    m_finish = new wxButton(m_topPanel, wxID_OK, wxT("&Finish"));
    m_cancel = new wxButton(m_topPanel, wxID_CANCEL, wxT("Cancel"));

    buttonLayout->Add(m_back, wxSizerFlags(0).Border(wxLEFT | wxBOTTOM, 5));
    buttonLayout->Add(m_next, wxSizerFlags(0).Border(wxLEFT | wxBOTTOM, 5));
    buttonLayout->Add(m_finish, wxSizerFlags(0).Border(wxLEFT | wxBOTTOM, 5));
    buttonLayout->Add(m_cancel, wxSizerFlags(0).Border(wxLEFT | wxBOTTOM | wxRIGHT, 5));
    m_finish->SetDefault();

    m_back->Enable(false);
    m_next->Enable(false);
    m_finish->Enable(false);

    m_topSizer->Add(buttonLayout, wxSizerFlags(0).Align(wxALIGN_BOTTOM | wxALIGN_RIGHT));

    m_topSizer->Show(m_nextPanel, false, true);
    m_topSizer->Layout();

    m_topPanel->SetSizer(m_topSizer);
    m_topSizer->SetSizeHints(this);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectWizardDialog::~ProjectWizardDialog()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::startWizard()
{
    m_wizardPage = 0;
    m_topSizer->Show(m_nextPanel, false, true);
    m_topSizer->Show(m_firstPanel, true, true);
    m_topSizer->Layout();

    // Make sure the dialog is being shown
    Show();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::initFirstPanel()
{
    // First panel that everything sits on
    m_firstPanel = new wxPanel(m_topPanel, wxID_ANY);

    // Main outside vertical layout
    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

    // Top horizontal layout (not used anymore?)
    //wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);

    // Wizard layout
    wxBoxSizer* wizardLayout = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* st1 = new wxStaticText(m_firstPanel, wxID_ANY, wxT("Project Type:"));
    wizardLayout->Add(st1, 0, wxRIGHT, 10);
    wxTextCtrl* tc = new wxTextCtrl(m_firstPanel, wxID_ANY);
    wizardLayout->Add(tc, 1);
    topSizer->Add(wizardLayout, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Skip some space
    topSizer->Add(-1, 10);

    // Tree View layout
    wxBoxSizer* treeLayout = new wxBoxSizer(wxHORIZONTAL);
    m_treeControl = new wxTreeCtrl(m_firstPanel, ID_WIZARD_TREE, wxDefaultPosition, wxSize(200, 200),
        wxTR_HAS_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT | wxTR_NO_LINES);

    treeLayout->Add(m_treeControl, wxSizerFlags(1).Expand());
    topSizer->Add(treeLayout, wxSizerFlags(1).Expand().Border(wxLEFT | wxRIGHT | wxTOP, 10));

    // Populate the tree control with project wizards
    struct WizardVisitor
    :   public I_ProjectWizardManager::I_WizardVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(const I_ProjectWizardType& _wizardType)
        {
            wxTreeItemId parent = m_pTreeControl->AppendItem(m_parent, std2wx(_wizardType.getName()), -1, -1, new ProjectWizardTreeItem(_wizardType));
        }

        virtual void end()
        {
        }

        WizardVisitor(wxTreeCtrl* _pTreeControl, wxTreeItemId _parent)
        :   m_pTreeControl(_pTreeControl)
        ,   m_parent(_parent)
        {
        }

    private:
        wxTreeCtrl*     m_pTreeControl;
        wxTreeItemId    m_parent;
    };

    // Populate the tree control with project types
    struct ProjectTypeVisitor : I_ProjectTypes::I_ProjectTypeVisitor
    {
        void begin()
        {
            m_parent = m_pTreeControl->AddRoot(wxT("Wizards"));
        }

        void visit(const I_ProjectType& _projectType)
        {
            wxTreeItemId parent = m_pTreeControl->AppendItem(m_parent, std2wx(_projectType.getName()), -1, -1, new ProjectTypeTreeItem(_projectType));

            WizardVisitor visitor(m_pTreeControl, parent);

            I_ProjectWizardManager::getSingleton().getWizardsForType(_projectType, visitor);
        }

        void end()
        {
        }

        ProjectTypeVisitor(wxTreeCtrl* _pTreeControl) : m_pTreeControl(_pTreeControl)
        {
        }

        wxTreeCtrl*     m_pTreeControl;
        wxTreeItemId    m_parent;
    } visitor(m_treeControl);

    m_pParent->getWorkbench().getProjectTypes().getProjectTypes(visitor);


    m_firstPanel->SetSizer(topSizer);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::onClose(wxCloseEvent& _event)
{
    EndDialog(0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::onBack(wxCommandEvent& _event)
{
    EndDialog(0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::onNext(wxCommandEvent& _event)
{
    // HACK: First time
    if (m_wizardPage == 0)
    {
        m_nextPanel->DestroyChildren();

    }

    ++m_wizardPage;

    m_topSizer->Show(m_firstPanel, false, true);
    m_topSizer->Show(m_nextPanel, true, true);
    m_topSizer->Layout();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::onFinish(wxCommandEvent& _event)
{
    wxTreeItemData* pItem = m_treeControl->GetItemData(m_treeControl->GetSelection());

    m_pProjectType = dynamic_cast<ProjectTypeTreeItem*>(pItem);

    if (m_pProjectType)
    {
        m_pParent->getWorkbench().createNewProject(m_pProjectType->getProjectType());
    }
    else
    {
    }

    EndDialog(0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::onCancel(wxCommandEvent& _event)
{
    EndDialog(0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::onWizardSelected(wxTreeEvent& _event)
{
    wxTreeItemData* pItem = m_treeControl->GetItemData(_event.GetItem());

    m_pProjectType = dynamic_cast<ProjectTypeTreeItem*>(pItem);

    if (m_pProjectType)
    {
        m_back->Enable(false);

        // TODO: Behave appropriately based on the availability of a file wizard
        m_next->Enable(false);
        m_finish->Enable(true);
        m_finish->SetDefault();
    }
    else
    {
        m_back->Enable(false);
        m_next->Enable(false);
        m_finish->Enable(false);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectWizardDialog::onKeyDown(wxTreeEvent& _event)
{
    if (_event.GetKeyCode() == 13)
    {
        if (m_pProjectType)
        {
            // TODO wizardNext or wizardFinish
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

