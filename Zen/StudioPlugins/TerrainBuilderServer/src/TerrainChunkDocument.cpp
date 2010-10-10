//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "TerrainChunkDocument.hpp"
#include "TerrainChunkNode.hpp"

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>
#include <Zen/Studio/Workbench/I_SceneView.hpp>

#include <boost/bind.hpp>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkDocument::TerrainChunkDocument(Zen::Studio::Workbench::I_Project& _project)
:   m_project(_project)
,   m_controller(*this)
,   m_pSubscriptionsGuard(Zen::Threading::MutexFactory::create())
,   m_loaded(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkDocument::~TerrainChunkDocument()
{
    Zen::Threading::MutexFactory::destroy(m_pSubscriptionsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
TerrainChunkDocument::getShortName() const
{
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
TerrainChunkDocument::getLongName() const
{
    // TODO Create a fully qualified name, including project, etc?
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project*
TerrainChunkDocument::getProject() const
{
    return &m_project;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Zen::Studio::Workbench::subscription<TerrainChunkDocument*, TerrainChunkDocument::pTerrainChunkView_type>   Subscription_type;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Zen::Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    {
        Subscription_type* pSubscription =
            dynamic_cast<Subscription_type*>(_wpSubscription.get());

        if (pSubscription != NULL)
        {
            delete pSubscription;
            return;
        }
    }

    /// TODO Error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkDocument::pSubscription_type
TerrainChunkDocument::subscribe(pTerrainChunkView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    // This lock needs to only guard the insert and not the notification,
    // otherwise a nested lock will occur, which will deadlock on some
    // platforms.
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_subscribers.insert(_pView);
    }

    return pSubscription_type(
        new Subscription_type(this, _pView),
        &destroySubscription
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_TerrainChunkController&
TerrainChunkDocument::getTerrainChunkController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkDocument::pExplorerNode_type
TerrainChunkDocument::getNode()
{
    return m_pNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::onDocumentModified(I_TerrainChunkDocument& _terrainChunkDocument)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::onAddProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::onValueModified(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{
    notifyViewsOfModification();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::onRemoveProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::unSubscribe(pTerrainChunkView_type _pView)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_subscribers.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::save()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::load(TerrainChunkNode& _node)
{
    m_pNode = &_node;

    // Subscribe to this node's onModified event so we can be notified in particular
    // about name changes.
    m_pNodeConnection = m_pNode->getNode()->onModified.connect(
        boost::bind(&TerrainChunkDocument::handleExplorerNodeModified, this, _1)
    );

    // Load this document from the database
    if (getProject() != NULL)
    {
    }

    m_loaded = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::notifyViewsOfModification()
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    for (TerrainChunkSubscribers_type::iterator iter = m_subscribers.begin();
         iter != m_subscribers.end(); ++iter)
    {
        (*iter)->onDocumentModified(*this);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkDocument::handleExplorerNodeModified(I_ExplorerNode* _pNode)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
TerrainChunkDocument::getTerrainChunkId() const
{
    return m_pNode->getTerrainChunkId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
