//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#include "BasicNetworkService.hpp"
#include "BasicNetworkAddress.hpp"
#include "BasicNetworkConnection.hpp"

// TR - This eventually should be part of the Makefile / configuration and not 
// defined here
#ifdef _WIN32 
#ifndef HOST_WIN32
#define HOST_WIN32
#endif
#else
#ifndef HOST_POSIX
#define HOST_POSIX
#endif
#endif

#include <IndieZen/framework/Networking/I_NetworkConnectionListener.hpp>
#include <IndieZen/framework/Networking/I_NetworkReadReadyListener.hpp>
#include <IndieZen/framework/Networking/I_NetworkWriteReadyListener.hpp>
#include <IndieZen/framework/Networking/Exceptions.hpp>

#include <IndieZen/framework/Threading/ThreadFactory.hpp>
#include <IndieZen/framework/Threading/ConditionFactory.hpp>
#include <IndieZen/framework/Threading/MutexFactory.hpp>

#include <IndieZen/framework/Threading/I_Runnable.hpp>
#include <IndieZen/framework/Threading/CriticalSection.hpp>

// Include the necessary headers based on the host platform
#ifdef HOST_WIN32
#include <winsock2.h>
#endif

#ifdef HOST_POSIX
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>
#define INVALID_SOCKET 0
#define SOCKET_ERROR  -1
#endif


// POSIX and WIN32 have slightly different implementations for select
// I've optimized as much as I can for both platforms and the following
// macros are used to pick which implementation to use.

#ifdef HOST_WIN32
  #define SELECT_RETURNS_SUBSET     /// This is more efficient for Win32 (invalid for POSIX)
//#define SELECT_RETURNS_FULL       /// Use this to test the POSIX style implementation
#endif

#ifdef HOST_POSIX
//#define SELECT_RETURNS_SUBSET     /// DO NOT USE.  Only SELECT_RETURNS_FULL works on POSIX
  #define SELECT_RETURNS_FULL
#endif

#ifdef SELECT_RETURNS_SUBSET
#ifdef SELECT_RETURNS_FULL
#error Do not define both SELECT_RETURNS_FULL and SELECT_RETURNS_SUBSET
#endif
#endif

#ifndef SELECT_RETURNS_SUBSET
#ifndef SELECT_RETURNS_FULL
#error Define one of SELECT_RETURNS_FULL and SELECT_RETURNS_SUBSET
#endif
#endif

#include <stddef.h>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace Networking {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BasicNetworkService_ReadRunnable
:	public Threading::I_Runnable
{
public:
	BasicNetworkService_ReadRunnable(BasicNetworkService* _pRunMe) { m_pRunMe = _pRunMe; }

	virtual void run() throw() { try { m_pRunMe->runReadThread(); } catch (...) { /* Error! */ } }
	virtual void stop() {}
private:
	BasicNetworkService*	m_pRunMe;

};	// class BasicNetworkService_ReadRunnable

class BasicNetworkService_WriteRunnable
:	public IndieZen::Threading::I_Runnable
{
public:
	BasicNetworkService_WriteRunnable(BasicNetworkService* _pRunMe) { m_pRunMe = _pRunMe; }

	virtual void run() throw() { try { m_pRunMe->runWriteThread(); } catch (...) { /* Error! */ } }
	virtual void stop() {}
private:
	BasicNetworkService*	m_pRunMe;

};	// class BasicNetworkService_ReadRunnable

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BasicNetworkService::BasicNetworkService(unsigned short _loopbackPort)
:	m_shuttingDown(false)
,	m_pReadStarted(Threading::ConditionFactory::create())
,	m_pWriteStarted(Threading::ConditionFactory::create())
,	m_readThreadStarted(false)
,   m_pReadThreadStartGuard(Threading::MutexFactory::create())
,	m_writeThreadStarted(false)
,   m_pWriteThreadStartGuard(Threading::MutexFactory::create())
,   m_wakeupClientSocket(INVALID_SOCKET)
,   m_loopbackPort(_loopbackPort)
,   m_pReadListener(NULL)
,   m_pWriteListener(NULL)
,   m_pConnectionsGuard(Threading::MutexFactory::create())
{
	// Create the read thread but don't start it until it's needed
	m_pReadThread = Threading::ThreadFactory::create(new BasicNetworkService_ReadRunnable(this));

	m_pWriteThread = Threading::ThreadFactory::create(new BasicNetworkService_WriteRunnable(this));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BasicNetworkService::~BasicNetworkService()
{
    // TODO Don't forget to clean up everything that was created in the constructor
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_NetworkConnection*
BasicNetworkService::connect(const I_NetworkAddress &_address)
{
	const BasicNetworkAddress* const pAddress = dynamic_cast<const BasicNetworkAddress*>(&_address);

	if (pAddress == NULL)
	{
		// TODO: Throw exception?  _address isn't valid for this service
		return NULL;
	}

	// Create the socket.  What if this address is UDP and not TCP?
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Connect, but check for errors
	if (::connect(clientSocket, pAddress->getSocketAddress(), pAddress->getSize()) == SOCKET_ERROR)
	{
		// TODO Determine the error; throw an exception?
		return NULL;
	}

	connection_ptr_type pNewConnection = new BasicNetworkConnection(this, clientSocket);

	// Make sure the read thread is started
	if (!m_readThreadStarted)
	{
		Threading::CriticalSection lock(m_pReadThreadStartGuard);

		// Double check to make sure the read thread wasn't started
		if (!m_readThreadStarted)
		{
			m_pReadThread->start();
			m_readThreadStarted = true;
		}
	}

	return pNewConnection;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::listen(const I_NetworkAddress& _address)
{
	const BasicNetworkAddress* const pAddress = dynamic_cast<const BasicNetworkAddress*>(&_address);

	if (pAddress == NULL)
	{
		// TODO: Throw exception?  _address isn't valid for this service
		return;
	}

	// Create the listening socket
	SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Bind the listening socket
	if (bind(listeningSocket, pAddress->getSocketAddress(), pAddress->getSize()) == SOCKET_ERROR)
	{
#ifdef HOST_WIN32
        int nErr = WSAGetLastError();
        if (nErr == WSAECONNRESET)
        {
        }
        else if (nErr == WSAEADDRINUSE)
        {
            std::cout << "Error, address " << pAddress << " is already in use." << std::endl;
        }
#endif
		throw std::exception("BasicNetworkingService::listen() Error binding to listeningSocket");
	}

    // Listen to the listening socket
	if (::listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
    {
		throw std::exception("BasicNetworkingService::listen() Error listening to listeningSocket");
    }

	m_waitingListeners.push_back(listeningSocket);

	wakeup();

	// TODO Should this logic be moved into the Thread::start() implementation?
	if (!m_readThreadStarted)
	{
		Threading::CriticalSection lock(m_pReadThreadStartGuard);
		if (!m_readThreadStarted)
		{
			m_pReadThread->start();
			m_readThreadStarted = true;
		}
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::registerConnectionListener(I_NetworkConnectionListener& _listener)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::unregisterConnectionListener(I_NetworkConnectionListener& _listener)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::setReadReadyListener(I_NetworkReadReadyListener* const _pListener)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::setWriteReadyListener(I_NetworkWriteReadyListener* const _pListener)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::requestWrite(I_NetworkConnection* const _pConnection)
{
	m_waitingWriters.push_back(_pConnection);
	wakeup();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static unsigned char sm_tmpBuffer = 0;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::wakeup()
{
    send(m_wakeupClientSocket, (const char*)&sm_tmpBuffer, 1, 0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::runReadThread()
{
    BasicNetworkAddress address(m_loopbackPort);

    fd_set read_set;
    fd_set write_set;
    fd_set except_set;

    int nFDs = FD_SETSIZE;

    // Initialize the sets
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&except_set);

    int nRC;

    // Timeout after 1 second.  Mostly the timeout is just to 
    // wake up from time to time on an unbusy system to 
    // check m_ShuttingDown 
    timeval timeout;

    connection_map_type connections;

    // Set of readers
    std::list<SOCKET> readers;

    // Set of listeners
    std::set<SOCKET> listeners;

    //----------------------------------------------------------
    // Main Loop
    //
    while(true)
    {
    	timeout.tv_sec = 1;
    	timeout.tv_usec = 0;

		nRC = ::select(nFDs, &read_set, &write_set, &except_set, &timeout);

        // Break if shutting down
        if (m_shuttingDown)
            break;

        // Timeout
        if (nRC <= -1)
        {
            // Just continue on errors unless they happen a lot
#ifdef HOST_WIN32
            int nErr = WSAGetLastError();
            if (nErr == WSAECONNRESET)
            {
                std::cout << "connection reset in select" << std::endl;
            }
#endif
            // DEBUG:
            std::cout << "In timeout." << std::endl;

            std::list<SOCKET> removeConnections;

            // HACK: Iterate through the connections and reset anything that's no longer connected
            for(connection_map_type::iterator iter = connections.begin(); iter != connections.end(); ++iter)
            {
                if (iter->second != NULL && iter->second->isConnected() == false)
                {
                    for(connection_listeners_type::iterator ppConnectionListener = m_connectionListeners.begin();
                        ppConnectionListener != m_connectionListeners.end(); ppConnectionListener++)
                    {
                        (*ppConnectionListener)->onClosedConnection(*(iter->second));
                    }

                    delete iter->second;
                    iter->second = NULL;
                    removeConnections.push_back(iter->first);
                }
            }

            if (!removeConnections.empty())
            {
                Threading::CriticalSection lock(m_pConnectionsGuard);

                for(std::list<SOCKET>::iterator iter = removeConnections.begin(); iter != removeConnections.end(); ++iter)
                {
                    m_connections.erase(*iter);
                }
            }

            for(std::list<SOCKET>::iterator iter = removeConnections.begin(); iter != removeConnections.end(); ++iter)
            {
                connections.erase(*iter);
            }

        }
        else if (nRC == 0)
        {
            // For now just continue

            // DEBUG:
            //std::cout << "In Read Timeout / nRC == 0." << std::endl;
        }
        else
        {
            // Iterate through the readers
            // WIN32 - Iterate through the returned FD set.
            // POSIX - Iterate through the readers and check to see if it's set
#ifdef SELECT_RETURNS_FULL
            for(std::list<SOCKET>::iterator iter = readers.begin(); iter != readers.end(); iter++)            
#elif defined SELECT_RETURNS_SUBSET
            for(SOCKET* iter = read_set.fd_array; iter < read_set.fd_array + read_set.fd_count; iter++)
#else
#error Either SELECT_RETURNS_FULL or SELECT_RETURNS_SUBSET needs to be defined
#endif
            {
#ifdef SELECT_RETURNS_FULL
                if (FD_ISSET(*iter, &read_set))
#endif
                {
                    // First, check for the listening sockets
                    std::set<SOCKET>::iterator listeningSocketPtr = listeners.find(*iter);
                    if (listeningSocketPtr != listeners.end())
                    {
                        SOCKET listeningSocket = *listeningSocketPtr;

                        SOCKET newSocket = accept(listeningSocket, NULL, NULL);

                        if (newSocket != SOCKET_ERROR)
                        {
                            readers.push_back(newSocket);
                            connection_ptr_type pNewConnection = new BasicNetworkConnection(this, newSocket);

                            {
                                Threading::CriticalSection lock(m_pConnectionsGuard);
                                m_connections[newSocket] = pNewConnection;
                            }

                            connections[newSocket] = pNewConnection;

                            // HACK!
                            // TODO: Use a condition variable or something?
                            //while(m_pListener == NULL)
                            //{
                            //    #ifdef HOST_WIN32
                            //        Sleep(100);
                            //    #else
                            //        usleep(100);
                            //    #endif
                            //}

                            for(connection_listeners_type::iterator ppConnectionListener = m_connectionListeners.begin();
                                ppConnectionListener != m_connectionListeners.end(); ppConnectionListener++)
                            {
                                (*ppConnectionListener)->onNewConnection(*pNewConnection);
                            }                            
                        }
                    }
                    else
                    {
                        // If it's not either of the two special cases, handle the default
                        // case.  Get the Connection object and make sure it's valid (i.e.
                        // still connected.)  The NULL / disconnected connections should
                        // be lazily deleted.

                        connection_ptr_type pReadConnection = connections[*iter];

                        if (pReadConnection)
                        {
                            // Sockets might be disconnected
                            try
                            {
                                m_pReadListener->onReadReady(*pReadConnection);
                            }
                            catch(disconnected_exception)
                            {
                                {
                                    Threading::CriticalSection lock(m_pConnectionsGuard);
                                    m_connections.erase(*iter);
                                }

                                connections.erase(*iter);

                                for(connection_listeners_type::iterator ppConnectionListener = m_connectionListeners.begin();
                                    ppConnectionListener != m_connectionListeners.end(); ppConnectionListener++)
                                {
                                    (*ppConnectionListener)->onClosedConnection(*pReadConnection);
                                }

                                // Delete the connection.
                                // TODO: Ooops, what if someone still has this?
                                // Reference count?  Delete it later?
                                delete pReadConnection;
                            }
                            catch(...)
                            {
                                // TODO: Client Application threw an exception...
                                // Log this?  Or what do I do?
                                // Possibly make the OnReadReady with a 
                                // signature of throw();
                            }
                        }
                        else
                        {
                            // Read
                            char buffer[4096];
                            recv(*iter, buffer, sizeof(buffer), 0);
                        }
                    }
                }
            }
        }

        // Recreate the list.
        FD_ZERO(&read_set);

        /// Handle anything that's requested a new connection
        waiting_connections_queue_type::collection_ptr_type pConnecters = m_waitingConnections.pop_front_all();
        for(waiting_connections_queue_type::collection_type::iterator iter = pConnecters->begin(); iter != pConnecters->end(); iter++)
        {
            int enable = 1;
            ::setsockopt(iter->first, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable));

            readers.push_back(iter->first);

            {
                Threading::CriticalSection lock(m_pConnectionsGuard);

                m_connections[iter->first] = iter->second;
            }

            connections[iter->first] = iter->second;

            for(connection_listeners_type::iterator ppConnectionListener = m_connectionListeners.begin();
                ppConnectionListener != m_connectionListeners.end(); ppConnectionListener++)
            {
                (*ppConnectionListener)->onNewConnection(*(iter->second));
            }
        }

        /// Handle new listeners
        waiting_listeners_queue_type::collection_ptr_type pListeners = m_waitingListeners.pop_front_all();
        for(waiting_listeners_queue_type::collection_type::iterator iter = pListeners->begin(); iter != pListeners->end(); iter++)
        {
            // A listener is in the listeners set and also in the readers list.
            listeners.insert(*iter);
            readers.push_back(*iter);
        }

        // Put the readers on the list
        // Right now that's every connection that has an I_Connection object
        // TODO: Should be everything that wasn't read this iteration, except then I'd
        // have to implement a way to wake up this thread to add the reader to the list
        // after it's been read.  For now it's ok because OnReadReady, which is called for all
        // sockets that have data ready) reads the socket.  If that changes (the reading is
        // done in another thread) then putting everything on the reader list could cause a
        // fairly tight loop to occur.
        for(std::list<SOCKET>::iterator iter = readers.begin(); iter != readers.end(); iter++)
        {
            connection_ptr_type pConnection = connections[*iter];

            if (pConnection)
            {
                FD_SET(*iter, &read_set);
            }
            else
            {
                // Listeners don't have connection pointers, so use something else
                // to determine if it's a valid reader.  For now, just assume
                // all listeners are valid readers
                std::set<SOCKET>::iterator listenerPtr = listeners.find(*iter);
                if (listenerPtr != listeners.end())
                {
                    FD_SET(*listenerPtr, &read_set);
                }
            }
        }
    }

    std::cout << "Shutting down TCP Service" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::runWriteThread()
{
    BasicNetworkAddress address(m_loopbackPort);
    SOCKET wakeupSocket = INVALID_SOCKET;

#ifdef HOST_POSIX
    SOCKET socket_vector[2];

    socketpair(AF_INET, SOCK_STREAM, IPPROTO_TCP, socket_vector);
    wakeupSocket = socket_vector[0];
    m_wakeupClientSocket = socket_vector[1];

    int enable = 1;
    setsockopt(wakeupSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable));
    setsockopt(m_wakeupClientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable));

#else
    SOCKET wakeupListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Bind to the address (any address, plus the specified port)
	if (::bind(wakeupListeningSocket, address.getSocketAddress(), address.getSize()) == SOCKET_ERROR)
    {
        // TODO: Handle errors better
#ifdef HOST_WIN32
        int nErr = WSAGetLastError();
        if (nErr == WSAECONNRESET)
        {
        }
        else
#endif
        throw network_exception("Error binding to wakeupListeningSocket in TCPClient_select::Run");
    }
	if (::listen(wakeupListeningSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        throw network_exception("Error listening to wakeupListeningSocket in TCPClient_select::Run");
    }
#endif

    fd_set read_set;
    fd_set write_set;
    fd_set except_set;

    int nFDs = FD_SETSIZE;

    // Initialize the sets
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&except_set);

    int nRC;

    // Timeout after 1 second.  Mostly the timeout is just to 
    // wake up from time to time on an unbusy system to 
    // check m_ShuttingDown 
    timeval timeout;

    std::list<SOCKET> readers;

    // Insert the wakeup socket
#ifdef HOST_POSIX
    FD_SET(wakeupSocket, &read_set);
#else
    FD_SET(wakeupListeningSocket, &read_set);

    // List of all of the sockets created with Connect, Listen and from the accept()
    readers.push_back(wakeupListeningSocket);
#endif

    // Set of waiting writers
    std::set<SOCKET> writers;

    // Set of listeners
    std::set<SOCKET> listeners;

    //----------------------------------------------------------
    // Main Loop
    //
    while(true)
    {
    	timeout.tv_sec = 1;
    	timeout.tv_usec = 0;

        nRC = select(nFDs, &read_set, &write_set, &except_set, &timeout);

        // Break if shutting down
        if (m_shuttingDown)
            break;

        // Timeout
        if (nRC <= -1)
        {
            // Just continue on errors unless they happen a lot
#ifdef HOST_WIN32
            int nErr = ::WSAGetLastError();
            if (nErr == WSAECONNRESET)
            {
                std::cout << "connection reset in select" << std::endl;
            }
#endif
            // DEBUG:
            std::cout << "In timeout." << std::endl;
        }
        else if (nRC == 0)
        {
            // For now just continue

            // DEBUG:
            //std::cout << "In timeout / nRC == 0." << std::endl;
        }
        else
        {
            // Iterate through the readers
            // WIN32 - Iterate through the returned FD set.
            // POSIX - Iterate through the readers and check to see if it's set
#ifdef SELECT_RETURNS_FULL
            for(std::list<SOCKET>::iterator iter = readers.begin(); iter != readers.end(); iter++)            
#elif defined SELECT_RETURNS_SUBSET
            for(SOCKET* iter = read_set.fd_array; iter < read_set.fd_array + read_set.fd_count; iter++)
#else
#error Either SELECT_RETURNS_FULL or SELECT_RETURNS_SUBSET needs to be defined
#endif
            {
#ifdef SELECT_RETURNS_FULL
                if (FD_ISSET(*iter, &read_set))
#endif
                {
                    #ifndef HOST_POSIX
                    // POSIX doesn't require this section of code since a socketpair is used instead.
                    if (*iter == wakeupListeningSocket)
                    {
                        // If it's the wakeupListiningSocket, accept it and create
                        // the wakeupSocket.
						wakeupSocket = ::accept(wakeupListeningSocket, NULL, NULL);
                        readers.push_back(wakeupSocket);
                    }
                    else
                    #endif // ! HOST_POSIX
                    if (*iter == wakeupSocket)
                    {
                        //std::cout << "Wakeup!" << std::endl;
                        // If it's the wakeup socket, just read a single byte
                        // Wakeup() sends a single byte
                        char buffer[256];
                        int bytesRead = recv(*iter, buffer, sizeof(buffer), 0);
                        if (bytesRead <= 0)
                        {
                            // TODO: Error, probably unrecoverable
                            std::cout << "Error reading from wakeup socket" << std::endl;
                        }
                        //std::cout << "End of Wakeup" << std::endl;
                    }
                    else
                    {
                        std::cout << "Error, RunWrite got an unexpected socket." << std::endl;
                    }
                }
            }

            // Iterate through the writers
            // WIN32 - Iterate through the returned FD set.
            // POSIX - Iterate through the readers and check to see if it's set
            std::list<SOCKET> deleteWriters;    // Used for deferred deletion of writers items
#ifdef SELECT_RETURNS_FULL
            for(std::set<SOCKET>::iterator iter = writers.begin(); iter != writers.end(); iter++)            
#elif defined SELECT_RETURNS_SUBSET
            for(SOCKET* iter = write_set.fd_array; iter < write_set.fd_array + write_set.fd_count; iter++)
#else
#error Either SELECT_RETURNS_FULL or SELECT_RETURNS_SUBSET needs to be defined
#endif
            {
#ifdef SELECT_RETURNS_FULL
                if (FD_ISSET(*iter, &write_set))
#endif
                {
                    connection_ptr_type pWriteConnection;

                    // TODO: Should we keep a local cache of this?
                    {
                        Threading::CriticalSection lock(m_pConnectionsGuard);
                        pWriteConnection = m_connections[*iter];
                    }

                    if (pWriteConnection)
                    {
                        // It's possible that the Client Application 
                        // might throw an exception.  
                        try
                        {
                            if (!m_pWriteListener->onWriteReady(*pWriteConnection))
                            {
                                deleteWriters.push_back(*iter);
                            }
                        }
                        catch(disconnected_exception&)
                        {
                            deleteWriters.push_back(*iter);

                            // TODO: What about deleting the connection?
                            // For now, just let the reading thread do the rest of the work
                        }
                        catch(...)
                        {
                            deleteWriters.push_back(*iter);

                            // TODO: Client Application threw an exception...
                            // Log this?  Or what do I do?
                            // Possibly make the onWriteReady with a 
                            // signature of throw(); (except that really doesn't work
                            // since we're expecting disconnected_exception sometimes)
                        }
                    }
                    else
                    {
                        // Write Ready on a NULL connection.  This should only happen once when
                        // a connection has disconnected when it was pending a write.
                    }
                }
            }

            // Now clean up the writers
            for(std::list<SOCKET>::iterator iter = deleteWriters.begin(); iter != deleteWriters.end(); iter++)
            {
                writers.erase(*iter);
            }
            deleteWriters.clear();
        }

        // Recreate the list.
        FD_ZERO(&read_set);
        FD_ZERO(&write_set);

        // If the wakeupSocket isn't connected yet, add the wakeupListeningSocket to the reader list
        // otherwise add the wakeupSocket
#ifdef HOST_POSIX
        FD_SET(wakeupSocket, &read_set);
#else
        if (wakeupSocket == INVALID_SOCKET)
        {
            FD_SET(wakeupListeningSocket, &read_set);
        }
        else
        {
            FD_SET(wakeupSocket, &read_set);
        }
#endif // HOST_POSIX

        // Add any new waiting writers to the writers list.
        waiting_writers_queue_type::collection_ptr_type pWriters = m_waitingWriters.pop_front_all();
        for(waiting_writers_queue_type::collection_type::iterator iter = pWriters->begin(); iter != pWriters->end(); iter++)
        {
            BasicNetworkConnection* pConnection = dynamic_cast<BasicNetworkConnection*>(*iter);

            // TODO: Check to make sure this socket still has a valid connection pointer before inserting it.
            writers.insert(pConnection->getSocket());
        }

        // Add all of the writers to the list
        for(std::set<SOCKET>::iterator iter = writers.begin();  iter != writers.end(); iter++)
        {
            FD_SET(*iter, &write_set);
        }
    }

    std::cout << "Shutting down TCP Service" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicNetworkService::initWakeupSocket()
{
    // TODO: On POSIX use a socketpair
#ifndef HOST_POSIX
    m_wakeupClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    BasicNetworkAddress address(INADDR_LOOPBACK, m_loopbackPort);

    /// Enable TCP_NODELAY so write combining / Nagle isn't used.
    int enable = 1;
    ::setsockopt(m_wakeupClientSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable));

    // Connect to the server side of the wakeup socket
    int nRC = ::connect(m_wakeupClientSocket, address.getSocketAddress(), address.getSize());

    //std::cout << "InitClientSideWakeupSocket return code:" << nRC << std::endl;
#endif

}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Networking
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
