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
#ifndef ZEN_COMMUNITY_SERVER_CHANNEL_MODEL_HPP_INCLUDED
#define ZEN_COMMUNITY_SERVER_CHANNEL_MODEL_HPP_INCLUDED

#include <Zen/Community/ChatCommon/I_ChannelModel.hpp>
#include <Zen/Community/ChatCommon/I_ChannelController.hpp>
#include <Zen/Community/ChatCommon/I_Channel.hpp>

#include <Zen/Community/ChatModel/I_ChatChannelDataMap.hpp>
#include <Zen/Community/ChatModel/I_ChatChannelDataCollection.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/future_return_value.hpp>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ChatService;

class ChannelModel
:   public Common::I_ChannelModel
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_Channel>                  pChannel_type;
    typedef Memory::managed_weak_ptr<Common::I_Channel>             wpChannel_type;

    typedef std::map<std::string, pChannel_type>                    Channels_type;

    typedef std::set<pChannelView_type>                             ChannelViews_type;

    typedef Event::future_return_value<pChannel_type>               FutureChannel_type;
    typedef Memory::managed_ptr<FutureChannel_type>                 pFutureChannel_type;

    typedef Community::Chat::I_ChatChannelDataCollection::pChatChannelDomainObject_type  pChannelDomainObject_type;
    /// @}

    /// @name I_ChannelModel implementation
    /// @{
public:
    virtual pFutureSubscription_type subscribe(Common::I_Session& _session, pChannelView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual Event::I_Event& getChannelEvent();
    /// @}

    /// @name ChannelModel implementation
    /// @{
public:
    pFutureChannel_type createChannel(Common::I_Session& _session, const std::string& _username, const std::string& _password);
    pFutureChannel_type getChannel(Common::I_Session& _session, const std::string& _username);
    void updateChannel(Common::I_Session& _session, Common::I_Channel& _channel);
    void removeChannel(Common::I_Session& _session, Common::I_Channel& _channel);

    void getChannels(Common::I_Session& _session, Common::I_ChannelController::I_ChannelVisitor& _visitor);

    /// Unsubscribe a view from the model.
    void unSubscribe(Common::I_Session& _session, pChannelView_type _pView);

    /// Fire the onAddChannel notification for all subscribed views.
    void onAddChannel(pChannel_type _pChannel);

    /// Fire the onUpdateChannel notification for all subscribed views.
    void onUpdateChannel(pChannel_type _pChannel);

    /// Fire the onRemoveChannel notification for all subscribed views.
    void onRemoveChannel(pChannel_type _pChannel);

    /// Load the data from the database.
    void loadDataModel();

    /// Cache channel
    void cacheChannel(ChatService& _service, pChannelDomainObject_type _pDO);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyChannel(wpChannel_type _wpChannel);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ChatService;
    explicit ChannelModel(ChatService& _service);
    virtual ~ChannelModel();
    /// @}

    /// @name Member variables
    /// @{
private:
    ChatService&                     m_service;
    Channels_type                       m_channels;

    /// Views subscribed to this model.
    ChannelViews_type                   m_views;

    /// Guard for m_views.
    Threading::I_Mutex*                 m_pViewsGuard;
    /// @}

};  // class ChannelModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SERVER_CHANNEL_MODEL_HPP_INCLUDED
