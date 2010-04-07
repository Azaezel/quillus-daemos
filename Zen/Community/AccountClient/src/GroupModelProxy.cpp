//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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

#include "GroupModelProxy.hpp"
#include "Group.hpp"
#include "AccountService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Enterprise/AppServer/I_Message.hpp>
#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Community/AccountCommon/subscription.hpp>

#include <Zen/Community/AccountProtocol/I_SubscribeGroupModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeGroupModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_CreateGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_ReadGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UpdateGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_DeleteGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_GroupModelUpdateMessage.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModelProxy::GroupModelProxy(AccountService& _service)
:   m_service(_service)
,   m_pViewsGuard(Threading::MutexFactory::create())
,   m_pGroupsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModelProxy::~GroupModelProxy()
{
    Threading::MutexFactory::destroy(m_pGroupsGuard);
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Common::subscription<GroupModelProxy*, GroupModelProxy::pGroupView_type> Subscription;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription* pSubscription =
        dynamic_cast<Subscription*>(
            _wpSubscription.get()
        );

    if( pSubscription != NULL )
    {
        delete pSubscription;
    }
    else
    {
        /// TODO Exception?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModelProxy::pFutureSubscription_type
GroupModelProxy::subscribe(Common::I_Session& _session, pGroupView_type _pView, pFilter_type _pFilter)
{
    /// TODO make asynchronous
    ProxySubscription* pProxySubscription = new ProxySubscription(_session);

    /// Create a new local future subscription object.
    pProxySubscription->m_pFutureSubscription = pFutureSubscription_type(
        new FutureSubscription_type()
    );

    pProxySubscription->m_pView = _pView;

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create a subscription request using pFutureSubscription as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_SubscribeGroupModelRequest, ProxySubscription*>
        request(pDestinationEndpoint, pProxySubscription);
    
    request->setSessionId(_session.getSessionId());

    m_service.send<Protocol::I_SubscribeGroupModelRequest, ProxySubscription*>
        (request, boost::bind(&AccountService::handleSubscribeGroupModelResponse, &m_service, _1, _2, _3));

    return pProxySubscription->m_pFutureSubscription;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event& 
GroupModelProxy::getGroupEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("Account::Client::GroupEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModelProxy::destroyGroup(wpGroup_type _wpGroup)
{
    Group* pRaw = 
        dynamic_cast<Group*>(_wpGroup.get());

    if( pRaw != NULL )
    {
        delete pRaw;
    }
    else
    {
        /// TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModelProxy::pFutureGroup_type 
GroupModelProxy::createGroup(Common::I_Session& _session, const std::string& _name, const std::string& _description)
{
    /// TODO make asynchronous
    ProxyGroup* pProxyGroup = new ProxyGroup();

    /// Create a new local future group object.
    pProxyGroup->m_pFutureGroup = pFutureGroup_type(
        new FutureGroup_type()
    );

    pProxyGroup->m_pGroup = pGroup_type(
        new Group(m_service),
        &GroupModelProxy::destroyGroup
    );

    typedef Memory::managed_ptr<Group>      pConcreteGroup_type;
    pConcreteGroup_type pConcreteGroup = pProxyGroup->m_pGroup.as<pConcreteGroup_type>();

    /// Populate the payload
    pConcreteGroup->setGroupName(_name);
    pConcreteGroup->setGroupDescription(_description);

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create a group creation request using ProxyGroup* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_CreateGroupRequest, ProxyGroup*>
        request(pDestinationEndpoint, pProxyGroup);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setGroupName(_name);
    request->setGroupDescription(_description);

    // Send the request
    m_service.send<Protocol::I_CreateGroupRequest, ProxyGroup*>
        (request, boost::bind(&AccountService::handleCreateGroupResponse, &m_service, _1, _2, _3));

    // This future group will be updated upon receipt of the
    // associated response.
    // TODO What happens if the response is never received?
    return pProxyGroup->m_pFutureGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModelProxy::pFutureGroup_type 
GroupModelProxy::getGroup(Common::I_Session& _session, const std::string& _name)
{
    /// TODO make asynchronous
    ProxyGroup* pProxyGroup = new ProxyGroup();

    /// Create a new local future group object.
    pProxyGroup->m_pFutureGroup = pFutureGroup_type(
        new FutureGroup_type()
    );

    pProxyGroup->m_pGroup = pGroup_type(
        new Group(m_service),
        &GroupModelProxy::destroyGroup
    );

    typedef Memory::managed_ptr<Group>      pConcreteGroup_type;
    pConcreteGroup_type pConcreteGroup = pProxyGroup->m_pGroup.as<pConcreteGroup_type>();

    /// Populate the payload
    pConcreteGroup->setGroupName(_name);

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create a group read request using ProxyGroup* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_ReadGroupRequest, ProxyGroup*>
        request(pDestinationEndpoint, pProxyGroup);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setGroupName(_name);

    // Send the request
    m_service.send<Protocol::I_ReadGroupRequest, ProxyGroup*>
        (request, boost::bind(&AccountService::handleReadGroupResponse, &m_service, _1, _2, _3));

    // This future group will be updated upon receipt of the
    // associated response.
    // TODO What happens if the response is never received?
    return pProxyGroup->m_pFutureGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModelProxy::updateGroup(Common::I_Session& _session, Common::I_Group& _group)
{
    /// TODO make asynchronous
    ProxyGroup* pProxyGroup = new ProxyGroup();

    /// Create a new local future group object.
    pProxyGroup->m_pFutureGroup = pFutureGroup_type(
        new FutureGroup_type()
    );

    Group* pGroup = dynamic_cast<Group*>(&_group);

    pProxyGroup->m_pGroup = pGroup->getSelfReference().lock();

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create a group read request using ProxyGroup* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_UpdateGroupRequest, ProxyGroup*>
        request(pDestinationEndpoint, pProxyGroup);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setGroupId(_group.getGroupId());
    request->setGroupName(_group.getGroupName());
    request->setGroupDescription(_group.getGroupDescription());

    // Send the request
    m_service.send<Protocol::I_UpdateGroupRequest, ProxyGroup*>
        (request, boost::bind(&AccountService::handleUpdateGroupResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModelProxy::removeGroup(Common::I_Session& _session, Common::I_Group& _group)
{
    /// TODO make asynchronous
    ProxyGroup* pProxyGroup = new ProxyGroup();

    /// Create a new local future group object.
    pProxyGroup->m_pFutureGroup = pFutureGroup_type(
        new FutureGroup_type()
    );

    Group* pGroup = dynamic_cast<Group*>(&_group);

    pProxyGroup->m_pGroup = pGroup->getSelfReference().lock();

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create a group read request using ProxyGroup* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_DeleteGroupRequest, ProxyGroup*>
        request(pDestinationEndpoint, pProxyGroup);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setGroupId(_group.getGroupId());

    // Send the request
    m_service.send<Protocol::I_DeleteGroupRequest, ProxyGroup*>
        (request, boost::bind(&AccountService::handleDeleteGroupResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModelProxy::getGroups(Common::I_Session& _session, Common::I_GroupController::I_GroupVisitor& _visitor)
{
    _visitor.begin();

    Groups_type::iterator iter = m_groups.begin();
    while( iter != m_groups.end() )
    {
        _visitor.visit(*iter->second);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModelProxy::unSubscribe(Common::I_Session& _session, pGroupView_type _pView)
{
    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an unsubscribe request using _pView as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_UnsubscribeGroupModelRequest, pGroupView_type>
        request(pDestinationEndpoint, _pView);

    request->setSessionId(_session.getSessionId());

    /// TODO Set the proxy Id.

    m_service.send<Protocol::I_UnsubscribeGroupModelRequest, pGroupView_type>
        (request, boost::bind(&AccountService::handleUnsubscribeGroupModelResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModelProxy::handleSubscription(ProxySubscription* _pProxySubscription)
{
    Threading::CriticalSection lock(m_pViewsGuard);

    m_views[_pProxySubscription->m_proxyId] = _pProxySubscription->m_pView;

    pSubscription_type pSubscription(
        new Subscription(
            _pProxySubscription->m_session,
            this,
            _pProxySubscription->m_pView
        ),
        &destroySubscription
    );

    _pProxySubscription->m_pFutureSubscription->setValue(pSubscription);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxySubscription;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModelProxy::handleUnsubscribe(pSubscription_type _pSubscription)
{
    Threading::CriticalSection lock(m_pViewsGuard);

    /// TODO Need to get a subscription's parent view here.
    //m_views.erase(_pSubscription->getParentView());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModelProxy::handleCreateGroup(ProxyGroup* _pProxyGroup)
{
    Threading::CriticalSection guard(m_pGroupsGuard);

    Groups_type::iterator iter = m_groups.find(_pProxyGroup->m_pGroup->getGroupName());
    if( iter == m_groups.end() )
    {
        m_groups[_pProxyGroup->m_pGroup->getGroupName()] = _pProxyGroup->m_pGroup;
    }
    else
    {
        // TODO Error?
    }

    _pProxyGroup->m_pFutureGroup->setValue(_pProxyGroup->m_pGroup);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxyGroup here?
    delete _pProxyGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModelProxy::handleReadGroup(ProxyGroup* _pProxyGroup)
{
    Threading::CriticalSection guard(m_pGroupsGuard);

    Groups_type::iterator iter = m_groups.find(_pProxyGroup->m_pGroup->getGroupName());
    if( iter != m_groups.end() )
    {
        iter->second = _pProxyGroup->m_pGroup;
    }
    else
    {
        // TODO Error?
    }

    _pProxyGroup->m_pFutureGroup->setValue(_pProxyGroup->m_pGroup);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxyGroup here?
    delete _pProxyGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModelProxy::handleUpdateGroup(ProxyGroup* _pProxyGroup)
{
    Threading::CriticalSection guard(m_pGroupsGuard);

    Groups_type::iterator iter = m_groups.find(_pProxyGroup->m_pGroup->getGroupName());
    if( iter != m_groups.end() )
    {
        iter->second = _pProxyGroup->m_pGroup;
    }
    else
    {
        // TODO Error?
    }

    _pProxyGroup->m_pFutureGroup->setValue(_pProxyGroup->m_pGroup);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxyGroup here?
    delete _pProxyGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModelProxy::handleDeleteGroup(ProxyGroup* _pProxyGroup)
{
    Threading::CriticalSection guard(m_pGroupsGuard);

    Groups_type::iterator iter = m_groups.find(_pProxyGroup->m_pGroup->getGroupName());
    if( iter != m_groups.end() )
    {
        m_groups.erase(iter);
    }
    else
    {
        // TODO Error?
    }

    _pProxyGroup->m_pFutureGroup->setValue(pGroup_type());

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxyGroup here?
    delete _pProxyGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModelProxy::handleUpdateNotification(pMessage_type _pMessage)
{
    Protocol::I_GroupModelUpdateMessage* pMessage =
        dynamic_cast<Protocol::I_GroupModelUpdateMessage*>(_pMessage.get());

    switch(pMessage->getUpdateKind())
    {

    case Protocol::I_GroupModelUpdateMessage::ADD:
        /// TODO Add account to account collection if it doesn't already
        /// exist and notify view specified by proxyId.
        {
            pGroup_type pGroup;
            Groups_type::iterator iter = m_groups.find(pMessage->getGroupName());
            if( iter == m_groups.end() )
            {
                Group* pRawGroup = new Group(m_service);
                
                pRawGroup->setGroupId(pMessage->getGroupId());
                pRawGroup->setGroupName(pMessage->getGroupName());
                pRawGroup->setGroupDescription(pMessage->getGroupDescription());

                pGroup = pGroup_type(
                    pRawGroup, &GroupModelProxy::destroyGroup
                );

                m_groups[pMessage->getGroupName()] = pGroup;
            }
            else
            {
                pGroup = iter->second;
                Group* pRawGroup = dynamic_cast<Group*>(pGroup.get());

                pRawGroup->setGroupId(pMessage->getGroupId());
                pRawGroup->setGroupName(pMessage->getGroupName());
                pRawGroup->setGroupDescription(pMessage->getGroupDescription());
            }

            GroupViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onAddGroup(*pGroup);
            }
        }
        break;

    case Protocol::I_GroupModelUpdateMessage::UPDATE:
        /// TODO Update account in account collection and
        /// notify view specified by proxyId.
        {
            pGroup_type pGroup;
            Groups_type::iterator iter = m_groups.find(pMessage->getGroupName());
            if( iter != m_groups.end() )
            {
                pGroup = iter->second;
                Group* pRawGroup = dynamic_cast<Group*>(pGroup.get());

                pRawGroup->setGroupId(pMessage->getGroupId());
                pRawGroup->setGroupName(pMessage->getGroupName());
                pRawGroup->setGroupDescription(pMessage->getGroupDescription());
            }
            else
            {
                throw Zen::Utility::runtime_exception("GroupModelProxy::handleUpdateNotification() : Group does not exist -- UPDATE Operation.");
            }

            GroupViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onUpdateGroup(*pGroup);
            }
        }
        break;

    case Protocol::I_GroupModelUpdateMessage::REMOVE:
        /// TODO Remove account from account collection and
        /// notify view specified by proxyId.
        {
            pGroup_type pGroup;
            Groups_type::iterator iter = m_groups.find(pMessage->getGroupName());
            if( iter != m_groups.end() )
            {
                m_groups.erase(iter);
            }
            else
            {
                //throw Zen::Utility::runtime_exception("GroupModelProxy::handleUpdateNotification() : Group does not exist -- DELETE Operation.");
            }

            GroupViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onRemoveGroup(*pGroup);
            }
        }
        break;

    default:
        break;

    };
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
