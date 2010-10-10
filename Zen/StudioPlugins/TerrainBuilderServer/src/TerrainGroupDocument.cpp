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
#include "TerrainGroupDocument.hpp"
#include "TerrainGroupNode.hpp"
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
TerrainGroupDocument::TerrainGroupDocument(Zen::Studio::Workbench::I_Project& _project)
:   m_project(_project)
,   m_controller(*this)
,   m_pSubscriptionsGuard(Zen::Threading::MutexFactory::create())
,   m_loaded(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupDocument::~TerrainGroupDocument()
{
    Zen::Threading::MutexFactory::destroy(m_pSubscriptionsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
TerrainGroupDocument::getShortName() const
{
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
TerrainGroupDocument::getLongName() const
{
    // TODO Create a fully qualified name, including project, etc?
    return m_pNode->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project*
TerrainGroupDocument::getProject() const
{
    return &m_project;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Zen::Studio::Workbench::subscription<TerrainGroupDocument*, TerrainGroupDocument::pTerrainGroupView_type>   SubscriptionTG_type;
typedef Zen::Studio::Workbench::subscription<TerrainGroupDocument*, TerrainGroupDocument::pSceneView_type>          SubscriptionScene_type;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Zen::Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    // Try the TG subscription
    {
        SubscriptionTG_type* pSubscription =
            dynamic_cast<SubscriptionTG_type*>(_wpSubscription.get());

        if (pSubscription != NULL)
        {
            delete pSubscription;
            return;
        }
    }

    // Try the Scene subscription
    {
        SubscriptionScene_type* pSubscription =
            dynamic_cast<SubscriptionScene_type*>(_wpSubscription.get());

        if (pSubscription != NULL)
        {
            delete pSubscription;
            return;
        }
    }

    /// TODO Error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupDocument::pSubscription_type
TerrainGroupDocument::subscribe(pTerrainGroupView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    // This lock needs to only guard the insert and not the notification,
    // otherwise a nested lock will occur, which will deadlock on some
    // platforms.
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_subscribersTG.insert(_pView);
    }

    return pSubscription_type(
        new SubscriptionTG_type(this, _pView),
        &destroySubscription
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupDocument::pSubscription_type
TerrainGroupDocument::subscribe(pSceneView_type _pView, pFilter_type _pFilter)
{
    // Cannot subscribe unless the document has already been loaded.
    assert(m_loaded);

    // This lock needs to only guard the insert and not the notification,
    // otherwise a nested lock will occur, which will deadlock on some
    // platforms.
    {
        Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

        m_subscribersScene.insert(_pView);
    }

    return pSubscription_type(
        new SubscriptionScene_type(this, _pView),
        &destroySubscription
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_TerrainGroupController&
TerrainGroupDocument::getTerrainGroupController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupDocument::I_SceneController&
TerrainGroupDocument::getSceneController()
{
    return m_controller;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupDocument::pExplorerNode_type
TerrainGroupDocument::getNode()
{
    return m_pNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesPublisher&
TerrainGroupDocument::getProperties()
{
    assert(m_loaded);

    TerrainGroupNode* pNode = dynamic_cast<TerrainGroupNode*>(m_pNode);
    return pNode->getProperties();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesPublisher&
TerrainGroupDocument::getEntityProperties(const I_SceneEntity& _entity)
{
    assert(m_loaded);

    TerrainGroupNode* pNode = dynamic_cast<TerrainGroupNode*>(m_pNode);
    TerrainChunkNode* pChunkNode = pNode->getTerrainChunkByEntity(_entity);

    if (pChunkNode != NULL)
    {
        return pChunkNode->getProperties();
    }
    else
    {
        throw Zen::Utility::runtime_exception("TerrainGroupDocument::getEntityProperties() : Entity is not a Terrain Chunk.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::onDocumentModified(I_TerrainGroupDocument& _terrainGroupDocument)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::onAddProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::onValueModified(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{
    notifyViewsOfModification();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::onRemoveProperty(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, Zen::Studio::Workbench::I_Property& _property)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::unSubscribe(pTerrainGroupView_type _pView)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_subscribersTG.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::unSubscribe(pSceneView_type _pView)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);
    m_subscribersScene.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::save()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::load(TerrainGroupNode& _node)
{
    m_pNode = &_node;

    // Subscribe to this node's onModified event so we can be notified in particular
    // about name changes.
    m_pNodeConnection = m_pNode->getNode()->onModified.connect(
        boost::bind(&TerrainGroupDocument::handleExplorerNodeModified, this, _1)
    );

    // Load this document from the database

    m_loaded = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::notifyViewsOfModification()
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    {
        for (TerrainGroupSubscribers_type::iterator iter = m_subscribersTG.begin();
             iter != m_subscribersTG.end(); ++iter)
        {
            (*iter)->onDocumentModified(*this);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupDocument::handleExplorerNodeModified(I_ExplorerNode* _pNode)
{
    Zen::Threading::CriticalSection lock(m_pSubscriptionsGuard);

    for (SceneSubscribers_type::iterator view = m_subscribersScene.begin();
         view != m_subscribersScene.end(); ++view)
    {
        // TODO Long name and short name should probably be different.
        (*view)->onNameModified(_pNode->getDisplayName(), _pNode->getDisplayName());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
TerrainGroupDocument::getTerrainGroupId() const
{
    return m_pNode->getTerrainGroupId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
