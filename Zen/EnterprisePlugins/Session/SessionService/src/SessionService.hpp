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
#ifndef ZEN_ENTERPRISE_SESSION_SERVICE_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_SERVICE_SESSION_SERVICE_HPP_INCLUDED

#include "Session.hpp"

#include <Zen/EnterprisePlugins/Session/SessionServer/I_SessionService.hpp>
#include <Zen/EnterprisePlugins/Session/SessionClient/Protocol/I_LoginResponse.hpp>

#include <Zen/EnterprisePlugins/Account/AccountClient/I_AccountService.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include "../Model/I_SessionDataMap.hpp"
#include "../Model/I_SessionDomainObject.hpp"
#include "../Model/I_SessionDataCollection.hpp"

#include <boost/cstdint.hpp>

#include <set>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Service {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SessionService
:	public Zen::Enterprise::AppServer::scriptable_generic_service
    <Server::I_SessionService, SessionService>
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

	/// @name I_SessionService implemenation
	/// @{
public:
    virtual void subscribe(Enterprise::Session::I_Session& _session, pEndpoint_type _pEndpoint, pResourceLocation_type _pLocation);
    virtual void terminateSession(pServerSession_type _pSession);
    virtual void requestLogin(pEndpoint_type _pDestinationEndpoint, 
                              pResourceLocation_type _pDestLocation,
                              const std::string& _name, 
                              const std::string& _password);
    virtual void requestLogout(Enterprise::Session::I_Session& _session);
    virtual Event::I_Event& getSessionEvent();
	/// @}

	/// @name SessionService implemenation
	/// @{
public:
    pScriptModule_type getScriptModule();

    void loadSessions();

    void handleLoginRequest(pRequest_type _pRequest, pResponseHandler_type);

    void handleLogoutRequest(pRequest_type _pRequest, pResponseHandler_type);

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Protocol::I_LoginResponse> pLoginResponse_type;
    void authenticateSession(pEndpoint_type _pEndpoint, 
                             pResourceLocation_type _pLocation, 
                             const std::string& _userId, 
                             const std::string& _password, 
                             pLoginResponse_type _pResponse,
                             pResponseHandler_type _pResponseHandler);

    void handleAuthenticationEvent(boost::any& _payload);

    void handleAuthenticationTimeoutEvent(boost::uint64_t _authenticationId);

    boost::uint64_t generateNativeSessionId();
	/// @}

	/// @name 'Structors
	/// @{
protected:
	friend class SessionServiceFactory;
			 SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
	virtual ~SessionService();
	/// @}

	/// @name Member Variables
	/// @{
private:
    pScriptEngine_type                                  m_pScriptEngine;
    Zen::Scripting::script_module*                      m_pScriptModule;
    Scripting::I_ObjectReference*                       m_pScriptObject;

    const Zen::Plugins::I_ConfigurationElement*         m_pAccountClientConfig;
    const Zen::Plugins::I_ConfigurationElement*         m_pAccountServiceConfig;
    const Zen::Plugins::I_ConfigurationElement*         m_pAccountProtocolConfig;
    const Zen::Plugins::I_ConfigurationElement*         m_pDatabaseConfig;

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Model::I_SessionDomainObject>    pSessionObject_type;
    typedef std::map<boost::uint64_t, pSessionObject_type>  Sessions_type;
    Sessions_type                                       m_sessionsMap;

    boost::uint64_t                                     m_lastSessionId;
    Zen::Threading::I_Mutex*                            m_pSesssionIdMutex;

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Client::I_AccountService>    pClientAccountService_type;
    pClientAccountService_type                          m_pClientAccountService;

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Protocol::I_LoginResponse>   pLoginResponse_type;
    typedef std::pair<pLoginResponse_type,pResponseHandler_type>                            LoginResponseHandlerPair_type;
    typedef std::map<boost::uint64_t, LoginResponseHandlerPair_type>                        LoginResponseMap_type;
    LoginResponseMap_type                               m_loginResponseMap;
	/// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Service
}	// namespace Session
}	// namespace Enterprise
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_SERVICE_SESSION_SERVICE_HPP_INCLUDED
