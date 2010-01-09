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
#ifndef ZEN_NETWORKING_I_ENDPOINT_HPP_INCLUDED
#define ZEN_NETWORKING_I_ENDPOINT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Enterprise {
    namespace AppServer {
        // HACK ack circular reference.  I_Endpoint might need to be moved to AppServer
        class I_ProtocolService;
    }   // namespace AppServer
    }   // namespace Enterprise
namespace Networking {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Address;

/// Basic Network Endpoint
class NETWORKING_DLL_LINK I_Endpoint
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Enterprise::AppServer::I_ProtocolService>  wpProtocolService_type;

    typedef Memory::managed_ptr<I_Address>  pAddress_type;
    /// @}

    /// @name I_Endpoint interface
    /// @{
public:
    /// Get the protocol adapter that is associated with this endpoint.
    virtual wpProtocolService_type getProtocolAdapter() const = 0;

    /// Get the network address that is associated with this endpoint.
    virtual const Zen::Networking::I_Address& getAddress() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Endpoint();
    virtual ~I_Endpoint();
    /// @}

};  // interface I_Endpoint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Networking
namespace Memory 
{
    /// I_Endpoint is managed by a factory
    template<>
    struct is_managed_by_factory<Networking::I_Endpoint> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_NETWORKING_I_ENDPOINT_HPP_INCLUDED
