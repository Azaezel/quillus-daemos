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
#ifndef ZEN_COMMUNITY_CHAT_COMMON_I_CHANNEL_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_COMMON_I_CHANNEL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Spaces/ObjectModel/I_Object.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Users;
class I_ChannelInfo;
class I_ChannelPermissions;
class I_ChannelSession;

/// Represents a chat channel
class CHATCOMMON_DLL_LINK I_Channel
:   public Spaces::ObjectModel::I_Object
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Users>                            pUsers_type;
    typedef Event::future_return_value<pUsers_type>                 pFutureUsers_type;

    typedef Memory::managed_ptr<I_ChannelInfo>                      pChannelInfo_type;
    typedef Event::future_return_value<pChannelInfo_type>           pFutureChannelInfo_type;

    typedef Memory::managed_ptr<I_ChannelSession>                   pChannelSession_type;
    typedef Event::future_return_value<pChannelSession_type>        pFutureChannelSession_type;

    typedef Memory::managed_ptr<I_ChannelPermissions>               pChannelPermissions_type;
    typedef Event::future_return_value<pChannelPermissions_type>    pFutureChannelPermissions_type;
    /// @}

    /// @name I_Channel interface
    /// @{
public:
    /// Get the channel name
    virtual const std::string& getName() const = 0;

    /// Get a snapshot of the current users in channel.
    /// @return List/collection of users in channel
    virtual pFutureUsers_type getUsers() = 0;

    /// Get channel information
    /// @return Channel info
    /// @todo TR - I think this needs to be collapsed.  I_Channel is I_ChannelInfo
    ///         and does not associated with a I_ChannelSession until 
    virtual pFutureChannelInfo_type getInfo() = 0;

    /// Join a chat channel
    /// @param _pChannel Channel to join
    /// @return Future chat channel session
    virtual pFutureChannelSession_type join() = 0;

    /// Get the channel permissions
    /// @return Channel permissions
    virtual pFutureChannelPermissions_type getPermissions() = 0;

    /// @}

    /// @name 'Structors
    /// @{
             I_Channel();
    virtual ~I_Channel();
    /// @}

};  // interface I_Channel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Chat
}   // namespace Community
namespace Memory {
    /// I_Channel is managed by a factory method
    template<>
    struct is_managed_by_factory<Community::Chat::Common::I_Channel>
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_COMMON_I_CHANNEL_HPP_INCLUDED
