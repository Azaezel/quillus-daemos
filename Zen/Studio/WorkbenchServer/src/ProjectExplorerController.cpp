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
#include "ProjectExplorerController.hpp"
#include "ProjectExplorerModel.hpp"
#include "WorkbenchService.hpp"
#include "ProjectExplorerNode.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/ThreadPool.hpp>

#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <Zen/Enterprise/Database/I_DatabaseStaticQuery.hpp>
#include <Zen/Enterprise/Database/I_DatabaseResult.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDomainObject.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerController::ProjectExplorerController(WorkbenchService& _workbenchService, I_ProjectExplorerModel& _model)
:   m_workbenchService(_workbenchService)
,   m_model(*dynamic_cast<ProjectExplorerModel*>(&_model))
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerController::~ProjectExplorerController()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WorkbenchService&
ProjectExplorerController::getWorkbenchService() const
{
    return m_workbenchService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerController::pFutureExplorerNode_type
ProjectExplorerController::createChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData)
{
    return getModel().createChildNode(_parent, _pData);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerController::graft(I_ExplorerNode& _node, I_ExplorerNode& _parent)
{
    throw Zen::Utility::runtime_exception("ProjectExplorerController::graft(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerController::createNewProject(pProject_type _pProject)
{
    getModel().createNewProject(_pProject);

    _pProject->onCreated();
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerController::loadExistingProject(pProject_type _pProject, I_ExplorerNodeDataMap::pExplorerNodeDomainObject_type _pExplorerNodeDO)
{
    getModel().loadExistingProject(_pProject, _pExplorerNodeDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerModel&
ProjectExplorerController::getModel() const
{
    return m_model;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
