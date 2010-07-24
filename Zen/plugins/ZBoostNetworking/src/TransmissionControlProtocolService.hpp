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
#ifndef ZEN_ENTERPRISE_APPSERVER_TCP_PROTOCOL_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_TCP_PROTOCOL_SERVICE_HPP_INCLUDED

#include "TCP/Connection.hpp"
#include "TCP/Message.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Core/Event/I_Event.hpp>

#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>

#include <boost/shared_ptr.hpp>

#include <map>
#include <string>
#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TransmissionControlProtocolService
:   public I_ProtocolService
,   public Memory::managed_self_ref<I_ProtocolService>
{
    /// @name Types
    /// @{
public:
    typedef std::vector<Threading::I_Thread*>                   Threads_type;
    typedef Memory::managed_weak_ptr<Networking::I_Endpoint>    wpEndpoint_type;

    typedef boost::shared_ptr<TCP::Connection>                  pConnection_type;
    typedef boost::asio::ip::tcp::endpoint                      EndpointKey_type;
    typedef std::map<EndpointKey_type, pConnection_type>        ConnectionMap_type;
    /// @}

    /// @name I_ProtocolService implementation
    /// @{
public:
    virtual I_ApplicationServer& getApplicationServer();
    virtual pEndpoint_type resolveEndpoint(const std::string& _address, const std::string& _port);
    virtual void sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint);
    virtual Event::I_Event& getConnectedEvent();
    virtual Event::I_Event& getDisconnectedEvent();
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual void setConfiguration(const Plugins::I_ConfigurationElement& _config);
    virtual Threading::I_Condition* prepareToStart(Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Threading::I_Condition* prepareToStop();
    virtual void stop();
    /// @}

    /// @name TransmissionControlProtocolService implementation
    /// @{
public:
    void startThreads();

    void handleAccept(const boost::system::error_code& _error);

    void createConnection();
    void asyncAccept();
    void destroyEndpoint(wpEndpoint_type _pEndpoint);

    /// This is called by the connection after it has established
    /// a TCP connection to a server.
    void onConnected(pConnection_type _pConnection);

    /// This is called by the connection after it has lost a
    /// connection to the server.
    void onDisconnected(pConnection_type _pConnection);
    void onHandleMessage(pConnection_type  _pConnection, TCP::MessageBuffer& _message);

    /// Get the location for this URL
    /// @see I_WebProtocolService::registerURL()
    //pResourceLocation_type getLocation(const std::string& _url);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ProtocolServiceFactory;

    explicit TransmissionControlProtocolService(I_ApplicationServer& _server);
    virtual ~TransmissionControlProtocolService();
    /// @}

    /// @name Inner Classes
    /// @{
public:

    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Reference to the parent application server to which this protocol service is bound
    AppServer::I_ApplicationServer&    m_appServer;

    /// IO Service to perform asynchronous operations
    boost::asio::io_service             m_ioService;
    boost::asio::io_service::work*      m_pWork;

    /// Acceptor used to listen for incoming connections
    boost::asio::ip::tcp::acceptor      m_acceptor;

    /// Address on which to bind
    std::string                         m_address;

    /// Port to which to bind
    std::string                         m_port;

    /// Number of threads
    int                                 m_threadCount;

    /// True if this is a server that is listening
    bool                                m_isServer;

    volatile bool                       m_threadsStarted;

    /// New connection used for the next accepted connection
    /// if this is a server side service.
    /// This is a shared pointer instead of a managed pointer
    /// because boost::bind<> handles shared_ptr correctly
    boost::shared_ptr<TCP::Connection> m_pNewConnection;

    /// Collection of threads
    Threads_type                        m_threads;

    /// Endpoint on which this service is listening (if any)
    boost::asio::ip::tcp::endpoint      m_endpoint;

    ConnectionMap_type                  m_connectionMap;

    /// Guard for m_connections.  
    /// Also used as a guard for m_threads
    Threading::I_Mutex*                 m_pConnectionsGuard;

    pMessageRegistry_type               m_pMessageRegistry;
    /// @}

};  // class TransmissionControlProtocolService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_TCP_PROTOCOL_SERVICE_HPP_INCLUDED
