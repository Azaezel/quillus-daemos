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
#ifndef ZEN_ENTERPRISE_SESSION_SERVER_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_SERVER_SESSION_SERVICE_HPP_INCLUDED

#include "../I_SessionService.hpp"

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include <boost/cstdint.hpp>

#include <set>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SessionService
:	public Zen::Enterprise::AppServer::scriptable_generic_service
		<I_SessionService, SessionService>
{
	/// @name Types
	/// @{
public:
    /// For now pSession_type is a raw pointer.
    /// The session is never destroyed until the system is shutdown
    /// or a logout occurs.
    typedef Enterprise::Session::I_Session*             pSession_type;
    typedef std::set<pSession_type>                     Sessions_type;
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

    /// Script interface for login.
    void scriptLogin(const std::string& _server, const std::string& _port, const std::string& _location, const std::string& _name, const std::string& _password);
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

    typedef std::map<pEndpoint_type, pSession_type>     EndpointIndex_type;
    EndpointIndex_type                                  m_endpointIndex;

    typedef std::map<std::string, pEndpoint_type>       AddressIndex_type;
    AddressIndex_type                                   m_addressIndex;

    typedef Zen::Memory::managed_ptr<Enterprise::Session::Client::I_SessionService> pClientSessionService_type;
    pClientSessionService_type                          m_pClientSessionService;
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Server
}	// namespace Session
}	// namespace Enterprise
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_SERVER_SESSION_SERVICE_HPP_INCLUDED
