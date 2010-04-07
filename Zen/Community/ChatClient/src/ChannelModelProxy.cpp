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

#include "ChannelModelProxy.hpp"
#include "Channel.hpp"
#include "ChatService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Enterprise/AppServer/I_Message.hpp>
#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Community/ChatCommon/subscription.hpp>

#include <Zen/Community/ChatProtocol/I_SubscribeChannelModelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_UnsubscribeChannelModelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_CreateChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_ReadChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_UpdateChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_DeleteChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_ChannelModelUpdateMessage.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChannelModelProxy::ChannelModelProxy(ChatService& _service)
:   m_service(_service)
,   m_pViewsGuard(Threading::MutexFactory::create())
,   m_pChannelsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChannelModelProxy::~ChannelModelProxy()
{
    Threading::MutexFactory::destroy(m_pChannelsGuard);
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Common::subscription<ChannelModelProxy*, ChannelModelProxy::pChannelView_type> Subscription;
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
ChannelModelProxy::pFutureSubscription_type
ChannelModelProxy::subscribe(Common::I_Session& _session, pChannelView_type _pView, pFilter_type _pFilter)
{
    /// TODO make async
    ProxySubscription* pProxySubscription = new ProxySubscription(_session);

    /// Create a new local future subscription object.
    pProxySubscription->m_pFutureSubscription = pFutureSubscription_type(
        new FutureSubscription_type()
    );

    pProxySubscription->m_pView = _pView;

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create a subscription request using pFutureSubscription as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_SubscribeChannelModelRequest, ProxySubscription*>
        request(pDestinationEndpoint, pProxySubscription);
    
    request->setSessionId(_session.getSessionId());

    m_service.send<Protocol::I_SubscribeChannelModelRequest, ProxySubscription*>
        (request, boost::bind(&ChatService::handleSubscribeChannelModelResponse, &m_service, _1, _2, _3));

    return pProxySubscription->m_pFutureSubscription;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event& 
ChannelModelProxy::getChannelEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("Channel::Client::ChannelEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModelProxy::destroyChannel(wpChannel_type _wpChannel)
{
    Channel* pRaw = 
        dynamic_cast<Channel*>(_wpChannel.get());

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
ChannelModelProxy::pFutureChannel_type 
ChannelModelProxy::createChannel(Common::I_Session& _session, const std::string& _name, const std::string& _description)
{
    /// TODO make async
    ProxyChannel* pProxyChannel = new ProxyChannel();

    /// Create a new local future subscription object
    pProxyChannel->m_pFutureChannel = pFutureChannel_type(
        new FutureChannel_type()
    );

    pProxyChannel->m_pChannel = pChannel_type(
        new Channel(m_service),
        &ChannelModelProxy::destroyChannel
    );

    typedef Memory::managed_ptr<Channel>    pConcreteChannel_type;
    pConcreteChannel_type pConcreteChannel = pProxyChannel->m_pChannel.as<pConcreteChannel_type>();

    /// Populate the payload
    pConcreteChannel->setChannelName(_name);
    pConcreteChannel->setChannelDescription(_description);

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an channel creation request using ProxyChannel* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_CreateChannelRequest, ProxyChannel*>
        request(pDestinationEndpoint, pProxyChannel);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setName(_name);
    request->setDescription(_description);

    // Send the request
    m_service.send<Protocol::I_CreateChannelRequest, ProxyChannel*>
        (request, boost::bind(&ChatService::handleCreateChannelResponse, &m_service, _1, _2, _3));

    // This future channel will be updated upon receipt of the 
    // associated response.
    // TODO What happens if the response is never received?
    return pProxyChannel->m_pFutureChannel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChannelModelProxy::pFutureChannel_type 
ChannelModelProxy::getChannel(Common::I_Session& _session, const std::string& _name)
{
    /// TODO make async
    ProxyChannel* pProxyChannel = new ProxyChannel();

    /// Create a new local future subscription object
    pProxyChannel->m_pFutureChannel = pFutureChannel_type(
        new FutureChannel_type()
    );

    pProxyChannel->m_pChannel = pChannel_type(
        new Channel(m_service),
        &ChannelModelProxy::destroyChannel
    );

    typedef Memory::managed_ptr<Channel>    pConcreteChannel_type;
    pConcreteChannel_type pConcreteChannel = pProxyChannel->m_pChannel.as<pConcreteChannel_type>();

    /// Populate the payload
    pConcreteChannel->setChannelName(_name);

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an channel read request using ProxyChannel* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_ReadChannelRequest, ProxyChannel*>
        request(pDestinationEndpoint, pProxyChannel);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setName(_name);

    // Send the request
    m_service.send<Protocol::I_ReadChannelRequest, ProxyChannel*>
        (request, boost::bind(&ChatService::handleReadChannelResponse, &m_service, _1, _2, _3));

    // This future channel will be updated upon receipt of the 
    // associated response.
    // TODO What happens if the response is never received?
    return pProxyChannel->m_pFutureChannel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModelProxy::updateChannel(Common::I_Session& _session, Common::I_Channel& _channel)
{
    /// TODO make async
    ProxyChannel* pProxyChannel = new ProxyChannel();

    /// Create a new local future subscription object
    pProxyChannel->m_pFutureChannel = pFutureChannel_type(
        new FutureChannel_type()
    );

    Channel* pChannel = dynamic_cast<Channel*>(&_channel);

    pProxyChannel->m_pChannel = pChannel->getSelfReference().lock();

    typedef Memory::managed_ptr<Channel>    pConcreteChannel_type;
    pConcreteChannel_type pConcreteChannel = pProxyChannel->m_pChannel.as<pConcreteChannel_type>();

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an channel read request using ProxyChannel* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_UpdateChannelRequest, ProxyChannel*>
        request(pDestinationEndpoint, pProxyChannel);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setChannelId(_channel.getChannelId());
    request->setName(_channel.getChannelName());
    request->setDescription(pChannel->getChannelDescription());

    // Send the request
    m_service.send<Protocol::I_UpdateChannelRequest, ProxyChannel*>
        (request, boost::bind(&ChatService::handleUpdateChannelResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModelProxy::removeChannel(Common::I_Session& _session, Common::I_Channel& _channel)
{
    /// TODO make async
    ProxyChannel* pProxyChannel = new ProxyChannel();

    /// Create a new local future subscription object
    pProxyChannel->m_pFutureChannel = pFutureChannel_type(
        new FutureChannel_type()
    );

    Channel* pChannel = dynamic_cast<Channel*>(&_channel);

    pProxyChannel->m_pChannel = pChannel->getSelfReference().lock();

    typedef Memory::managed_ptr<Channel>    pConcreteChannel_type;
    pConcreteChannel_type pConcreteChannel = pProxyChannel->m_pChannel.as<pConcreteChannel_type>();

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an channel read request using ProxyChannel* as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_DeleteChannelRequest, ProxyChannel*>
        request(pDestinationEndpoint, pProxyChannel);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setChannelId(_channel.getChannelId());

    // Send the request
    m_service.send<Protocol::I_DeleteChannelRequest, ProxyChannel*>
        (request, boost::bind(&ChatService::handleDeleteChannelResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChannelModelProxy::getChannels(Common::I_Session& _session, Common::I_ChannelController::I_ChannelVisitor& _visitor)
{
    _visitor.begin();

    Channels_type::iterator iter = m_channels.begin();
    while( iter != m_channels.end() )
    {
        _visitor.visit(*iter->second);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModelProxy::unSubscribe(Common::I_Session& _session, pChannelView_type _pView)
{
    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create a subscription request using pFutureSubscription as the payload.
    Zen::Enterprise::AppServer::create_request<Protocol::I_UnsubscribeChannelModelRequest, pChannelView_type>
        request(pDestinationEndpoint, _pView);
    
    request->setSessionId(_session.getSessionId());

    /// TODO Set the proxy Id.
    
    m_service.send<Protocol::I_UnsubscribeChannelModelRequest, pChannelView_type>
        (request, boost::bind(&ChatService::handleUnsubscribeChannelModelResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChannelModelProxy::handleSubscription(ProxySubscription* _pProxySubscription)
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
ChannelModelProxy::handleUnsubscribe(pSubscription_type _pSubscription)
{
    Threading::CriticalSection lock(m_pViewsGuard);

    /// TODO Need to get a subscription's parent view here.
    //m_views.erase(_pSubscription->getParentView());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChannelModelProxy::handleCreateChannel(ProxyChannel* _pProxyChannel)
{
    Threading::CriticalSection guard(m_pChannelsGuard);

    Channels_type::iterator iter = m_channels.find(_pProxyChannel->m_pChannel->getChannelName());
    if( iter == m_channels.end() )
    {
        m_channels[_pProxyChannel->m_pChannel->getChannelName()] = _pProxyChannel->m_pChannel;
    }
    else
    {
        // TODO Error?
    }

    _pProxyChannel->m_pFutureChannel->setValue(_pProxyChannel->m_pChannel);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyChannel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChannelModelProxy::handleReadChannel(ProxyChannel* _pProxyChannel)
{
    Threading::CriticalSection guard(m_pChannelsGuard);

    Channels_type::iterator iter = m_channels.find(_pProxyChannel->m_pChannel->getChannelName());
    if( iter != m_channels.end() )
    {
        iter->second = _pProxyChannel->m_pChannel;
    }
    else
    {
        // TODO Error?
    }

    _pProxyChannel->m_pFutureChannel->setValue(_pProxyChannel->m_pChannel);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyChannel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChannelModelProxy::handleUpdateChannel(ProxyChannel* _pProxyChannel)
{
    Threading::CriticalSection guard(m_pChannelsGuard);

    Channels_type::iterator iter = m_channels.find(_pProxyChannel->m_pChannel->getChannelName());
    if( iter != m_channels.end() )
    {
        iter->second = _pProxyChannel->m_pChannel;
    }
    else
    {
        // TODO Error?
    }

    _pProxyChannel->m_pFutureChannel->setValue(_pProxyChannel->m_pChannel);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyChannel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChannelModelProxy::handleDeleteChannel(ProxyChannel* _pProxyChannel)
{
    Threading::CriticalSection guard(m_pChannelsGuard);

    Channels_type::iterator iter = m_channels.find(_pProxyChannel->m_pChannel->getChannelName());
    if( iter != m_channels.end() )
    {
        m_channels.erase(iter);
    }
    else
    {
        // TODO Error?
    }

    _pProxyChannel->m_pFutureChannel->setValue(pChannel_type());

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyChannel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModelProxy::handleUpdateNotification(pMessage_type _pMessage)
{
    Protocol::I_ChannelModelUpdateMessage* pMessage =
        dynamic_cast<Protocol::I_ChannelModelUpdateMessage*>(_pMessage.get());

    switch(pMessage->getUpdateKind())
    {

    case Protocol::I_ChannelModelUpdateMessage::ADD:
        // Add channel to channel collection if it doesn't already
        // exist and notify view specified by proxyId.
        {
            pChannel_type pChannel;
            Channels_type::iterator iter = m_channels.find(pMessage->getName());
            if( iter == m_channels.end() )
            {
                Channel* pRawChannel = new Channel(m_service);
                
                pRawChannel->setChannelId(pMessage->getChannelId());
                pRawChannel->setChannelName(pMessage->getName());
                pRawChannel->setChannelDescription(pMessage->getDescription());

                pChannel = pChannel_type(
                    pRawChannel, &ChannelModelProxy::destroyChannel
                );

                m_channels[pMessage->getName()] = pChannel;
            }
            else
            {
                pChannel = iter->second;
                Channel* pRawChannel = dynamic_cast<Channel*>(pChannel.get());

                pRawChannel->setChannelId(pMessage->getChannelId());
                pRawChannel->setChannelName(pMessage->getName());
                pRawChannel->setChannelDescription(pMessage->getDescription());
            }

            ChannelViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onAddChannel(*pChannel);
            }
        }
        break;

    case Protocol::I_ChannelModelUpdateMessage::UPDATE:
        // Update channel in channel collection and
        // notify view specified by proxyId.
        {
            pChannel_type pChannel;
            Channels_type::iterator iter = m_channels.find(pMessage->getName());
            if( iter != m_channels.end() )
            {
                pChannel = iter->second;
                Channel* pRawChannel = dynamic_cast<Channel*>(pChannel.get());

                pRawChannel->setChannelId(pMessage->getChannelId());
                pRawChannel->setChannelName(pMessage->getName());
                pRawChannel->setChannelDescription(pMessage->getDescription());
            }
            else
            {
                throw Zen::Utility::runtime_exception("ChannelModelProxy::handleUpdateNotification() : Channel does not exist -- UPDATE Operation.");
            }

            ChannelViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onUpdateChannel(*pChannel);
            }
        }
        break;

    case Protocol::I_ChannelModelUpdateMessage::REMOVE:
        // Remove channel from channel collection and
        // notify view specified by proxyId.
        {
            pChannel_type pChannel;
            Channels_type::iterator iter = m_channels.find(pMessage->getName());
            if( iter != m_channels.end() )
            {
                m_channels.erase(iter);
            }
            else
            {
                //throw Zen::Utility::runtime_exception("ChannelModelProxy::handleUpdateNotification() : Channel does not exist -- DELETE Operation.");
            }

            ChannelViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onRemoveChannel(*pChannel);
            }
        }
        break;

    default:
        break;

    };
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
