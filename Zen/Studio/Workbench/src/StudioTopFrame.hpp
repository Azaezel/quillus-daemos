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
#ifndef ZENSTUDIO_WORKBENCH_STUDIO_TOP_FRAME_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_STUDIO_TOP_FRAME_HPP_INCLUDED

#include "../I_StudioTopFrame.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>

#include <wx/wxprec.h>
#include <wx/aui/aui.h>

class ProjectExplorerView;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Workbench;

class I_ProjectExplorerController;
class I_ProjectExplorerModel;
class I_ProjectExplorerWidget;

class FileWizardDialog;
class ProjectWizardDialog;

class WorkspaceExplorer;

class ExplorerNotebook;
class StatusNotebook;
class WorkspaceNotebook;
class ToolNotebook;
class PropertiesView;

class StudioTopFrame
:   public wxFrame
,   public I_StudioTopFrame
{
    enum 
    {
        ID_NEW_PROJECT = wxID_HIGHEST + 1,
        ID_NEW_FILE,
        ID_OPEN_PROJECT,
        ID_OPEN_FILE
    };

    typedef Memory::managed_ptr<I_ProjectExplorerModel>         pProjectExplorerModel_type;
    typedef Memory::managed_ptr<I_ProjectExplorerController>    pProjectExplorerController_type;

    /// @name I_StudioTopFrame implementation
    /// @{
public:
    virtual I_Notebook& getExplorerNotebook();
    virtual I_Notebook& getStatusNotebook();
    virtual I_Notebook& getWorkspaceNotebook();
    virtual I_Notebook& getToolNotebook();
    virtual wxWindow* getWindow();
    /// @}

    /// @name StudioTopFrame implementation
    /// @{
public:
    WorkspaceExplorer& getWorkspaceExplorer();
    Workbench& getWorkbench();
    I_ProjectExplorerController& getProjectExplorer();
    I_ProjectExplorerWidget& getProjectExplorerWidget();
    PropertiesView& getPropertiesView();
    void createMenus();
    /// @}

    /// @name Event Handlers
    /// @{
private:
	DECLARE_EVENT_TABLE()
public:
    void onExit(wxCommandEvent& _event);
    void onNewFile(wxCommandEvent& _event);
    void onNewProject(wxCommandEvent& _event);
    void onOpenFile(wxCommandEvent& _event);
    void onOpenProject(wxCommandEvent& _event);

    void onAllowNotebookDnD(wxAuiNotebookEvent& _event);
    /// @}

    /// @name 'Structors
    /// @{
public:
             StudioTopFrame(Workbench* _pWorkbench, const wxChar* _pTitle, int _xpos, int _ypos, int _width, int _height);
    virtual ~StudioTopFrame();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wxAuiManager            m_mgr;

    wxMenuBar*                  m_pMenuBar;

    //FileNewMenu*            m_pFileNewMenu;
    //FileNewMenu*            m_pFileOpenMenu;

    Workbench*                  m_pWorkbench;
    ExplorerNotebook*           m_pExplorerNotebook;
    StatusNotebook*             m_pStatusNotebook;
    WorkspaceNotebook*          m_pWorkspaceNotebook;
    ToolNotebook*               m_pToolNotebook;

    WorkspaceExplorer*          m_pWorkspaceExplorer;
    ProjectExplorerView*        m_pProjectExplorer;
    pProjectExplorerModel_type      m_pProjectExplorerModel;
    pProjectExplorerController_type m_pProjectExplorerController;

    FileWizardDialog*           m_pFileWizardDialog;
    ProjectWizardDialog*        m_pProjectWizardDialog;

    PropertiesView*             m_pPropertiesView;
    /// @}

};  // class StudioTopFrame

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_STUDIO_TOP_FRAME_HPP_INCLUDED

