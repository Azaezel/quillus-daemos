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
#ifndef ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_SESSION_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_SESSION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Spaces/ObjectModel/I_Object.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_Session;
    }   // namespace Common
namespace Chat {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Channel;
class I_Channels;
class I_ChannelSession;

// Chat service session
class CHATCOMMON_DLL_LINK I_ChatSession
:   public Spaces::ObjectModel::I_Object
{
    /// @name Types
    /// @{
public:
    enum ChatSessionState_type
    {
        CONNECTED,
        DISCONNECTED
    };

    typedef Memory::managed_ptr<Community::Common::I_Session>   pSession_type;

    typedef Memory::managed_ptr<I_Channel>                      pChannel_type;

    typedef Memory::managed_ptr<I_Channels>                     pChannels_type;
    typedef Event::future_return_value<pChannels_type>          FutureChannels_type;
    typedef Memory::managed_ptr<FutureChannels_type>            pFutureChannels_type;

    typedef Memory::managed_ptr<I_ChannelSession>               pChannelSession_type;
    typedef Event::future_return_value<pChannelSession_type>    FutureChannelSession_type;
    typedef Memory::managed_ptr<FutureChannelSession_type>      pFutureChannelSession_type;
    /// @}

    /// @name I_ChatSession interface
    /// @{
public:
    /// Get the chat session state
    virtual ChatSessionState_type getState() const = 0;

    /// Get the parent login session
    ///     Note: A reference to the parent login session should
    ///           retained locally by the chat session instance.
    virtual pSession_type getLoginSession() const = 0;

    /// Disconnect the chat session
    virtual void disconnect() = 0;

    /// Get channel list
    /// @return Future chat channel list/collection
    virtual pFutureChannels_type getChannels() = 0;
    /// @}

    /// @name 'Structors
    /// @{
             I_ChatSession();
    virtual ~I_ChatSession();
    /// @}

};  // interface I_ChatSession

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Chat
}   // namespace Community
namespace Memory {
    // I_ChatSession is managed by a factory method
    template<>
    struct is_managed_by_factory<Community::Chat::Common::I_ChatSession>
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_SESSION_HPP_INCLUDED
