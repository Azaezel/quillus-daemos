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

#include "ChannelModel.hpp"
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
#include <Zen/Community/ChatCommon/I_Channel.hpp>
#include <Zen/Community/ChatCommon/I_ChannelView.hpp>

#include <Zen/Community/ChatModel/I_ChatChannelDomainObject.hpp>
#include <Zen/Community/ChatModel/I_ChatChannelDataMap.hpp>
#include <Zen/Community/ChatModel/I_ChatChannelDataCollection.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChannelModel::ChannelModel(ChatService& _service)
:   m_service(_service)
,   m_pViewsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChannelModel::~ChannelModel()
{
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Common::subscription<ChannelModel*, ChannelModel::pChannelView_type> Subscription;
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
ChannelModel::pFutureSubscription_type
ChannelModel::subscribe(Common::I_Session& _session, pChannelView_type _pView, pFilter_type _pFilter)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make async
    pFutureSubscription_type pReturnValue(new FutureSubscription_type());

    Threading::CriticalSection lock(m_pViewsGuard);

    m_views.insert(_pView);

    // This view needs to be notified of all of the existing model.
    for(Channels_type::iterator iter = m_channels.begin(); iter != m_channels.end(); iter++)
    {
        // Notify the view of an channel
        _pView->onAddChannel(*iter->second.get());
    }

    pSubscription_type pSubscription(
        new Subscription(
            _session,
            this,
            _pView
        ),
        &destroySubscription
    );

    pReturnValue->setValue(pSubscription);

    return pReturnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event& 
ChannelModel::getChannelEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("Channel::Server::ChannelEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::destroyChannel(wpChannel_type _wpChannel)
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
ChannelModel::pFutureChannel_type 
ChannelModel::createChannel(Common::I_Session& _session, const std::string& _name, const std::string& _description)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make this async
    pFutureChannel_type returnValue(new FutureChannel_type);

    // Get the data map.
    Community::Chat::I_ChatChannelDataMap::pChatChannelDataMap_type pChannelDM =
        Community::Chat::I_ChatChannelDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    // Create a new domain object.
    Community::Chat::I_ChatChannelDataMap::pChatChannelDomainObject_type pChannelDO =
        pChannelDM->createNew();

    // Populate the values.
    pChannelDO->getName()           = _name;
    pChannelDO->getDescription()    = _description;

    // Update the object (and in this case, insert it).
    boost::uint64_t newChannelId    = pChannelDM->update(pChannelDO)->getValue();
    pChannelDO->getChatChannelId()  = newChannelId;

    Channel* pRawChannel = new Channel(m_service, pChannelDO);

    pChannel_type pChannel(
        pRawChannel,
        &destroyChannel
    );

    m_channels[_name] = pChannel;

    onAddChannel(pChannel);

    returnValue->setValue(pChannel);

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChannelModel::pFutureChannel_type 
ChannelModel::getChannel(Common::I_Session& _session, const std::string& _name)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make this async
    pFutureChannel_type returnValue(new FutureChannel_type);

    pChannel_type pChannel;
    Channels_type::iterator iter = m_channels.find(_name);
    if(iter != m_channels.end() )
    {
        pChannel = iter->second;        
    }

    returnValue->setValue(pChannel);

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::updateChannel(Common::I_Session& _session, Common::I_Channel& _channel)
{
    /// TODO Check session (i.e. session user) against permissions.

    // Get the data map.
    Community::Chat::I_ChatChannelDataMap::pChatChannelDataMap_type pChannelDM =
        Community::Chat::I_ChatChannelDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    Channel* pRawChannel = dynamic_cast<Channel*>(&_channel);

    // Update the object.
    pChannelDM->update(pRawChannel->getDO())->getValue();

    onUpdateChannel(pRawChannel->getSelfReference().lock());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::removeChannel(Common::I_Session& _session, Common::I_Channel& _channel)
{
    /// TODO Check session (i.e. session user) against permissions.

    throw Zen::Utility::runtime_exception("ChannelModel::removeChannel() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChannelModel::getChannels(Common::I_Session& _session, Common::I_ChannelController::I_ChannelVisitor& _visitor)
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
ChannelModel::unSubscribe(Common::I_Session& _session, pChannelView_type _pView)
{
    Threading::CriticalSection lock(m_pViewsGuard);
    m_views.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::onAddChannel(pChannel_type _pChannel)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    ChannelViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(ChannelViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(ChannelViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onAddChannel(*_pChannel);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::onUpdateChannel(pChannel_type _pChannel)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    ChannelViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(ChannelViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(ChannelViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onUpdateChannel(*_pChannel);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::onRemoveChannel(pChannel_type _pChannel)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    ChannelViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(ChannelViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(ChannelViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onRemoveChannel(*_pChannel);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::loadDataModel()
{
    // Get the data map.
    Community::Chat::I_ChatChannelDataMap::pChatChannelDataMap_type pChannelDM =
        Community::Chat::I_ChatChannelDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    Community::Chat::I_ChatChannelDataMap::pFutureChatChannelDataCollection_type pChannelDC =
        pChannelDM->getAll();

    struct ChannelVisitor
    :   public Community::Chat::I_ChatChannelDataCollection::I_CollectionVisitor
    {
        typedef Community::Chat::I_ChatChannelDataCollection::pChatChannelDomainObject_type  pChatChannelDomainObject_type;
        typedef Memory::managed_ptr<Common::I_Channel>                                  pChannel_type;

        virtual void begin()
        {
        }

        virtual void visit(pChatChannelDomainObject_type _pDomainObject)
        {
            m_model.cacheChannel(m_service,_pDomainObject);
            // Queue the channel to be loaded.
        }

        virtual void end()
        {
        }

        ChannelVisitor(ChatService& _service, ChannelModel& _model)
        :   m_service(_service)
        ,   m_model(_model)
        {
        }

    private:
        ChatService&        m_service;
        ChannelModel&       m_model;
    };  // struct ChannelVisitor

    ChannelVisitor visitor(m_service, *this);
    pChannelDC->getValue()->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChannelModel::cacheChannel(ChatService& _service, pChannelDomainObject_type _pDO)
{
    Channels_type::iterator iter = m_channels.find(_pDO->getName());
    if( iter == m_channels.end() )
    {
        m_channels[_pDO->getName()] = pChannel_type(
            new Channel(_service,_pDO),
            &destroyChannel
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
