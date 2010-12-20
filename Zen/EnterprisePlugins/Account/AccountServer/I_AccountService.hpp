//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
#ifndef ZEN_ENTERPRISE_ACCOUNT_SERVER_I_ACCOUNT_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_SERVER_I_ACCOUNT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/EnterprisePlugins/Account/AccountClient/I_AccountService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
    namespace Session {
        class I_Session;
    }   // namespace Session
namespace Account {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Account;

/// Account Service interface.
/// @see I_ApplicationServer::getApplication() to get an instance of an
///     implementation of this interface.
class ACCOUNT_SERVER_DLL_LINK I_AccountService
:   public Zen::Enterprise::Account::Client::I_AccountService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Server::I_Account>                         pAccount_type;
    typedef Zen::Memory::managed_weak_ptr<Server::I_Account>                    wpAccount_type;
    typedef Zen::Event::future_return_value<pAccount_type>                      FutureAccount_type;
    typedef Zen::Memory::managed_ptr<FutureAccount_type>                        pFutureAccount_type;
    /// @}

    /// @name I_AccountService interface
    /// @{
public:
    /// Create an account.
    /// This service must be in possession of a valid subscription before calling this method.
    /// @see subscribe().
    virtual pFutureAccount_type createAccount(const std::string& _userId, const std::string& _password) = 0;

    /// Subscribe to an account service.
    /// If an existing subscription exists, it is replaced with the one obtained by this method.
    virtual void subscribe(Enterprise::Session::I_Session& _session, pEndpoint_type _pEndpoint, pResourceLocation_type _pLocation) = 0;

    /// Update the account on the server.
    /// This service must be in possession of a valid subscription before calling this method.
    /// @see subscribe().
    virtual pFutureAccount_type updateAccount(pAccount_type _pAccount) = 0;

    /// Delete the account on the server.
    /// This service must be in possession of a valid subscription before calling this method.
    /// @see subscribe().
    virtual void deleteAccount(pAccount_type _pAccount) = 0;
    /// @}

    /// @name 'Structors
    /// @{
public:
             I_AccountService();
    virtual ~I_AccountService();
    /// @}

};  // interface I_AccountService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_SERVER_I_ACCOUNT_SERVICE_HPP_INCLUDED
