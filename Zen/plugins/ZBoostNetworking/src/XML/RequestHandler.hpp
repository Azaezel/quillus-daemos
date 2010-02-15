//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
#ifndef ZEN_ENTERPRISE_APPSERVER_XML_REQUEST_HANDLER_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_XML_REQUEST_HANDLER_HPP_INCLUDED
#include "RequestParser.hpp"
#include "RequestGrammar.hpp"
#include "RequestParserState.hpp"

#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Enterprise {
namespace AppServer {
    class I_ResourceLocation;
    class I_Request;
    class I_Response;
namespace XML {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Connection;

class RequestHandler
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ResourceLocation>         pResourceLocation_type;
    typedef Memory::managed_ptr<Networking::I_Endpoint>     pEndpoint_type;
    typedef boost::shared_ptr<Connection>                   pConnection_type;
    typedef boost::array<char, 8192>::iterator              iterator_t;

    typedef Memory::managed_weak_ptr<I_Request>             wpRequest_type;
    typedef Memory::managed_weak_ptr<I_ResponseHandler>     wpResponseHandler_type;

    typedef I_ResponseHandler::pResponse_type               pResponse_type;
    /// @}

    /// @name RequestHandler implementation
    /// @{
public:
    /// @todo return the end of the parse, in case needs to read more and continue parsing
    void parse(iterator_t _begin, iterator_t _end);

    /// Handle a fully parsed request
    void handleRequest(RequestParserState& _parserState);

private:
    friend class ResponseHandler;

    /// Handle the response.
    void handleResponse(pResponse_type _pResponse);

protected:
    void destroyRequest(wpRequest_type _pRequest);
    void destroyResponseHandler(wpResponseHandler_type _pResponseHandler);
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit RequestHandler(pConnection_type _pConnection, pEndpoint_type _pEndpoint);
    virtual ~RequestHandler();
    /// }@

    /// @name Member Variables
    /// @{
private:
    pConnection_type                    m_pConnection;
    pEndpoint_type                      m_pEndpoint;

    RequestParserState                  m_parserState;
    RequestGrammar                      m_grammar;
    /// @}

};  // class RequestHandler

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace XML
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENTERPRISE_APPSERVER_XML_REQUEST_HANDLER_HPP_INCLUDED
