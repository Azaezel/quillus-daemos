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
#ifndef ZEN_ENTERPRISE_APPSERVER_XML_PROTOCOL_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_XML_PROTOCOL_SERVICE_HPP_INCLUDED

#include "../../XML/I_XMLProtocolService.hpp"
#include "Connection.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

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
    class ProtocolServiceFactory;
namespace XML {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RequestHandler;

class XMLProtocolService
:   public I_XMLProtocolService
,   public Memory::managed_self_ref<I_ProtocolService>
{
    /// @name Types
    /// @{
public:
    typedef std::vector<Threading::I_Thread*>                   Threads_type;
    typedef Memory::managed_weak_ptr<Networking::I_Endpoint>    wpEndpoint_type;
    typedef std::pair<std::string, pResourceLocation_type>      LocationPair_type;
    typedef std::list<LocationPair_type>                        Locations_type;
    /// @}

    /// @name I_XMLProtocolService implementation
    /// @{
public:
    virtual void registerURL(const std::string& _url, pResourceLocation_type _pDestination);
    /// @}

    /// @name I_ProtocolService implementation
    /// @{
public:
    virtual I_ApplicationServer& getApplicationServer();
    virtual pEndpoint_type resolveEndpoint(const std::string& _address, const std::string& _port);
    virtual void sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint);
    virtual void setMessageRegistry(pMessageRegistry_type _pMessageRegistry);
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

    /// @name XMLProtocolService implementation
    /// @{
public:
    void handleAccept(const boost::system::error_code& _error);

    void createConnection();
    void asyncAccept();

    void destroyEndpoint(wpEndpoint_type _pEndpoint);

    /// Get the location for this URL
    /// @see I_XMLProtocolService::registerURL()
    pResourceLocation_type getLocation(const std::string& _url);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class Zen::Enterprise::AppServer::ProtocolServiceFactory;

    explicit XMLProtocolService(I_ApplicationServer& _server);
    virtual ~XMLProtocolService();
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

    /// Acceptor used to listen for incoming connections
    boost::asio::ip::tcp::acceptor      m_acceptor;

    /// Address on which to bind
    std::string                         m_address;

    /// Port to which to bind
    std::string                         m_port;

    /// Request handler
    XML::RequestHandler*               m_pRequestHandler;

    /// Number of threads
    int                                 m_threadCount;

    /// This is a shared pointer instead of a managed pointer
    /// because boost::bind<> handles shared_ptr correctly
    boost::shared_ptr<XML::Connection> m_pNewConnection;

    /// Collection of threads
    Threads_type                        m_threads;

    boost::asio::ip::tcp::endpoint      m_endpoint;

    /// All of the locations being handled by this protocol adapter.
    /// The first entry in the pair is the string used to partially
    /// match the URL.  The second entry is the location used by
    /// the application server to dispatch the request to the correct
    /// application service.  The longest match will be used.
    ///
    /// Examples:
    ///     /       will match / and /xyz
    ///     /xyz    will match /xyz
    ///
    /// If there are more than one matches, the longest match will be used.
    /// In the above example /xyz will actually end up matching /xyz even
    /// though / also matches.
    Locations_type                      m_locations;

    /// Guard for m_locations
    Threading::I_Mutex*                 m_pLocationsGuard;
    /// @}

};  // class XMLProtocolService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace XML
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_XML_PROTOCOL_SERVICE_HPP_INCLUDED
