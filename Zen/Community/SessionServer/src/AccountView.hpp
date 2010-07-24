//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_COMMUNITY_SESSION_SERVER_ACCOUNT_VIEW_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSION_SERVER_ACCOUNT_VIEW_HPP_INCLUDED

#include <Zen/Community/AccountCommon/I_AccountView.hpp>

#include <boost/cstdint.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AccountView
:   public Common::I_AccountView
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, std::string>      UsernameToPassword_type;
    typedef std::map<boost::uint64_t, std::string>  IdToUsername_type;
    /// @}

    /// @name I_AccountView implementation
    /// @{
public:
    virtual void onAddAccount(Common::I_Account& _account);
    virtual void onRemoveAccount(Common::I_Account& _account);
    virtual void onUpdateAccount(Common::I_Account& _account);
    /// @}

    /// @name AccountView implementation
    /// @{
public:
    bool authenticate(const std::string& _username, const std::string& _password);
    /// @}

    /// @name 'Structors
    /// @{
public:
             AccountView();
    virtual ~AccountView();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Threading::I_Mutex*             m_pAccountsGuard;
    IdToUsername_type               m_accountsIdx;
    UsernameToPassword_type         m_accounts;
    /// @}

};  // class AccountView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSION_SERVER_ACCOUNT_VIEW_HPP_INCLUDED
