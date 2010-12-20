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

#include "SessionService.hpp"
#include "Session.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>
#include <Zen/Core/Event/I_Action.hpp>
#include <Zen/Core/Event/I_ActionMap.hpp>
#include <Zen/Core/Event/I_Connection.hpp>

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

#include <Zen/EnterprisePlugins/Session/SessionServer/Protocol/I_SessionProtocolManager.hpp>
#include <Zen/EnterprisePlugins/Session/SessionClient/Protocol/I_LoginRequest.hpp>
#include <Zen/EnterprisePlugins/Session/SessionClient/Protocol/I_LoginResponse.hpp>
#include <Zen/EnterprisePlugins/Session/SessionClient/Protocol/I_LogoutRequest.hpp>
#include <Zen/EnterprisePlugins/Session/SessionClient/Protocol/I_LogoutResponse.hpp>

#include <boost/any.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Service {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service<Zen::Enterprise::Session::Server::I_SessionService, SessionService>(_appServer)
,   m_pScriptObject(NULL)
,   m_pSesssionIdMutex(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::~SessionService()
{
    Threading::MutexFactory::destroy(m_pSesssionIdMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SessionService::getScriptTypeName()
{
    static std::string sm_name("SessionService");
    return sm_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
SessionService::getScriptObject()
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
SessionService::pScriptModule_type
SessionService::getScriptModule()
{
    return m_pScriptModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    m_pScriptEngine = _pScriptEngine;

    // TODO change this so the Community module can be shared
    static Zen::Scripting::script_module module(_pScriptEngine, "Enterprise");

    m_pScriptModule = &module;

    module.addType<SessionService>(getScriptTypeName(), "Session Service")
        //.addMethod("login", &SessionService::scriptLogin)
        //.addMethod("getSessionEvent", &SessionService::getSessionEvent)
        .createGlobalObject("sessionService", this)
    ;

    module.activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);

    m_pDatabaseConfig = _config.getChild("database");
    m_pAccountClientConfig = _config.getChild("account-client");
    m_pAccountServiceConfig = _config.getChild("account-service");
    m_pAccountProtocolConfig = _config.getChild("account-protocol");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Enterprise::Session::Protocol::Client::I_SessionProtocolManager::getSingleton().install(getApplicationServer());
    Zen::Enterprise::Session::Protocol::Server::I_SessionProtocolManager::getSingleton().install(getApplicationServer());

    registerRequestHandler(Zen::Enterprise::Session::Protocol::I_LoginRequest::getStaticMessageType(),
        boost::bind(&SessionService::handleLoginRequest, this, _1, _2));

    registerRequestHandler(Zen::Enterprise::Session::Protocol::I_LogoutRequest::getStaticMessageType(),
        boost::bind(&SessionService::handleLogoutRequest, this, _1, _2));

    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = getApplicationServer().getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation(
            m_pAccountClientConfig->getAttribute("location")
        )
    );

    m_pClientAccountService = pService.as<pClientAccountService_type>();

    Zen::Event::I_Event::pAction_type pAction = getApplicationServer()
        .getEventService()
            ->getActionMap("SessionService")
                .createAction("authenticationHandler", boost::bind(&SessionService::handleAuthenticationEvent, this, _1))
                    .getSelfReference().lock();

    m_pClientAccountService->getAuthenticationEvent().connect(pAction);

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::start()
{
    super::start();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStop()
{
    return super::prepareToStop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::stop()
{
    super::stop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleLoginRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Zen::Enterprise::Session::Protocol::I_LoginRequest* pRequest =
        dynamic_cast<Zen::Enterprise::Session::Protocol::I_LoginRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // TODO Should we allow duplicate logins?

        // Create response
        Zen::Memory::managed_ptr<Zen::Enterprise::Session::Protocol::I_LoginResponse> pResponse
            (
                Zen::Enterprise::Session::Protocol::I_LoginResponse::create(
                    pRequest->getDestinationEndpoint(),
                    pRequest->getSourceEndpoint(),
                    getServiceLocation(),
                    pRequest->getSourceLocation(),
                    pRequest->getMessageId()
                )
            );

        pEndpoint_type pAccountServiceEndpoint = 
            getApplicationServer().getProtocol(m_pAccountProtocolConfig->getAttribute("name"))
                ->resolveEndpoint(
                    m_pAccountServiceConfig->getAttribute("address"),
                    m_pAccountServiceConfig->getAttribute("port")
                );

        pResourceLocation_type pAccountServiceLocation =
            Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
                .createLocation(
                    m_pAccountServiceConfig->getAttribute("location")
                );

        authenticateSession(
            pAccountServiceEndpoint,
            pAccountServiceLocation,
            pRequest->getUserId(),
            pRequest->getPassword(),
            pResponse,
            _pResponseHandler
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleLogoutRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Zen::Enterprise::Session::Protocol::I_LogoutRequest* pRequest =
        dynamic_cast<Zen::Enterprise::Session::Protocol::I_LogoutRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // TODO Should we allow duplicate logins?

        // Create response
        Zen::Memory::managed_ptr<Zen::Enterprise::Session::Protocol::I_LogoutResponse> pResponse
            (
                Zen::Enterprise::Session::Protocol::I_LogoutResponse::create(
                    pRequest->getDestinationEndpoint(),
                    pRequest->getSourceEndpoint(),
                    getServiceLocation(),
                    pRequest->getSourceLocation(),
                    pRequest->getMessageId()
                )
            );

        const Session* pSession = dynamic_cast<const Session*>(&pRequest->getSession());

        Zen::Threading::CriticalSection guard(m_pSesssionIdMutex);
        Session::SessionId sessionId = pSession->getSessionId();
        Session::NativeSessionId* pNative =
            dynamic_cast<Session::NativeSessionId*>(sessionId.m_pNativeSessionId);
        assert(pNative != NULL);

        Zen::Enterprise::Session::Model::I_SessionDataMap::pDataMap_type pSessionDM = 
            Zen::Enterprise::Session::Model::I_SessionDataMap::create(
                getApplicationServer().getDatabaseConnection(
                    m_pDatabaseConfig->getAttribute("connection")
                )
            );

        Sessions_type::iterator iter = m_sessionsMap.find(pNative->getRawSessionId());
        if (iter != m_sessionsMap.end())
        {
            m_sessionsMap.erase(iter);
        }

        pResponse->setSession(*pSession);

        // Handle the response.
        _pResponseHandler->handleResponse(
            pResponse.as<Zen::Enterprise::Session::Protocol::I_LogoutResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::authenticateSession(pEndpoint_type _pEndpoint,
                                    pResourceLocation_type _pLocation,
                                    const std::string& _userId,
                                    const std::string& _password,
                                    pLoginResponse_type _pResponse,
                                    pResponseHandler_type _pResponseHandler)
{
    boost::uint64_t authenticationId = 
        m_pClientAccountService->authenticate(
            _pEndpoint,
            _pLocation,
            _userId,
            _password
        );

    m_loginResponseMap[authenticationId] = LoginResponseHandlerPair_type(_pResponse,_pResponseHandler);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleAuthenticationEvent(boost::any& _payload)
{
    Zen::Enterprise::Account::I_AccountService::AuthenticationPayload payload =
        boost::any_cast<Zen::Enterprise::Account::I_AccountService::AuthenticationPayload>(_payload);

    LoginResponseMap_type::iterator iter = m_loginResponseMap.find(payload.m_id);
    if (iter != m_loginResponseMap.end())
    {
        pLoginResponse_type pResponse = iter->second.first;
        pResponseHandler_type pResponseHandler = iter->second.second;

        if (payload.m_authenticated)
        {
            Session* pSession = new Session(
                generateNativeSessionId(),
                pResponse->getSourceEndpoint(),
                pResponse->getSourceLocation(),
                Zen::Enterprise::Session::I_Session::CONNECTED
            );

            Zen::Threading::CriticalSection guard(m_pSesssionIdMutex);
            Session::SessionId sessionId = pSession->getSessionId();
            Session::NativeSessionId* pNative =
                dynamic_cast<Session::NativeSessionId*>(sessionId.m_pNativeSessionId);
            assert(pNative != NULL);

            Zen::Enterprise::Session::Model::I_SessionDataMap::pDataMap_type pSessionDM = 
                Zen::Enterprise::Session::Model::I_SessionDataMap::create(
                    getApplicationServer().getDatabaseConnection(
                        m_pDatabaseConfig->getAttribute("connection")
                    )
                );

            pSessionObject_type pSessionObject = pSessionDM->createNew();
            pSessionObject->getSessionId() = pNative->getRawSessionId();
            pSessionObject->getAccountId() = payload.m_account.getAccountId().toString();
            pSessionObject->getEndpoint() = pResponse->getDestinationEndpoint()->toString();
            pSessionObject->getLocation() = pResponse->getDestinationLocation()->toString();
            pSessionObject->getStatus() = pSession->getSessionState();
            /// TODO Figure out why the following line is generating boost related linker errors.
            //pSessionObject->getStartTime() = boost::posix_time::to_simple_string(boost::posix_time::ptime(boost::posix_time::second_clock::universal_time()));
            pSessionObject->getEndTime() = std::string();

            pSessionDM->update(pSessionObject);

            m_sessionsMap[pNative->getRawSessionId()] = pSessionObject;

            pResponse->setSession(*pSession);
        }
        else
        {
            Session* pSession = new Session(
                0,
                pResponse->getSourceEndpoint(),
                pResponse->getSourceLocation(),
                Zen::Enterprise::Session::I_Session::NOT_AUTHORIZED
            );
        }

        // Handle the response.
        pResponseHandler->handleResponse(
            pResponse.as<Zen::Enterprise::Session::Protocol::I_LoginResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SessionService::subscribe(Enterprise::Session::I_Session& _session, pEndpoint_type _pEndpoint, pResourceLocation_type _pLocation)
{
    throw Zen::Utility::runtime_exception("SessionService::subscribe() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SessionService::terminateSession(pServerSession_type _pSession)
{
    throw Zen::Utility::runtime_exception("SessionService::subscribe() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleAuthenticationTimeoutEvent(boost::uint64_t _authenticationId)
{
    throw Zen::Utility::runtime_exception("SessionService::handleAuthenticationTimeoutEvent() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::requestLogin(pEndpoint_type _pDestinationEndpoint,
                             pResourceLocation_type _pDestLocation,
                             const std::string& _name,
                             const std::string& _password)
{
    throw Zen::Utility::runtime_exception("SessionService::requestLogin(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::requestLogout(Enterprise::Session::I_Session& _session)
{
    throw Zen::Utility::runtime_exception("SessionService::requestLogout(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
SessionService::getSessionEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("SessionService::Service::SessionEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::loadSessions()
{
    using namespace Zen::Enterprise::Session::Model;

    m_sessionsMap.clear();

    I_SessionDataMap::pFutureDataCollection_type sessionDC =
        I_SessionDataMap::create(
            getApplicationServer().getDatabaseConnection(
                m_pDatabaseConfig->getAttribute("connection")
            )
        )->getAll();

    class SessionVisitor
    :   public I_SessionDataCollection::I_CollectionVisitor
    {
    public:
        virtual void begin()
        {
        }

        virtual void visit(I_SessionDataCollection::pDomainObject_type _pDomainObject)
        {
            m_sessionsMap[_pDomainObject->getSessionId().getInt64Value()] =
                _pDomainObject;
        }

        virtual void end()
        {
        }

        SessionVisitor(Sessions_type& _sessionsMap)
        :   m_sessionsMap(_sessionsMap)
        {
        }

    private:
        Sessions_type&                m_sessionsMap;
    };

    // Get the sessions.  There should be only one (or none)
    SessionVisitor sessionVisitor(m_sessionsMap);
    sessionDC->getValue()->getAll(sessionVisitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
SessionService::generateNativeSessionId()
{
    static Zen::Threading::SpinLock sm_spinLock;

    Zen::Threading::xCriticalSection lock(sm_spinLock);
    return m_lastSessionId = ++m_lastSessionId == 0 ? 1 : m_lastSessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Service
}   // namespace Session
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
