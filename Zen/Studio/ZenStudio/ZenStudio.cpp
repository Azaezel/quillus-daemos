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

#include "ZenStudio.hpp"
#include "WorkbenchLocationDlg.h"

#include <Zen/Core/Plugins/I_Application.hpp>
#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Studio/Workbench/I_WorkbenchManager.hpp>
#include <Zen/Studio/Workbench/I_Workbench.hpp>
#include <Zen/Studio/Workbench/I_StudioTopFrame.hpp>
#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <wx/image.h>
#include <wx/config.h>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#include <stdexcept>

DECLARE_APP(Zen::Studio::ZenStudio)
IMPLEMENT_APP(Zen::Studio::ZenStudio)

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZenStudio::ZenStudio()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZenStudio::~ZenStudio()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenStudio::OnInit()
{
    std::auto_ptr<wxConfig> pConfig(new wxConfig(wxT("ZenStudio")));

    wxString lastWorkspace;

    if (pConfig->HasEntry(wxT("LastWorkspace")))
    {
        pConfig->Read(wxT("LastWorkspace"), &lastWorkspace);
    }

    addHandlers();

    // Display the "select workspace path" dialog first
    std::auto_ptr<WorkbenchLocationDlg> pDialog(new WorkbenchLocationDlg(NULL));
    pDialog->SetValue(lastWorkspace);

    if (pDialog->ShowModal() == wxID_OK)
    {
        std::string value(wx2std(pDialog->GetValue()));

        boost::filesystem::path workbenchPath = boost::filesystem::system_complete
            (
            boost::filesystem::path(value, boost::filesystem::native)
            ).normalize();

        lastWorkspace = pDialog->GetValue();
        boost::filesystem::path controlPath = workbenchPath / ".workbench";

        if (boost::filesystem::exists(controlPath))
        {
            boost::filesystem::path configPath = controlPath / "config.xml";

            Zen::Plugins::I_PluginManager& pluginManager = Zen::Plugins::I_PluginManager::getSingleton();

            Zen::Plugins::I_PluginManager::app_ptr_type pApp = pluginManager.installApplication(configPath);

            // Create the main workbench object.
            m_pWorkbench = Workbench::I_WorkbenchManager::getSingleton().createWorkbench();

            // Initialize the workbench
            if (m_pWorkbench->initialize(workbenchPath))
            {
                // If the initializaiton was successful, make it the top window
                // of this application.
                SetTopWindow(m_pWorkbench->getMainWindow().getWindow());

                pConfig->Write(wxT("LastWorkspace"), lastWorkspace);
                return true;
            }
            else
            {
                std::auto_ptr<wxMessageDialog> pMsgDlg(new wxMessageDialog(NULL, wxT("Error initializing Workbench.  This possibly is the result of a corrupted workbench.sqlite."),
                    wxT("Error"), wxOK | wxICON_ERROR | wxSTAY_ON_TOP));

                pMsgDlg->ShowModal();
            }
        }
        else
        {
            std::auto_ptr<wxMessageDialog> pMsgDlg(new wxMessageDialog(NULL, wxT("That is not a valid workspace directory and\r\nZen Studio does not support creating workspaces.\r\n(Yet)"),
                wxT("Error"), wxOK | wxICON_ERROR | wxSTAY_ON_TOP));

            pMsgDlg->ShowModal();
        }
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
ZenStudio::OnExit()
{
    if (m_pWorkbench.isValid())
    {
        m_pWorkbench.reset();
    }
    wxImage::CleanUpHandlers();

    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
ZenStudio::OnRun()
{
    try
    {
        return wxApp::OnRun();
    }
    catch(std::exception& _ex)
    {
        // TODO Pop up a message box?
        std::cout << "ZenStudio::OnRun(): Exception! " << _ex.what() << std::endl;
    }

    return 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZenStudio::addHandlers()
{
    wxImage::AddHandler(new wxPNGHandler());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Studio
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

