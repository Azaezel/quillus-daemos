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
#ifndef ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_CONTROLLER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_CONTROLLER_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_ProjectExplorerModel;
class WorkbenchService;
class ProjectExplorerModel;

class ProjectExplorerController
:   public I_ProjectExplorerController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ProjectExplorerController implementation
    /// @{
public:
    virtual I_WorkbenchService& getWorkbenchService() const;
    virtual void createNewProject(pProject_type _pProject);
    virtual void loadExistingProject(pProject_type _pProject, I_ExplorerNodeDataMap::pExplorerNodeDomainObject_type _pExplorerNodeDO);
    virtual pFutureExplorerNode_type createChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData);
    virtual void graft(I_ExplorerNode& _node, I_ExplorerNode& _parent);
    /// @}

    /// @name ProjectExplorerController implementation
    /// @{
public:
    ProjectExplorerModel& getModel() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit ProjectExplorerController(WorkbenchService& _workbenchService, I_ProjectExplorerModel& _model);
    virtual ~ProjectExplorerController();
    /// @}

    /// @name Member Variables
    /// @{
private:
    WorkbenchService&                           m_workbenchService;
    ProjectExplorerModel&                       m_model;
    /// @}

};  // class ProjectExplorerController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_CONTROLLER_HPP_INCLUDED
