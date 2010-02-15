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
#ifndef ZEN_ENTERPRISE_APPSERVER_TCP_CONNECTION_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_TCP_CONNECTION_HPP_INCLUDED
#include "Message.hpp"

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
    class TransmissionControlProtocolService;
namespace TCP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//class RequestHandler;
//class Reply;

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

    /// Connect to an endpoint.
    /// This connection is not connected yet.
    void connect(pEndpoint_type _pEndpoint);

    /// The connection is "connected" so start processing it 
    /// but please make it asynchronous :P
    void start(pEndpoint_type _pEndpoint);

    /// Write a message
    void write(const char* _buffer, boost::uint32_t _size);

    /// Get the endpoint to which this Connection is connected
    pEndpoint_type getPeer() const;

    TransmissionControlProtocolService& getProtocolService();

protected:
    /// Read some more
    void readMore();

    void writeMore();

    /// Called after the header is read
    void handleReadHeader(const boost::system::error_code& _errorCode);

    /// Called after the body was read
    void handleReadBody(const boost::system::error_code& _errorCode);

    /// Called after a write has completed.
    void handleWrite(const boost::system::error_code& _errorCode);

    /// Called after a connect has completed.
    void handleConnect(const boost::system::error_code& _errorCode);

    /// @name 'Structors
    /// @{
public:
    explicit Connection(boost::asio::io_service& _ioService, TransmissionControlProtocolService& _protocolService);
    virtual ~Connection();

private:
    /// Socket for this connection.
    boost::asio::ip::tcp::socket        m_socket;

    /// Used to ensure the Connection's handlers 
    boost::asio::io_service::strand     m_strand;

    TransmissionControlProtocolService&  m_protocolService;

    /// This is the endpoint to which this Connection is connected
    pEndpoint_type      m_pPeerEndpoint;

    /// True if this Connection is connected
    bool                m_connected;

    MessageBuffer       m_readMessage;

    typedef std::list<MessageBuffer*>   MessageQueue_type;
    Threading::I_Mutex*                 m_pWriteQueueGuard;
    MessageQueue_type                   m_writeMessages;
    /// @}

};  // class Connection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TCP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_TCP_CONNECTION_HPP_INCLUDED
