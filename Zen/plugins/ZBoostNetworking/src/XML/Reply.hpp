//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ENTERPRISE_APPSERVER_XML_REPLY_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_XML_REPLY_HPP_INCLUDED

#include "../../XML/I_XMLResponse.hpp"

#include "Header.hpp"

#include <boost/asio.hpp>

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include <vector>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace XML {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// @deprecated
class Reply
:   public I_XMLResponse
{
    /// @name Types
    /// @{
public:
    typedef std::vector<Header>                     Headers_type;
    typedef std::vector<boost::asio::const_buffer>  Buffers_type;
    /// @}

    /// @name I_Message implementation
    /// @{
public:
    virtual pEndpoint_type getSourceEndpoint();
    virtual pEndpoint_type getDestinationEndpoint();
    virtual pResourceLocation_type getDestinationLocation();
    virtual pMessageHeader_type getMessageHeader() const;
    virtual boost::uint32_t getMessageId() const;
    virtual pMessageType_type getMessageType() const;
    virtual void serialize(pMessageHeader_type _pHeader, boost::archive::polymorphic_iarchive& _archive, const int _version);
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name I_Response implementation
    /// @{
public:
    virtual unsigned int getRequestMessageId() const;
    /// @}

    /// @name Reply implementation
    /// @{
public:
    /// Stream the reply into a string stream.
    ///
    /// I don't like this implementation and in reality it should probably use one
    /// of the serialize methods above but I don't have that working just yet.
    void toBuffers(std::stringstream& _stream);

    //static void initStockReplies();
    //static const Reply& stockReply(StatusType _status);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Reply(pEndpoint_type _pDestinationEndpoint, StatusType _status, 
                 const std::string& _body, const std::string& _contentType, 
                 unsigned int _requestMessageId);
             Reply();
    virtual ~Reply();
    /// }@

    /// @name Member Variables
    /// @{
private:
    Headers_type        m_headers;
    StatusType          m_status;
    std::string         m_content;

    pEndpoint_type      m_pDestinationEndpoint;
    unsigned int        m_requestMessageId;

    //Buffers_type        m_buffers;

    /// @}

};  // class Reply

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace XML
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_XML_REPLY_HPP_INCLUDED
