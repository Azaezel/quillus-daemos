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
#include "Connection.hpp"
#include "RequestHandler.hpp"
#include "Reply.hpp"

#include "../HyperTextTransportProtocolService.hpp"

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <boost/bind.hpp>
//#include <boost/logic/tribool.hpp>
//#include <boost/tuple/tuple.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Connection::Connection(boost::asio::io_service& _ioService, HyperTextTransportProtocolService& _protocolService)
:   m_socket(_ioService)
,   m_strand(_ioService)
,   m_protocolService(_protocolService)
,   m_pHandler(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Connection::~Connection()
{
    // m_pHandler is destroyed in Connection::write()
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::asio::ip::tcp::socket&
Connection::getSocketReference()
{
    return m_socket;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::start(pEndpoint_type _pEndpoint)
{
    // The request handler will keep this Connection object alive until all
    // of the requests have been processed.
    // TODO Use a pool of RequestHandler because they're expensive to create
    m_pHandler = new RequestHandler(shared_from_this(), _pEndpoint);

    /// Now read some
    readMore();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::readMore()
{
    // Asynchronously read some data from the connection into the buffer.
    // Using shared_from_this() will prevent this Connection object from
    // being destroyed while data is being read.
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
        m_strand.wrap(
            boost::bind(&Connection::handleRead, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)
        )
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::write(pResponse_type _pResponse)
{
    /// Keep a reference to this reponse so the buffers don't get destroyed
    //m_pResponse = _pResponse;

    Reply* pReply = dynamic_cast<Reply*>(_pResponse.get());
    if (pReply)
    {
#if 0   // async
        std::stringstream output;
        pReply->toBuffers(output);

        m_response = output.str();

        m_buffers.reserve(1);
        m_buffers.push_back(boost::asio::buffer(m_response));

        boost::asio::async_write(m_socket, m_buffers, 
            m_strand.wrap(
                boost::bind(&Connection::handleWrite, shared_from_this(),
                    boost::asio::placeholders::error
                )
            )
        );
#else
        // sync
        std::stringstream output;
        pReply->toBuffers(output);

        m_response = output.str();

        std::vector<boost::asio::const_buffer> buffers;
        buffers.reserve(1);
        buffers.push_back(boost::asio::buffer(m_response));

        boost::asio::write(m_socket, buffers);
        boost::system::error_code ignoredError;
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignoredError);

        // TODO Improve the lifetime handling of m_pHandler
        delete m_pHandler;
#endif
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HyperTextTransportProtocolService&
Connection::getProtocolService()
{
    return m_protocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::handleRead(const boost::system::error_code& _errorCode, std::size_t _bytesTransferred)
{
    if (!_errorCode)
    {
        // Parse this buffer into the requests
        m_pHandler->parse(m_buffer.data(), m_buffer.data() + _bytesTransferred);

#if 0 // deprecated.  All of this is now done in the handler
        if (result)
        {
            // Handle the requests
            m_protocolService.getApplicationServer().handleRequest(
            m_requestHandler.handleRequest(m_request, m_reply);

            boost::asio::async_write(m_socket, m_reply.toBuffers(),
                m_strand.wrap(
                    boost::bind(&Connection::handleWrite, shared_from_this(),
                        boost::asio::placeholders::error
                    )
                )
            );

        }
        else if (!result)
        {
            // Bad request, reply with an error
            m_reply = HTTP::Reply::stockReply(HTTP::Reply::bad_request);
            boost::asio::async_write(m_socket, m_reply.toBuffers(),
                m_strand.wrap(
                    boost::bind(&Connection::handleWrite, shared_from_this(),
                        boost::asio::placeholders::error
                    )
                )
            );
        }
        else
        {
            m_socket.async_read_some(boost::asio::buffer(m_buffer),
                m_strand.wrap(
                    boost::bind(&Connection::handleRead, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred
                    )
                )
            );
        }
#endif

    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Connection::handleWrite(const boost::system::error_code& _errorCode)
{
    if (!_errorCode)
    {
        boost::system::error_code ignoredError;
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignoredError);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
