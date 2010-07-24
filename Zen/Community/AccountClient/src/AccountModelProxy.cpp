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

#include "AccountModelProxy.hpp"
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

#include <Zen/Community/AccountProtocol/I_SubscribeAccountModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeAccountModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_CreateAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_ReadAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UpdateAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_DeleteAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_AccountModelUpdateMessage.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModelProxy::AccountModelProxy(AccountService& _service)
:   m_service(_service)
,   m_pViewsGuard(Threading::MutexFactory::create())
,   m_pAccountsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModelProxy::~AccountModelProxy()
{
    Threading::MutexFactory::destroy(m_pAccountsGuard);
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    AccountModelProxy::Subscription* pSubscription =
        dynamic_cast<AccountModelProxy::Subscription*>(
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
AccountModelProxy::pFutureSubscription_type
AccountModelProxy::subscribe(pEndpoint_type _pDestinationEndpoint, pAccountView_type _pView, pFilter_type _pFilter)
{
    /// TODO make async
    ProxySubscription* pProxySubscription = new ProxySubscription(_pDestinationEndpoint);

    /// Create a new local future subscription object.
    pProxySubscription->m_pFutureSubscription = pFutureSubscription_type(
        new FutureSubscription_type()
    );

    pProxySubscription->m_pView = _pView;

    // Create a subscription request using pFutureSubscription as the payload.
    Zen::Enterprise::AppServer::create_request<Zen::Community::Account::Protocol::I_SubscribeAccountModelRequest, ProxySubscription*>
        request(_pDestinationEndpoint, pProxySubscription);
    
    m_service.send<Zen::Community::Account::Protocol::I_SubscribeAccountModelRequest, ProxySubscription*>
        (request, boost::bind(&AccountService::handleSubscribeAccountModelResponse, &m_service, _1, _2, _3));

    return pProxySubscription->m_pFutureSubscription;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event& 
AccountModelProxy::getAccountEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("Account::Client::AccountEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModelProxy::destroyAccount(wpAccount_type _wpAccount)
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
AccountModelProxy::pFutureAccount_type 
AccountModelProxy::createAccount(Common::I_Session& _session, const std::string& _username, const std::string& _password)
{
    /// TODO make async
    ProxyAccount* pProxyAccount = new ProxyAccount();

    /// Create a new local future subscription object
    pProxyAccount->m_pFutureAccount = pFutureAccount_type(
        new FutureAccount_type()
    );

    pProxyAccount->m_pAccount = pAccount_type(
        new Account(m_service),
        &AccountModelProxy::destroyAccount
    );

    typedef Memory::managed_ptr<Account>    pConcreteAccount_type;
    pConcreteAccount_type pConcreteAccount = pProxyAccount->m_pAccount.as<pConcreteAccount_type>();

    /// Populate the payload
    pConcreteAccount->setUsername(_username);
    pConcreteAccount->setPassword(_password);

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an account creation request using ProxyAccount* as the payload.
    Zen::Enterprise::AppServer::create_request<Zen::Community::Account::Protocol::I_CreateAccountRequest, ProxyAccount*>
        request(pDestinationEndpoint, pProxyAccount);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setUsername(_username);
    request->setPassword(_password);

    // Send the request
    m_service.send<Zen::Community::Account::Protocol::I_CreateAccountRequest, ProxyAccount*>
        (request, boost::bind(&AccountService::handleCreateAccountResponse, &m_service, _1, _2, _3));

    // This future account will be updated upon receipt of the 
    // associated response.
    // TODO What happens if the response is never received?
    return pProxyAccount->m_pFutureAccount;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountModelProxy::pFutureAccount_type 
AccountModelProxy::getAccount(Common::I_Session& _session, const std::string& _username)
{
    /// TODO make async
    ProxyAccount* pProxyAccount = new ProxyAccount();

    /// Create a new local future subscription object
    pProxyAccount->m_pFutureAccount = pFutureAccount_type(
        new FutureAccount_type()
    );

    pProxyAccount->m_pAccount = pAccount_type(
        new Account(m_service),
        &AccountModelProxy::destroyAccount
    );

    typedef Memory::managed_ptr<Account>    pConcreteAccount_type;
    pConcreteAccount_type pConcreteAccount = pProxyAccount->m_pAccount.as<pConcreteAccount_type>();

    /// Populate the payload
    pConcreteAccount->setUsername(_username);

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an account read request using ProxyAccount* as the payload.
    Zen::Enterprise::AppServer::create_request<Zen::Community::Account::Protocol::I_ReadAccountRequest, ProxyAccount*>
        request(pDestinationEndpoint, pProxyAccount);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setUsername(_username);

    // Send the request
    m_service.send<Zen::Community::Account::Protocol::I_ReadAccountRequest, ProxyAccount*>
        (request, boost::bind(&AccountService::handleReadAccountResponse, &m_service, _1, _2, _3));

    // This future account will be updated upon receipt of the 
    // associated response.
    // TODO What happens if the response is never received?
    return pProxyAccount->m_pFutureAccount;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModelProxy::updateAccount(Common::I_Session& _session, Common::I_Account& _account)
{
    /// TODO make async
    ProxyAccount* pProxyAccount = new ProxyAccount();

    /// Create a new local future subscription object
    pProxyAccount->m_pFutureAccount = pFutureAccount_type(
        new FutureAccount_type()
    );

    Account* pAccount = dynamic_cast<Account*>(&_account);

    pProxyAccount->m_pAccount = pAccount->getSelfReference().lock();

    typedef Memory::managed_ptr<Account>    pConcreteAccount_type;
    pConcreteAccount_type pConcreteAccount = pProxyAccount->m_pAccount.as<pConcreteAccount_type>();

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an account read request using ProxyAccount* as the payload.
    Zen::Enterprise::AppServer::create_request<Zen::Community::Account::Protocol::I_UpdateAccountRequest, ProxyAccount*>
        request(pDestinationEndpoint, pProxyAccount);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setAccountId(_account.getAccountId());
    request->setUsername(_account.getUsername());
    request->setPassword(pAccount->getPassword());

    // Send the request
    m_service.send<Zen::Community::Account::Protocol::I_UpdateAccountRequest, ProxyAccount*>
        (request, boost::bind(&AccountService::handleUpdateAccountResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModelProxy::removeAccount(Common::I_Session& _session, Common::I_Account& _account)
{
    /// TODO make async
    ProxyAccount* pProxyAccount = new ProxyAccount();

    /// Create a new local future subscription object
    pProxyAccount->m_pFutureAccount = pFutureAccount_type(
        new FutureAccount_type()
    );

    Account* pAccount = dynamic_cast<Account*>(&_account);

    pProxyAccount->m_pAccount = pAccount->getSelfReference().lock();

    typedef Memory::managed_ptr<Account>    pConcreteAccount_type;
    pConcreteAccount_type pConcreteAccount = pProxyAccount->m_pAccount.as<pConcreteAccount_type>();

    typedef Memory::managed_ptr<Zen::Networking::I_Endpoint>    pEndpoint_type;
    pEndpoint_type pDestinationEndpoint = _session.getEndpoint();

    // Create an account read request using ProxyAccount* as the payload.
    Zen::Enterprise::AppServer::create_request<Zen::Community::Account::Protocol::I_DeleteAccountRequest, ProxyAccount*>
        request(pDestinationEndpoint, pProxyAccount);

    // Populate the request
    request->setSessionId(_session.getSessionId());
    request->setAccountId(_account.getAccountId());

    // Send the request
    m_service.send<Zen::Community::Account::Protocol::I_DeleteAccountRequest, ProxyAccount*>
        (request, boost::bind(&AccountService::handleDeleteAccountResponse, &m_service, _1, _2, _3));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModelProxy::getAccounts(Common::I_Session& _session, Common::I_AccountController::I_AccountVisitor& _visitor)
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
AccountModelProxy::unSubscribe(Subscription* _pSubscription)
{
    pEndpoint_type pDestinationEndpoint = _pSubscription->getEndpoint();

    // Create a subscription request using pFutureSubscription as the payload.
    Zen::Enterprise::AppServer::create_request<Zen::Community::Account::Protocol::I_UnsubscribeAccountModelRequest, pAccountView_type>
        request(pDestinationEndpoint, _pSubscription->getView());
    
    boost::uint64_t proxyId;
    AccountViewIdx_type::iterator idxIter = m_viewIdx.find(_pSubscription->getView());
    if( idxIter != m_viewIdx.end() )
    {
        proxyId = idxIter->second;

        AccountViews_type::iterator viewIter = m_views.find(proxyId);
        if( viewIter != m_views.end() )
        {
            m_views.erase(viewIter);
        }

        m_viewIdx.erase(idxIter);

        request.m_pRawRequest->setProxyId(proxyId);

        m_service.send<Zen::Community::Account::Protocol::I_UnsubscribeAccountModelRequest, pAccountView_type>
            (request, boost::bind(&AccountService::handleUnsubscribeAccountModelResponse, &m_service, _1, _2, _3));
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModelProxy::handleSubscription(ProxySubscription* _pProxySubscription)
{
    Threading::CriticalSection lock(m_pViewsGuard);

    m_views[_pProxySubscription->m_proxyId] = _pProxySubscription->m_pView;

    pSubscription_type pSubscription(
        new Subscription(
            _pProxySubscription->m_pEndpoint,
            this,
            _pProxySubscription->m_pView
        ),
        &destroySubscription
    );

    _pProxySubscription->m_pFutureSubscription->setValue(pSubscription);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxySubscription;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModelProxy::handleUnsubscribe(pSubscription_type _pSubscription)
{
    Threading::CriticalSection lock(m_pViewsGuard);

    /// TODO Need to get a subscription's parent view here.
    //m_views.erase(_pSubscription->getParentView());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModelProxy::handleCreateAccount(ProxyAccount* _pProxyAccount)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    Accounts_type::iterator iter = m_accounts.find(_pProxyAccount->m_pAccount->getUsername());
    if( iter == m_accounts.end() )
    {
        m_accounts[_pProxyAccount->m_pAccount->getUsername()] = _pProxyAccount->m_pAccount;
    }
    else
    {
        // TODO Error?
    }

    _pProxyAccount->m_pFutureAccount->setValue(_pProxyAccount->m_pAccount);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyAccount;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModelProxy::handleReadAccount(ProxyAccount* _pProxyAccount)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    Accounts_type::iterator iter = m_accounts.find(_pProxyAccount->m_pAccount->getUsername());
    if( iter != m_accounts.end() )
    {
        iter->second = _pProxyAccount->m_pAccount;
    }
    else
    {
        // TODO Error?
    }

    _pProxyAccount->m_pFutureAccount->setValue(_pProxyAccount->m_pAccount);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyAccount;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModelProxy::handleUpdateAccount(ProxyAccount* _pProxyAccount)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    Accounts_type::iterator iter = m_accounts.find(_pProxyAccount->m_pAccount->getUsername());
    if( iter != m_accounts.end() )
    {
        iter->second = _pProxyAccount->m_pAccount;
    }
    else
    {
        // TODO Error?
    }

    _pProxyAccount->m_pFutureAccount->setValue(_pProxyAccount->m_pAccount);

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyAccount;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountModelProxy::handleDeleteAccount(ProxyAccount* _pProxyAccount)
{
    Threading::CriticalSection guard(m_pAccountsGuard);

    Accounts_type::iterator iter = m_accounts.find(_pProxyAccount->m_pAccount->getUsername());
    if( iter != m_accounts.end() )
    {
        m_accounts.erase(iter);
    }
    else
    {
        // TODO Error?
    }

    _pProxyAccount->m_pFutureAccount->setValue(pAccount_type());

    /// TODO should I use a boost::shared_ptr<> or managed_ptr<> 
    /// for ProxySubscription here?
    delete _pProxyAccount;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountModelProxy::handleUpdateNotification(pMessage_type _pMessage)
{
    Zen::Community::Account::Protocol::I_AccountModelUpdateMessage* pMessage =
        dynamic_cast<Zen::Community::Account::Protocol::I_AccountModelUpdateMessage*>(_pMessage.get());

    switch(pMessage->getUpdateKind())
    {

    case Zen::Community::Account::Protocol::I_AccountModelUpdateMessage::ADD:
        // Add account to account collection if it doesn't already
        // exist and notify view specified by proxyId.
        {
            pAccount_type pAccount;
            Accounts_type::iterator iter = m_accounts.find(pMessage->getUsername());
            if( iter == m_accounts.end() )
            {
                Account* pRawAccount = new Account(m_service);
                
                pRawAccount->setAccountId(pMessage->getAccountId());
                pRawAccount->setUsername(pMessage->getUsername());
                pRawAccount->setPassword(pMessage->getPassword());

                pAccount = pAccount_type(
                    pRawAccount, &AccountModelProxy::destroyAccount
                );

                m_accounts[pMessage->getUsername()] = pAccount;
            }
            else
            {
                pAccount = iter->second;
                Account* pRawAccount = dynamic_cast<Account*>(pAccount.get());

                pRawAccount->setAccountId(pMessage->getAccountId());
                pRawAccount->setUsername(pMessage->getUsername());
                pRawAccount->setPassword(pMessage->getPassword());
            }

            AccountViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onAddAccount(*pAccount);
            }
        }
        break;

    case Zen::Community::Account::Protocol::I_AccountModelUpdateMessage::UPDATE:
        // Update account in account collection and
        // notify view specified by proxyId.
        {
            pAccount_type pAccount;
            Accounts_type::iterator iter = m_accounts.find(pMessage->getUsername());
            if( iter != m_accounts.end() )
            {
                pAccount = iter->second;
                Account* pRawAccount = dynamic_cast<Account*>(pAccount.get());

                pRawAccount->setAccountId(pMessage->getAccountId());
                pRawAccount->setUsername(pMessage->getUsername());
                pRawAccount->setPassword(pMessage->getPassword());
            }
            else
            {
                throw Zen::Utility::runtime_exception("AccountModelProxy::handleUpdateNotification() : Account does not exist -- UPDATE Operation.");
            }

            AccountViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onUpdateAccount(*pAccount);
            }
        }
        break;

    case Zen::Community::Account::Protocol::I_AccountModelUpdateMessage::REMOVE:
        // Remove account from account collection and
        // notify view specified by proxyId.
        {
            pAccount_type pAccount;
            Accounts_type::iterator iter = m_accounts.find(pMessage->getUsername());
            if( iter != m_accounts.end() )
            {
                m_accounts.erase(iter);
            }
            else
            {
                //throw Zen::Utility::runtime_exception("AccountModelProxy::handleUpdateNotification() : Account does not exist -- DELETE Operation.");
            }

            AccountViews_type::iterator viewIter = m_views.find(pMessage->getProxyId());
            if( viewIter != m_views.end() )
            {
                viewIter->second->onRemoveAccount(*pAccount);
            }
        }
        break;

    default:
        break;

    };
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
