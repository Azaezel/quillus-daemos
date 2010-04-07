//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include <boost/asio.hpp>

#if 0
#define BOOST_SPIRIT_DEBUG
#endif

#include "RequestHandler.hpp"
#include "RequestGrammar_impl.hpp"
#include "Request.hpp"
#include "Reply.hpp"

#include "../HyperTextTransportProtocolService.hpp"
#include "Connection.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>
#include <Zen/Enterprise/AppServer/I_Response.hpp>

#include <boost/lambda/lambda.hpp>

#include <algorithm>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RequestHandler::RequestHandler(pConnection_type _pConnection, pEndpoint_type _pEndpoint)
:   m_pConnection(_pConnection)
,   m_pEndpoint(_pEndpoint)
,   m_parserState()
,   m_grammar(m_parserState)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RequestHandler::~RequestHandler()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RequestHandler::parse(iterator_t _begin, iterator_t _end)
{
    m_parserState.reset(this);

    boost::tribool result;

    //result = m_requestParser.parse(this, _begin, _end);

    std::for_each(_begin, _end, std::cout << boost::lambda::_1);

    boost::spirit::classic::parse_info<iterator_t> info = boost::spirit::classic::parse(_begin, _end, m_grammar);

    // TODO Read some more if not all of it was read.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ResponseHandler
: public I_ResponseHandler
{
public:
    virtual void handleResponse(pResponse_type _pResponse)
    {
        m_parent.handleResponse(_pResponse);
    }

    ResponseHandler(RequestHandler& _parent)
    :   m_parent(_parent)
    {
    }

    virtual ~ResponseHandler()
    {
    }

private:
    RequestHandler& m_parent;

};  // class ResponseHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RequestHandler::handleRequest(RequestParserState& _parserState)
{
    // Get the resource location.
    Request::pResourceLocation_type pResourceLocation(m_pConnection->getProtocolService().getLocation(_parserState.getURL()));

    Request* pRawRequest = new Request(m_pConnection, m_pEndpoint, pResourceLocation);
    I_ApplicationServer::pRequest_type pRequest(pRawRequest, boost::bind(&RequestHandler::destroyRequest, this, _1));

    ResponseHandler* pRawResponseHandler = new ResponseHandler(*this);

    I_ApplicationServer::pResponseHandler_type pResponseHandler(pRawResponseHandler, boost::bind(&RequestHandler::destroyResponseHandler, this, _1));

    m_pConnection->getProtocolService().getApplicationServer().handleRequest(pRequest, pResponseHandler);

    // And last but not least, reset the parser state
    _parserState.reset(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RequestHandler::handleResponse(pResponse_type _pResponse)
{
    Reply* pReply = dynamic_cast<Reply*>(_pResponse.get());
    if (pReply)
    {
        m_pConnection->write(_pResponse);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RequestHandler::destroyRequest(wpRequest_type _pRequest)
{
    Request* pRequest = dynamic_cast<Request*>(_pRequest.get());

    if (pRequest)
    {
        delete pRequest;
    }
    else
    {
        throw Utility::runtime_exception("RequestHandler::destroyRequest(): Error, wrong type!");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RequestHandler::destroyResponseHandler(wpResponseHandler_type _pResponseHandler)
{
    ResponseHandler* pResponseHandler = dynamic_cast<ResponseHandler*>(_pResponseHandler.get());

    if (pResponseHandler)
    {
        delete pResponseHandler;
    }
    else
    {
        throw Utility::runtime_exception("RequestHandler::destroyResponseHandler(): Error, wrong type!");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
