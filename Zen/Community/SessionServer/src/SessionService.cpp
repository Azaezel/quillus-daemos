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
#include "SessionService.hpp"
#include "Session.hpp"
#include "Attribute.hpp"
#include "AccountView.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Community/SessionProtocol/I_SessionProtocolManager.hpp>
#include <Zen/Community/SessionProtocol/I_LoginRequest.hpp>
#include <Zen/Community/SessionProtocol/I_LoginResponse.hpp>

#include <Zen/Community/AccountCommon/I_AccountView.hpp>
#include <Zen/Community/AccountCommon/I_AccountModel.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service<Zen::Community::Common::I_SessionService, SessionService>(_appServer)
,   m_pScriptObject(NULL)
,   m_pSesssionIdMutex(Threading::MutexFactory::create())
,   m_lastSessionId(0)
,   m_pAccountView(new AccountView(), &destroyAccountView)
,   m_pRootSession(new Session(*this,0,Session::CONNECTED,pEndpoint_type()))
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
    static std::string sm_name("SessionServer");
    return sm_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
SessionService::getScriptObject()
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
    static Zen::Scripting::script_module module(_pScriptEngine, "Community");

    m_pScriptModule = &module;

    module.addType<SessionService>(getScriptTypeName(), "Session Server Service")
        //.addMethod("login", &SessionService::scriptLogin)
        //.addMethod("getSessionEvent", &SessionService::getSessionEvent)
        .createGlobalObject("sessionServer", this)
    ;

    module.activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);

    /// TODO Get database name from configuration data? -- mgray 02/22/2010
    m_pDatabaseConfig = _config.getChild("database");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Community::Protocol::I_SessionProtocolManager::getSingleton().install(getApplicationServer());

    registerRequestHandler(Protocol::I_LoginRequest::getStaticMessageType(), 
        boost::bind(&SessionService::handleLoginRequest, this, _1, _2));

    //getApplicationServer().registerDefaultSessionService(getSelfReference().lock());

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::start()
{
    // Instead of accessing the database, take m_pAccountView
    // and subscribe to an I_AccountModel from the Account services.
//    typedef Memory::managed_ptr<Common::I_AccountModel> pAccountModel_type;
//    Common::I_AccountService::pFutureAccountModel_type pFutureModel =
//        getApplicationServer().getDefaultAccountService()
//            ->createAccountModel();
//
//    pFutureModel->getValue()->subscribe(*m_pRootSession, m_pAccountView);

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
    Protocol::I_LoginRequest* pRequest =
        dynamic_cast<Protocol::I_LoginRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // TODO Should we allow duplicate logins?

        // Create response
        Zen::Memory::managed_ptr<Protocol::I_LoginResponse> pResponse
            (
                Protocol::I_LoginResponse::create(
                    pRequest->getDestinationEndpoint(),
                    pRequest->getSourceEndpoint(),
                    pRequest->getMessageId()
                )
            );

        AccountView* pAccountView = dynamic_cast<AccountView*>(m_pAccountView.get());

        bool success = false;
        if( pAccountView != NULL )
        {
            success = pAccountView->authenticate(
                pRequest->getUserId(),
                pRequest->getPassword()
            );
        }

        if( success )
        {
            pResponse->setStatus(Protocol::I_LoginResponse::CONNECTED);

            Session* pSession = new Session(
                *this, 
                generateSessionId(),
                /// TODO This is a hack that only works because the
                /// I_Session::SessionState_type and I_LoginResponse::SessionState_type
                /// enums are identical.  If one changes, the other one needs
                /// to be updated -- either that, or we find the *right* way
                /// to perform the following below -- mgray 02/22/2010
                static_cast<Session::SessionState_type>(pResponse->getStatus()), 
                pResponse->getSourceEndpoint()
            );

            Zen::Threading::CriticalSection guard(m_pSesssionIdMutex);
            m_sessions[pSession->getSessionId()] = pSession;

            pResponse->setSessionId(pSession->getSessionId());
        }
        else
        {
            pResponse->setStatus(Protocol::I_LoginResponse::NOT_AUTHORIZED);
        }
#if 0
        UsersToAccount_type::const_iterator iter = m_usersMap.find(pRequest->getUserId());
        if( iter != m_usersMap.end() )
        {
            if( iter->second->getPassword().getStringValue() == 
                pRequest->getPassword() )
            {
                pResponse->setStatus(Protocol::I_LoginResponse::CONNECTED);

                Session* pSession = new Session(
                    *this, 
                    generateSessionId(),
                    /// TODO This is a hack that only works because the
                    /// I_Session::SessionState_type and I_LoginResponse::SessionState_type
                    /// enums are identical.  If one changes, the other one needs
                    /// to be updated -- either that, or we find the *right* way
                    /// to perform the following below -- mgray 02/22/2010
                    static_cast<Session::SessionState_type>(pResponse->getStatus()), 
                    pResponse->getSourceEndpoint()
                );

                Zen::Threading::CriticalSection guard(m_pSesssionIdMutex);
                m_sessions[pSession->getSessionId()] = pSession;

                pResponse->setSessionId(pSession->getSessionId());
            }
            else
            {
                pResponse->setStatus(Protocol::I_LoginResponse::NOT_AUTHORIZED);
            }
        }
        else
        {
            pResponse->setStatus(Protocol::I_LoginResponse::NOT_AUTHORIZED);
        }
#endif  // 0
        
        // Handle the response.
        _pResponseHandler->handleResponse(
            pResponse.as<Protocol::I_LoginResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::requestLogin(pEndpoint_type _pDestinationEndpoint,
                              const std::string& _name,
                              const std::string& _password)
{
    // TODO Fire an event to which the controller listens and responds
    // by executing a login.
    throw Zen::Utility::runtime_exception("SessionService::requestLogin(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Common::I_Session&
SessionService::getSession(boost::uint64_t _sessionId)
{
    SessionIdSession_type::iterator iter = m_sessions.find(_sessionId);
    if( iter != m_sessions.end() )
    {
        return *iter->second;
    }

    throw Zen::Utility::runtime_exception("SessionService::getSession() : Error, session is not valid.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
SessionService::getSessionEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("SessionService::Server::SessionEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::loadAccounts()
{
    using namespace Zen::Community::Session;

    m_usersMap.clear();

    I_AccountDataMap::pFutureAccountDataCollection_type accountDC =
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

        virtual void visit(I_AccountDataCollection::pAccountDomainObject_type _pDomainObject)
        {
            m_usersMap[_pDomainObject->getUser().getStringValue()] =
                _pDomainObject;
        }

        virtual void end()
        {
        }

        AccountVisitor(UsersToAccount_type& _usersMap)
        :   m_usersMap(_usersMap)
        {
        }

    private:
        UsersToAccount_type&                m_usersMap;
    };


    // Get the accounts.  There should be only one (or none)
    AccountVisitor accountVisitor(m_usersMap);
    accountDC->getValue()->getAll(accountVisitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint32_t
SessionService::generateSessionId()
{
    static Zen::Threading::SpinLock sm_spinLock;

    Zen::Threading::xCriticalSection lock(sm_spinLock);
    return m_lastSessionId = ++m_lastSessionId == 0 ? 1 : m_lastSessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::destroyAccountView(wpAccountView_type _pAccountView)
{
    delete dynamic_cast<AccountView*>(_pAccountView.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
