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
#ifndef ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_MESSAGE_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_MESSAGE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Object.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ChatChannel;
class I_Channel;

class CHATCOMMON_DLL_LINK I_ChatMessage
:   public Spaces::ObjectModel::I_Object
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Channel>                      pChannel_type;
    /// @}

    /// @name I_ChatMessage interface
    /// @{
public:
    /// Get the channel from whence this message came.
    virtual pChannel_type getChannel() const = 0;

    /// Get the message text.
    virtual const std::string& getMessageText() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
             I_ChatMessage();
    virtual ~I_ChatMessage();
    /// @}

};  // interface I_ChatMessage

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Chat
}   // namespace Community
namespace Memory {
    // I_ChatMessage is managed by a factory method
    template<>
    struct is_managed_by_factory<Community::Chat::Common::I_ChatMessage>
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_COMMON_I_CHAT_MESSAGE_HPP_INCLUDED
