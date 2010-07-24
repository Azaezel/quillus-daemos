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
#ifndef ZEN_COMMUNITY_SERVER_ACCOUNT_MODEL_HPP_INCLUDED
#define ZEN_COMMUNITY_SERVER_ACCOUNT_MODEL_HPP_INCLUDED

#include <Zen/Community/AccountCommon/I_AccountModel.hpp>
#include <Zen/Community/AccountCommon/I_AccountController.hpp>
#include <Zen/Community/AccountCommon/I_Account.hpp>
#include <Zen/Community/AccountCommon/subscription.hpp>

#include <Zen/Community/AccountModel/I_AccountDataMap.hpp>
#include <Zen/Community/AccountModel/I_AccountDataCollection.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/future_return_value.hpp>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

class AccountModel
:   public Common::I_AccountModel
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_Account>                  pAccount_type;
    typedef Memory::managed_weak_ptr<Common::I_Account>             wpAccount_type;

    typedef std::map<std::string, pAccount_type>                    Accounts_type;

    typedef std::set<pAccountView_type>                             AccountViews_type;

    typedef Event::future_return_value<pAccount_type>               FutureAccount_type;
    typedef Memory::managed_ptr<FutureAccount_type>                 pFutureAccount_type;

    typedef Community::Account::I_AccountDataCollection::pAccountDomainObject_type  pAccountDomainObject_type;
    /// @}

    /// @name I_AccountModel implementation
    /// @{
public:
    virtual pFutureSubscription_type subscribe(pEndpoint_type _pDestinationEndpoint, pAccountView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual Event::I_Event& getAccountEvent();
    /// @}

    /// @name AccountModel implementation
    /// @{
public:
    pFutureAccount_type createAccount(Common::I_Session& _session, const std::string& _username, const std::string& _password);
    pFutureAccount_type getAccount(Common::I_Session& _session, const std::string& _username);
    void updateAccount(Common::I_Session& _session, Common::I_Account& _account);
    void removeAccount(Common::I_Session& _session, Common::I_Account& _account);

    void getAccounts(Common::I_Session& _session, Common::I_AccountController::I_AccountVisitor& _visitor);

    /// Unsubscribe a view from the model.
    typedef Common::subscription<AccountModel*, pAccountView_type> Subscription;
    void unSubscribe(Subscription* _pSubscription);

    /// Fire the onAddAccount notification for all subscribed views.
    void onAddAccount(pAccount_type _pAccount);

    /// Fire the onUpdateAccount notification for all subscribed views.
    void onUpdateAccount(pAccount_type _pAccount);

    /// Fire the onRemoveAccount notification for all subscribed views.
    void onRemoveAccount(pAccount_type _pAccount);

    /// Load the data from the database.
    void loadDataModel();

    /// Cache account
    void cacheAccount(AccountService& _service, pAccountDomainObject_type _pDO);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyAccount(wpAccount_type _wpAccount);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class AccountService;
    explicit AccountModel(AccountService& _service);
    virtual ~AccountModel();
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
    /// @}

};  // class AccountModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SERVER_ACCOUNT_MODEL_HPP_INCLUDED
