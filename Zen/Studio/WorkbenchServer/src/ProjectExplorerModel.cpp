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
#include "ProjectExplorerModel.hpp"
#include "ProjectExplorerNode.hpp"
#include "WorkbenchService.hpp"

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerView.hpp>

#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDataCollection.hpp>

#include <Zen/Studio/WorkbenchModel/I_ProjectDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_ProjectDataMap.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerModel::ProjectExplorerModel(WorkbenchService& _workbenchService)
:   m_workbenchService(_workbenchService)
,   m_pViewsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerModel::~ProjectExplorerModel()
{
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef subscription<ProjectExplorerModel*, ProjectExplorerModel::pExplorerView_type> Subscription;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription* pSubscription = dynamic_cast<Subscription*>(_wpSubscription.get());

    if (pSubscription)
    {
        delete pSubscription;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerModel::pSubscription_type
ProjectExplorerModel::subscribe(pExplorerView_type _pView, pFilter_type _pFilter)
{
    Threading::CriticalSection lock(m_pViewsGuard);

    m_views.insert(_pView);

    // This view needs to be notified of all of the existing model.
    for(Projects_type::iterator iter = m_projects.begin(); iter != m_projects.end(); iter++)
    {
        // Notify the view of a project
        _pView->onAddProject(iter->second);

        // Also notify the view of all child nodes.
        notifyViewOfChildren(_pView, *iter->second->getNode());
    }

    return pSubscription_type(new Subscription(this, _pView), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::unSubscribe(pExplorerView_type _pView)
{
    Threading::CriticalSection lock(m_pViewsGuard);
    m_views.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::createNewProject(pProject_type _pProject)
{
    ProjectExplorerNode* pNewNode = 
        new ProjectExplorerNode(*this, _pProject.as<I_ExplorerNode::pUserData_type>());

    boost::uint64_t newNodeId = insertExplorerNode(_pProject->getType().getName(), 0);

    pNewNode->setNodeId(newNodeId);

    // Get the data map.
    I_ProjectDataMap::pProjectDataMap_type pProjectDM =
        I_ProjectDataMap::create(m_workbenchService.getDatabaseConnection());

    // Create a new domain object.
    I_ProjectDataMap::pProjectDomainObject_type pProjectDO =
        pProjectDM->createNew();

    // Populate the values.
    pProjectDO->getName()           = _pProject->getNode()->getDisplayName();
    pProjectDO->getExplorerNodeId() = newNodeId;

    // Update the object (and in this case, insert it).
    pProjectDM->update(pProjectDO);

    _pProject->setProjectId(pProjectDO->getProjectId());

    onAddProject(_pProject);

    _pProject->insert(m_workbenchService.getDatabaseConnection());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::loadExistingProject(pProject_type _pProject, I_ExplorerNodeDataMap::pExplorerNodeDomainObject_type _pExplorerNodeDO)
{
    ProjectExplorerNode* pNewNode = 
        new ProjectExplorerNode(*this, _pProject.as<I_ExplorerNode::pUserData_type>());

    pNewNode->setNodeId(_pExplorerNodeDO->getExplorerNodeId());

    loadExistingChildren(pNewNode);

    _pProject->load(m_workbenchService.getDatabaseConnection());

    onAddProject(_pProject);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::loadExistingChildren(pExplorerNode_type _pParentNode)
{
    // Get the explorer node data map.
    I_ExplorerNodeDataMap::pExplorerNodeDataMap_type pExplorerNodeDM =
        I_ExplorerNodeDataMap::create(m_workbenchService.getDatabaseConnection());

    I_ExplorerNodeDataMap::pFutureExplorerNodeDataCollection_type 
        pChildrenDC = pExplorerNodeDM->getChildren(_pParentNode->getNodeId());

    struct NodeVisitor 
    :   public I_ExplorerNodeDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(I_ExplorerNodeDataCollection::pExplorerNodeDomainObject_type _pDomainObject)
        {
            boost::uint64_t explorerNodeId = _pDomainObject->getExplorerNodeId();
            std::string nodeType = _pDomainObject->getNodeType();
            boost::uint64_t parentNodeId = _pDomainObject->getParentNodeId();

            // Get the user data for this node from the workbench.
            // This involves loading an extension, and so we leave it to the
            // workbench to handle it correctly.
            I_ExplorerNode::pUserData_type pUserData =             
                m_workbenchService.createNodeUserData(explorerNodeId, nodeType, m_parentNode);

            // ProjectExplorerModel will correctly construct the node
            // and fire the appropriate events.
            pExplorerNode_type pNode = m_model.loadChildNode(m_parentNode, pUserData, _pDomainObject);

            // This is a recursive process, so decend to the next level.
            m_model.loadExistingChildren(pNode);
        }

        virtual void end()
        {
        }

        NodeVisitor(ProjectExplorerModel& _model, WorkbenchService& _workbenchService, pExplorerNode_type _pParentNode)
        :   m_model(_model)
        ,   m_workbenchService(_workbenchService)
        ,   m_parentNode(*_pParentNode)
        {
        }

    private:
        ProjectExplorerModel&                   m_model;
        WorkbenchService&                       m_workbenchService;
        I_ExplorerNode&                         m_parentNode;
    };

    NodeVisitor visitor(*this, m_workbenchService, _pParentNode);

    pChildrenDC->getValue()->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerModel::pFutureExplorerNode_type
ProjectExplorerModel::createChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData)
{
    ProjectExplorerNode* pParent = dynamic_cast<ProjectExplorerNode*>(&_parent);

    if (pParent == NULL)
    {
        throw Utility::runtime_exception("ProjectExplorerModel::createChildNode(): Error, parent is wrong type.");
    }

    ProjectExplorerNode* pNewNode = new ProjectExplorerNode(*this, _pData, pParent);

    boost::uint64_t newNodeId = insertExplorerNode(_pData->getType().getName(), _parent.getNodeId());

    pNewNode->setNodeId(newNodeId);

    onAddNode(pNewNode);

    pFutureExplorerNode_type returnValue(new FutureExplorerNode_type);
    returnValue->setValue(pNewNode);

    _pData->insert(m_workbenchService.getDatabaseConnection());

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectExplorerModel::pExplorerNode_type
ProjectExplorerModel::loadChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData, I_ExplorerNodeDataCollection::pExplorerNodeDomainObject_type _pNodeDO)
{
    ProjectExplorerNode* pParent = dynamic_cast<ProjectExplorerNode*>(&_parent);

    if (pParent == NULL)
    {
        throw Utility::runtime_exception("ProjectExplorerModel::loadChildNode(): Error, parent is wrong type.");
    }

    ProjectExplorerNode* pNewNode = new ProjectExplorerNode(*this, _pData, pParent);

    boost::uint64_t explorerNodeId = _pNodeDO->getExplorerNodeId();
    std::string nodeType = _pNodeDO->getNodeType();
    boost::uint64_t parentNodeId = _pNodeDO->getParentNodeId();

    pNewNode->setNodeId(explorerNodeId);

    // Load from the database before firing the onAddNode() event.
    // This gives the node a chance to populate additional before the event
    // handler(s) display it.
    _pData->load(m_workbenchService.getDatabaseConnection());

    onAddNode(pNewNode);

    return pNewNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ExplorerNode&
ProjectExplorerModel::getRootNode()
{
    throw Zen::Utility::runtime_exception("Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::int64_t
ProjectExplorerModel::insertExplorerNode(const std::string& _nodeType, boost::uint64_t _parentNodeId)
{
    // Get the data map.
    I_ExplorerNodeDataMap::pExplorerNodeDataMap_type pExplorerNodeDM =
        I_ExplorerNodeDataMap::create(m_workbenchService.getDatabaseConnection());

    // Create a new domain object.
    I_ExplorerNodeDataMap::pExplorerNodeDomainObject_type pExplorerNodeDO =
        pExplorerNodeDM->createNew();

    // Populate the values.
    pExplorerNodeDO->getNodeType()      = _nodeType;
    pExplorerNodeDO->getParentNodeId()  = _parentNodeId;

    // Update the object (and in this case, insert it).
    return pExplorerNodeDM->update(pExplorerNodeDO)->getValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::onAddProject(pProject_type _pProject)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    ExplorerViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(ExplorerViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }

        m_projects[_pProject->getNode()->getDisplayName()] = _pProject;
    }

    for(ExplorerViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onAddProject(_pProject);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::onAddNode(pExplorerNode_type _pNode)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    ExplorerViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(ExplorerViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }

    for(ExplorerViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onAddNode(*_pNode);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::notifyViewOfChildren(pExplorerView_type _pView, I_ExplorerNode& _node)
{
    struct ExplorerNodeVisitor
    : public I_ExplorerNode::I_ExplorerNodeVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(I_ExplorerNode& _node)
        {
            m_pView->onAddNode(_node);

            m_model.notifyViewOfChildren(m_pView, _node);
        }

        virtual void end()
        {
        }

        ExplorerNodeVisitor(pExplorerView_type _pView, ProjectExplorerModel& _model)
        :   m_pView(_pView)
        ,   m_model(_model)
        {
        }

    private:
        pExplorerView_type      m_pView;
        ProjectExplorerModel&   m_model;

    };  // struct ExplorerNodeVisitor

    ExplorerNodeVisitor visitor(_pView, *this);
    _node.getChildren(visitor);
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::loadDataModel()
{
    // Get the data map.
    I_ProjectDataMap::pProjectDataMap_type pProjectDM =
        I_ProjectDataMap::create(m_workbenchService.getDatabaseConnection());

    I_ProjectDataMap::pFutureProjectDataCollection_type pProjectDC =
        pProjectDM->getAll();

    struct ProjectVisitor
    :   public I_ProjectDataCollection::I_CollectionVisitor
    {
        typedef I_ProjectDataCollection::pProjectDomainObject_type  pProjectDomainObject_type;
        typedef Memory::managed_ptr<I_Project>                      pProject_type;

        virtual void begin()
        {
        }

        virtual void visit(pProjectDomainObject_type _pDomainObject)
        {
            // Queue the project to be loaded.
            m_workbench.queueProjectLoad(_pDomainObject);
        }

        virtual void end()
        {
            delete this;
        }

        ProjectVisitor(WorkbenchService& m_workbench)
        :   m_workbench(m_workbench)
        {
        }

    private:
        WorkbenchService&   m_workbench;
    };  // struct ProjectVisitor

    ProjectVisitor* pProjectVisitor = new ProjectVisitor(m_workbenchService);
    pProjectDC->getValue()->getAll(*pProjectVisitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectExplorerModel::onNodeModified(Zen::Studio::Workbench::I_ExplorerNode* _pNode)
{
    ModifiedNodeEvent(_pNode);

    // Tell the node's user data to save.
    _pNode->getUserData()->save(m_workbenchService.getDatabaseConnection());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
