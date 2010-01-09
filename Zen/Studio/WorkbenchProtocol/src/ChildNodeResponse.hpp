//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio Workbench Framework
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
#ifndef ZEN_STUDIO_WORKBENCH_CHILD_NODE_RESPONSE_HPP_INCLUDED
#define ZEN_STUDIO_WORKBENCH_CHILD_NODE_RESPONSE_HPP_INCLUDED

#include "../I_ChildNodeResponse.hpp"

#include "Message.hpp"

#include <Zen/Enterprise/AppServer/I_MessageFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Enterprise {
        namespace AppServer {
            class I_ApplicationServer;
            class I_MessageRegistry;
            class I_MessageType;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class ChildNodeResponse
:   public I_ChildNodeResponse
,   public Message
{
    /// @name Types
    /// @{
public:
    enum { type = 4 };  // TODO Should we be hardcoding this?
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_MessageType>         pMessageType_type;
    /// @}

    /// @name Zen::Enterprise::AppServer::I_Message implementation
    /// @{
public:
    virtual pEndpoint_type getSourceEndpoint() { return Message::getSourceEndpoint(); }
    virtual pEndpoint_type getDestinationEndpoint() { return Message::getDestinationEndpoint(); }
    virtual pMessageHeader_type getMessageHeader() const { return Message::getMessageHeader(); }

    virtual pResourceLocation_type getDestinationLocation();
    virtual void serialize(pMessageHeader_type _pMessageHeader, boost::archive::polymorphic_iarchive& _archive, const int _version);
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name ChildNodeResponse implementation
    /// @{
public:
    virtual unsigned int getMessageId() const { return Message::getMessageId(); }
    /// @}

    /// @name Static methods
    /// @{
public:
    static void registerMessage(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);

    static pMessageHeader_type createMessageHeader();

    static void destroy(wpResponse_type _wpResponse);
    /// @}

    /// @name 'Structors
    /// @{
public:
    friend class MessageFactory;
             /// This constructor is used by the static create
             /// methods for creating outbound messages.
             ChildNodeResponse(pEndpoint_type _pSourceEndpoint,
                               pEndpoint_type _pDestinationEndpoint);
             /// This constructor is used by the message factory
             /// for creating inbound messages.
             ChildNodeResponse(pMessageHeader_type _pMessageHeader,
                               pEndpoint_type _pSourceEndpoint,
                               pEndpoint_type _pDestinationEndpoint);
    virtual ~ChildNodeResponse();
    /// @}

    /// @name Member Variables
    /// @{
private:
    unsigned int                    m_id;

    static pMessageType_type                                sm_pType;
    static Zen::Enterprise::AppServer::I_MessageRegistry*   sm_pMessageRegistry;
    /// @}

};  // class ChildNodeResponse

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}	// namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_STUDIO_WORKBENCH_CHILD_NODE_RESPONSE_HPP_INCLUDED
