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
#ifndef ZEN_APPSERVER_I_MESSAGE_HEADER_HPP_INCLUDED
#define ZEN_APPSERVER_I_MESSAGE_HEADER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

namespace boost {
namespace archive {
    class polymorphic_oarchive;
} // namespace archive
} // namespace boost

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MessageType;

class APPSERVER_DLL_LINK I_MessageHeader
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_MessageType>          pMessageType_type;
    /// @}

    /// @name I_MessageHeader interface.
    /// @{
public:
    /// Get the type of this message.
    virtual pMessageType_type getMessageType() const = 0;

    /// Get the message id of this message.
    virtual boost::uint32_t getMessageId() const = 0;

    /// Get the associated request id of this message.
    /// This value is 0 if this message is a request a message, and
    /// non-zero if this message is a response.  This value
    /// is the message Id of the request that resulted in the
    /// response.
    virtual boost::uint32_t getRequestId() const = 0;

    /// Serialize the message header.
    /// Deserialization is done in I_MessageRegistry::getMessageHeader().
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MessageHeader();
    virtual ~I_MessageHeader();
    /// @}

};  // interface I_MessageHeader

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
namespace Memory {
    // I_MessageHeader is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::AppServer::I_MessageHeader> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_I_MESSAGE_HEADER_HPP_INCLUDED
