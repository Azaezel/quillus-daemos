//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Unit Tests
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

#include "Suite.hpp"
#include "SessionClientTests.hpp"

#include <Zen/Core/Threading/I_Thread.hpp>

#include <Zen/Core/Event/I_EventService.hpp>
#include <Zen/Core/Event/I_EventQueue.hpp>
#include <Zen/Core/Event/I_Action.hpp>
#include <Zen/Core/Event/I_ActionMap.hpp>
#include <Zen/Core/Event/I_Connection.hpp>

#include <Zen/Enterprise/AppServer/I_Container.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Session/I_SessionService.hpp>

#include <Zen/EnterprisePlugins/Session/SessionClient/I_SessionService.hpp>
#include <Zen/EnterprisePlugins/Session/SessionClient/I_Session.hpp>
#include <Zen/EnterprisePlugins/Session/SessionServer/I_SessionService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace UnitTests {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Register the suite
CPPUNIT_TEST_SUITE_REGISTRATION(SessionClientTests);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer* SessionClientTests::sm_pAppServer = NULL;
bool SessionClientTests::sm_initialized = false;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SessionClientTests::setUp()
{
    if (!sm_initialized)
    {
        int argc = 7;
        const char* argv[] =
        {
            "SessionClientTests",
            "-c",
            "Enterprise/Session/SessionClientTests.xml",
            "-l",
            "lua",
            "-s",
            "Enterprise/Session/SessionClientTests.lua"
        };

        Zen::Enterprise::AppServer::I_Container& container =
            Zen::Enterprise::AppServer::I_Container::getSingleton();

        container.init(argc,argv);

        sm_pAppServer = &container.getApplicationServer();

        sm_initialized = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SessionClientTests::tearDown()
{
    Zen::Enterprise::AppServer::I_Container::getSingleton().stop();
    sm_pAppServer = NULL;
    sm_initialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionClientTests::testClientLoad()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/sessionClient")
    );

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionClient was not loaded.",
        CPPUNIT_ASSERT(pService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::I_SessionService>    pBaseSessionService_type;
    pBaseSessionService_type pBaseSessionService = pService.as<pBaseSessionService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionClient could not be cast to Zen::Enterprise::Session::I_SessionService.",
        CPPUNIT_ASSERT(pBaseSessionService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Client::I_SessionService>    pClientSessionService_type;
    pClientSessionService_type pClientSessionService = pService.as<pClientSessionService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionClient could not be case to Zen::Enterprise::Session::Client::I_SessionService.",
        CPPUNIT_ASSERT(pClientSessionService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Server::I_SessionService>    pServerSessionService_type;
    pServerSessionService_type pServerSessionService = pService.as<pServerSessionService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionClient cast to Zen::Enterprise::Session::Server::I_SessionService, but should not have.",
        CPPUNIT_ASSERT(!pServerSessionService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionClientTests::testServiceLoad()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/sessionService")
    );

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionService was not loaded.",
        CPPUNIT_ASSERT(pService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::I_SessionService>    pBaseSessionService_type;
    pBaseSessionService_type pBaseSessionService = pService.as<pBaseSessionService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionService could not be cast to Zen::Enterprise::Session::I_SessionService.",
        CPPUNIT_ASSERT(pBaseSessionService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Client::I_SessionService>    pClientSessionService_type;
    pClientSessionService_type pClientSessionService = pService.as<pClientSessionService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionService could not be cast to Zen::Enterprise::Session::Client::I_SessionService.",
        CPPUNIT_ASSERT(pClientSessionService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Server::I_SessionService>   pServerSessionService_type;
    pServerSessionService_type pServerSessionService = pService.as<pServerSessionService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/sessionService could not be cast to Zen::Enterprise::Session::Server::I_SessionService.",
        CPPUNIT_ASSERT(pServerSessionService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void handleLoginSuccessEvent(boost::any& _payload)
{
    typedef Zen::Enterprise::Session::I_Session*    pSession_type;
    pSession_type pSession = boost::any_cast<pSession_type>(_payload);

    /// TODO Check the session payload to determine state of the login session.
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Login failed.",
        CPPUNIT_ASSERT(pSession->getSessionState() == Zen::Enterprise::Session::I_Session::CONNECTED)
    );
}

void
SessionClientTests::testLoginSuccess()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/sessionClient")
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Client::I_SessionService>    pClientSessionService_type;
    pClientSessionService_type pClientSessionService = pService.as<pClientSessionService_type>();

    Zen::Event::I_Event::pAction_type pAction = pClientSessionService
        ->getApplicationServer()
            .getEventService()
                ->getActionMap("SessionClientTests")
                    .createAction("loginHandler", handleLoginSuccessEvent)
                        .getSelfReference()
                            .lock();

    pClientSessionService->getSessionEvent().connect(pAction);

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;
    pEndpoint_type pEndpoint = sm_pAppServer->getProtocol("sessionClient")
        ->resolveEndpoint("localhost","10000");

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>    pResourceLocation_type;
    pResourceLocation_type pLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
        .createLocation("/sessionService");
    
    pClientSessionService->requestLogin(pEndpoint,pLocation, "testUser", "testPassword");

    Zen::Event::I_EventQueue& eventQueue(sm_pAppServer->getEventService()->getEventQueue("default"));

    eventQueue.dispatchAllEvents(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void handleLoginFailEvent(boost::any& _payload)
{
    typedef Zen::Enterprise::Session::I_Session*    pSession_type;
    pSession_type pSession = boost::any_cast<pSession_type>(_payload);

    /// TODO Check the session payload to determine state of the login session.
    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Login succeeded.",
        CPPUNIT_ASSERT(pSession->getSessionState() == Zen::Enterprise::Session::I_Session::NOT_AUTHORIZED)
    );
}

void
SessionClientTests::testLoginFail()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/sessionClient")
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Session::Client::I_SessionService>    pClientSessionService_type;
    pClientSessionService_type pClientSessionService = pService.as<pClientSessionService_type>();

    Zen::Event::I_Event::pAction_type pAction = pClientSessionService
        ->getApplicationServer()
            .getEventService()
                ->getActionMap("SessionClientTests")
                    .createAction("loginHandler", handleLoginFailEvent)
                        .getSelfReference()
                            .lock();

    pClientSessionService->getSessionEvent().connect(pAction);

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;
    pEndpoint_type pEndpoint = sm_pAppServer->getProtocol("sessionClient")
        ->resolveEndpoint("localhost","10000");

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>    pResourceLocation_type;
    pResourceLocation_type pLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
        .createLocation("/sessionService");
    
    pClientSessionService->requestLogin(pEndpoint,pLocation, "testUser", "wrongPassword");

    Zen::Event::I_EventQueue& eventQueue(sm_pAppServer->getEventService()->getEventQueue("default"));

    eventQueue.dispatchAllEvents(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void handleLoginTimeoutEvent(boost::any& _payload)
{
    /// TODO Implement unit test for a timed out login operation.
}

void
SessionClientTests::testLoginTimeout()
{
    /// TODO Implement unit test for a timed out login operation.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionClientTests::testLogout()
{
    /// TODO Implement unit test for a logout operation.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UnitTests
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
