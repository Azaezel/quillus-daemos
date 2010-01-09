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
#include "StudioTopFrame.hpp"
#include "Workbench.hpp"

#include "ExplorerNotebook.hpp"
#include "StatusNotebook.hpp"
#include "WorkspaceNotebook.hpp"
#include "ToolNotebook.hpp"

#include "PropertiesView.hpp"

#include "ProjectWizardDialog.hpp"
#include "FileWizardDialog.hpp"

#include "WorkspaceExplorer.hpp"

#include "ProjectExplorerView.h"

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

/// Yet another HACK to avoid some M$ idiocy... This one deverves a big WTF!
#ifdef Yield
#undef Yield
#endif
#include <wx/window.h>

#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BEGIN_EVENT_TABLE(StudioTopFrame, wxFrame)
    EVT_MENU(wxID_EXIT, StudioTopFrame::onExit)
    EVT_MENU(StudioTopFrame::ID_NEW_FILE, StudioTopFrame::onNewFile)
    EVT_MENU(StudioTopFrame::ID_NEW_PROJECT, StudioTopFrame::onNewProject)
    EVT_MENU(StudioTopFrame::ID_OPEN_FILE, StudioTopFrame::onOpenFile)
    EVT_MENU(StudioTopFrame::ID_OPEN_PROJECT, StudioTopFrame::onOpenProject)
    EVT_AUINOTEBOOK_ALLOW_DND(wxID_ANY, StudioTopFrame::onAllowNotebookDnD)
END_EVENT_TABLE()

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
StudioTopFrame::StudioTopFrame(Workbench* _pWorkbench, const wxChar* _pTitle, int _xpos, int _ypos, int _width, int _height)
:   wxFrame((wxFrame*)NULL, -1, _pTitle, wxPoint(_xpos, _ypos), wxSize(_width, _height))
,   m_pWorkbench(_pWorkbench)
,   m_pExplorerNotebook(NULL)
,   m_pStatusNotebook(NULL)
,   m_pWorkspaceNotebook(NULL)
,   m_pToolNotebook(NULL)
,   m_pWorkspaceExplorer(NULL)
,   m_pFileWizardDialog(NULL)
,   m_pProjectWizardDialog(NULL)
,   m_pPropertiesView(NULL)
{
    m_mgr.SetManagedWindow(this);

    SetIcon(wxIcon(wxT("./icons/IZLogo_16.png"), wxBITMAP_TYPE_PNG));

    createMenus();

    // Create the status bar
    CreateStatusBar();
    GetStatusBar()->SetStatusText(wxT("Ready"));

    // create some toolbars
    wxToolBar* tb1 = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                   wxTB_FLAT | wxTB_NODIVIDER);
    tb1->SetToolBitmapSize(wxSize(16,16));
    tb1->AddTool(101, wxT("NewFile"), wxArtProvider::GetBitmap(wxART_NEW), wxT("New File"));
    tb1->AddTool(102, wxT("OpenFile"), wxArtProvider::GetBitmap(wxART_FILE_OPEN), wxT("Open"));
    tb1->AddTool(103, wxT("SaveFile"), wxArtProvider::GetBitmap(wxART_FILE_SAVE), wxT("Save"));
    tb1->AddTool(104, wxT("SaveAs"), wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS), wxT("Save As"));
    tb1->Realize();

    m_mgr.AddPane(getExplorerNotebook().getWindow(), wxAuiPaneInfo().
                  Name(wxT("explorer")).Caption(wxT("Explorer")).
                  CloseButton(false).MinimizeButton(true).PinButton(true).
                  Left()
                  );

    m_mgr.AddPane(getStatusNotebook().getWindow(), wxAuiPaneInfo().
                  Name(wxT("status")).CaptionVisible(false).
                  //Gripper(true).GripperTop(true).PinButton(true).
                  CloseButton(false).MinimizeButton(true).
                  Bottom()
                  );

    m_mgr.AddPane(getToolNotebook().getWindow(), wxAuiPaneInfo().
                  Name(wxT("tool")).Caption(wxT("Tools")).
                  CloseButton(false).MinimizeButton(true).
                  Right()
                  );

    m_mgr.AddPane(getWorkspaceNotebook().getWindow(), wxAuiPaneInfo().
                  Name(wxT("workspace")).CaptionVisible(true).
                  CloseButton(false).MaximizeButton(true).
                  Center() //.PaneBorder(true)
                  );

    m_mgr.AddPane(tb1, wxAuiPaneInfo().
                  Name(wxT("toolbar1")).Caption(wxT("File Toolbar")).
                  ToolbarPane().Top());

    FILE* pFile = fopen("default.perspective", "r");
    if (pFile)
    {
        char szBuffer[4096];
        fgets(szBuffer, sizeof(szBuffer) - 1, pFile);
        fclose(pFile);
        std::string buffer(szBuffer);
        m_mgr.LoadPerspective(std2wx(buffer));
    }

#if 0
    m_mgr.AddPane(getExplorer().getWindow(), wxAuiPaneInfo().
                  Name(wxT("test8")).Caption(wxT("Workspace")).
                  Left().Layer(1).Position(1).
                  CloseButton(true).MaximizeButton(true));
#endif

    m_mgr.Update();

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
StudioTopFrame::~StudioTopFrame()
{
    m_mgr.UnInit();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StudioTopFrame::createMenus()
{

    // Create main menu bar
    m_pMenuBar = new wxMenuBar;

    // Create the File New menu
    wxMenu* fileNewMenu = new wxMenu;
    fileNewMenu->Append(ID_NEW_PROJECT, wxT("Project..."), wxT("Create a new project"));
    fileNewMenu->Append(ID_NEW_FILE, wxT("File..."), wxT("Create a new file"));

    // Create the File Open menu
    wxMenu* fileOpenMenu = new wxMenu;
    fileOpenMenu->Append(ID_OPEN_PROJECT, wxT("Project..."), wxT("Open a project"));
    fileOpenMenu->Append(ID_OPEN_FILE, wxT("File..."), wxT("Open a file"));

    wxMenu* file_menu = new wxMenu;

    file_menu->Append(wxID_NEW, wxT("&New"), fileNewMenu);
    file_menu->Append(wxID_OPEN, wxT("&Open"), fileOpenMenu);
    file_menu->AppendSeparator();
    file_menu->Append(wxID_CLOSE, wxT("&Close"));

    file_menu->AppendSeparator();
    file_menu->Append(wxID_EXIT, wxT("E&xit"));

    m_pMenuBar->Append(file_menu, wxT("&File"));
    SetMenuBar(m_pMenuBar);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Notebook&
StudioTopFrame::getExplorerNotebook()
{
    if (m_pExplorerNotebook == NULL)
    {
        m_pExplorerNotebook = new ExplorerNotebook(this);

        getWorkspaceExplorer();
    }

    return *m_pExplorerNotebook;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Notebook&
StudioTopFrame::getStatusNotebook()
{
    if (m_pStatusNotebook == NULL)
    {
        m_pStatusNotebook = new StatusNotebook(this);
    }

    return *m_pStatusNotebook;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Notebook&
StudioTopFrame::getWorkspaceNotebook()
{
    if (m_pWorkspaceNotebook == NULL)
    {
        m_pWorkspaceNotebook = new WorkspaceNotebook(this);
    }

    return *m_pWorkspaceNotebook;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_Notebook&
StudioTopFrame::getToolNotebook()
{
    if (m_pToolNotebook == NULL)
    {
        m_pToolNotebook = new ToolNotebook(this);

        m_pPropertiesView = new PropertiesView(m_pToolNotebook->getWindow(), m_pWorkbench);
        wxWindow* pPropertiesPage = m_pToolNotebook->addPage(m_pPropertiesView, "Properties");

        // TODO Tell the workbench about this property view?
    }

    return *m_pToolNotebook;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkspaceExplorer&
StudioTopFrame::getWorkspaceExplorer()
{
    if (m_pWorkspaceExplorer == NULL)
    {
        m_pWorkspaceExplorer = new WorkspaceExplorer(getExplorerNotebook().getWindow());

        // TODO TR - Move ownership of this to Workbench.
        I_WorkbenchService::pFutureProjectExplorerModel_type pFuture =
            getWorkbench().getWorkbenchService().createProjectExplorerModel();

        m_pProjectExplorerModel = pFuture->getValue();

        m_pProjectExplorer = new ProjectExplorerView(m_pExplorerNotebook->getWindow());
        m_pProjectExplorer->setModel(m_pProjectExplorerModel);

        wxWindow* pProjectPage = getExplorerNotebook().addPage(m_pProjectExplorer, "Projects");

        // TODO TR - Move ownership of this to Workbench.
        m_pProjectExplorerController = getWorkbench().getWorkbenchService().create(*m_pProjectExplorerModel)->getValue();
    }

    return *m_pWorkspaceExplorer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
wxWindow*
StudioTopFrame::getWindow()
{
    return this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Workbench&
StudioTopFrame::getWorkbench()
{
    return *m_pWorkbench;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ProjectExplorerController&
StudioTopFrame::getProjectExplorer()
{
    return *m_pProjectExplorerController;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ProjectExplorerWidget&
StudioTopFrame::getProjectExplorerWidget()
{
    return *m_pProjectExplorer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PropertiesView&
StudioTopFrame::getPropertiesView()
{
    // Call this to make sure the properties view has been created.
    getToolNotebook();

    // Return the properties view.
    return *m_pPropertiesView;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void StudioTopFrame::onExit(wxCommandEvent& WXUNUSED(_event))
{
    wxString perspective = m_mgr.SavePerspective();

    FILE* pFile = fopen("default.perspective", "w+");

    fprintf(pFile, "%s", wx2std(perspective).c_str());
    fclose(pFile);

    Close(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StudioTopFrame::onAllowNotebookDnD(wxAuiNotebookEvent& _event)
{
    // for the purpose of this test application, explicitly
    // allow all notebook drag and drop events
    _event.Allow();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StudioTopFrame::onNewFile(wxCommandEvent& _event)
{
    if (m_pFileWizardDialog == NULL)
    {
        m_pFileWizardDialog = new FileWizardDialog(this);
    }

    // Start the wizard
    m_pFileWizardDialog->startWizard();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StudioTopFrame::onNewProject(wxCommandEvent& _event)
{
    if (m_pProjectWizardDialog == NULL)
    {
        m_pProjectWizardDialog = new ProjectWizardDialog(this);
    }

    // Start the wizard
    m_pProjectWizardDialog->startWizard();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StudioTopFrame::onOpenFile(wxCommandEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StudioTopFrame::onOpenProject(wxCommandEvent& _event)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

