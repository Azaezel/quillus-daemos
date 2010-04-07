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
#ifndef ZEN_COMMUNITY_CLIENT_ACCOUNT_MODEL_PROXY_HPP_INCLUDED
#define ZEN_COMMUNITY_CLIENT_ACCOUNT_MODEL_PROXY_HPP_INCLUDED

#include <Zen/Community/AccountCommon/I_AccountModel.hpp>
#include <Zen/Community/AccountCommon/I_AccountController.hpp>
#include <Zen/Community/AccountCommon/I_AccountView.hpp>
#include <Zen/Community/AccountCommon/I_Account.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Enterprise {
        namespace AppServer {
            class I_Message;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

class AccountModelProxy
:   public Common::I_AccountModel
{
    /// @name Forward declarations
    /// @{
public:
    struct ProxySubscription;
    struct ProxyAccount;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_Account>                  pAccount_type;
    typedef Memory::managed_weak_ptr<Common::I_Account>             wpAccount_type;

    typedef std::map<std::string, pAccount_type>                    Accounts_type;

    typedef std::map<boost::uint64_t, pAccountView_type>            AccountViews_type;

    typedef Event::future_return_value<pAccount_type>               FutureAccount_type;
    typedef Memory::managed_ptr<FutureAccount_type>                 pFutureAccount_type;
    /// @}

    /// @name I_AccountModel implementation
    /// @{
public:
    virtual pFutureSubscription_type subscribe(Common::I_Session& _session, pAccountView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual Event::I_Event& getAccountEvent();
    /// @}

    /// @name AccountModelProxy implementation
    /// @{
public:
    pFutureAccount_type createAccount(Common::I_Session& _session, const std::string& _username, const std::string& _password);
    pFutureAccount_type getAccount(Common::I_Session& _session, const std::string& _username);
    void updateAccount(Common::I_Session& _session, Common::I_Account& _account);
    void removeAccount(Common::I_Session& _session, Common::I_Account& _account);

    void getAccounts(Common::I_Session& _session, Common::I_AccountController::I_AccountVisitor& _visitor);

    /// Unsubscribe a view from the model.
    void unSubscribe(Common::I_Session& _session, pAccountView_type _pView);

    /// Handle a remote subscription.
    /// TODO Should the view be a managed_ptr<>?
    void handleSubscription(ProxySubscription* _pProxySubscription);

    /// Handle a remote unsubscribe.
    /// TODO Should the view be a managed_ptr<>?
    void handleUnsubscribe(pSubscription_type _pSubscription);

    /// Handle an account creation
    void handleCreateAccount(ProxyAccount* _pProxyAccount);

    /// Handle an account read
    void handleReadAccount(ProxyAccount* _pProxyAccount);

    /// Handle an account update
    void handleUpdateAccount(ProxyAccount* _pProxyAccount);

    /// Handle an account deletion
    void handleDeleteAccount(ProxyAccount* _pProxyAccount);

    /// Handle a remote update notification.
    typedef Memory::managed_ptr<Enterprise::AppServer::I_Message>   pMessage_type;
    void handleUpdateNotification(pMessage_type _pMessage);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyAccount(wpAccount_type _wpAccount);
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct ProxySubscription
    {
        pFutureSubscription_type    m_pFutureSubscription;
        pAccountView_type           m_pView;
        Common::I_Session&          m_session;
        boost::uint64_t             m_proxyId;

        ProxySubscription(Common::I_Session& _session) : m_pFutureSubscription(), m_pView(), m_session(_session), m_proxyId(0) {}
    };  // struct ProxySubscription

    struct ProxyAccount
    {
        pFutureAccount_type         m_pFutureAccount;
        pAccount_type               m_pAccount;

        ProxyAccount() : m_pFutureAccount(), m_pAccount() {}
    };  // struct ProxyAccount
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit AccountModelProxy(AccountService& _service);
    virtual ~AccountModelProxy();
    /// @}

    /// @name Member variables
    /// @{
private:
    AccountService&                     m_service;
    Accounts_type                       m_accounts;

    /// Views subscribed to this model.
    AccountViews_type                   m_views;

    /// Guard for m_views.
    Threading::I_Mutex*                 m_pViewsGuard;

    /// Guard for groups
    Threading::I_Mutex*                 m_pAccountsGuard;
    /// @}

};  // class AccountModelProxy

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CLIENT_ACCOUNT_MODEL_PROXY_HPP_INCLUDED
