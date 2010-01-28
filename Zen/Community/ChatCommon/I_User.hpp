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
#ifndef ZEN_COMMUNITY_CHAT_COMMON_I_USER_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_COMMON_I_USER_HPP_INCLUDED

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

class I_UserInfo;
class I_UserPermissions;

/// Chat user (should this inherit from something like
/// Login::Common::I_User or expose said functionality through composition?)
class CHATCOMMON_DLL_LINK I_User
:   public Spaces::ObjectModel::I_Object
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_UserInfo>                     pUserInfo_type;
    typedef Event::future_return_value<pUserInfo_type>          pFutureUserInfo_type;

    typedef Memory::managed_ptr<I_UserPermissions>              pUserPermissions_type;
    typedef Event::future_return_value<pUserPermissions_type>   pFutureUserPermissions_type;
    /// @}

    /// @name I_User interface
    /// @{
public:
    /// Get the user name
    virtual const std::string& getName() const = 0;

    /// Get the user information
    virtual pFutureUserInfo_type getInfo() = 0;

    /// Get the user permissions
    virtual pFutureUserPermissions_type getPermissions() = 0;
    /// @}

    /// @name 'Structors
    /// @{
             I_User();
    virtual ~I_User();
    /// @}

};  // interface I_User

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Chat
}   // namespace Community
namespace Memory {
    // I_User is managed by a factory method
    template<>
    struct is_managed_by_factory<Community::Chat::Common::I_User>
        :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_COMMON_I_USER_HPP_INCLUDED
