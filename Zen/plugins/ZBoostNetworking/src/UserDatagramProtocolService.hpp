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
#ifndef ZEN_ENTERPRISE_APPSERVER_UDP_PROTOCOL_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_UDP_PROTOCOL_SERVICE_HPP_INCLUDED

#include "../UDP/I_UDPService.hpp"
#include "UDP/SendTaskAllocator.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Threading/I_Thread.hpp>

#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>
#include <Zen/Enterprise/Networking/I_Address.hpp>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Event {
        class I_Event;
    }   // namespace Event
namespace Enterprise {
namespace AppServer {
namespace UDP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class MessageBuffer;

class UserDatagramProtocolService
:   public I_UDPService
,   public Zen::Memory::managed_self_ref<Zen::Enterprise::AppServer::I_ProtocolService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Networking::I_Address>                            pAddress_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Networking::I_Address>                       wpAddress_type;

    typedef Zen::Memory::managed_weak_ptr<Zen::Enterprise::AppServer::I_ProtocolService>    wpProtocolService_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Networking::I_Endpoint>                      wpEndpoint_type;
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_ResourceLocation>        pResourceLocation_type;

    typedef std::vector< Zen::Threading::I_Thread* >                                        threadCollection_type;

    enum { MAX_LENGTH = 4096 };
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual Zen::Threading::I_Condition* prepareToStart(Zen::Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Zen::Threading::I_Condition* prepareToStop();
    virtual void stop();
    /// @}

    /// @name I_ProtocolService implementation
    /// @{
public:
    virtual Zen::Enterprise::AppServer::I_ApplicationServer& getApplicationServer();
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual pEndpoint_type resolveEndpoint(const std::string& _address, const std::string& _port);
    virtual void sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint);
    virtual Event::I_Event& getConnectedEvent();
    virtual Event::I_Event& getDisconnectedEvent();
    /// @}

    /// @name I_UDPService implementation
    /// @{
public:
    /// @}

    /// @name UserDatagramProtocolService implementation
    /// @{
public:
    void receiveFrom();
    void handleReceiveFrom(boost::shared_ptr<MessageBuffer>, const boost::system::error_code& _error, size_t _bytesReceived);
    void destroyEndpoint(wpEndpoint_type& _pEndpoint);
    void handleSendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint);
    pAddress_type createAddress(boost::asio::ip::address& _address);
    void destroyAddress(wpAddress_type& _pAddress);
    /// @}

    /// @name Service locations
    /// @{
public:
    static pResourceLocation_type getLocation(const std::string& _locationName);
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit UserDatagramProtocolService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    virtual ~UserDatagramProtocolService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Reference to the parent application server to which this protocol service is bound
    Zen::Enterprise::AppServer::I_ApplicationServer&    m_appServer;

    /// Primary async IO service
    boost::asio::io_service                             m_ioService;
    boost::asio::io_service::work*                      m_pWork;

    /// Address on which to bind
    std::string                                         m_address;

    /// Port on which to bind
    std::string                                         m_port;

    /// Number of threads (defaults to 1)
    const int                                           m_threadCount;

    /// Collection of threads
    threadCollection_type                               m_threads;

    /// UDP Socket
    boost::asio::ip::udp::socket*                       m_pSocket;

    /// Primary App Server ThreadPool.  Use this for short running tasks.
    Zen::Threading::ThreadPool*                         m_pThreadPool;

    /// Local Send TaskPool.
    Zen::Threading::ThreadPool::TaskPool                m_sendTaskPool;

    SendTaskAllocator                                   m_sendTaskAllocator;

    pMessageRegistry_type                               m_pMessageRegistry;
    /// @}

};  // UserDatagramProtocolService class

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UDP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_UDP_PROTOCOL_SERVICE_HPP_INCLUDED
