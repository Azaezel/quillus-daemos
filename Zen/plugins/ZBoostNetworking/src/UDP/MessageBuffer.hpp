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
#ifndef ZEN_ENTERPRISE_APPSERVER_UDP_MESSAGE_BUFFER_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_UDP_MESSAGE_BUFFER_HPP_INCLUDED

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/cstdint.hpp> 

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace UDP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class UserDatagramProtocolService;

/// Message Buffer
/// This message is used to store the message before it's decoded
/// or after it's been encoded.
class MessageBuffer
:   public boost::enable_shared_from_this<MessageBuffer>
,   private boost::noncopyable
{
public:
    enum { HEADER_LENGTH = 4 };
    enum { MAX_BODY_LENGTH = 1500 };
    enum { MAX_LENGTH = MAX_BODY_LENGTH + HEADER_LENGTH };

public:
    char* getData();
    char* getBody();

    /// Get the full length of the message including the header
    boost::uint32_t getLength();

    /// Get the length of the body
    boost::uint32_t getBodyLength();

    /// Set the length of the body
    void setBodyLength(boost::uint32_t _length);

    /// Decode the header.
    /// (network order to machine order)
    bool decodeHeader();

    /// Encode the header.
    /// (machine order to network order)
    void encodeHeader();

    boost::asio::ip::udp::endpoint& getEndpoint();
    void handleReceiveFrom(const boost::system::error_code& _error, size_t _bytesReceived);

public:
             MessageBuffer(UserDatagramProtocolService& _protocolService);
    virtual ~MessageBuffer();

private:
    UserDatagramProtocolService&        m_protocolService;
    boost::asio::ip::udp::endpoint      m_senderEndpoint;

    union
    {
        boost::uint32_t                 m_bodyLength;
        char                            m_data[HEADER_LENGTH + MAX_BODY_LENGTH];
    };

};  // class MessageBuffer

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UDP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_UDP_MESSAGE_BUFFER_HPP_INCLUDED
