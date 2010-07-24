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
#ifndef NOMINMAX
#define NOMINMAX
#endif
// This must be included first thanks to some Winblows crap.
#include <boost/asio.hpp>

#include "TransmissionControlProtocolService.hpp"

#include "Endpoint.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/ThreadFactory.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Core/Event/I_Event.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Enterprise/AppServer/I_Message.hpp>
#include <Zen/Enterprise/AppServer/I_MessageFactory.hpp>
#include <Zen/Enterprise/AppServer/I_MessageType.hpp>
#include <Zen/Enterprise/AppServer/I_MessageRegistry.hpp>
#include <Zen/Enterprise/AppServer/I_MessageHeader.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>

#include <boost/bind.hpp>

#include <boost/archive/polymorphic_binary_oarchive.hpp>
#include <boost/archive/polymorphic_binary_iarchive.hpp>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <iostream>

#include <stdlib.h>
#include <string.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TransmissionControlProtocolService::TransmissionControlProtocolService(I_ApplicationServer& _server)
:   m_appServer(_server)
,   m_ioService()
,   m_pWork(NULL)
,   m_acceptor(m_ioService)
,   m_address()
,   m_port()
,   m_threadCount(2)
,   m_threadsStarted(false)
,   m_pConnectionsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TransmissionControlProtocolService::~TransmissionControlProtocolService()
{
    Threading::MutexFactory::destroy(m_pConnectionsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ApplicationServer&
TransmissionControlProtocolService::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::setConfiguration(const Plugins::I_ConfigurationElement& _config)
{
    m_address = _config.getAttribute("address");
    m_port = _config.getAttribute("port");
    m_threadCount = strtol(_config.getAttribute("threads").c_str(), NULL, 10);

    if (m_address.empty() || m_port.empty())
    {
        m_isServer = false;
    }
    else
    {
        m_isServer = true;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TransmissionControlProtocolService::pEndpoint_type
TransmissionControlProtocolService::resolveEndpoint(const std::string& _address, const std::string& _port)
{
    boost::asio::ip::tcp::resolver resolver(m_ioService);
    // TODO _address and _port or v4() and m_port?
    boost::asio::ip::tcp::resolver::query query(_address, _port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

    pEndpoint_type pEndpoint(new Endpoint(getSelfReference(), endpoint), boost::bind(&TransmissionControlProtocolService::destroyEndpoint, this, _1));

    // Default to true.  Generally an endpoint is outbound when resolveEndpoint()
    // is called.  Since "listen()" is not a valid method (listen ports are 
    // determined by the configuration) then we probably aren't ever creating
    // a non-local endpoint.
    pEndpoint->setIsLocal(false);
    return pEndpoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint)
{
    // TODO Push onto a queue and handle in a worker thread?
    {
        typedef Memory::managed_ptr<Endpoint> pConcreteEndpoint_type;
        pConcreteEndpoint_type pEndpoint(_pEndpoint.as<pConcreteEndpoint_type>());

        pConnection_type pConnection;

        // Find or create the connection.
        {
            Threading::CriticalSection lock(m_pConnectionsGuard);

            // Find the connection associated with this endpoint.
            ConnectionMap_type::iterator iter = m_connectionMap.find(pEndpoint->getEndpoint());

            if (iter == m_connectionMap.end())
            {
                if (m_isServer)
                {
                    // TODO Error?
                    return;
                }

                pConnection = m_pNewConnection;

                createConnection();

                // Assume this is an outbound endpoint.
                pEndpoint->setIsLocal(false);

                m_connectionMap[pEndpoint->getEndpoint()] = pConnection;
                pConnection->connect(_pEndpoint);
            }
            else
            {
                pConnection = iter->second;
            }
        }

        // TODO Create a task to handle this asynchronously?
        std::stringstream buffer;

        boost::archive::polymorphic_binary_oarchive archive(buffer,
                                                            boost::archive::no_header |
                                                            boost::archive::no_tracking);

        // Serialize the header
        // TODO Handle the version correctly
        _pMessage->getMessageHeader()->serialize(archive, 0);

        // Serialize the rest of the message
        _pMessage->serialize(archive, 0);

        pConnection->write(buffer.str().c_str(), (boost::uint32_t)buffer.str().length());

    }

    // Make sure the threads have been started.
    startThreads();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
TransmissionControlProtocolService::getConnectedEvent()
{
    return getApplicationServer().getEventService()->getEvent("TransmissionControlProtocolService/connectedEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event&
TransmissionControlProtocolService::getDisconnectedEvent()
{
    return getApplicationServer().getEventService()->getEvent("TransmissionControlProtocolService/disconnectedEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::destroyEndpoint(wpEndpoint_type _pEndpoint)
{
    Endpoint* pEndpoint = dynamic_cast<Endpoint*>(_pEndpoint.get());
    if (pEndpoint != NULL)
    {
        delete pEndpoint;
    }
    else
    {
        // TODO Error
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Threading::I_Condition*
TransmissionControlProtocolService::prepareToStart(Threading::ThreadPool& _threadPool)
{
    if (m_isServer)
    {
        // Resolve the address
        boost::asio::ip::tcp::resolver resolver(m_ioService);
        boost::asio::ip::tcp::resolver::query query(m_address, m_port);
        boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

        // Bind to the address
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_acceptor.bind(endpoint);
    }

    // Create a new connection
    createConnection();

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::start()
{
    if (m_isServer)
    {
        // Start listening
        boost::system::error_code ec;
        m_acceptor.listen(boost::asio::socket_base::max_connections, ec);

        if (ec)
        {
            std::cout << "Error: " << ec << std::endl;
        }

        // Asyncronously accept a new connection
        asyncAccept();
    }
    
    startThreads();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Threading::I_Condition*
TransmissionControlProtocolService::prepareToStop()
{
    delete m_pWork;

    Threading::CriticalSection lock(m_pConnectionsGuard);

    for(Threads_type::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
    {
        (*iter)->stop();
    }

    m_ioService.stop();

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::stop()
{
    // Join all of the threads
    for(Threads_type::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
    {
        (*iter)->join();
        Threading::ThreadFactory::destroy(*iter);
    }

    // Don't lock until we're done joining, otherwise we may get
    // a deadlock.
    Threading::CriticalSection lock(m_pConnectionsGuard);

    m_threads.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
void
TransmissionControlProtocolService::startThreads()
{
    // Check to make sure the threads have not been started
    if (m_threadsStarted)
    {
        return;
    }

    // Not started, but need to double-check while inside of
    // a critical section
    Threading::CriticalSection lock(m_pConnectionsGuard);

    // Still not started?
    if (m_threadsStarted)
    {
        return;
    }

    // Start the threads

    class Runnable
    :   public Threading::I_Runnable
    {
    public:
        virtual void run() throw()
        {
            while(!m_stopping)
            {
                try
                {
                    boost::system::error_code ec;
                    m_ioService.run(ec);
                    
                    if( ec )
                    {
                        std::cout << "ASIO Error: " << ec << std::endl;
                    }
                }
                catch(std::exception& _ex)
                {
                    std::cout << "Exception in ASIO loop: " << _ex.what() << std::endl;
                }
                catch(...)
                {
                    std::cout << "Unknown exception in ASIO loop" << std::endl;
                }
            }
        }

        virtual void stop()
        {
            m_stopping = true;
        }

        Runnable(boost::asio::io_service& _ioService) 
            : m_ioService(_ioService), m_stopping(false) {}
        
        boost::asio::io_service& m_ioService;
        volatile bool m_stopping;
    };

    m_pWork = new boost::asio::io_service::work(m_ioService);

    // Reserve the correct amount of space.
    m_threads.reserve(m_threadCount);

    // Start some threads that will execute m_ioService.run()
    for(int x = 0; x < m_threadCount; x++)
    {
        Zen::Threading::I_Thread* pThread = Zen::Threading::ThreadFactory::create(new Runnable(m_ioService));
        m_threads.push_back(pThread);
        pThread->start();
    }

    m_threadsStarted = true;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::handleAccept(const boost::system::error_code& _error)
{
    if (!_error)
    {
        // The new connection is now connected, so start it.
        pEndpoint_type pEndpoint(new Endpoint(getSelfReference(), m_endpoint), boost::bind(&TransmissionControlProtocolService::destroyEndpoint, this, _1));

        // This endpoint is not local since it was established from an accept.
        pEndpoint->setIsLocal(false);

        m_pNewConnection->start(pEndpoint);

        // Now, create another connection and do an async accept on it.
        createConnection();

        // And asynchronously accept the new connection.
        asyncAccept();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::createConnection()
{
    boost::shared_ptr<TCP::Connection> pConnection(new TCP::Connection(m_ioService, *this));
    m_pNewConnection.swap(pConnection);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::asyncAccept()
{
    // TODO Add endpoint here?

    // Start the async accept using handleAccept() as the callback
    m_acceptor.async_accept(m_pNewConnection->getSocketReference(),
        m_endpoint,
        boost::bind(&TransmissionControlProtocolService::handleAccept, this,
        boost::asio::placeholders::error)
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::onConnected(pConnection_type _pConnection)
{
    Threading::CriticalSection lock(m_pConnectionsGuard);

    typedef Memory::managed_ptr<Endpoint>   pConcreteEndpoint_type;

    m_connectionMap[_pConnection->getPeer().as<pConcreteEndpoint_type>()->getEndpoint()] = _pConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::onDisconnected(pConnection_type _pConnection)
{
    Threading::CriticalSection lock(m_pConnectionsGuard);

    typedef Memory::managed_ptr<Endpoint>   pConcreteEndpoint_type;

    m_connectionMap.erase(_pConnection->getPeer().as<pConcreteEndpoint_type>()->getEndpoint());

    // Dispatch this event.
    getDisconnectedEvent().fireEvent(_pConnection->getPeer());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TransmissionControlProtocolService::onHandleMessage(pConnection_type  _pConnection, TCP::MessageBuffer& _message)
{
#if 1
    boost::iostreams::stream<boost::iostreams::array> 
        stream(_message.getBody(), _message.getBodyLength());

    boost::archive::polymorphic_binary_iarchive archive(stream, 
                                                        boost::archive::no_header |
                                                        boost::archive::no_tracking);


    
    // Deserialize the header
    I_Message::pMessageHeader_type pHeader = getApplicationServer().getMessageRegistry()->getMessageHeader(archive);

    // Construct the appropriate message
    // The way we're doing it now, we don't know which one of these to call
    // create() or createResponse().
    // If we put that detail in the header somehow, then we can either call
    // the correct method... *or* we can let the create() method figure it out.
    pMessage_type pMessage = pHeader->getMessageType()->getMessageFactory()
        ->create(
            pHeader, 
            _pConnection->getPeer(), 
            pEndpoint_type()
        );

    // Deserialize the message
    pMessage->serialize(pHeader, archive, 0);

    // Send the message to the application server
    getApplicationServer().handleMessage(pMessage);
#else
    std::stringbuf buffer(std::ios_base::in|std::ios_base::binary);

    buffer.pubsetbuf(_message.getBody(), _message.getBodyLength());

    std::stringstream stream;
    buffer.pubseekpos(0);
    stream.str(buffer.str());

    boost::archive::polymorphic_binary_iarchive archive(stream, 
                                                        boost::archive::no_header |
                                                        boost::archive::no_tracking);


    
    // Deserialize the header
    I_Message::pMessageHeader_type pHeader = getApplicationServer().getMessageRegistry()->getMessageHeader(archive);

    // Construct the appropriate message
    pMessage_type pMessage = pHeader->getMessageType()->getMessageFactory()
        ->create(
            pHeader, 
            _pConnection->getPeer(), 
            pEndpoint_type()
        );

    // Deserialize the message
    pMessage->serialize(pHeader, archive, 0);

    // Send the message to the application server
    getApplicationServer().handleMessage(pMessage);
#endif
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
