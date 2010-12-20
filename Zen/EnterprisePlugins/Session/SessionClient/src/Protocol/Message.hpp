//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ENTERPRISE_SESSION_PROTOCOL_MESSAGE_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_PROTOCOL_MESSAGE_HPP_INCLUDED

#include "../../Protocol/I_Message.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
    namespace AppServer {
        class I_MessageFactory;
    }   // namespace AppServer
namespace Session {
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Message
:   public Zen::Enterprise::Session::Protocol::I_Message
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_MessageFactory>      pMessageFactory_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_MessageFactory> wpMessageFactory_type;
    /// @}

    /// @name Zen::Enterprise::AppServer::I_Message implementation
    /// @{
public:
    virtual pEndpoint_type getSourceEndpoint();
    virtual pEndpoint_type getDestinationEndpoint();
    virtual pMessageHeader_type getMessageHeader() const;
    virtual pResourceLocation_type getSourceLocation() const;
    virtual pResourceLocation_type getDestinationLocation() const;
    /// @}
    
    /// @name I_Message implementation
    /// @{
public:
    virtual boost::uint64_t getMessageId() const;
    virtual pMessageType_type getMessageType() const;
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyMessageFactory(wpMessageFactory_type _wpMessageFactory);
    static boost::uint64_t getNewMessageId();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class MessageFactory;
             Message(pMessageHeader_type _pMessageHeader,
                     pEndpoint_type _pSourceEndpoint,
                     pEndpoint_type _pDestinationEndpoint);
    virtual ~Message();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    static pMessageFactory_type sm_pMessageFactory;
private:
    pEndpoint_type          m_pSourceEndpoint;
    pEndpoint_type          m_pDestinationEndpoint;
    pMessageHeader_type     m_pMessageHeader;
    pResourceLocation_type  m_pDestinationLocation;
    /// @}

};  // class Message

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}   // namespace Session
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_PROTOCOL_MESSAGE_HPP_INCLUDED
