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
#include "HyperTextTransportProtocolService.hpp"

#include "Endpoint.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/ThreadFactory.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Enterprise/AppServer/I_Message.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_MessageRegistry.hpp>

#include <boost/bind.hpp>

#include <iostream>

#include <stdlib.h>
#include <string.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HyperTextTransportProtocolService::HyperTextTransportProtocolService(I_ApplicationServer& _server)
:   m_appServer(_server)
,   m_ioService()
,   m_pWork(NULL)
,   m_acceptor(m_ioService)
,   m_pRequestHandler(NULL)
,   m_pLocationsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HyperTextTransportProtocolService::~HyperTextTransportProtocolService()
{
    Threading::MutexFactory::destroy(m_pLocationsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::registerURL(const std::string& _url, pResourceLocation_type _pDestination)
{
    Threading::CriticalSection lock(m_pLocationsGuard);

    m_locations.push_back(std::make_pair<std::string, pResourceLocation_type>(_url, _pDestination));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ApplicationServer&
HyperTextTransportProtocolService::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::setConfiguration(const Plugins::I_ConfigurationElement& _config)
{
    m_address = _config.getAttribute("address");
    m_port = _config.getAttribute("port");
    m_threadCount = strtol(_config.getAttribute("threads").c_str(), NULL, 10);

    class ConfigVisitor
    : public Zen::Plugins::I_ConfigurationElement::I_ConfigurationElementVisitor
    {
    public:
        virtual void begin()
        {
        }

        virtual void visit(const Zen::Plugins::I_ConfigurationElement& _element)
        {
            ;
            ;

            m_protocolService.registerURL(
                _element.getAttribute("url"),
                I_ApplicationServerManager::getSingleton().createLocation(_element.getAttribute("destination"))
            );
        }

        virtual void end()
        {
        }

        ConfigVisitor(HyperTextTransportProtocolService& _protocolService)
        :   m_protocolService(_protocolService)
        {
        }

    private:
        HyperTextTransportProtocolService&  m_protocolService;
    };

    ConfigVisitor visitor(*this);
    _config.getChildren("map", visitor);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HyperTextTransportProtocolService::pEndpoint_type
HyperTextTransportProtocolService::resolveEndpoint(const std::string& _address, const std::string& _port)
{
    boost::asio::ip::tcp::resolver resolver(m_ioService);
    // TODO _address and _port or v4() and m_port?
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), m_port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

    return pEndpoint_type(new Endpoint(getSelfReference(), endpoint), boost::bind(&HyperTextTransportProtocolService::destroyEndpoint, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::sendTo(pMessage_type _pMessage, pEndpoint_type _pEndpoint)
{
    // This is intentionally not implemented as you cannot use sendTo for HTTP.
    throw Utility::runtime_exception("HyperTextTransportProtocolService::sendTo(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::destroyEndpoint(wpEndpoint_type _pEndpoint)
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
HyperTextTransportProtocolService::prepareToStart(Threading::ThreadPool& _threadPool)
{
    // Resolve the address
    boost::asio::ip::tcp::resolver resolver(m_ioService);
    boost::asio::ip::tcp::resolver::query query(m_address, m_port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

    // Bind to the address
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(endpoint);

    // Create a new connection
    createConnection();

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::start()
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
                    m_ioService.run();
                }
                catch(...)
                {
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

    // Start some threads that will execute m_ioService.run()
    for(int x = 0; x < m_threadCount; x++)
    {
        Zen::Threading::I_Thread* pThread = Zen::Threading::ThreadFactory::create(new Runnable(m_ioService));
        m_threads.push_back(pThread);
        pThread->start();
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Threading::I_Condition*
HyperTextTransportProtocolService::prepareToStop()
{
    delete m_pWork;

    for(Threads_type::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
    {
        (*iter)->stop();
    }

    m_ioService.stop();

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::stop()
{
    for(Threads_type::iterator iter = m_threads.begin(); iter != m_threads.end(); iter++)
    {
        (*iter)->join();
        Threading::ThreadFactory::destroy(*iter);
    }

    m_threads.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::handleAccept(const boost::system::error_code& _error)
{
    if (!_error)
    {
        // The new connection is now connected, so start it.
        pEndpoint_type pEndpoint(new Endpoint(getSelfReference(), m_endpoint), boost::bind(&HyperTextTransportProtocolService::destroyEndpoint, this, _1));

        m_pNewConnection->start(pEndpoint);

        // Now, create another connection and do an async accept on it.
        createConnection();

        // And asynchronously accept the new connection.
        asyncAccept();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::createConnection()
{
    boost::shared_ptr<HTTP::Connection> pConnection(new HTTP::Connection(m_ioService, *this));
    m_pNewConnection.swap(pConnection);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
HyperTextTransportProtocolService::asyncAccept()
{
    // TODO Add endpoint here?

    // Start the async accept using handleAccept() as the callback
    m_acceptor.async_accept(m_pNewConnection->getSocketReference(),
        m_endpoint,
        boost::bind(&HyperTextTransportProtocolService::handleAccept, this,
        boost::asio::placeholders::error)
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
HyperTextTransportProtocolService::pResourceLocation_type
HyperTextTransportProtocolService::getLocation(const std::string& _url)
{
    Threading::CriticalSection lock(m_pLocationsGuard);
    unsigned maximumMatched = 0;

    pResourceLocation_type returnValue;

    for(Locations_type::iterator iter = m_locations.begin(); iter != m_locations.end(); iter++)
    {
        const std::string& url = iter->first;
        const int matchLen = std::min(url.length(), _url.length());
        if(strncmp(url.c_str(), _url.c_str(), matchLen) == 0)
        {
            if (matchLen > maximumMatched)
            {
                returnValue = iter->second;
                maximumMatched = matchLen;
            }
        }
    }

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
