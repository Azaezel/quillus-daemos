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
#include "../Protocol/I_AccountProtocolManager.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_Event.hpp>
#include <Zen/Core/Event/I_Action.hpp>
#include <Zen/Core/Event/I_ActionMap.hpp>
#include <Zen/Core/Event/I_Connection.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::AccountService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service <Zen::Enterprise::Account::Server::I_AccountService, AccountService>(_appServer)
,   m_pScriptObject(NULL)
,   m_pScriptModule(NULL)
,   m_pScriptEngine()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::~AccountService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
AccountService::getScriptTypeName()
{
    static std::string sm_name("AccountServer");
    return sm_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
AccountService::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(getScriptModule(),
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
    m_pScriptModule = new Zen::Scripting::script_module(_pScriptEngine, "Enterprise");

    m_pScriptModule->addType<AccountService>(getScriptTypeName(), "Account Server Service")
        .addMethod("authenticate", &AccountService::scriptAuthenticate)
        .addMethod("getAuthenticationEvent", &AccountService::getAuthenticationEvent)
        .createGlobalObject("accountServer", this)
    ;

    Account::registerScriptModule(*m_pScriptModule);

    m_pScriptModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
AccountService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Enterprise::Account::Protocol::Client::I_AccountProtocolManager::getSingleton().install(getApplicationServer());
    Zen::Enterprise::Account::Protocol::Server::I_AccountProtocolManager::getSingleton().install(getApplicationServer());

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::start()
{
    // Allow the super class to start up.
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
    if (m_pClientAccountService.isValid())
    {
        return m_pClientAccountService->authenticate(
            _pDestinationEndpoint,
            _pDestLocation,
            _name,
            _password
        );
    }

    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
AccountService::getAuthenticationEvent()
{
    return getApplicationServer().getEventService()->createEvent("AccountService::Client::AuthenticationEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::scriptAuthenticate(const std::string& _server, const std::string& _port, const std::string& _location, const std::string& _name, const std::string& _password)
{
    // TODO Don't hard-code this protocol name.
    pEndpoint_type pEndpoint = getApplicationServer().getProtocol("accountClient")
        ->resolveEndpoint(_server, _port);
    
    // TODO make sure this endpoint doesn't already exist in m_endpointIndex
    // TODO Guard for m_endpointIndex
    AddressIndex_type::iterator iter = m_addressIndex.find(pEndpoint->toString());
    if (iter != m_addressIndex.end())
    {
        // This is an address that has already been used, so re-use
        // the original one.
        pEndpoint = iter->second;
    }
    else
    {
        // This is a new address, save the endpoint.
        m_addressIndex[pEndpoint->toString()] = pEndpoint;
    }

    // TODO Save the endpoint and associate it with this server and port?
    
    // For now lets just assume we only have one "scriptAuthenticate" request.

    authenticate(
        pEndpoint, 
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
            .createLocation(_location),
        _name, 
        _password
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
