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

#include "AccountView.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>

#include <Zen/Community/AccountCommon/I_Account.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountView::AccountView()
:   m_accountsIdx()
,   m_accounts()
,   m_pAccountsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountView::~AccountView()
{
    Threading::MutexFactory::destroy(m_pAccountsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountView::onAddAccount(Common::I_Account& _account)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    IdToUsername_type::iterator iter = m_accountsIdx.find(_account.getAccountId());
    if( iter == m_accountsIdx.end() )
    {
        m_accountsIdx[_account.getAccountId()] = _account.getUsername();
        m_accounts[_account.getUsername()] = _account.getPassword();
    }
    else
    {
        throw Zen::Utility::runtime_exception("AccountView::onAddAccount() : Error, account already present in view.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountView::onRemoveAccount(Common::I_Account& _account)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    IdToUsername_type::iterator iter = m_accountsIdx.find(_account.getAccountId());
    if( iter != m_accountsIdx.end() )
    {
        UsernameToPassword_type::iterator account = m_accounts.find(m_accountsIdx[_account.getAccountId()]);
        if( account != m_accounts.end() )
        {
            m_accounts.erase(account);
        }
        m_accountsIdx.erase(iter);
    }

    /// TODO Error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountView::onUpdateAccount(Common::I_Account& _account)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    IdToUsername_type::iterator iter = m_accountsIdx.find(_account.getAccountId());
    if( iter != m_accountsIdx.end() )
    {
        if( _account.getUsername() != iter->second )
        {
            UsernameToPassword_type::iterator account = m_accounts.find(iter->second);
            if( account != m_accounts.end() )
            {
                m_accounts.erase(account);
            }
            iter->second = _account.getUsername();
        }

        m_accounts[iter->second] = _account.getPassword();
    }
    
    /// TODO Error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
AccountView::authenticate(const std::string& _username, const std::string& _password)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    UsernameToPassword_type::iterator iter = m_accounts.find(_username);
    if( iter != m_accounts.end() )
    {
        return ( _password == iter->second );
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
