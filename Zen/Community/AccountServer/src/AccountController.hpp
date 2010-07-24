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
#ifndef ZEN_COMMUNITY_SERVER_ACCOUNT_CONTROLLER_HPP_INCLUDED
#define ZEN_COMMUNITY_SERVER_ACCOUNT_CONTROLLER_HPP_INCLUDED

#include <Zen/Community/AccountCommon/I_AccountController.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;
class AccountModel;

class AccountController
:   public Common::I_AccountController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_AccountController implementation
    /// @{
public:
    virtual Common::I_AccountService& getAccountService() const;
    virtual pFutureAccount_type createAccount(Common::I_Session& _session, const std::string& _username, const std::string& _password);
    virtual pFutureAccount_type getAccount(Common::I_Session& _session, const std::string& _username);
    virtual void updateAccount(Common::I_Session& _session, Common::I_Account& _account);
    virtual void removeAccount(Common::I_Session& _session, Common::I_Account& _account);
    virtual void getAccounts(Common::I_Session& _session, I_AccountVisitor& _visitor);
    /// @}

    /// @name AccountController implementation
    /// @{
public:
    AccountModel& getModel() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit AccountController(AccountService& _service, Common::I_AccountModel& _model);
    virtual ~AccountController();
    /// @}

    /// @name Member variables
    /// @{
private:
    AccountService&                 m_service;
    AccountModel&                   m_model;
    /// @}

};  // class AccountController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SERVER_ACCOUNT_CONTROLLER_HPP_INCLUDED
