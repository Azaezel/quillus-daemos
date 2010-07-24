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
#ifndef ZEN_COMMUNITY_CLIENT_CHANNEL_MODEL_PROXY_HPP_INCLUDED
#define ZEN_COMMUNITY_CLIENT_CHANNEL_MODEL_PROXY_HPP_INCLUDED

#include <Zen/Community/ChatCommon/I_ChannelModel.hpp>
#include <Zen/Community/ChatCommon/I_ChannelController.hpp>
#include <Zen/Community/ChatCommon/I_ChannelView.hpp>
#include <Zen/Community/ChatCommon/I_Channel.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Enterprise {
        namespace AppServer {
            class I_Message;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ChatService;

class ChannelModelProxy
:   public Common::I_ChannelModel
{
    /// @name Forward declarations
    /// @{
public:
    struct ProxySubscription;
    struct ProxyChannel;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_Channel>                  pChannel_type;
    typedef Memory::managed_weak_ptr<Common::I_Channel>             wpChannel_type;

    typedef std::map<std::string, pChannel_type>                    Channels_type;

    typedef std::map<boost::uint64_t, pChannelView_type>            ChannelViews_type;

    typedef Event::future_return_value<pChannel_type>               FutureChannel_type;
    typedef Memory::managed_ptr<FutureChannel_type>                 pFutureChannel_type;
    /// @}

    /// @name I_ChannelModel implementation
    /// @{
public:
    virtual pFutureSubscription_type subscribe(Common::I_Session& _session, pChannelView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual Event::I_Event& getChannelEvent();
    /// @}

    /// @name ChannelModelProxy implementation
    /// @{
public:
    pFutureChannel_type createChannel(Common::I_Session& _session, const std::string& _name, const std::string& _description);
    pFutureChannel_type getChannel(Common::I_Session& _session, const std::string& _name);
    void updateChannel(Common::I_Session& _session, Common::I_Channel& _channel);
    void removeChannel(Common::I_Session& _session, Common::I_Channel& _channel);

    void getChannels(Common::I_Session& _session, Common::I_ChannelController::I_ChannelVisitor& _visitor);

    /// Unsubscribe a view from the model.
    void unSubscribe(Common::I_Session& _session, pChannelView_type _pView);

    /// Handle a remote subscription.
    /// TODO Should the view be a managed_ptr<>?
    void handleSubscription(ProxySubscription* _pProxySubscription);

    /// Handle a remote unsubscribe.
    /// TODO Should the view be a managed_ptr<>?
    void handleUnsubscribe(pSubscription_type _pSubscription);

    /// Handle an channel creation
    void handleCreateChannel(ProxyChannel* _pProxyChannel);

    /// Handle an channel read
    void handleReadChannel(ProxyChannel* _pProxyChannel);

    /// Handle an channel update
    void handleUpdateChannel(ProxyChannel* _pProxyChannel);

    /// Handle an channel deletion
    void handleDeleteChannel(ProxyChannel* _pProxyChannel);

    /// Handle a remote update notification.
    typedef Memory::managed_ptr<Enterprise::AppServer::I_Message>   pMessage_type;
    void handleUpdateNotification(pMessage_type _pMessage);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyChannel(wpChannel_type _wpChannel);
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct ProxySubscription
    {
        pFutureSubscription_type    m_pFutureSubscription;
        pChannelView_type           m_pView;
        Common::I_Session&          m_session;
        boost::uint64_t             m_proxyId;

        ProxySubscription(Common::I_Session& _session) : m_pFutureSubscription(), m_pView(), m_session(_session), m_proxyId(0) {}
    };  // struct ProxySubscription

    struct ProxyChannel
    {
        pFutureChannel_type         m_pFutureChannel;
        pChannel_type               m_pChannel;

        ProxyChannel() : m_pFutureChannel(), m_pChannel() {}
    };  // struct ProxyChannel
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit ChannelModelProxy(ChatService& _service);
    virtual ~ChannelModelProxy();
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

    /// Guard for groups
    Threading::I_Mutex*                 m_pChannelsGuard;
    /// @}

};  // class ChannelModelProxy

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CLIENT_CHANNEL_MODEL_PROXY_HPP_INCLUDED
