//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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

#include "ChatService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Community/ChatProtocol/I_ChatProtocolManager.hpp>
#include <Zen/Community/ChatProtocol/I_ChatRequest.hpp>
#include <Zen/Community/ChatProtocol/I_ChatResponse.hpp>
#include <Zen/Community/ChatProtocol/I_ChatSessionRequest.hpp>
#include <Zen/Community/ChatProtocol/I_ChatSessionResponse.hpp>

#include <Zen/Community/LoginCommon/I_Session.hpp>

#include <Zen/Community/ChatCommon/I_ChatSession.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::ChatService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   m_appServer(_appServer)
,   m_pThreadPool(NULL)
,   m_pHandlersMutex(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::~ChatService()
{
    Zen::Threading::MutexFactory::destroy(m_pHandlersMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    /// TODO Set chat service configuration here.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
ChatService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    Zen::Community::Chat::Protocol::I_ChatProtocolManager::getSingleton().install(m_appServer);

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::start()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
ChatService::prepareToStop()
{
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::handleMessage(pMessage_type _pMessage)
{
    // Should always be an I_ChatResponse
    Zen::Memory::managed_ptr<Protocol::I_ChatResponse> pResponse(
        _pMessage.as<Zen::Memory::managed_ptr<Protocol::I_ChatResponse> >());

    if(pResponse.isValid())
    {
        Zen::Threading::CriticalSection guard(m_pHandlersMutex);
        Handlers_type::iterator iter = m_responseHandlers.find(pResponse->getRequestMessageId());

        if(iter != m_responseHandlers.end())
        {
            // Dispatch the response
            iter->second.get()->handleResponse(pResponse);

            m_responseHandlers.erase(iter);
        }
        else
        {
            // Probably a duplicate response... just ignore it
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_ChatRequest* pRequest =
        dynamic_cast<Protocol::I_ChatRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        Handlers_type::iterator iter = m_responseHandlers.find(pRequest->getRequestId());
        if( iter == m_responseHandlers.end() )
        {
            m_responseHandlers[pRequest->getRequestId()] = _pResponseHandler;
        }

        _pRequest->getDestinationEndpoint()->getProtocolAdapter().lock()->sendTo(
            _pRequest,
            _pRequest->getDestinationEndpoint()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer&
ChatService::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::requestChatSession(pEndpoint_type _pDestinationEndpoint,
                                pSession_type _pSession,
                                pResponseHandler_type _pResponseHandler)
{
    pRequest_type pRequest = Chat::Protocol::I_ChatSessionRequest::create(pEndpoint_type(),
                                                                          _pDestinationEndpoint,
                                                                          _pSession);

    handleRequest(pRequest, _pResponseHandler);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ResponseHandler
:   public Zen::Enterprise::AppServer::I_ResponseHandler
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_ResponseHandler>    wpResponseHandler_type;
    /// @}

    /// @name I_ResponseHandler implementation
    /// @{
public:
    virtual void handleResponse(pResponse_type _pResponse)
    {
        Chat::Protocol::I_ChatSessionResponse*
            pResponse = dynamic_cast<Zen::Community::Chat::Protocol::I_ChatSessionResponse*>(_pResponse.get());

        // TODO Create a chat session out of I_ChatSessionResponse and assign it to m_pFutureChatSession.
        throw Zen::Utility::runtime_exception("ResponseHandler::handleResponse() : Error, not implemented.");
    }
    /// @}

    /// @name ResponseHandler implementation
    /// @{
public:
    ChatService::pFutureChatSession_type getChatSession()
    {
        return m_pFutureChatSession;
    }
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroy(wpResponseHandler_type _wpResponseHandler)
    {
        ResponseHandler* pResponseHandler =
            dynamic_cast<ResponseHandler*>(_wpResponseHandler.get());

        if( pResponseHandler != NULL )
        {
            delete pResponseHandler;
        }
        else
        {
            throw Zen::Utility::runtime_exception("ResponseHandler::destroy() : Invalid type.");
        }
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
             ResponseHandler() {}
    virtual ~ResponseHandler() {}
    /// @}

    /// @name Member variables
    /// @{
private:
    ChatService::pFutureChatSession_type        m_pFutureChatSession;
    /// @}

};  // class ResponseHandler 

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::pFutureChatSession_type
ChatService::connect(pSession_type _pSession)
{
    ResponseHandler* pRawResponseHandler(new ResponseHandler());

    pResponseHandler_type pResponseHandler(pRawResponseHandler, &ResponseHandler::destroy);

    requestChatSession(_pSession->getEndpoint(),
                       _pSession,
                       pResponseHandler);

    return pRawResponseHandler->getChatSession();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Chat
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
