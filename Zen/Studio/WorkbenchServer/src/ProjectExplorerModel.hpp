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
#ifndef ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_MODEL_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_MODEL_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDataCollection.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/future_return_value.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class ProjectExplorerNode;
class WorkbenchService;

class ProjectExplorerModel
:   public I_ProjectExplorerModel
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, pProject_type>    Projects_type;

    /// Not a managed pointer since it's really just used as a reference.
    typedef Event::future_return_value<I_ExplorerNode*>     FutureExplorerNode_type;
    typedef Memory::managed_ptr<FutureExplorerNode_type>    pFutureExplorerNode_type;

    typedef I_ExplorerNode*                         pExplorerNode_type;

    /// Set of weak pointers to views.
    typedef std::set<pExplorerView_type>            ExplorerViews_type;
    /// @}

    /// @name I_ExplorerModel implementation
    /// @{
public:
    virtual I_ExplorerNode& getRootNode();
    /// @}

    /// @name I_ProjectExplorerModel interface
    /// @{
public:
    virtual pSubscription_type subscribe(pExplorerView_type _pView, pFilter_type _pFilter = pFilter_type());
    /// @}

    /// @name ProjectExplorerModel implementation
    /// @{
public:
    void createNewProject(pProject_type _pProject);

    pFutureExplorerNode_type createChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData);

    /// Load an existing project from the database.
    /// @see WorkbenchService::createProjectExplorerModel() for the sequence of related calls.
    void loadExistingProject(pProject_type _pProject, I_ExplorerNodeDataMap::pExplorerNodeDomainObject_type _pExplorerNodeDO);

    /// Load the existing child nodes from the database.
    /// This method is recursive and indirectly calls itself for each grandchildren.
    /// @see WorkbenchService::createProjectExplorerModel() for the sequence of related calls.
    void loadExistingChildren(pExplorerNode_type _pParentNode);

    /// Load a child node.
    /// The node was loaded from the database.
    pExplorerNode_type loadChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData, I_ExplorerNodeDataCollection::pExplorerNodeDomainObject_type _pNodeDO);

    /// Insert an explorer node in the data model.
    /// @return explorerNodeId of the new node.
    boost::int64_t insertExplorerNode(const std::string& _nodeType, boost::uint64_t _parentNodeId);

    void unSubscribe(pExplorerView_type _pView);

    /// Fire the onAddProject notification for all subscribed views.
    void onAddProject(pProject_type _pProject);

    /// Fire the onAddNode notification for all subscribed views.
    void onAddNode(pExplorerNode_type _pNode);

    /// Fire the onAddNode notification for all of the children of
    /// the specified node.  
    /// @precondition m_pViewsGuard is assumed to be locked.
    void notifyViewOfChildren(pExplorerView_type _pView, I_ExplorerNode& _node);

    /// Load the data from the database.
    /// @see WorkbenchService::createProjectExplorerModel() for the sequence of related calls.
    void loadDataModel();

    /// This method is called by ProjectExplorerNode and notifies the model
    /// when a node has been modified.
    void onNodeModified(I_ExplorerNode* _pNode);

    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class WorkbenchService;
    /// Constructor for the ProjectExplorerModel.
    /// @see WorkbenchService::createProjectExplorerModel() for the sequence of related calls.
    explicit ProjectExplorerModel(WorkbenchService& _workbenchService);
    virtual ~ProjectExplorerModel();
    /// @}

    /// @name Inner Types
    /// @{
public:
    /// @}

    /// @name Member Variables
    /// @{
private:
    WorkbenchService&                       m_workbenchService;
    Projects_type                           m_projects;

    /// Views subscribed to this model.
    ExplorerViews_type                      m_views;

    /// Guard for m_views.
    Threading::I_Mutex*                     m_pViewsGuard;
    /// @}
};  // class ProjectExplorerModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_MODEL_HPP_INCLUDED
