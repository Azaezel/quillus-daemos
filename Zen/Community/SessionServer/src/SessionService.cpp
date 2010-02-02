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

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Community/SessionProtocol/I_SessionProtocolManager.hpp>
#include <Zen/Community/SessionProtocol/I_LoginRequest.hpp>
#include <Zen/Community/SessionProtocol/I_LoginResponse.hpp>

#include <Zen/Community/SessionModel/I_AccountDataCollection.hpp>
#include <Zen/Community/SessionModel/I_AccountDomainObject.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::SessionService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   m_appServer(_appServer)
,   m_pThreadPool(NULL)
,   m_pHandlersMutex(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SessionService::~SessionService()
{
    Zen::Threading::MutexFactory::destroy(m_pHandlersMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    // TODO Get configuration information
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    Zen::Community::Protocol::I_SessionProtocolManager::getSingleton().install(m_appServer);

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::start()
{
    // TODO Get the app server database and load the entire Account table
    // into memory.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
SessionService::prepareToStop()
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleMessage(pMessage_type _pMessage)
{
    throw Utility::runtime_exception("SessionService::handleMessage(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_LoginRequest* pRequest =
        dynamic_cast<Protocol::I_LoginRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // TODO Handle this
        throw Zen::Utility::runtime_exception("SessionService::handleRequest(): Error, not implemented");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer&
SessionService::getApplicationServer()
{
    return m_appServer;
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
Event::I_Event&
SessionService::getSessionEvent()
{
    // TODO Return the model session event.
    throw Zen::Utility::runtime_exception("SessionService::getSessionEvent(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SessionService::loadAccounts()
{
    using namespace Zen::Community::Session;

    throw Zen::Utility::runtime_exception("SessionService::loadAccounts(): Error, not implemented");

    // TODO get the database connection name.

    // TODO Implement a MVC and load all of the accounts into
    // the model.
#if 0

    I_AccountDataMap::pFutureAccountDataCollection_type accountDC
        = I_AccountDataMap::create(getApplicationServer().getDatabaseConnection("todo"))
        ->getAll();

    class AccountVisitor
    :   public I_AccountDataCollection::I_CollectionVisitor
    {
    public:
        virtual void begin()
        {
        }

        virtual void visit(I_AccountDataCollection::pAccountDomainObject_type _pDomainObject)
        {

        }

        virtual void end()
        {
        }

        AccountVisitor()
        :   m_request(_request)
        ,   m_loginSuccess(false)
        {
        }

    private:
        Protocol::I_LoginRequest&           m_request;
        bool                                m_loginSuccess;
    };


    // Get the accounts.  There should be only one (or none)
    AccountVisitor accountVisitor();
    accountDC->getValue()->getAll(accountVisitor);
#endif

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
