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

#include "AccountModel.hpp"
#include "Account.hpp"
#include "AccountService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Enterprise/AppServer/I_Message.hpp>
#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Community/AccountCommon/subscription.hpp>
#include <Zen/Community/AccountCommon/I_Account.hpp>
#include <Zen/Community/AccountCommon/I_AccountView.hpp>

#include <Zen/Community/AccountModel/I_AccountDomainObject.hpp>
#include <Zen/Community/AccountModel/I_AccountDataMap.hpp>
#include <Zen/Community/AccountModel/I_AccountDataCollection.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModel::AccountModel(AccountService& _service)
:   m_service(_service)
,   m_pViewsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModel::~AccountModel()
{
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Common::subscription<AccountModel*, AccountModel::pAccountView_type> Subscription;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription* pSubscription =
        dynamic_cast<Subscription*>(
            _wpSubscription.get()
        );

    if( pSubscription != NULL )
    {
        delete pSubscription;
    }
    else
    {
        /// TODO Exception?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModel::pFutureSubscription_type
AccountModel::subscribe(Common::I_Session& _session, pAccountView_type _pView, pFilter_type _pFilter)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make async
    pFutureSubscription_type pReturnValue(new FutureSubscription_type());

    Threading::CriticalSection lock(m_pViewsGuard);

    m_views.insert(_pView);

    // This view needs to be notified of all of the existing model.
    for(Accounts_type::iterator iter = m_accounts.begin(); iter != m_accounts.end(); iter++)
    {
        // Notify the view of an account
        _pView->onAddAccount(*iter->second.get());
    }

    pSubscription_type pSubscription(
        new Subscription(
            _session,
            this,
            _pView
        ),
        &destroySubscription
    );

    pReturnValue->setValue(pSubscription);

    return pReturnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event& 
AccountModel::getAccountEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("Account::Server::AccountEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::destroyAccount(wpAccount_type _wpAccount)
{
    Account* pRaw = 
        dynamic_cast<Account*>(_wpAccount.get());

    if( pRaw != NULL )
    {
        delete pRaw;
    }
    else
    {
        /// TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModel::pFutureAccount_type 
AccountModel::createAccount(Common::I_Session& _session, const std::string& _username, const std::string& _password)
{
	// TODO Instead of returning a future, fire the account event indicating
	// that this account has been created.

    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make this async
    pFutureAccount_type returnValue(new FutureAccount_type);

    // Get the data map.
    Community::Account::I_AccountDataMap::pAccountDataMap_type pAccountDM =
        Community::Account::I_AccountDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    // Create a new domain object.
    Community::Account::I_AccountDataMap::pAccountDomainObject_type pAccountDO =
        pAccountDM->createNew();

    // Populate the values.
    pAccountDO->getUser()           = _username;
    pAccountDO->getPassword()       = _password;

    // Update the object (and in this case, insert it).
    boost::uint64_t newAccountId    = pAccountDM->update(pAccountDO)->getValue();
    pAccountDO->getAccountId()      = newAccountId;

    Account* pRawAccount = new Account(m_service, pAccountDO);

    pAccount_type pAccount(
        pRawAccount,
        &destroyAccount
    );

    m_accounts[_username] = pAccount;

    onAddAccount(pAccount);

    returnValue->setValue(pAccount);

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModel::pFutureAccount_type 
AccountModel::getAccount(Common::I_Session& _session, const std::string& _username)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make this async
    pFutureAccount_type returnValue(new FutureAccount_type);

    pAccount_type pAccount;
    Accounts_type::iterator iter = m_accounts.find(_username);
    if(iter != m_accounts.end() )
    {
        pAccount = iter->second;        
    }

    returnValue->setValue(pAccount);

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::updateAccount(Common::I_Session& _session, Common::I_Account& _account)
{
    /// TODO Check session (i.e. session user) against permissions.

    // Get the data map.
    Community::Account::I_AccountDataMap::pAccountDataMap_type pAccountDM =
        Community::Account::I_AccountDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    Account* pRawAccount = dynamic_cast<Account*>(&_account);

    // Update the object.
    pAccountDM->update(pRawAccount->getDO())->getValue();

    onUpdateAccount(pRawAccount->getSelfReference().lock());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::removeAccount(Common::I_Session& _session, Common::I_Account& _account)
{
    /// TODO Check session (i.e. session user) against permissions.

    throw Zen::Utility::runtime_exception("AccountModel::removeAccount() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModel::getAccounts(Common::I_Session& _session, Common::I_AccountController::I_AccountVisitor& _visitor)
{
    _visitor.begin();

    Accounts_type::iterator iter = m_accounts.begin();
    while( iter != m_accounts.end() )
    {
        _visitor.visit(*iter->second);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::unSubscribe(Common::I_Session& _session, pAccountView_type _pView)
{
    Threading::CriticalSection lock(m_pViewsGuard);
    m_views.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::onAddAccount(pAccount_type _pAccount)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    AccountViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(AccountViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(AccountViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onAddAccount(*_pAccount);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::onUpdateAccount(pAccount_type _pAccount)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    AccountViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(AccountViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(AccountViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onUpdateAccount(*_pAccount);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::onRemoveAccount(pAccount_type _pAccount)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    AccountViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(AccountViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(AccountViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onRemoveAccount(*_pAccount);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::loadDataModel()
{
    // Get the data map.
    Community::Account::I_AccountDataMap::pAccountDataMap_type pAccountDM =
        Community::Account::I_AccountDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    Community::Account::I_AccountDataMap::pFutureAccountDataCollection_type pAccountDC =
        pAccountDM->getAll();

    struct AccountVisitor
    :   public Community::Account::I_AccountDataCollection::I_CollectionVisitor
    {
        typedef Community::Account::I_AccountDataCollection::pAccountDomainObject_type  pAccountDomainObject_type;
        typedef Memory::managed_ptr<Common::I_Account>                                  pAccount_type;

        virtual void begin()
        {
        }

        virtual void visit(pAccountDomainObject_type _pDomainObject)
        {
            m_model.cacheAccount(m_service,_pDomainObject);
            // Queue the account to be loaded.
        }

        virtual void end()
        {
        }

        AccountVisitor(AccountService& _service, AccountModel& _model)
        :   m_service(_service)
        ,   m_model(_model)
        {
        }

    private:
        AccountService&     m_service;
        AccountModel&       m_model;
    };  // struct AccountVisitor

    AccountVisitor visitor(m_service, *this);
    pAccountDC->getValue()->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModel::cacheAccount(AccountService& _service, pAccountDomainObject_type _pDO)
{
    Accounts_type::iterator iter = m_accounts.find(_pDO->getUser());
    if( iter == m_accounts.end() )
    {
        m_accounts[_pDO->getUser()] = pAccount_type(
            new Account(_service,_pDO),
            &destroyAccount
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
