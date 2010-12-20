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
#ifndef ZEN_ENTERPRISE_ACCOUNT_SERVICE_ACCOUNT_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_SERVICE_ACCOUNT_SERVICE_HPP_INCLUDED

#include "Account.hpp"

#include <Zen/EnterprisePlugins/Account/AccountServer/I_AccountService.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include "../Model/I_AccountDataMap.hpp"
#include "../Model/I_AccountDomainObject.hpp"
#include "../Model/I_AccountDataCollection.hpp"

#include <boost/cstdint.hpp>

#include <set>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Service {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AccountService
:	public Zen::Enterprise::AppServer::scriptable_generic_service
    <Server::I_AccountService, AccountService>
{
	/// @name Types
	/// @{
public:
	/// @}

	/// @name I_StartupShutdownParticipant implementation
	/// @{
public:
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual Zen::Threading::I_Condition* prepareToStart(Zen::Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Zen::Threading::I_Condition* prepareToStop();
    virtual void stop();
	/// @}

	/// @name I_ScriptableType implementation
	/// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Scripting::I_ObjectReference* getScriptObject();
	/// @}

	/// @name I_ScriptableService implementation
	/// @{
public:
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
	/// @}

	/// @name I_AccountService implemenation
	/// @{
public:
    virtual pFutureAccount_type createAccount(const std::string& _userId, const std::string& _password);
    virtual void subscribe(Enterprise::Session::I_Session& _session, pEndpoint_type _pEndpoint, pResourceLocation_type _pLocation);
    virtual pFutureAccount_type updateAccount(pAccount_type _pAccount);
    virtual void deleteAccount(pAccount_type _pAccount);
    virtual boost::uint64_t authenticate(pEndpoint_type _pDestinationEndpoint, 
                                         pResourceLocation_type _pDestLocation,
                                         const std::string& _name, 
                                         const std::string& _password);
    virtual Event::I_Event& getAuthenticationEvent();
	/// @}

	/// @name AccountService implemenation
	/// @{
public:
    pScriptModule_type getScriptModule();

    void loadAccounts();

    void handleAuthenticateRequest(pRequest_type _pRequest, pResponseHandler_type);

    boost::uint64_t generateNativeAccountId();
	/// @}

	/// @name 'Structors
	/// @{
protected:
	friend class AccountServiceFactory;
			 AccountService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
	virtual ~AccountService();
	/// @}

	/// @name Member Variables
	/// @{
private:
    pScriptEngine_type                                  m_pScriptEngine;
    Zen::Scripting::script_module*                      m_pScriptModule;
    Scripting::I_ObjectReference*                       m_pScriptObject;

    const Zen::Plugins::I_ConfigurationElement*         m_pDatabaseConfig;

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Model::I_AccountDomainObject>    pAccountObject_type;
    typedef std::map<boost::uint64_t, pAccountObject_type>  Accounts_type;
    Accounts_type                                       m_accountsMap;

    typedef std::map<std::string, boost::uint64_t>  UsersToAccountIds_type;
    UsersToAccountIds_type                              m_usersToAccountIds;

    boost::uint64_t                                     m_lastAccountId;
    Zen::Threading::I_Mutex*                            m_pAccountIdMutex;

    Zen::Threading::I_Condition*                        m_pAccountsLoadedCondition;

    /// TODO Use an AccountClient service here for authentication.
    //typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Client::I_AccountService>    pClientAccountService_type;
    //pClientAccountService_type                          m_pClientAccountService;
	/// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Service
}	// namespace Account
}	// namespace Enterprise
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_SERVICE_ACCOUNT_SERVICE_HPP_INCLUDED
