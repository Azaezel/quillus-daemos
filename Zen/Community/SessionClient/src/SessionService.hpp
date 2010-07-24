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
#ifndef ZEN_COMMUNITY_SESSIONCLIENT_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONCLIENT_SESSION_SERVICE_HPP_INCLUDED

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include <Zen/Community/SessionCommon/I_SessionService.hpp>

#include <Zen/Community/SessionProtocol/I_LoginRequest.hpp>
#include <Zen/Community/SessionProtocol/I_LoginResponse.hpp>

#include <boost/cstdint.hpp>

#include <set>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_Attribute;
        class I_Session;
    }   // namespace Common
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Session;

class SessionService
:   public Zen::Enterprise::AppServer::scriptable_generic_service
        <Zen::Community::Common::I_SessionService, SessionService>
{
    /// @name Types
    /// @{
public:
    /// For now pSession_type is a raw pointer.
    /// The session is never destroyed until the system is shutdown.
    typedef Common::I_Session*                          pSession_type;
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

    /// @name I_ScriptableType
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

    /// @name I_SessionService implementation
    /// @{
public:
    virtual void requestLogin(pEndpoint_type _pDestinationEndpoint, 
                              const std::string& _name, 
                              const std::string& _password);
private:
    virtual Common::I_Session& getSession(boost::uint64_t _sessionId);
public:
    virtual Event::I_Event& getSessionEvent();
    /// @}

    /// @name SessionService implementation
    /// @{
public:
    pScriptModule_type getScriptModule();

    /// Script interface for login.
    void scriptLogin(const std::string& _server, const std::string& _port, const std::string& _name, const std::string& _password);

    typedef Zen::Community::Session::Protocol::I_LoginResponse::pResponse_type pLoginResponse_type;
    void handleLoginResponse(pResponse_type _pResponse, Zen::Community::Session::Protocol::I_LoginRequest& _request, Session* _pSession);

    void handleOnDisconnected(boost::any _anyEndpoint);
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

    Sessions_type                                       m_sessions;

    typedef std::map<boost::uint32_t, pSession_type>    SessionIdIndex_type;
    SessionIdIndex_type                                 m_sessionIdIndex;

    typedef std::map<pEndpoint_type, pSession_type>     EndpointIndex_type;
    EndpointIndex_type                                  m_endpointIndex;

    typedef std::map<std::string, pEndpoint_type>       AddressIndex_type;
    AddressIndex_type                                   m_addressIndex;

    /// @}

};  // class SessionService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONCLIENT_SESSION_SERVICE_HPP_INCLUDED
