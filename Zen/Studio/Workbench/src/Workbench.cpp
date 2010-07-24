//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Hack to get around some MSVC stupidity
#ifdef _MSC_VER
#define NOMINMAX
#endif

#include "Workbench.hpp"
#include "StudioTopFrame.hpp"
#include "WorkspaceNotebook.hpp"

#include "SpreadSheetView.hpp"
#include "SceneViewWidget.hpp"
#include "PropertiesView.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Plugins/I_Application.hpp>
#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>

#include <Zen/Engine/Rendering/I_RenderingManager.hpp>
#include <Zen/Engine/Rendering/I_RenderingService.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>

#include <Zen/Studio/Workbench/wxUtilities.hpp>
#include <Zen/Studio/Workbench/I_ContributorService.hpp>
#include <Zen/Studio/Workbench/I_ContributorServiceFactory.hpp>
#include <Zen/Studio/Workbench/I_PropertiesHandle.hpp>

#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectService.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectTypes.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectServiceManager.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <Zen/Studio/WorkbenchCommon/II_ProjectTypes.hpp>

#include <wx/gdicmn.h>
#include <wx/textdlg.h>

#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Workbench::Workbench()
:   m_projectTypes(*I_ProjectTypes::create())
,   m_pMainFrame(NULL)
{
    m_viewFactories["spreadsheet"] = boost::bind(&Workbench::createSpreadSheetView, this, _1, _2, _3);
    m_viewFactories["ogrescene"] = boost::bind(&Workbench::createOgreSceneView, this, _1, _2, _3);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Workbench::~Workbench()
{
    m_pAppServer->stop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Workbench::initialize(boost::filesystem::path& _workspacePath)
{
    m_workspacePath = _workspacePath;
    m_controlPath = _workspacePath / ".workbench";

    Zen::Enterprise::AppServer::I_ApplicationServerManager& manager =
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton();

    // Initialize the main application server
    m_pAppServer = &Enterprise::AppServer::I_ApplicationServer::getInstance("main");

    // Load the protocol adapter and install it as the "server" protocol adapter.
    m_pProtocolService = manager.createProtocolService(*m_pAppServer, "tcp-binary");
    m_pAppServer->installProtocol(m_pProtocolService, "server");

    Zen::Plugins::I_PluginManager& pluginManager = Zen::Plugins::I_PluginManager::getSingleton();
    Zen::Plugins::I_PluginManager::app_ptr_type pApp = pluginManager.getApplication();

    // Get the application configuration
    typedef Plugins::I_ConfigurationElement::const_ptr_type pConfig_type;
    pConfig_type pAppConfig = pApp->getConfiguration().getConfigurationElement("application");

    // Use the application configuration to load application services and protocol adapters.
    pConfig_type pApplications = pAppConfig->getChild("applications");
    if (pApplications)
    {
        m_pAppServer->installApplications(pApplications);
    }

    // Install the protocols
    pConfig_type pProtocols = pAppConfig->getChild("protocols");

    if (pProtocols)
    {
        m_pAppServer->installProtocols(pProtocols);
    }

    // The application server must be started before any applications are actually installed.
    // The require() makes it so that this call will block until the application server
    // is fully operational.
    m_pAppServer->start()->requireCondition();

    // TODO Use the service registry to find the workbench instead of hard-coding
    // this location.


    m_pWorkbenchService = m_pAppServer->getApplication(manager.createLocation("/workbench"))
        .as<pWorkbenchService_type>();

    if (!m_pWorkbenchService.isValid())
    {
        return false;
    }

    if (!m_pWorkbenchService->setWorkspacePath(m_workspacePath))
    {
        return false;
    }

    // Install all of the contributor extensions
    installContributors();

    // Create the mainframe
    int displayWidth, displayHeight;

    wxDisplaySize(&displayWidth, &displayHeight);

    m_pMainFrame = new StudioTopFrame(this, wxT("Zen Studio"), 0, 0,
	    std::min(1024, displayWidth), std::min(768, displayHeight));

    Internal::II_ProjectTypes* pProjectTypes = dynamic_cast<Internal::II_ProjectTypes*>(&m_projectTypes);

    if( pProjectTypes != NULL )
    {
        pProjectTypes->init();
    }
    else
    {
        throw Zen::Utility::runtime_exception("Workbench::initialize() : Invalid type.");
    }

    activateContributors();

    // TODO Possibly need to refresh the mainframe after the contributors
    // have been activated?  Or possibly Show(true) is good enough.

    m_pMainFrame->Show(true);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_StudioTopFrame&
Workbench::getMainWindow()
{
    return *m_pMainFrame;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ProjectTypes&
Workbench::getProjectTypes()
{
    return m_projectTypes;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_View&
Workbench::getView(const std::string& _viewType, pDocument_type _pDocument, bool _newInstance)
{
    // TODO Guard

    int instance = 0;

    while(true)
    {
        Views_type::iterator iter;

        // First look for _viewType::_viewName
        std::stringstream name;
        name << _viewType << "::" << _pDocument->getLongName() << "::" << instance;

        iter = m_views.find(name.str());

        if (iter != m_views.end())
        {
            if(_newInstance)
            {
                // If creating a new instance then this instance number is already
                // in use, so skip it and check the next instance.
                // TODO This algorithm is fairly inefficient.. possibly consider
                // improving it, or is it worth it?
                instance++;
                continue;
            }
            else
            {
                // If not creating a new instance, just return the first one found.
                return *(iter->second);
            }
        }
        else
        {
            return createView(_viewType, _pDocument, instance + 1);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ProjectExplorerController&
Workbench::getProjectExplorerController()
{
    // TR - Why does the mainframe own this instead of the workbench?
    // We should probably move ownership of both the controller and the
    // model down to the workbench, but mainframe should maintain
    // ownership of the view.
    return m_pMainFrame->getProjectExplorer();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ProjectExplorerWidget&
Workbench::getProjectExplorerWidget()
{
    return m_pMainFrame->getProjectExplorerWidget();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Enterprise::AppServer::I_ApplicationServer&
Workbench::getAppServer() const
{
    return *m_pAppServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WorkbenchService&
Workbench::getWorkbenchService() const
{
    return *m_pWorkbenchService.get();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Workbench::pRenderingService_type
Workbench::getRenderingService()
{
    // HACK Hard coded to Ogre, but should get this value from a configuration
    // or something.

    Workbench::pRenderingService_type pRenderingService = Zen::Engine::Rendering::I_RenderingManager::getSingleton()
        .create("ogre");

    return pRenderingService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Workbench::pPropertiesHandle_type
Workbench::registerProperties(const std::string& _propertySetName, I_PropertiesPublisher& _publisher)
{
    return m_pMainFrame->getPropertiesView().registerProperties(_propertySetName, _publisher);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Workbench::createNewProject(const I_ProjectType& _projectType)
{
    I_ProjectServiceManager::pProjectService_type pProjectService =
        I_ProjectServiceManager::getSingleton().createProjectService(_projectType, getSelfReference());

    std::auto_ptr<wxTextEntryDialog> pTextEntryDialog(new wxTextEntryDialog(m_pMainFrame, wxT("Message"), wxT("Enter the project name")));
    if (pTextEntryDialog->ShowModal() == wxID_OK)
    {
        std::string projectName(wx2std(pTextEntryDialog->GetValue()));

        // Verify that this project name can be a valid path.
        boost::filesystem::path projectPath =
            boost::filesystem::system_complete(getWorkbenchService().getWorkspacePath() / projectName);

        if (projectPath.is_complete())
        {
            I_ProjectExplorerController& controller = m_pMainFrame->getProjectExplorer();

            if (!boost::filesystem::exists(projectPath))
            {
                boost::filesystem::create_directories(projectPath);
            }

            I_ProjectService::pProject_type pProject = pProjectService->createProject(controller, projectName);

            // TODO Fire an event and let the controller subscribe to that event instead of doing this directly.
            controller.createNewProject(pProject);
        }
        else
        {
            // TODO Send to the system log
            wxMessageBox("The project name must be a valid directory name.  Please try again.", "Error",
                wxOK_DEFAULT, m_pMainFrame);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Workbench::installContributor(Plugins::I_ExtensionPoint::pExtension_type _pExtension, const std::string& _className)
{
    I_ContributorServiceFactory* pFactory = dynamic_cast<I_ContributorServiceFactory*>
        (&_pExtension->getClassFactory());

    if (pFactory)
    {
        I_ContributorServiceFactory::pContributorService_type
            pService = pFactory->create(_className, *this);

        m_contributors.push_back(pService);
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Workbench::installContributors()
{
    Plugins::I_ExtensionRegistry::pExtensionPoint_type pExtensionPoint = Plugins::I_ExtensionRegistry::getSingleton().getExtensionPoint(I_ContributorService::getNamespace(), I_ContributorService::getExtensionPointName());

    // Get the Explorer Node Action extensions


    struct ExtensionVisitor
    :   public Plugins::I_ExtensionPoint::I_ExtensionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(Plugins::I_ExtensionPoint::pExtension_type _pExtension)
        {
            const Plugins::I_ConfigurationElement& config = _pExtension->getConfigurationElement();

            std::string name = config.getName();

            Plugins::I_ConfigurationElement const * const pProjectConfig = config.getChild("contributor-service");

            if (pProjectConfig)
            {
                m_workbench.installContributor(_pExtension, pProjectConfig->getAttribute("class"));
            }
            else
            {
                // TODO Error?
            }
        }

        virtual void end()
        {
        }

        ExtensionVisitor(Workbench& _workbench)
        :   m_workbench(_workbench)
        {}

        Workbench&              m_workbench;
    };

    ExtensionVisitor extensionVisitor(*this);
    pExtensionPoint->getExtensions(extensionVisitor);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Workbench::activateContributors()
{
    for(ContributorServices_type::iterator iter = m_contributors.begin(); iter != m_contributors.end(); iter++)
    {
        (*iter)->enableActions();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_View&
Workbench::createView(const std::string& _viewType, pDocument_type _pDocument, const int _instance)
{
    ViewFactories_type::iterator iter = m_viewFactories.find(_viewType);

    if(iter != m_viewFactories.end())
    {
        View* pView = iter->second(_viewType, _pDocument, _instance);

        std::stringstream name;
        name << _viewType << "::" << _pDocument->getLongName() << "::" << _instance;

        m_views[name.str()] = pView;

        // TODO The page name should be "short name : instance" in the cases where
        // instance != 0.
        m_pMainFrame->getWorkspaceNotebook().addPage((I_View*)pView, _pDocument->getShortName());

        return *pView;
    }
    else
    {
        std::stringstream errorMessage;
        errorMessage << "Error, " << _viewType << " is not a valid view type.";
        throw Utility::runtime_exception(errorMessage.str());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
View*
Workbench::createSpreadSheetView(const std::string& _viewType, pDocument_type _pDocument, const int _instance)
{
    return new SpreadSheetView(m_pMainFrame->getWorkspaceNotebook().getWindow(), this, _pDocument, _instance);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
View*
Workbench::createOgreSceneView(const std::string& _viewType, pDocument_type _pDocument, const int _instance)
{
    return new SceneViewWidget(m_pMainFrame->getWorkspaceNotebook().getWindow(), this, _pDocument, _instance);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
