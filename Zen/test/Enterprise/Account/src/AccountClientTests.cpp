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
#include "AccountClientTests.hpp"

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

#include <Zen/Enterprise/Account/I_AccountService.hpp>

#include <Zen/EnterprisePlugins/Account/AccountClient/I_AccountService.hpp>
#include <Zen/EnterprisePlugins/Account/AccountClient/I_Account.hpp>
#include <Zen/EnterprisePlugins/Account/AccountServer/I_AccountService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace UnitTests {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Register the suite
CPPUNIT_TEST_SUITE_REGISTRATION(AccountClientTests);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer* AccountClientTests::sm_pAppServer = NULL;
bool AccountClientTests::sm_initialized = false;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountClientTests::setUp()
{
    if (!sm_initialized)
    {
        int argc = 7;
        const char* argv[] =
        {
            "AccountClientTests",
            "-c",
            "Enterprise/Account/AccountClientTests.xml",
            "-l",
            "lua",
            "-s",
            "Enterprise/Account/AccountClientTests.lua"
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
AccountClientTests::tearDown()
{
    Zen::Enterprise::AppServer::I_Container::getSingleton().stop();
    sm_pAppServer = NULL;
    sm_initialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountClientTests::testClientLoad()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/accountClient")
    );

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountClient was not loaded.",
        CPPUNIT_ASSERT(pService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::I_AccountService>    pBaseAccountService_type;
    pBaseAccountService_type pBaseAccountService = pService.as<pBaseAccountService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountClient could not be cast to Zen::Enterprise::Account::I_AccountService.",
        CPPUNIT_ASSERT(pBaseAccountService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Client::I_AccountService>    pClientAccountService_type;
    pClientAccountService_type pClientAccountService = pService.as<pClientAccountService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountClient could not be cast to Zen::Enterprise::Account::Client::I_AccountService.",
        CPPUNIT_ASSERT(pClientAccountService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Server::I_AccountService>   pServerAccountService_type;
    pServerAccountService_type pServerAccountService = pService.as<pServerAccountService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountClient cast to Zen::Enterprise::Account::Server::I_AccountService, but should not have.",
        CPPUNIT_ASSERT(!pServerAccountService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountClientTests::testServiceLoad()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/accountService")
    );

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountService was not loaded.",
        CPPUNIT_ASSERT(pService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::I_AccountService>    pBaseAccountService_type;
    pBaseAccountService_type pBaseAccountService = pService.as<pBaseAccountService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountService could not be cast to Zen::Enterprise::Account::I_AccountService.",
        CPPUNIT_ASSERT(pBaseAccountService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Client::I_AccountService>    pClientAccountService_type;
    pClientAccountService_type pClientAccountService = pService.as<pClientAccountService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountService could not be cast to Zen::Enterprise::Account::Client::I_AccountService.",
        CPPUNIT_ASSERT(pClientAccountService.isValid())
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Server::I_AccountService>   pServerAccountService_type;
    pServerAccountService_type pServerAccountService = pService.as<pServerAccountService_type>();

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Service at localhost/accountService could not be cast to Zen::Enterprise::Account::Server::I_AccountService.",
        CPPUNIT_ASSERT(pServerAccountService.isValid())
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void handleAuthenticationPassEvent(boost::any& _payload)
{
    Zen::Enterprise::Account::I_AccountService::AuthenticationPayload payload =
        boost::any_cast<Zen::Enterprise::Account::I_AccountService::AuthenticationPayload>(_payload);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Authentication failed.",
        CPPUNIT_ASSERT(payload.m_authenticated)
    );
}

void
AccountClientTests::testAuthenticatePass()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/accountClient")
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Client::I_AccountService>    pClientAccountService_type;
    pClientAccountService_type pClientAccountService = pService.as<pClientAccountService_type>();

    Zen::Event::I_Event::pAction_type pAction = pClientAccountService
        ->getApplicationServer()
            .getEventService()
                ->getActionMap("AccountClientTests")
                    .createAction("authenticationHandler", handleAuthenticationPassEvent)
                        .getSelfReference()
                            .lock();

    pClientAccountService->getAuthenticationEvent().connect(pAction);

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;
    pEndpoint_type pEndpoint = sm_pAppServer->getProtocol("accountClient")
        ->resolveEndpoint("localhost","10001");

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>    pResourceLocation_type;
    pResourceLocation_type pLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
        .createLocation("/accountService");
    
    pClientAccountService->authenticate(pEndpoint,pLocation, "testUser", "testPassword");

    Zen::Event::I_EventQueue& eventQueue(sm_pAppServer->getEventService()->getEventQueue("default"));

    eventQueue.dispatchAllEvents(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void handleAuthenticationFailEvent(boost::any& _payload)
{
    Zen::Enterprise::Account::I_AccountService::AuthenticationPayload payload =
        boost::any_cast<Zen::Enterprise::Account::I_AccountService::AuthenticationPayload>(_payload);

    CPPUNIT_ASSERT_ASSERTION_PASS_MESSAGE(
        "Authentication succeeded.",
        CPPUNIT_ASSERT(payload.m_authenticated)
    );
}

void
AccountClientTests::testAuthenticateFail()
{
    Zen::Enterprise::AppServer::I_ApplicationServer::pApplicationService_type pService = sm_pAppServer->getApplication(
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/accountClient")
    );

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::Account::Client::I_AccountService>    pClientAccountService_type;
    pClientAccountService_type pClientAccountService = pService.as<pClientAccountService_type>();

    Zen::Event::I_Event::pAction_type pAction = pClientAccountService
        ->getApplicationServer()
            .getEventService()
                ->getActionMap("AccountClientTests")
                    .createAction("authenticationHandler", handleAuthenticationFailEvent)
                        .getSelfReference()
                            .lock();

    pClientAccountService->getAuthenticationEvent().connect(pAction);

    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>   pEndpoint_type;
    pEndpoint_type pEndpoint = sm_pAppServer->getProtocol("accountClient")
        ->resolveEndpoint("localhost","10001");

    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>    pResourceLocation_type;
    pResourceLocation_type pLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
        .createLocation("/accountService");
    
    pClientAccountService->authenticate(pEndpoint,pLocation, "testUser", "wrongPassword");

    Zen::Event::I_EventQueue& eventQueue(sm_pAppServer->getEventService()->getEventQueue("default"));

    eventQueue.dispatchAllEvents(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void handleAuthenticationTimeoutEvent(boost::any& _payload)
{
    /// TODO Implement unit test for a timed out authenticate operation.
}

void
AccountClientTests::testAuthenticateTimeout()
{
    /// TODO Implement unit test for a timed out authenticate operation.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UnitTests
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
