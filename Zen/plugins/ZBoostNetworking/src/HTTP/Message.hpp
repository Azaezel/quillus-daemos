//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_ENTERPRISE_APPSERVER_HTTP_MESSAGE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_HTTP_MESSAGE_HPP_INCLUDED

#include <Zen/Enterprise/AppServer/I_Message.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace HTTP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Message
:   public I_Message
{
    /// @name I_Message implementation
    /// @{
public:
    virtual pEndpoint_type getSourceEndpoint();
    virtual pEndpoint_type getDestinationEndpoint();
    virtual pResourceLocation_type getDestinationLocation();
    virtual unsigned int getMessageId() const;
    virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Message(pEndpoint_type _pSourceEndpoint, pEndpoint_type _pDestinationEndpoint, pResourceLocation_type _pResourceLocation);
    virtual ~Message();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pEndpoint_type              m_pSourceEndpoint;
    pEndpoint_type              m_pDestinationEndpoint;
    pResourceLocation_type      m_pDestinationLocation;
    /// @}

};  // class Message

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace HTTP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_HTTP_MESSAGE_HPP_INCLUDED
