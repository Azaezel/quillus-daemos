//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio Workbench Framework
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
#ifndef ZEN_STUDIO_WORKBENCH_SERVER_WORKBENCH_SERVICE_HPP_INCLUDED
#define ZEN_STUDIO_WORKBENCH_SERVER_WORKBENCH_SERVICE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>

#include <Zen/Enterprise/Database/I_DatabaseManager.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

#include <Zen/Studio/WorkbenchModel/I_ProjectDataCollection.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;

class WorkbenchService
:   public Zen::Studio::Workbench::I_WorkbenchService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Database::I_DatabaseManager::pDatabaseService_type         pDatabaseService_type;

    typedef Zen::Plugins::I_ConfigurationElement::const_ptr_type            pConfig_type;
    typedef I_ProjectDataCollection::pProjectDomainObject_type              pProjectDomainObject_type;

    typedef Memory::managed_ptr<I_Project>                                  pProject_type;
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual Zen::Threading::I_Condition* prepareToStart(Zen::Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Zen::Threading::I_Condition* prepareToStop();
    virtual void stop();
    /// @}

    /// @name I_RequestHandler implementation
    /// @{
public:
    virtual void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    /// @}

    /// @name I_ApplicationService implementation
    /// @{
public:
    virtual Zen::Enterprise::AppServer::I_ApplicationServer& getApplicationServer();
    virtual pResourceLocation_type getServiceLocation();
    virtual void handleMessage(pMessage_type _pMessage);
    /// @}

    /// @name I_WorkbenchService implementation
    /// @{
public:
    virtual bool setWorkspacePath(const boost::filesystem::path& _controlPath);
    virtual const boost::filesystem::path& getWorkspacePath();
    virtual const boost::filesystem::path& getControlPath();
    virtual pDatabaseConnection_type getDatabaseConnection();

    /// This method creates a ProjectExplorerModel and then loads
    /// the data by calling ProjectExplorerModel::loadDataModel(), which
    /// iterates through the Project database records and calls queueProjectLoad() for
    /// each record.  After all of the records have been queued, createProjectExplorerModel()
    /// then calls loadProjects() which iterates through the queued Project database records
    /// and uses the extension registry to construct the appropriate extension that implements
    /// I_Project.
    /// Then the I_Project's model ProjectExplorerModel::loadExistingProject() is called
    /// to load all of the child explorer nodes which in turn calls ProjectExplorerModel::loadExistingChildren().
    /// @see ProjectExplorerModel::loadDataModel()
    /// @see WorkbenchService::queueProjectLoad()
    /// @see WorkbenchService::loadProjects()
    /// @see ProjectExplorerModel::loadExistingProject()
    /// @see ProjectExplorerModel::loadExistingChildren()
    virtual pFutureProjectExplorerModel_type createProjectExplorerModel();

    virtual pFutureProjectExplorerController_type create(I_ProjectExplorerModel& _model);
    virtual pSpreadSheetDocument_type createSpreadSheetDocument(pExplorerNode_type _pNode, I_Project* _pProject, I_SpreadSheetDocument::pUserData_type _pUserData);
    virtual pSceneModel_type createSceneModel(pExplorerNode_type _pNode, I_Project* _pProject, I_SceneModel::pUserData_type _pUserData);
    virtual I_ExplorerNode::pUserData_type createNodeUserData(boost::uint64_t _explorerNodeId, const std::string& _nodeType, I_ExplorerNode& _parentNode);
    virtual I_SpreadSheetCell::pUserData_type createSpreadSheetCellUserData(I_SpreadSheetDocument& _document, boost::uint64_t _spreadSheetCellId, const std::string& _cellType, int _row, int _column);
    /// @}

	/// @name WorkbenchService implementation
	/// @{
public:
    static void destroyModel(wpProjectExplorerModel_type _pModel);
    static void destroyController(wpProjectExplorerController_type _pController);

private:
    friend class ProjectExplorerModel;
    /// Queue a project to be loaded from the database.
    /// This is called by ProjectExplorerModel::loadDataModel()
    /// @see WorkbenchService::createProjectExplorerModel() for the sequence of related calls.
    void queueProjectLoad(pProjectDomainObject_type _pDomainObject);

    /// Load all of the queued projects.
    /// This is called by WorkbenchService::createProjectExplorerModel()
    /// after calling ProjectExplorerModel::loadDataModel().
    /// @see WorkbenchService::createProjectExplorerModel() for the sequence of related calls.
    void loadProjects();

private:
    /// Destroy a spreadsheet document.
    void destroySpreadSheetDocument(Memory::managed_weak_ptr<I_SpreadSheetDocument> _wpDocument);
    void destroySceneModel(Memory::managed_weak_ptr<I_SceneModel> _wpDocument);
	/// @}

    /// @name 'Structors
    /// @{
protected:
    friend class WorkbenchServiceFactory;
             WorkbenchService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    virtual ~WorkbenchService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Enterprise::AppServer::I_ApplicationServer&                m_appServer;
    pResourceLocation_type                                          m_pLocation;
    Zen::Threading::ThreadPool*                                     m_pThreadPool;

    /// Path to the workbench control directory where the SQLite database
    /// and other information is stored.
    boost::filesystem::path                                         m_controlPath;

    /// Path to the root workspace directory.
    boost::filesystem::path                                         m_workspacePath;

    /// Main database service for the workspace.
    pDatabaseService_type                                           m_pDatabase;

    /// Main database connection for the workspace.
    pDatabaseConnection_type                                        m_pConnection;

    pProjectExplorerModel_type                                      m_pProjectExplorerModel;
    pProjectExplorerController_type                                 m_pProjectExplorerController;

    typedef std::list<pProjectDomainObject_type>                    ProjectsToLoad_type;

    /// These are projects that are in the database but have not been fully loaded
    /// yet.
    ProjectsToLoad_type                                             m_projectsToLoad;
    /// @}

};  // class WorkbenchService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_STUDIO_WORKBENCH_SERVER_WORKBENCH_SERVICE_HPP_INCLUDED
