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
namespace XML {
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
    m_headers[1].m_value = "application/xml";

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

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace XML
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
