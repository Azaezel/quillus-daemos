//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
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
#ifndef ZEN_COMMUNITY_CHAT_COMMON_I_CHANNEL_SESSION_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_COMMON_I_CHANNEL_SESSION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Object.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ChatSession;

class I_Channel;

/// Channel session
class CHATCOMMON_DLL_LINK I_ChannelSession
:   public Spaces::ObjectModel::I_Object
{
    /// @name Types
    /// @{
public:
    enum ChannelSessionState_type
    {
        ACTIVE,
        INACTIVE
    };

    typedef Memory::managed_ptr<I_ChatSession>  pChatSession_type;
    typedef Memory::managed_ptr<I_Channel>      pChannel_type;
    /// @}

    /// @name I_ChannelSession interface
    /// @{
public:
    /// Get the channel session state
    virtual ChannelSessionState_type getState() const = 0;

    /// Get the channel associated with this session
    virtual pChannel_type getChannel() const = 0;

    /// Get the parent chat session
    ///     Note: A reference to the parent chat session should
    ///           retained locally by the channel session instance.
    virtual pChatSession_type getChatSession() const = 0;

    /// Get the messages as a subscription.
    virtual pSubscription_type getMessages(pView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Get the users for this channel as a subscription.
    virtual pSubscription_type getUsers(pView_type _pView, pFilter_type _pFilter = pFilter_type()) = 0;

    /// Leave the channel and deactivate the channel session
    virtual void leaveChannel() = 0;
    /// @}

    /// @name 'Structors
    /// @{
             I_ChannelSession();
    virtual ~I_ChannelSession();
    /// @}

};  // interface I_ChannelSession

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Chat
}   // namespace Community
namespace Memory {
    // I_ChannelSession is managed by a factory method
    template<>
    struct is_managed_by_factory<Community::Chat::Common::I_ChannelSession>
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_COMMON_I_CHANNEL_SESSION_HPP_INCLUDED
