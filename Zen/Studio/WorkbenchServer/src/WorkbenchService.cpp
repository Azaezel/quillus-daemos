//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio Workbench Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#include "WorkbenchService.hpp"

#include "ProjectExplorerModel.hpp"
#include "ProjectExplorerController.hpp"

#include "SpreadSheetDocument.hpp"
#include "SceneModel.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_ExtensionPoint.hpp>
#include <Zen/Core/Plugins/I_Extension.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeFactory.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCellFactory.hpp>

#include <Zen/Studio/WorkbenchProtocol/I_WorkbenchProtocolManager.hpp>

#include <Zen/Studio/WorkbenchModel/I_ProjectDomainObject.hpp>

#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDomainObject.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkbenchService::WorkbenchService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   m_appServer(_appServer)
,   m_pThreadPool(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkbenchService::~WorkbenchService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
WorkbenchService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    Zen::Studio::Workbench::I_WorkbenchProtocolManager::getSingleton().install(m_appServer);

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::start()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
WorkbenchService::prepareToStop()
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::handleMessage(pMessage_type _pMessage)
{
    // TODO Derive from scriptable_generic_service 
    throw Utility::runtime_exception("WorkbenchService::handleMessage(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    // TODO Derive from scriptable_generic_service 
    throw Utility::runtime_exception("WorkbenchService::handleMessage(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer&
WorkbenchService::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
WorkbenchService::setWorkspacePath(const boost::filesystem::path& _workspacePath)
{
    m_workspacePath = _workspacePath;
    m_controlPath = m_workspacePath / ".workbench";

    // Connect to the database
    boost::filesystem::path dbPath = m_controlPath / "workbench.sqlite";
    if (!boost::filesystem::exists(dbPath))
    {
        // TODO Create it.
        return false;
    }

    Zen::Database::I_DatabaseManager::config_type config;

    m_pDatabase = Zen::Database::I_DatabaseManager::getSingleton().createDatabaseService("sqlite", config);

    if (!m_pDatabase.isValid())
    {
        return false;
    }

    // Connect to the SQLite database
    config["fileName"] = dbPath.string();
    m_pConnection = m_pDatabase->connect("workbench", config);

    // Make sure the connection is valid
    if (!m_pConnection.isValid())
    {
        return false;
    }

    // Set the workspace path to the the

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::filesystem::path&
WorkbenchService::getWorkspacePath()
{
    return m_workspacePath;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::filesystem::path&
WorkbenchService::getControlPath()
{
    return m_controlPath;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkbenchService::pDatabaseConnection_type
WorkbenchService::getDatabaseConnection()
{
    return m_pConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::destroyModel(wpProjectExplorerModel_type _pModel)
{
    ProjectExplorerModel* pModel = dynamic_cast<ProjectExplorerModel*>(_pModel.get());
    if (pModel != NULL)
    {
        delete pModel;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkbenchService::pFutureProjectExplorerModel_type
WorkbenchService::createProjectExplorerModel()
{
    pFutureProjectExplorerModel_type pModel(new FutureProjectExplorerModel_type);

    ProjectExplorerModel* pRawModel = new ProjectExplorerModel(*this);

    m_pProjectExplorerModel = pProjectExplorerModel_type(pRawModel, &destroyModel);
    pModel->setValue(m_pProjectExplorerModel);

    // This will populate m_projectsToLoad by calling loadProject().
    pRawModel->loadDataModel();

    create(*pRawModel);

    // TODO Probably shouldn't share a database connection with the
    // data model because calling this next method might be done
    // before loadDataModel() is complete, which might cause us
    // to try to create a new transaction before the one that
    // loadDataModel() creates is done.
    // For now this is ok because the current database plugin
    // implementation is single threaded.
    loadProjects();

    return pModel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::destroyController(wpProjectExplorerController_type _pController)
{
    ProjectExplorerController* pController = dynamic_cast<ProjectExplorerController*>(_pController.get());
    if (pController != NULL)
    {
        delete pController;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkbenchService::pFutureProjectExplorerController_type
WorkbenchService::create(I_ProjectExplorerModel& _model)
{
    pFutureProjectExplorerController_type pController(new FutureProjectExplorerController_type);

    if (m_pProjectExplorerController == NULL)
    {
        m_pProjectExplorerController = pProjectExplorerController_type(new ProjectExplorerController(*this, _model), &destroyController);
    }

    pController->setValue(m_pProjectExplorerController);

    return pController;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkbenchService::pSpreadSheetDocument_type
WorkbenchService::createSpreadSheetDocument(pExplorerNode_type _pNode, I_Project* _pProject, I_SpreadSheetDocument::pUserData_type _pUserData)
{
    // TODO The primary key for the document is _pNode->getNodeId();
    // TODO Cache the document so that a duplicate document is not created.
    SpreadSheetDocument* pRawDocument = new SpreadSheetDocument(_pProject, _pUserData);

    // TODO This should return a future so that the load() can be done in the background
    pSpreadSheetDocument_type pDocument(pRawDocument, boost::bind(&WorkbenchService::destroySpreadSheetDocument, this, _1));

    pRawDocument->load(_pNode);

    return pDocument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::destroySpreadSheetDocument(Memory::managed_weak_ptr<I_SpreadSheetDocument> _wpDocument)
{
    SpreadSheetDocument* pRawDocument = dynamic_cast<SpreadSheetDocument*>(_wpDocument.get());

    if (pRawDocument)
    {
        delete pRawDocument;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorkbenchService::pSceneModel_type
WorkbenchService::createSceneModel(pExplorerNode_type _pNode, I_Project* _pProject, I_SceneModel::pUserData_type _pUserData)
{
    // TODO The primary key for the document is _pNode->getNodeId();
    // TODO Cache the document so that a duplicate document is not created.
    SceneModel* pRawDocument = new SceneModel(*this, _pProject, _pUserData);

    // TODO This should return a future so that the load() can be done in the background
    pSceneModel_type pDocument(pRawDocument, boost::bind(&WorkbenchService::destroySceneModel, this, _1));

    pRawDocument->load(_pNode);

    return pDocument;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::destroySceneModel(Memory::managed_weak_ptr<I_SceneModel> _wpDocument)
{
    SceneModel* pRawDocument = dynamic_cast<SceneModel*>(_wpDocument.get());

    if (pRawDocument)
    {
        delete pRawDocument;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::queueProjectLoad(pProjectDomainObject_type _pDomainObject)
{
    m_projectsToLoad.push_back(_pDomainObject);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorkbenchService::loadProjects()
{
    // Get the explorer node data map.
    I_ExplorerNodeDataMap::pExplorerNodeDataMap_type pExplorerNodeDM =
        I_ExplorerNodeDataMap::create(getDatabaseConnection());

    // Loop through all of the projects.
    for(ProjectsToLoad_type::iterator iter = m_projectsToLoad.begin(); iter != m_projectsToLoad.end(); iter++)
    {
        // Get the data node associated with this project
        const boost::int64_t explorerNodeId = (*iter)->getExplorerNodeId();

        I_ExplorerNodeDataMap::pExplorerNodeDomainObject_type
            pExplorerNode = pExplorerNodeDM->getByKey(explorerNodeId)->getValue();

        // Find the correct extension point for this project type and load it

        Plugins::I_ExtensionRegistry::pExtensionPoint_type
            pExtensionPoint = Plugins::I_ExtensionRegistry::getSingleton().getExtensionPoint("Zen::Studio::Workbench", "ExplorerNode");

        if (pExtensionPoint.get() != NULL)
        {
            const std::string extension(pExplorerNode->getNodeType().getStringValue());
            Plugins::I_Extension::extension_ptr_type pExtension = pExtensionPoint->getExtension(extension);

            if (pExtension)
            {
                // TODO Cache this factory / extension?
                I_ExplorerNodeFactory* pFactory = dynamic_cast<I_ExplorerNodeFactory*>(&pExtension->getClassFactory());

                // Create the project.
                pProject_type pProject = pFactory->createProject(*(m_pProjectExplorerController.get()), (*iter), pExplorerNode);

                pProject->setProjectId((*iter)->getProjectId());
                pProject->setName((*iter)->getName());

                m_pProjectExplorerController->loadExistingProject(pProject, pExplorerNode);
            }
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ExplorerNode::pUserData_type
WorkbenchService::createNodeUserData(boost::uint64_t _explorerNodeId, const std::string& _nodeType, I_ExplorerNode& _parentNode)
{
    // Find the correct extension for this node user data and load it
    Plugins::I_ExtensionRegistry::pExtensionPoint_type
        pExtensionPoint = Plugins::I_ExtensionRegistry::getSingleton().getExtensionPoint("Zen::Studio::Workbench", "ExplorerNode");

    if (pExtensionPoint.get() != NULL)
    {
        Plugins::I_Extension::extension_ptr_type pExtension = pExtensionPoint->getExtension(_nodeType);

        if (pExtension)
        {
            // TODO Cache this factory / extension?
            I_ExplorerNodeFactory* pFactory = dynamic_cast<I_ExplorerNodeFactory*>(&pExtension->getClassFactory());

            // Create the project.
            return pFactory->createNodeUserData(_explorerNodeId, _nodeType, _parentNode);
        }
    }

    // TODO Error?
    std::cout << "WorkbenchService::createNodeUserData(): Error" << std::endl;
    return I_ExplorerNode::pUserData_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SpreadSheetCell::pUserData_type
WorkbenchService::createSpreadSheetCellUserData(I_SpreadSheetDocument& _document, boost::uint64_t _spreadSheetCellId, const std::string& _cellType, int _row, int _column)
{
    // Find the correct extension for this node user data and load it
    Plugins::I_ExtensionRegistry::pExtensionPoint_type
        pExtensionPoint = Plugins::I_ExtensionRegistry::getSingleton().getExtensionPoint("Zen::Studio::Workbench", "SpreadSheetCell");

    if (pExtensionPoint.get() != NULL)
    {
        Plugins::I_Extension::extension_ptr_type pExtension = pExtensionPoint->getExtension(_cellType);

        if (pExtension)
        {
            // TODO Cache this factory / extension?
            I_SpreadSheetCellFactory* pFactory = dynamic_cast<I_SpreadSheetCellFactory*>(&pExtension->getClassFactory());

            // Create the project.
            return pFactory->createSpreadSheetCell(_document, _spreadSheetCellId, _cellType, _row, _column);
        }
    }

    // TODO Error?
    return I_SpreadSheetCell::pUserData_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
