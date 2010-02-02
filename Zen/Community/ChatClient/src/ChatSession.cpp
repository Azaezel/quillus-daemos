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

#include "ChatSession.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Spaces/ObjectModel/I_ObjectType.hpp>
#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_Request.hpp>

#include <Zen/Community/LoginCommon/I_Session.hpp>

#include <Zen/Community/ChatCommon/I_ChatService.hpp>
#include <Zen/Community/ChatCommon/I_Channels.hpp>

#include <Zen/Community/ChatProtocol/I_ChannelListRequest.hpp>
#include <Zen/Community/ChatProtocol/I_ChannelListResponse.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatSession::ChatSession(pService_type _pService, pSession_type _pSession)
:   m_pLoginSession(_pSession)
,   m_pService(_pService)
,   m_state(DISCONNECTED)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatSession::~ChatSession()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatSession::pObjectCollection_type
ChatSession::getChildObjects(pObjectType_type _pType,
                             pFilter_type _pFilter)
{
    throw Zen::Utility::runtime_exception("Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatSession::ChatSessionState_type
ChatSession::getState() const
{
    return m_state;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatSession::pSession_type
ChatSession::getLoginSession() const
{
    return m_pLoginSession;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatSession::disconnect()
{
    throw Zen::Utility::runtime_exception("Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatSession::requestChannels(pEndpoint_type _pDestinationEndpoint,
                             pChatSession_type _pChatSession,
                             pResponseHandler_type _pResponseHandler)
{
    pRequest_type pRequest = Chat::Protocol::I_ChannelListRequest::create(pEndpoint_type(),
                                                                          _pDestinationEndpoint,
                                                                          _pChatSession);

    m_pService->handleRequest(pRequest, _pResponseHandler);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatSession::destroy(wpChatSession_type _wpChatSession)
{
    ChatSession* pChatSession =
        dynamic_cast<ChatSession*>(_wpChatSession.get());

    if( pChatSession != NULL )
    {
        delete pChatSession;
    }
    else
    {
        throw Zen::Utility::runtime_exception("ChatSession::destroy() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ResponseHandler
:   public Zen::Enterprise::AppServer::I_ResponseHandler
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_ResponseHandler> wpResponseHandler_type;
    /// @}

    /// @name I_ResponseHandler implementation
    /// @{
public:
    virtual void handleResponse(pResponse_type _pResponse)
    {
        Chat::Protocol::I_ChannelListResponse*
            pResponse = dynamic_cast<Zen::Community::Chat::Protocol::I_ChannelListResponse*>(_pResponse.get());

        throw Zen::Utility::runtime_exception("ResponseHandler::handleResponse() : Error, not implemented.");
    }
    /// @}

    /// @name ResponseHandler implementation
    /// @{
public:
    ChatSession::pFutureChannels_type getChannels()
    {
        return m_pFutureChannels;
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
    ChatSession::pFutureChannels_type   m_pFutureChannels;
    /// @}

};  // class ResponseHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatSession::pFutureChannels_type
ChatSession::getChannels()
{
    ResponseHandler* pRawResponseHandler(new ResponseHandler());

    pResponseHandler_type pResponseHandler(pRawResponseHandler, &ResponseHandler::destroy);

    requestChannels(m_pLoginSession->getEndpoint(),
                    pChatSession_type(this, destroy),
                    pResponseHandler);

    return pRawResponseHandler->getChannels();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Chat
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
