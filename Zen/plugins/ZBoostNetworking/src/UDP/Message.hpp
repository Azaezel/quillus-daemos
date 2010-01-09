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
#ifndef ZEN_ENTERPRISE_APPSERVER_UDP_MESSAGE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_UDP_MESSAGE_HPP_INCLUDED

#include "../../UDP/I_Message.hpp"
#include "Endpoint.hpp"

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <boost/serialization/serialization.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace UDP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Message
:   public virtual I_Message
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Message implementation
    /// @{
public:
    virtual pEndpoint_type getSourceEndpoint();
    virtual pEndpoint_type getDestinationEndpoint();
    virtual pResourceLocation_type getDestinationLocation();

    virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name UDP::I_Message implementation
    /// @{
public:
    virtual unsigned int getMessageID() const;

    virtual unsigned int getServiceID() const;
    /// @}

    /// @name Message implementation
    /// @{
public:
    static unsigned int getNewMessageID();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             Message(unsigned int _serviceID,
                     pEndpoint_type _pSourceEndpoint,
                     pEndpoint_type _pDestinationEndpoint,
                     pResourceLocation_type _pDestinationLocation,
                     unsigned int _messageID);
public:
    virtual ~Message();
    /// @}

    /// @name Member Variables
    /// @{
private:
    unsigned int                m_serviceID;
    pEndpoint_type              m_pSourceEndpoint;
    pEndpoint_type              m_pDestinationEndpoint;
    pResourceLocation_type      m_pDestinationLocation;

    boost::int32_t              m_messageID;
    /// @}

};  // Message class

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UDP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_UDP_MESSAGE_HPP_INCLUDED
