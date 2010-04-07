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
#ifndef ZEN_APPSERVER_I_MESSAGE_REGISTRY_HPP_INCLUDED
#define ZEN_APPSERVER_I_MESSAGE_REGISTRY_HPP_INCLUDED
#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

namespace boost {
namespace archive {
    class polymorphic_iarchive;
} // namespace archive
} // namespace boost

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_MessageType;
class I_MessageHeader;

class APPSERVER_DLL_LINK I_MessageRegistry
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_MessageType>              pMessageType_type;
    typedef Memory::managed_ptr<I_MessageHeader>            pMessageHeader_type;
    typedef Memory::managed_ptr<I_MessageRegistry>          pMessageRegistry_type;
    /// @}

    /// @name I_MessageRegistry interface.
    /// @{
public:
    /// Register a message type.
    virtual void registerMessageType(pMessageType_type _pMessageType) = 0;

    /// Unregister a message type.
    virtual void unregisterMessageType(pMessageType_type _pMessageType) = 0;

    /// Get the message header.
    /// The implementation should use the archive to deserialize the application level
    /// message header.
    virtual pMessageHeader_type getMessageHeader(boost::archive::polymorphic_iarchive& _archive) const = 0;

    /// Create a message header for the given message type.
    virtual pMessageHeader_type createMessageHeader(pMessageType_type _pMessageType, boost::uint32_t _messageId, boost::uint32_t _requestId) const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MessageRegistry();
    virtual ~I_MessageRegistry();
    /// @}

};  // interface I_MessageRegistry

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
namespace Memory {
    // I_MessageRegistry is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::AppServer::I_MessageRegistry> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_I_MESSAGE_REGISTRY_HPP_INCLUDED
