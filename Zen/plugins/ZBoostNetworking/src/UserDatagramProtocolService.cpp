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

#include "UserDatagramProtocolService.hpp"
#include "UDP/Message.hpp"
#include "UDP/MessageBuffer.hpp"
#include "UDP/Address.hpp"

#include "UDP/Endpoint.hpp"

#include "UDP/SendTask.hpp"
#include "UDP/SendTaskAllocator.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/ThreadFactory.hpp>

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

#include <boost/lexical_cast.hpp>

#include <sstream>
#include <iostream>

#include <stdlib.h>
#include <string.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace UDP {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::UserDatagramProtocolService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   m_appServer(_appServer)
,   m_ioService()
,   m_pWork(NULL)
,   m_address()
,   m_port()
,   m_threadCount(2)
,   m_pSocket(NULL)
,   m_sendTaskAllocator()
,   m_sendTaskPool(m_sendTaskAllocator)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::~UserDatagramProtocolService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    m_address = _config.getAttribute("address");
    m_port = _config.getAttribute("port");

    // TODO Need to support thread count as an optional parameter
    //m_threadCount = boost::lexical_cast<long,std::string>(_config.getAttribute("threads"));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::pEndpoint_type
UserDatagramProtocolService::resolveEndpoint(const std::string& _address,
                                             const std::string& _port)
{
    boost::asio::ip::udp::resolver resolver(m_ioService);
    boost::asio::ip::udp::resolver::query query(_address,_port);
    boost::asio::ip::udp::endpoint endpoint = *resolver.resolve(query);

    boost::asio::ip::address address = endpoint.address();
    pAddress_type pAddress = createAddress(address);
    return pEndpoint_type(new Endpoint(getSelfReference(), endpoint, pAddress),
        boost::bind(&UserDatagramProtocolService::destroyEndpoint,this,_1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::destroyEndpoint(wpEndpoint_type& _pEndpoint)
{
    // Delete the Endpoint
    Endpoint* pEndpoint = dynamic_cast<Endpoint*>(_pEndpoint.get());

    if( pEndpoint != NULL )
    {
        delete pEndpoint;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::Enterprise::AppServer::UDP::UserDatagramProtocolService::destroyEndpoint() : _pEndpoint is not a valid UDP::Endpoint.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
UserDatagramProtocolService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    m_pThreadPool = &_threadPool;

    // Resolve the address
    boost::asio::ip::udp::resolver resolver(m_ioService);
    boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), m_port);
    boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::udp::endpoint endpoint = *resolver.resolve(query);

    m_pSocket = new boost::asio::ip::udp::socket(m_ioService, endpoint);
    //m_pSocket = new boost::asio::ip::udp::socket(m_ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));

    // Ready to go, so don't bother returning a condition variable
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::start()
{
    receiveFrom();

    class Runnable
    :   public Zen::Threading::I_Runnable
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
                        std::cout << "Error: " << ec << std::endl;
                    }
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
        :   m_ioService(_ioService)
        ,   m_stopping(false)
        {
        }

        boost::asio::io_service& m_ioService;
        volatile bool m_stopping;
    };

    m_pWork = new boost::asio::io_service::work(m_ioService);

    // Start some threads that will execute m_ioService.run()
    for(int x = 0; x < m_threadCount; ++x)
    {
        Zen::Threading::I_Thread* pThread =
            Zen::Threading::ThreadFactory::create(new Runnable(m_ioService));
        m_threads.push_back(pThread);
        pThread->start();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
UserDatagramProtocolService::prepareToStop()
{
    delete m_pWork;

    m_sendTaskPool.stop();

    for(threadCollection_type::iterator iter = m_threads.begin();
        iter != m_threads.end(); iter++ )
    {
        (*iter)->stop();
    }

    m_ioService.stop();

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::stop()
{
    for(threadCollection_type::iterator iter = m_threads.begin();
        iter != m_threads.end(); iter++ )
    {
        (*iter)->join();
        Zen::Threading::ThreadFactory::destroy(*iter);
    }

    m_threads.clear();

    m_pThreadPool = NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_ApplicationServer&
UserDatagramProtocolService::getApplicationServer()
{
    return m_appServer;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::receiveFrom()
{
    // TODO Pool MessageBuffer?
    boost::shared_ptr<MessageBuffer> pBuffer(new MessageBuffer(*this));
    m_pSocket->async_receive_from
    (
    boost::asio::buffer(pBuffer->getData(), MessageBuffer::MAX_LENGTH),
        pBuffer->getEndpoint(),
        boost::bind
        (
            &MessageBuffer::handleReceiveFrom, pBuffer,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::sendTo(pMessage_type _pMessage,
                                    pEndpoint_type _pEndpoint)
{
    // Can't use async send since we need a buffer.
    // TODO Use UDP::MessageBuffer?
    // See http://www.indiezen.org/wiki/irclogs/2009/03/06/#UTC2009-03-06T16:26:56

    SendTask* pTask = dynamic_cast<SendTask*>(&m_sendTaskPool.take());
    pTask->initialize(this, _pMessage, _pEndpoint);

    m_pThreadPool->pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::handleSendTo(pMessage_type _pMessage,
                                          pEndpoint_type _pEndpoint)
{
    std::stringstream buffer;

    boost::archive::polymorphic_binary_oarchive archive(buffer,
                                                        boost::archive::no_header |
                                                        boost::archive::no_tracking);

    // Handle the header
    // TODO Handle the version correctly
    _pMessage->getMessageHeader()->serialize(archive, 0);

    // Serialize the rest of the message
    // TODO Handle the version correctly
    _pMessage->serialize(archive, 0);

    const int bodyLength = buffer.str().length();

    if (bodyLength <= MessageBuffer::MAX_BODY_LENGTH)
    {
        MessageBuffer msg(*this);
        msg.setBodyLength(bodyLength);
        msg.encodeHeader();

        memcpy(msg.getBody(), buffer.str().c_str(), bodyLength);

        m_pSocket->send_to(boost::asio::buffer(msg.getData(), bodyLength + MessageBuffer::HEADER_LENGTH),
                           dynamic_cast<Endpoint*>(_pEndpoint.get())->getEndpoint());
    }
    else
    {
        std::cout << "Error: Message too large while sending" << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::handleReceiveFrom(boost::shared_ptr<MessageBuffer> _pMessageBuffer,
                                               const boost::system::error_code& _error,
                                               size_t _bytesReceived)
{
    if( !_error && _bytesReceived > 0 && _pMessageBuffer->decodeHeader())
    {
        std::stringbuf buffer;
        buffer.sputn(_pMessageBuffer->getBody(), _pMessageBuffer->getBodyLength());
        buffer.pubseekpos(0);

        std::stringstream stream;
        stream.str(buffer.str());

        /// @todo Make sure _bytesReceived isn't too terribly big,
        /// but since this is UDP, does it matter?

        boost::archive::polymorphic_binary_iarchive archive(stream,
                                                            boost::archive::no_header |
                                                            boost::archive::no_tracking);

        pMessageHeader_type pMessageHeader = m_appServer.getMessageRegistry()->getMessageHeader(archive);
        boost::asio::ip::address address = _pMessageBuffer->getEndpoint().address();
        pMessage_type pMessage = pMessageHeader->getMessageType()->getMessageFactory()
            ->create(
                pMessageHeader,
                pEndpoint_type(
                    new Endpoint(
                        getSelfReference(),
                        _pMessageBuffer->getEndpoint(),
                        createAddress(address)
                    ),
                    boost::bind(&UserDatagramProtocolService::destroyEndpoint, this, _1)
                ),
                pEndpoint_type()
            );

        // TODO Handle version correctly
        pMessage->serialize(pMessageHeader, archive, 0);

        getApplicationServer().handleMessage(pMessage);

        // Loop back around and receive some more data
        receiveFrom();
    }
    else
    {
        if (_error)
        {
            std::cout << "Error: " << boost::system::system_error(_error).what() << std::endl;
        }
        else
        {
            std::cout << "Error: Message too large while receiving" << std::endl;
        }
        receiveFrom();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::pResourceLocation_type
UserDatagramProtocolService::getLocation(const std::string& _locationName)
{
    return Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation(_locationName);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
UserDatagramProtocolService::pAddress_type
UserDatagramProtocolService::createAddress(boost::asio::ip::address &_address)
{
    Address* pRawAddress = new Address(_address);

    pAddress_type pAddress(pRawAddress,
        boost::bind(&UserDatagramProtocolService::destroyAddress,this,_1));

    return pAddress;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
UserDatagramProtocolService::destroyAddress(wpAddress_type& _pAddress)
{
    Address* pAddress = dynamic_cast<Address*>(_pAddress.get());

    if( pAddress != NULL )
    {
        delete pAddress;
    }
    else
    {
        throw Zen::Utility::runtime_exception("UserDatagramProtocolService::destroyAddress() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace UDP
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
