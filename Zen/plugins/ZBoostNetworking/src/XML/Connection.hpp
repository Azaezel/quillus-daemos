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
#ifndef ZEN_ENTERPRISE_APPSERVER_XML_CONNECTION_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_XML_CONNECTION_HPP_INCLUDED

#include "RequestParser.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <boost/asio.hpp>
#include <boost/array.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Enterprise {
namespace AppServer {
    class I_Request;
    class I_Response;
namespace XML {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class XMLProtocolService;
class RequestHandler;
class Reply;

class Connection
:   public boost::enable_shared_from_this<Connection>
,   private boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>     pEndpoint_type;
    typedef Memory::managed_ptr<I_Request>                  pRequest_type;
    typedef Memory::managed_ptr<I_Response>                 pResponse_type;
    /// @}

public:
    /// Get the socket associated with the Connection
    boost::asio::ip::tcp::socket& getSocketReference();

    /// The connection is "connected" so start processing it 
    /// but please make it asynchronous :P
    void start(pEndpoint_type _pEndpoint);

    /// Read some more
    void readMore();

    /// Write a reply and close
    void write(pResponse_type _pResponse);

    XMLProtocolService& getProtocolService();

protected:
    /// Called after a read has completed
    void handleRead(const boost::system::error_code& _errorCode, std::size_t _bytesTransferred);

    /// Called after a write has completed.
    void handleWrite(const boost::system::error_code& _errorCode);

    /// @name 'Structors
    /// @{
public:
    explicit Connection(boost::asio::io_service& _ioService, XMLProtocolService& _protocolService);
    virtual ~Connection();

private:
    /// Socket for this connection.
    boost::asio::ip::tcp::socket        m_socket;

    /// Used to ensure the Connection's handlers 
    boost::asio::io_service::strand     m_strand;

    /// Buffer for reading.
    boost::array<char, 8192>            m_buffer;

    XMLProtocolService&                 m_protocolService;

    RequestHandler*                     m_pHandler;

    pResponse_type                      m_pResponse;

    std::auto_ptr<char>                 m_pResponseBuffer;

    std::vector<boost::asio::const_buffer> m_buffers;

    std::string                         m_response;
    /// @}

};  // class Connection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace XML
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_XML_CONNECTION_HPP_INCLUDED
