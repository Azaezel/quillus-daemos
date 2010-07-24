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
#ifndef ZEN_COMMUNITY_SERVER_GROUP_VIEW_PROXY_HPP_INCLUDED
#define ZEN_COMMUNITY_SERVER_GROUP_VIEW_PROXY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Community/AccountCommon/I_GroupView.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

/// Group View Proxy
/// This is a lightweight view implementation
/// used as a remote proxy for networked 
/// subscriptions to a model.
class GroupViewProxy
:   public Common::I_GroupView
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>     pEndpoint_type;
    /// @}

    /// @name I_GroupView implementation
    /// @{
public:
    virtual void onAddGroup(Common::I_Group& _group);
    virtual void onRemoveGroup(Common::I_Group& _group);
    virtual void onUpdateGroup(Common::I_Group& _group);
    /// @}

    /// @name GroupViewProxy implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             GroupViewProxy(AccountService& _service, pEndpoint_type _pEndpoint);
    virtual ~GroupViewProxy();
    /// @}

    /// @name Member variables
    /// @{
private:
    AccountService&             m_service;

    pEndpoint_type              m_pEndpoint;
    /// @}

};  // class GroupViewProxy

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SERVER_GROUP_VIEW_PROXY_HPP_INCLUDED
