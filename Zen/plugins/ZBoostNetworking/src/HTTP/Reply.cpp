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

#include "Reply.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>
#include <Zen/Enterprise/AppServer/I_MessageHeader.hpp>

#include <boost/lexical_cast.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class StatusHeaderMap 
:   public std::map<Reply::StatusType, std::string>
{
public:
    StatusHeaderMap()
    {
        insert(std::make_pair<Reply::StatusType, std::string>(
            Reply::ok, "HTTP/1.0 200 OK\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::created, "HTTP/1.0 201 Created\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::accepted, "HTTP/1.0 202 Accepted\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::no_content, "HTTP/1.0 204 No Content\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::multiple_choices, "HTTP/1.0 300 Multiple Choices\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::moved_permanently, "HTTP/1.0 301 Moved Permanently\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::moved_temporarily, "HTTP/1.0 302 Moved Temporarily\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::not_modified, "HTTP/1.0 304 Not Modified\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::bad_request, "HTTP/1.0 400 Bad Request\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::unauthorized, "HTTP/1.0 401 Unauthorized\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::forbidden, "HTTP/1.0 403 Forbidden\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::not_found, "HTTP/1.0 404 Not Found\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::internal_server_error, "HTTP/1.0 500 Internal Server Error\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::not_implemented, "HTTP/1.0 501 Not Implemented\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::bad_gateway, "HTTP/1.0 502 Bad Gateway\r\n")
        );
        insert(std::make_pair<Reply::StatusType, const char*>(
            Reply::service_unavailable, "HTTP/1.0 503 Service Unavailable\r\n")
        );

    }
} static sm_statusHeaderMap;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
class StatusReplyMap 
:   public std::map<Reply::StatusType, const char*>
{
public:
    StatusReplyMap()
    {
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::ok,
            ""
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::created,
            "<html>"
            "<head><title>Created</title></head>"
            "<body><h1>201 Created</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::accepted,
            "<html>"
            "<head><title>Accepted</title></head>"
            "<body><h1>202 Accepted</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::no_content,
            "<html>"
            "<head><title>No Content</title></head>"
            "<body><h1>204 Content</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::multiple_choices,
            "<html>"
            "<head><title>Multiple Choices</title></head>"
            "<body><h1>300 Multiple Choices</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::moved_permanently,
            "<html>"
            "<head><title>Moved Permanently</title></head>"
            "<body><h1>301 Moved Permanently</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::moved_temporarily,
            "<html>"
            "<head><title>Moved Temporarily</title></head>"
            "<body><h1>302 Moved Temporarily</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::not_modified,
            "<html>"
            "<head><title>Not Modified</title></head>"
            "<body><h1>304 Not Modified</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::bad_request,
            "<html>"
            "<head><title>Bad Request</title></head>"
            "<body><h1>400 Bad Request</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::unauthorized,
            "<html>"
            "<head><title>Unauthorized</title></head>"
            "<body><h1>401 Unauthorized</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::forbidden,
            "<html>"
            "<head><title>Forbidden</title></head>"
            "<body><h1>403 Forbidden</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::not_found,
            "<html>"
            "<head><title>Not Found</title></head>"
            "<body><h1>404 Not Found</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::internal_server_error,
            "<html>"
            "<head><title>Internal Server Error</title></head>"
            "<body><h1>500 Internal Server Error</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::not_implemented,
            "<html>"
            "<head><title>Not Implemented</title></head>"
            "<body><h1>501 Not Implemented</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::bad_gateway,
            "<html>"
            "<head><title>Bad Gateway</title></head>"
            "<body><h1>502 Bad Gateway</h1></body>"
            "</html>"
        ));
        insert(std::make_pair<Reply::StatusType, const char*>(Reply::service_unavailable,
            "<html>"
            "<head><title>Service Unavailable</title></head>"
            "<body><h1>503 Service Unavailable</h1></body>"
            "</html>"
        ));


    }
} static sm_statusReplyMap;

static std::map<Reply::StatusType, Reply*>  sm_stockReplies;
#endif 

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::Reply(pEndpoint_type _pDestinationEndpoint, StatusType _status, const std::string& _body, const std::string& _contentType)
:   m_pDestinationEndpoint(_pDestinationEndpoint)
,   m_content(_body)
{
    m_status = _status;
    //m_content = sm_statusReplyMap[_status];
    m_headers.resize(2);
    m_headers[0].m_name = "Content-Length";
    m_headers[0].m_value = boost::lexical_cast<std::string>((int)m_content.size());
    m_headers[1].m_name = "Content-Type";
    m_headers[1].m_value = _contentType;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::Reply()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::~Reply()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::pEndpoint_type
Reply::getSourceEndpoint()
{
    return pEndpoint_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::pEndpoint_type
Reply::getDestinationEndpoint()
{
    return m_pDestinationEndpoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::pResourceLocation_type
Reply::getSourceLocation()
{
    // TODO Implement?
    return pResourceLocation_type();
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::pResourceLocation_type
Reply::getDestinationLocation()
{
    // TODO Implement?
    return pResourceLocation_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::pMessageHeader_type
Reply::getMessageHeader() const
{
    throw Utility::runtime_exception("Reply::getMessageHeader(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
Reply::getMessageId() const
{
    // This really shouldn't be used for HTTP messages.
    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
Reply::getRequestMessageId() const
{
    // This really shouldn't be used for HTTP messages.
    return 0;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Reply::serialize(pMessageHeader_type _pHeader, boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    throw Utility::runtime_exception("Reply::getMessageHeader(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Reply::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    throw Utility::runtime_exception("Reply::getMessageHeader(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void
Reply::initStockReplies()
{
    stockReply(ok);
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated
const Reply&
Reply::stockReply(StatusType _status)
{
    std::map<Reply::StatusType, Reply*>::iterator iter = sm_stockReplies.find(_status);

    if (iter == sm_stockReplies.end())
    {
        Reply* pReply = new Reply;
        pReply->m_status = _status;
        pReply->m_content = sm_statusReplyMap[_status];
        pReply->m_headers.resize(2);
        pReply->m_headers[0].m_name = "Content-Length";
        pReply->m_headers[0].m_value = boost::lexical_cast<std::string>((int)pReply->m_content.size());
        pReply->m_headers[1].m_name = "Content-Type";
        pReply->m_headers[1].m_value = "text/html";

        return *pReply;
    }

    return *(iter->second);
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void
Reply::toBuffers(Buffers_type& _buffers)
{
    _buffers.clear();

    _buffers.reserve(3 + m_headers.size());

    _buffers.push_back(boost::asio::buffer(sm_statusHeaderMap[m_status]));

    for(Headers_type::iterator iter = m_headers.begin(); iter != m_headers.end(); iter++)
    {

        std::string tmp = iter->m_name;

        _buffers.push_back(boost::asio::buffer(iter->m_name));
        _buffers.push_back(boost::asio::buffer(": "));
        _buffers.push_back(boost::asio::buffer(iter->m_value));
        _buffers.push_back(boost::asio::buffer("\r\n"));
    }

    _buffers.push_back(boost::asio::buffer("\r\n"));
    _buffers.push_back(boost::asio::buffer(m_content));

}
#else
void
Reply::toBuffers(std::stringstream& _stream)
{
    _stream << sm_statusHeaderMap[m_status];

    for(Headers_type::iterator iter = m_headers.begin(); iter != m_headers.end(); iter++)
    {
        _stream << iter->m_name << ": " << iter->m_value << "\r\n";
    }

    _stream << "\r\n" << m_content;
}
#endif
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Reply::pMessageType_type
Reply::getMessageType() const
{
    throw Zen::Utility::runtime_exception("HTTP::Reply::getMessageType() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
