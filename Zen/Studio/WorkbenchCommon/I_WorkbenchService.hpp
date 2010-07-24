//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio Workbench Framework
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
#ifndef ZEN_STUDIO_WORKBENCH_I_WORKBENCH_SERVICE_HPP_INCLUDED
#define ZEN_STUDIO_WORKBENCH_I_WORKBENCH_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/Networking/I_Endpoint.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetDocument.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetCell.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SceneModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <boost/filesystem.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ProjectExplorerModel;
class I_ProjectExplorerController;
class I_ExplorerNode;

/// Workbench Service interface.
/// See this discussion for more details about this design.
/// http://www.indiezen.org/wiki/irclogs/2009/03/19/#UTC2009-03-19T14:27:25
/// @see I_ApplicationServer::getApplication() to get an instance of an
///     implementation of this interface.
class WORKBENCHCOMMON_DLL_LINK I_WorkbenchService
:   public Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>                 pEndpoint_type;

	typedef Memory::managed_ptr<I_WorkbenchService>			            pService_type;

    typedef Memory::managed_ptr<I_ProjectExplorerModel>                 pProjectExplorerModel_type;
    typedef Memory::managed_weak_ptr<I_ProjectExplorerModel>            wpProjectExplorerModel_type;
    typedef Event::future_return_value<pProjectExplorerModel_type>      FutureProjectExplorerModel_type;
    typedef Memory::managed_ptr<FutureProjectExplorerModel_type>        pFutureProjectExplorerModel_type;

    typedef Memory::managed_ptr<I_ProjectExplorerController>            pProjectExplorerController_type;
    typedef Memory::managed_weak_ptr<I_ProjectExplorerController>       wpProjectExplorerController_type;
    typedef Event::future_return_value<pProjectExplorerController_type> FutureProjectExplorerController_type;
    typedef Memory::managed_ptr<FutureProjectExplorerController_type>   pFutureProjectExplorerController_type;

    typedef Memory::managed_ptr<I_SpreadSheetDocument>                  pSpreadSheetDocument_type;
    typedef Memory::managed_ptr<I_SceneModel>                           pSceneModel_type;

    typedef I_ExplorerNode*                                             pExplorerNode_type;

    typedef Zen::Database::I_DatabaseService::pDatabaseConnection_type  pDatabaseConnection_type;
    /// @}

    /// @name I_WorkbenchService interface
    /// @{
public:
    /// Set the path to the folder for this workspace.
    /// This also verifies that the control path (./workbench) exists.
    /// @return true if the path and the control path are valid.
    virtual bool setWorkspacePath(const boost::filesystem::path& _path) = 0;

    /// Get the path to the root folder for this workspace.
    virtual const boost::filesystem::path& getWorkspacePath() = 0;

    /// Get the control path for this workbench.
    virtual const boost::filesystem::path& getControlPath() = 0;

    /// Get the database connection for this workbench.
    virtual pDatabaseConnection_type getDatabaseConnection() = 0;

    /// Create a project explorer model.
    virtual pFutureProjectExplorerModel_type createProjectExplorerModel() = 0;

    /// Create a project controller that controls a model.
    virtual pFutureProjectExplorerController_type create(I_ProjectExplorerModel& _model) = 0;

    /// Create a SpreadSheet document.
    /// This will create an empty SpreadSheet document.
    /// @param _pNode Node at which the document is created.  If the document already
    ///     exists then it will be opened.
    /// @param _pProject The project associated with this document.  NULL is allowed
    ///     if this document is not associated with a project.
    /// @todo Instead of _pUserData, pass a string that is the extension name of the
    ///     document user data.
    /// @todo Should this be a generic createDocument with _pNode and extension names
    ///     for the document and the document user data?  I'm beginning to think yes.
    virtual pSpreadSheetDocument_type createSpreadSheetDocument(pExplorerNode_type _pNode, I_Project* _pProject, I_SpreadSheetDocument::pUserData_type _pUserData) = 0;

    /// Create a scene model.
    /// This creates an empty Scene model.
    /// @param _pProject The project associated with this scene model.  NULL is allowed
    ///     if this model is not associated with a project.
    virtual pSceneModel_type createSceneModel(pExplorerNode_type _pNode, I_Project* _pProject, I_SceneModel::pUserData_type _pUserData) = 0;

    /// Create the user data associated with a given explorer node.
    virtual I_ExplorerNode::pUserData_type createNodeUserData(boost::uint64_t _explorerNodeId, const std::string& _nodeType, I_ExplorerNode& _parentNode) = 0;

    /// Create the user data associated with a given SpreadSheetCell.
    virtual I_SpreadSheetCell::pUserData_type createSpreadSheetCellUserData(I_SpreadSheetDocument& _document, boost::uint64_t _spreadSheetCellId, const std::string& _cellType, int _row, int _column) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WorkbenchService();
    virtual ~I_WorkbenchService();
    /// @}

};  // interface I_WorkbenchService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory
{
    /// I_WorkbenchService is managed by a factory
    template<>
    struct is_managed_by_factory<Studio::Workbench::I_WorkbenchService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_STUDIO_WORKBENCH_I_WORKBENCH_SERVICE_HPP_INCLUDED
