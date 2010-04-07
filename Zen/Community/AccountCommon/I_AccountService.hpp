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
#ifndef ZEN_COMMUNITY_ACCOUNTCOMMON_I_ACCOUNT_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTCOMMON_I_ACCOUNT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Account;    
class I_AccountModel;
class I_AccountController;
class I_Group;
class I_GroupModel;
class I_GroupController;
class I_PermissibleResource;

/// Account Service interface.
/// See this discussion for more details about this design.
/// http://www.indiezen.org/wiki/irclogs/2009/03/19/#UTC2009-03-19T14:27:25
/// @see I_ApplicationServer::getApplication() to get an instance of an 
///     implementation of this interface.
class ACCOUNTCOMMON_DLL_LINK I_AccountService
:   public Enterprise::AppServer::I_ApplicationService
{
    /// @name Forward declarations
    /// @{
public:
    struct I_AccountVisitor;
    struct I_GroupVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_AccountService>			        pService_type;

    typedef Memory::managed_ptr<I_AccountModel>                     pAccountModel_type;
    typedef Memory::managed_weak_ptr<I_AccountModel>                wpAccountModel_type;
    typedef Event::future_return_value<pAccountModel_type>          FutureAccountModel_type;
    typedef Memory::managed_ptr<FutureAccountModel_type>            pFutureAccountModel_type;

    typedef Memory::managed_ptr<I_AccountController>                pAccountController_type;
    typedef Memory::managed_weak_ptr<I_AccountController>           wpAccountController_type;
    typedef Event::future_return_value<pAccountController_type>     FutureAccountController_type;
    typedef Memory::managed_ptr<FutureAccountController_type>       pFutureAccountController_type;

    typedef Memory::managed_ptr<I_GroupModel>                       pGroupModel_type;
    typedef Memory::managed_weak_ptr<I_GroupModel>                  wpGroupModel_type;
    typedef Event::future_return_value<pGroupModel_type>            FutureGroupModel_type;
    typedef Memory::managed_ptr<FutureGroupModel_type>              pFutureGroupModel_type;

    typedef Memory::managed_ptr<I_GroupController>                  pGroupController_type;
    typedef Memory::managed_weak_ptr<I_GroupController>             wpGroupController_type;
    typedef Event::future_return_value<pGroupController_type>       FutureGroupController_type;
    typedef Memory::managed_ptr<FutureGroupController_type>         pFutureGroupController_type;
    /// @}

    /// @name I_AccountService interface
    /// @{
public:
    /// Create an account.
    virtual I_Account& createAccount(const std::string& _userName, const std::string& _password) = 0;

    /// Create a group.
    virtual I_Group& createGroup(const std::string& _groupName) = 0;

    /// Create a permissible resource.
    virtual I_PermissibleResource& createPermissibleResource(const std::string& _resourceName) = 0;

    /// Set the owner of a resource.
    virtual void setResourceOwner(I_PermissibleResource& _resource, I_Account& _owner) = 0;

    virtual pFutureAccountModel_type createAccountModel() = 0;

    virtual pFutureAccountController_type createAccountController(I_AccountModel& _model) = 0;

    virtual pFutureGroupModel_type createGroupModel() = 0;

    virtual pFutureGroupController_type createGroupController(I_GroupModel& _model) = 0;

    /// TODO These should be propagated to I_PermissibleResource
    /// and I_AccountService should be made to extend I_PermissibleResource
    virtual boost::uint32_t getAccessFlags() const = 0;
    virtual void setAccessFlags(boost::uint32_t _accessFlags) = 0;
    virtual const Common::I_Account& getOwner() const = 0;
    virtual void setOwner(const Common::I_Account& _owner) = 0;
    virtual const Common::I_Group& getGroup() const = 0;
    virtual void setGroup(const Common::I_Group& _group) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_AccountService();
public:
    virtual ~I_AccountService();
    /// @}

};  // interface I_AccountService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTCOMMON_I_ACCOUNT_SERVICE_HPP_INCLUDED
