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
#ifndef ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Community {
    namespace Common {
        class I_Session;
    }   // namespace Common
namespace Chat {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ChatSession;

class I_Channel;
class I_Channels;
class I_ChannelSession;

/// Chat Service interface.
/// @see I_ApplicationServer::getApplication() to get an instance of an
///     implementation of this interface.
class CHATCOMMON_DLL_LINK I_ChatService
:   public Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>         pEndpoint_type;

    typedef Memory::managed_ptr<I_ChatService>                  pService_type;

    typedef Memory::managed_ptr<Community::Common::I_Session>   pSession_type;

    typedef Memory::managed_ptr<I_ChatSession>                  pChatSession_type;
    typedef Event::Event<pChatSession_type>                     ChatSessionEvent_type;
    typedef Event::future_return_value<pChatSession_type>       FutureChatSession_type;
    typedef Memory::managed_ptr<FutureChatSession_type>         pFutureChatSession_type;
    /// @}

    /// @name I_ChatService interface
    /// @{
public:
    /// Connect to the chat server
    /// @param _pSession Current active login session
    /// @return Future chat session
    virtual pFutureChatSession_type connect(pSession_type _pSession) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// This event will fire when the session status changes.
    ChatSessionEvent_type onSessionStatusChange;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatService();
public:
    virtual ~I_ChatService();
    /// @}

};  // interface I_ChatService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Chat
}   // namespace Community
namespace Memory {
    // I_ChatService is managed by a factory method
    template<>
    struct is_managed_by_factory<Community::Chat::Common::I_ChatService>
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_SERVICE_HPP_INCLUDED
