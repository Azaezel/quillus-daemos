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
#ifndef ZEN_COMMUNITY_ACCOUNTCOMMON_ACCOUNT_CONTROLLER_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTCOMMON_ACCOUNT_CONTROLLER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Account;
class I_AccountModel;
class I_AccountService;
class I_Session;

class ACCOUNTCOMMON_DLL_LINK I_AccountController
:   public boost::noncopyable
{
    /// @name Forward declarations
    /// @{
public:
    struct I_AccountVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Account>                          pAccount_type;
    typedef Event::future_return_value<pAccount_type>               FutureAccount_type;
    typedef Memory::managed_ptr<FutureAccount_type>                 pFutureAccount_type;
    /// @}

    /// @name I_AccountController interface
    /// @{
public:
    /// Get the account service
    virtual I_AccountService& getAccountService() const = 0;

    /// Create a new account
    virtual pFutureAccount_type createAccount(I_Session& _session, const std::string& _username, const std::string& _password) = 0;

    /// Get an existing account
    virtual pFutureAccount_type getAccount(I_Session& _session, const std::string& _username) = 0;

    /// Update an existing account
    virtual void updateAccount(I_Session& _session, Common::I_Account& _account) = 0;

    /// Remove an existing account
    virtual void removeAccount(I_Session& _session, Common::I_Account& _account) = 0;

    /// Visit all existing accounts
    virtual void getAccounts(I_Session& _session, I_AccountVisitor& _visitor) = 0;
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct I_AccountVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_Account& _account) = 0;
        virtual void end() = 0;
    };  // interface I_AccountVisitor
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_AccountController();
    virtual ~I_AccountController();
    /// @}

};  // interface I_AccountController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
	/// I_AccountController is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_AccountController> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTCOMMON_ACCOUNT_CONTROLLER_HPP_INCLUDED
