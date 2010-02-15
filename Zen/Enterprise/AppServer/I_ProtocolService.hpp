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
#ifndef ZEN_APPSERVER_I_PROTOCOL_SERVICE_HPP_INCLUDED
#define ZEN_APPSERVER_I_PROTOCOL_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Plugins/I_StartupShutdownParticipant.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ApplicationServer;
class I_Message;
class I_MessageRegistry;
class I_MessageHeader;

class APPSERVER_DLL_LINK I_ProtocolService
:   public Plugins::I_StartupShutdownParticipant
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Message>                  pMessage_type;
    typedef Memory::managed_ptr<I_MessageHeader>            pMessageHeader_type;
    typedef Memory::managed_ptr<Networking::I_Endpoint>     pEndpoint_type;
    typedef Memory::managed_ptr<I_MessageRegistry>          pMessageRegistry_type;
    /// @}

    /// @name I_ProtocolService interface.
    /// @{
public:
    /// Get the application server which is hosting this protocol service.
    /// This must be the same reference that was passed to the factory create
    /// method.
    /// @see I_ProtocolServiceFactory
    virtual I_ApplicationServer& getApplicationServer() = 0;

    /// Resolve the endpoint for an address and port specified by strings.
    /// If the resolver and the operating system supports it, _port can also 
    /// contain service names such as "http".
    virtual pEndpoint_type resolveEndpoint(const std::string& _address, const std::string& _port) = 0;

    /// Send a message to an endpoint, potentially establishing the connection first.
    virtual void sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint) = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProtocolService();
    virtual ~I_ProtocolService();
    /// @}

};  // interface I_ProtocolService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
namespace Memory 
{
    /// I_ProtocolService is managed by a factory
    template<>
    struct is_managed_by_factory<Enterprise::AppServer::I_ProtocolService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_I_PROTOCOL_SERVICE_HPP_INCLUDED
