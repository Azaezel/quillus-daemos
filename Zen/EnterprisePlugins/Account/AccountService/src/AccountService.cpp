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

#include "AccountService.hpp"
#include "Account.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Core/Threading/I_Condition.hpp>
#include <Zen/Core/Threading/ConditionFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/EnterprisePlugins/Account/AccountServer/Protocol/I_AccountProtocolManager.hpp>
#include <Zen/EnterprisePlugins/Account/AccountClient/Protocol/I_AuthenticateRequest.hpp>
#include <Zen/EnterprisePlugins/Account/AccountClient/Protocol/I_AuthenticateResponse.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Service {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::AccountService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service<Zen::Enterprise::Account::Server::I_AccountService, AccountService>(_appServer)
,   m_pScriptObject(NULL)
,   m_pAccountIdMutex(Threading::MutexFactory::create())
,   m_pAccountsLoadedCondition(Zen::Threading::ConditionFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::~AccountService()
{
    Threading::ConditionFactory::destroy(m_pAccountsLoadedCondition);
    Threading::MutexFactory::destroy(m_pAccountIdMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
AccountService::getScriptTypeName()
{
    static std::string sm_name("AccountService");
    return sm_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
AccountService::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(
            getScriptModule(),
            getScriptModule()->getScriptType(getScriptTypeName()),
            this
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::pScriptModule_type
AccountService::getScriptModule()
{
    return m_pScriptModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    m_pScriptEngine = _pScriptEngine;

    // TODO change this so the Community module can be shared
    static Zen::Scripting::script_module module(_pScriptEngine, "Enterprise");

    m_pScriptModule = &module;

    module.addType<AccountService>(getScriptTypeName(), "Account Service")
        //.addMethod("login", &AccountService::scriptLogin)
        //.addMethod("getAccountEvent", &AccountService::getAccountEvent)
        .createGlobalObject("accountService", this)
    ;

    module.activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);

    m_pDatabaseConfig = _config.getChild("database");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
AccountService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Enterprise::Account::Protocol::Client::I_AccountProtocolManager::getSingleton().install(getApplicationServer());
    Zen::Enterprise::Account::Protocol::Server::I_AccountProtocolManager::getSingleton().install(getApplicationServer());

    registerRequestHandler(Zen::Enterprise::Account::Protocol::I_AuthenticateRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleAuthenticateRequest, this, _1, _2));

    loadAccounts();

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::start()
{
    super::start();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
AccountService::prepareToStop()
{
    return super::prepareToStop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::stop()
{
    super::stop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::handleAuthenticateRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    m_pAccountsLoadedCondition->requireCondition();

    Zen::Enterprise::Account::Protocol::I_AuthenticateRequest* pRequest =
        dynamic_cast<Zen::Enterprise::Account::Protocol::I_AuthenticateRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // TODO Should we allow duplicate logins?

        // Create response
        Zen::Memory::managed_ptr<Zen::Enterprise::Account::Protocol::I_AuthenticateResponse> pResponse
            (
                Zen::Enterprise::Account::Protocol::I_AuthenticateResponse::create(
                    pRequest->getDestinationEndpoint(),
                    pRequest->getSourceEndpoint(),
                    getServiceLocation(),
                    pRequest->getSourceLocation(),
                    pRequest->getMessageId()
                )
            );

        Zen::Threading::CriticalSection guard(m_pAccountIdMutex);

        Account account;
        UsersToAccountIds_type::const_iterator iter = m_usersToAccountIds.find(pRequest->getUserId());
        if (iter != m_usersToAccountIds.end())
        {
            boost::uint64_t rawAccountId = iter->second;

            Accounts_type::const_iterator accountIter = m_accountsMap.find(rawAccountId);
            if (accountIter != m_accountsMap.end())
            {
                if (pRequest->getPassword() == accountIter->second->getPassword().getStringValue())
                {
                    pResponse->setAuthenticated(true);
                    account = Account(accountIter->second);
                }
            }
        }

        pResponse->setAccount(account);
        
        // Handle the response.
        _pResponseHandler->handleResponse(
            pResponse.as<Zen::Enterprise::Account::Protocol::I_AuthenticateResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::pFutureAccount_type 
AccountService::createAccount(const std::string& _userId, const std::string& _password)
{
    throw Zen::Utility::runtime_exception("AccountService::createAccount() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::subscribe(Enterprise::Session::I_Session& _session, pEndpoint_type _pEndpoint, pResourceLocation_type _pLocation)
{
    throw Zen::Utility::runtime_exception("AccountService::subscribe() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::pFutureAccount_type 
AccountService::updateAccount(pAccount_type _pAccount)
{
    throw Zen::Utility::runtime_exception("AccountService::updateAccount() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::deleteAccount(pAccount_type _pAccount)
{
    throw Zen::Utility::runtime_exception("AccountService::deleteAccount() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
AccountService::authenticate(pEndpoint_type _pDestinationEndpoint,
                             pResourceLocation_type _pDestLocation,
                             const std::string& _name,
                             const std::string& _password)
{
    throw Zen::Utility::runtime_exception("AccountService::requestLogin(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
AccountService::getAuthenticationEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("AccountService::Service::AuthenticationEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::loadAccounts()
{
    using namespace Zen::Enterprise::Account::Model;

    m_accountsMap.clear();
    m_usersToAccountIds.clear();

    I_AccountDataMap::pFutureDataCollection_type accountDC =
        I_AccountDataMap::create(
            getApplicationServer().getDatabaseConnection(
                m_pDatabaseConfig->getAttribute("connection")
            )
        )->getAll();

    class AccountVisitor
    :   public I_AccountDataCollection::I_CollectionVisitor
    {
    public:
        virtual void begin()
        {
        }

        virtual void visit(I_AccountDataCollection::pDomainObject_type _pDomainObject)
        {
            m_accountsMap[_pDomainObject->getAccountId().getInt64Value()] =
                _pDomainObject;
            m_usersToAccountIds[_pDomainObject->getUserId().getStringValue()] =
                _pDomainObject->getAccountId().getInt64Value();
        }

        virtual void end()
        {
        }

        AccountVisitor(Accounts_type& _accountsMap, UsersToAccountIds_type& _usersToAccountIds)
        :   m_accountsMap(_accountsMap)
        ,   m_usersToAccountIds(_usersToAccountIds)
        {
        }

    private:
        Accounts_type&          m_accountsMap;
        UsersToAccountIds_type& m_usersToAccountIds;
    };

    // Get the accounts.  There should be only one (or none)
    AccountVisitor accountVisitor(m_accountsMap, m_usersToAccountIds);
    accountDC->getValue()->getAll(accountVisitor);

    m_pAccountsLoadedCondition->assertCondition();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
AccountService::generateNativeAccountId()
{
    static Zen::Threading::SpinLock sm_spinLock;

    Zen::Threading::xCriticalSection lock(sm_spinLock);
    return m_lastAccountId = ++m_lastAccountId == 0 ? 1 : m_lastAccountId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Service
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
