//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Project.hpp"
#include "Folder.hpp"

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ProjectExplorerModel.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_ProjectListener.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectElement.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

#include <iostream>

#include <string>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Project::Project(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, const std::string& _name)
:   Zen::Studio::Workbench::I_Project(_name)    // This must be first
,   Zen::Studio::Workbench::Project(_controller, _name)
,   GameBuilder::I_Project()
,   m_pSubscriptionsGuard(Zen::Threading::MutexFactory::create())
{
    GameObjectType::onOpened.connect(boost::bind(&Project::handleOpenedDocument, this, _1));
    GameObjectType::onClosed.connect(boost::bind(&Project::handleClosedDocument, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Project::~Project()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
Project::getType() const
{
    return I_Project::getNodeType();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::insert(pDatabaseConnection_type _pDBConn)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::load(pDatabaseConnection_type _pDBConn)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::save(pDatabaseConnection_type _pDBConn)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::remove(pDatabaseConnection_type _pDBConn)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::onCreated()
{
    /// Create the default child node
    Zen::Studio::Workbench::I_ExplorerNode::pUserData_type pData(new Folder(*this, "Game Object Types"));

    getController().createChildNode(*getNode(), pData);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Zen::Studio::Workbench::subscription<Project*, Project::pProjectListener_type>    ProjectSubscription_type;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Zen::Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    // Try the ProjectSubscription_type
    {
        ProjectSubscription_type* pSubscription = dynamic_cast<ProjectSubscription_type*>(_wpSubscription.get());

        if (pSubscription)
        {
            std::cout << "Project::unsubscribe()" << std::endl;
            delete pSubscription;
            return;
        }
    }

    // Else error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Project::pSubscription_type
Project::subscribe(pProjectListener_type _pListener, pFilter_type _pFilter)
{
    // Only the insert needs to be guarded.
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_listeners.insert(_pListener);
    }

    std::cout << "Project::subscribe()" << std::endl;

    // Notify the listener of the current state of the project.
    notifyView(_pListener);

    return pSubscription_type(new ProjectSubscription_type(this, _pListener), &destroySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::unSubscribe(pProjectListener_type _pListener)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_listeners.erase(_pListener);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace {
struct ExplorerNodeVisitor
:   public Zen::Studio::Workbench::I_ExplorerNode::I_ExplorerNodeVisitor
{
    virtual void begin();
    virtual void visit(Zen::Studio::Workbench::I_ExplorerNode& _node);
    virtual void end();

    ExplorerNodeVisitor(Project::pProjectListener_type _pListener);

private:
    Project::pProjectListener_type    m_pListener;
};  // struct ExplorerNodeVisitor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ExplorerNodeVisitor::ExplorerNodeVisitor(Project::pProjectListener_type _pListener)
:   m_pListener(_pListener)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExplorerNodeVisitor::begin()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExplorerNodeVisitor::visit(Zen::Studio::Workbench::I_ExplorerNode& _node)
{
    I_Folder* pFolder = dynamic_cast<I_Folder*>(_node.getUserData().get());
    I_GameObjectType* pGOT = dynamic_cast<I_GameObjectType*>(_node.getUserData().get());

    // If it's a GOT, tell the listener about it.
    if (pGOT != NULL)
    {
        m_pListener->gameObjectTypeCreated(*pGOT);
    }

    // If this node is a I_GameObjectType or a I_Folder, get it's children.
    if (pFolder != NULL || pGOT != NULL)
    {
        ExplorerNodeVisitor visitor(m_pListener);
        _node.getChildren(visitor);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ExplorerNodeVisitor::end()
{
}

} // namespace anonymous
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::notifyView(pProjectListener_type _pListener)
{
    // Iterate through all of the I_GameObjectType and notify the listener.

    ExplorerNodeVisitor visitor(_pListener);

    getNode()->getChildren(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::handleOpenedDocument(GameObjectTypeDocument* _pDocument)
{
    // TODO Guard
    m_documentSubscriptions[_pDocument] = _pDocument->subscribe(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::handleClosedDocument(GameObjectTypeDocument* _pDocument)
{
    // TODO Guard
    m_documentSubscriptions.erase(_pDocument);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::onDocumentModified(I_GameObjectTypeDocument& _gameObjectTypeDocument)
{
    std::cout << "Project::onDocumentModified()" << std::endl;
    notifyViewsOfChange(_gameObjectTypeDocument);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::onNewElement(I_GameObjectElement& _element, int _position)
{
    // No need to notify listeners until the element is modified.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::onElementRemoved(int _position)
{
    // TODO Implement.  How to get to the document?
    throw Zen::Utility::runtime_exception("Project::onElementRemoved(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::onElementModified(I_GameObjectElement& _element, int _row)
{
    notifyViewsOfChange(_element.getDocument());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Project::notifyViewsOfChange(I_GameObjectTypeDocument& _document)
{
    // This lock is redundant since we're only using a single thread.  When
    // we switch to mulltiple threads, I'll work on updating the event
    // model and re-evaluate the need for all locks.
    //Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    for(ProjectSubscribers_type::iterator iter = m_listeners.begin(); iter != m_listeners.end(); iter++)
    {
        (*iter)->gameObjectTypeDocumentModified(_document);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
