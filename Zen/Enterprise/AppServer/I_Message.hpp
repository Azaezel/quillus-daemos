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
#ifndef ZEN_APPSERVER_I_MESSAGE_HPP_INCLUDED
#define ZEN_APPSERVER_I_MESSAGE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

namespace boost {
namespace archive {
    class polymorphic_iarchive;
    class polymorphic_oarchive;
} // namespace archive
} // namespace boost

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MessageHeader;
class I_MessageType;

/// I_Message interface.
class APPSERVER_DLL_LINK I_Message
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>     pEndpoint_type;
    typedef Memory::managed_ptr<I_ResourceLocation>         pResourceLocation_type;
    typedef Memory::managed_ptr<I_MessageHeader>            pMessageHeader_type;
    typedef Memory::managed_ptr<I_MessageType>              pMessageType_type;
    /// @}

    /// @name I_Message interface.
    /// @{
public:
    /// Get the source endpoint of this message
    /// @return The destination endpoint, or invalid() if this message originated from
    ///     the current process.
    virtual pEndpoint_type getSourceEndpoint() = 0;

    /// Get the destination endpiont of this message
    /// @return The destination endpoint, or invalid() if this message is destined for 
    ///     the current process.
    virtual pEndpoint_type getDestinationEndpoint() = 0;

    /// Get the source resource of this message.
    virtual pResourceLocation_type getSourceLocation() = 0;

    /// Get the destination resource of this message.
    virtual pResourceLocation_type getDestinationLocation() = 0;

    /// Get the message header.
    virtual pMessageHeader_type getMessageHeader() const = 0;

    /// Get the message Id of this message.
    virtual boost::uint64_t getMessageId() const = 0;

    /// This method returns the message type.
    virtual pMessageType_type getMessageType() const = 0;

    /// Deserialize this message from an input archive.
    /// The protocol adapter deserializes the header, creates the appropriate message
    /// using the message registry, and then deserializes the rest of the message using
    /// this method.
    virtual void serialize(pMessageHeader_type _pHeader, boost::archive::polymorphic_iarchive& _archive, const int _version) = 0;

    /// Serialize this message to an output archive.
    /// The protocol adapter serializes the header and then call this
    /// method to serialize the rest of the message.
    virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Message();
    virtual ~I_Message();
    /// @}

};  // interface I_Message

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
namespace Memory {
    // I_Message is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::AppServer::I_Message> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_I_MESSAGE_HPP_INCLUDED
