//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
// Copyright (C)        2009 Joshua Cassity
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
//  Joshua Cassity jcassity@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_APPSERVER_APPLICATION_SERVER_HPP_INCLUDED
#define ZEN_APPSERVER_APPLICATION_SERVER_HPP_INCLUDED

#include "../I_ApplicationServer.hpp"

#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Threading/ThreadPool.hpp>

#include <Zen/Core/Plugins/I_Configuration.hpp>

#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Database {
        class I_DatabaseService;
        class I_DatabaseConnection;
    }   // namespace Database
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ApplicationServer
:   public I_ApplicationServer
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, pProtocolService_type>                ProtocolServices_type;
    typedef std::map<pResourceLocation_type, pApplicationService_type>  ApplicationServices_type;
    /// @}

    /// @name Enums 
    /// @{
public:
    enum ServerStates
    {
        INITIALIZED,
        STARTING,
        STARTED,
        STOPPING,
        STOPPED
    };
    /// @}

    /// @name I_ApplicationServer implementation
    /// @{
public:
    virtual Zen::Threading::I_Condition* start();
    virtual void stop();
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptEngine_type getDefaultScriptEngine();
    virtual void installProtocols(pConfig_type _pProtocolsConfig);
    virtual void installProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName);
    virtual pProtocolService_type getProtocol(const std::string& _protocolName);
    virtual void installApplications(pConfig_type _pAppServicesConfig);
    virtual void installApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation);
    virtual void configureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig);
    virtual pApplicationService_type getApplication(pResourceLocation_type _pServiceLocation) const;
    virtual void getResourceLocations(I_ResourceLocationVisitor& _visitor) const;
    virtual pMessageRegistry_type getMessageRegistry();
    virtual void handleMessage(pMessage_type _pMessage);
    virtual void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    virtual void handleSessionEvent(pSessionEvent_type _pSessionEvent);
    virtual pDatabaseConnection_type getDatabaseConnection(const std::string& _database, Zen::Threading::I_Thread::ThreadId& _threadId);
    /// @}

    /// @name ApplicationServer implementation
    /// @{
public:
    /// Install a protocol service.
    ///
    /// This is called by an InstallProtocolServiceTask that is queued into m_installQueue
    ///
    /// @note Internal use only
    void handleInstallProtocol(pProtocolService_type _pProtocolService, const std::string& _protocolName);

    /// Install an application service.
    ///
    /// This is called by an InstallApplicationServiceTask that is queued into m_installQueue
    ///
    /// @note Internal use only
    void handleInstallApplication(pApplicationService_type _pApplicationService, pResourceLocation_type _pRootLocation);

    void handleConfigureApplication(pApplicationService_type _pApplicationService, pConfig_type _pConfig);

    Threading::ThreadPool& getSharedThreadPool() { return m_sharedThreadPool; }

    typedef std::map<std::string,std::string> config_type;
    void createDatabaseEntry(const std::string& _database, config_type& _config);
    /// @}

    /// @name Inner classes
    /// @{
private:
    class DatabaseConnections
    {
        /// @name Types
        /// @{
    public:
        typedef Zen::Memory::managed_ptr<Zen::Database::I_DatabaseService>              pDatabaseService_type;
        typedef std::map<std::string,std::string>                                       config_type;
        typedef Zen::Memory::managed_ptr<Zen::Database::I_DatabaseConnection>           pDatabaseConnection_type;
        typedef std::map<Zen::Threading::I_Thread::ThreadId,pDatabaseConnection_type>   DatabaseConnections_type;
        /// @}

        /// @name DatabaseConnections implementation
        /// @{
    public:
        pDatabaseConnection_type getConnection(Zen::Threading::I_Thread::ThreadId& _threadId);
        /// @}

        /// @name 'Structors
        /// @{
                 DatabaseConnections(pDatabaseService_type _pService, config_type _connectionConfig);
                ~DatabaseConnections();
        /// @}

        /// @name Member Variables
        /// @{
    private:
        pDatabaseService_type           m_pDatabaseService;
        config_type                     m_connectionConfig;
        DatabaseConnections_type        m_databaseConnections;
        Zen::Threading::I_Mutex*        m_databaseConnectionsMutex;
        /// @}

    };  // class DatabaseConnections
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             ApplicationServer();
    virtual ~ApplicationServer();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ServerStates                m_currentState;

    /// Default script engine.
    pScriptEngine_type          m_pScriptEngine;

    /// ThreadPool that's shared among all of the services and protocols.
    Threading::ThreadPool       m_sharedThreadPool;

    /// Queue of items that require installation
    /// plus the thread that is used to perform
    /// the installation.
    Threading::ThreadPool       m_installQueue;

    /// Queue of items that require shutdown
    /// plus the thread that is used to perform
    /// the shutdown.
    Threading::ThreadPool       m_shutdownQueue;

    /// Guard for m_protocolServices consistency
    Threading::I_Mutex*         m_pProtocolGuard;

    /// Collection of installed protocol services
    ProtocolServices_type       m_protocolServices;

    /// Guard for the m_applicatinoServices consistency
    Threading::I_Mutex*         m_pApplicationGuard;

    /// Collection of installed application services
    ApplicationServices_type    m_applicationServices;

    pMessageRegistry_type       m_pMessageRegistry_type;

    typedef Zen::Memory::managed_ptr<DatabaseConnections>       pDatabaseConnections_type;
    typedef std::map<std::string, pDatabaseConnections_type>    DatabaseConnectionsMap_type;
    DatabaseConnectionsMap_type m_databaseConnectionsMap;
    /// @}

};  // interface I_ApplicationServer

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_APPLICATION_SERVER_HPP_INCLUDED
