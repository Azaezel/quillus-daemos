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
#ifndef ZEN_COMMUNITY_SESSIONSERVER_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONSERVER_SESSION_SERVICE_HPP_INCLUDED

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include <Zen/Community/SessionCommon/I_SessionService.hpp>

#include <Zen/Enterprise/Database/I_DatabaseManager.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>

#include <Zen/Community/SessionModel/I_AccountDataMap.hpp>
#include <Zen/Community/SessionModel/I_AccountDomainObject.hpp>
#include <Zen/Community/SessionModel/I_AccountDataCollection.hpp>

#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <boost/cstdint.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Community {
    namespace Common {
        class I_Attribute;
        class I_AccountView;
    }   // namespace Common
    namespace Session {
        class I_AccountDomainObject;
    }   // namespace Session
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Session;

class SessionService
:   public Zen::Enterprise::AppServer::scriptable_generic_service
        <Zen::Community::Common::I_SessionService, SessionService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Community::Session::I_AccountDomainObject>    pAccount_type;
    typedef std::map<std::string, pAccount_type>                                        UsersToAccount_type;

    typedef Memory::managed_ptr<Common::I_AccountView>                                  pAccountView_type;
    typedef Memory::managed_weak_ptr<Common::I_AccountView>                             wpAccountView_type;

    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;
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
    virtual Common::I_Session& getSession(boost::uint64_t _sessionId);
    virtual Event::I_Event& getSessionEvent();
    /// @}

    /// @name SessionService implementation
    /// @{
private:
    pScriptModule_type getScriptModule();

    void loadAccounts();

    void handleLoginRequest(pRequest_type _pRequest, pResponseHandler_type);

    boost::uint32_t generateSessionId();

    void subscribeView();
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyAccountView(wpAccountView_type _pAccountView);
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
    const Zen::Plugins::I_ConfigurationElement*         m_pDatabaseConfig;
    const Zen::Plugins::I_ConfigurationElement*         m_pAccountConfig;

    pAccountView_type                                   m_pAccountView;

    UsersToAccount_type                                 m_usersMap;
    
    typedef std::map<boost::uint64_t, Session*>         SessionIdSession_type;
    SessionIdSession_type                               m_sessions;

    Session*                                            m_pRootSession;

    boost::uint32_t                                     m_lastSessionId;
    Zen::Threading::I_Mutex*                            m_pSesssionIdMutex;

    bool                                                m_viewSubscribed;

    pSubscription_type                                  m_pSubscription;
    /// @}

};  // class SessionService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONSERVER_SESSION_SERVICE_HPP_INCLUDED
