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
#ifndef INDIEZEN_BASIC_NETWORKING_BASIC_NETWORK_SERVICE_HPP_INCLUDED
#define INDIEZEN_BASIC_NETWORKING_BASIC_NETWORK_SERVICE_HPP_INCLUDED

#include "BasicNetworkAddress.hpp"
#include "BasicNetworkConnection.hpp"

#include <IndieZen/framework/Networking/I_NetworkService.hpp>

#include <IndieZen/framework/Threading/I_Condition.hpp>
#include <IndieZen/framework/Threading/I_Thread.hpp>
#include <IndieZen/framework/Threading/I_Mutex.hpp>
#include <IndieZen/framework/Threading/ThreadSafeQueue.hpp>

#include <list>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace Networking {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Basic network service interface
class BasicNetworkService
:   public I_NetworkService
{
    /// @name Types
    /// @{
public:
    typedef BasicNetworkConnection*											connection_ptr_type;
    typedef std::map<SOCKET, connection_ptr_type>							connection_map_type;
    typedef I_NetworkConnectionListener*									connection_listener_ptr_type;
    typedef std::set<connection_listener_ptr_type>							connection_listeners_type;
    typedef std::map<SOCKET, connection_ptr_type>                           connection_map_type;
    typedef std::pair<SOCKET, connection_ptr_type>                          connection_pair_type;

    typedef Threading::ThreadSafeQueue<std::list<I_NetworkConnection*> >	waiting_writers_queue_type;
    typedef Threading::ThreadSafeQueue<std::list<SOCKET> >					waiting_listeners_queue_type;
    typedef Threading::ThreadSafeQueue<std::list<connection_pair_type> >    waiting_connections_queue_type;

    /// @}

    /// @name I_NetworkService implementation
    /// @{
public:
    virtual I_NetworkConnection* connect(const I_NetworkAddress& _address);
    virtual void listen(const I_NetworkAddress& _address);

    virtual void registerConnectionListener(I_NetworkConnectionListener& _listener);
    virtual void unregisterConnectionListener(I_NetworkConnectionListener& _listener);

    virtual void setReadReadyListener(I_NetworkReadReadyListener* const _pListener);
    virtual void setWriteReadyListener(I_NetworkWriteReadyListener* const _pListener);

    virtual void requestWrite(I_NetworkConnection* const _pConnection);
    /// @}

    /// @name Additional Implementation
    /// Although some of these are public, they should all be treated as protected.
    /// @{
protected:
    /// Wake up the writer thread
    void wakeup();

    /// Initialize the wakeup socket
    void initWakeupSocket();

public:
    /// Method for the read worker thread
    void runReadThread();

    /// Method for the write worker thread
    void runWriteThread();
    /// @}

    /// @name 'Structors
    /// @{
public:
             BasicNetworkService(unsigned short _loopbackPort);
    virtual ~BasicNetworkService();
    /// @}

    /// @name Member variables
    /// @{
private:
    volatile bool					m_shuttingDown;

    Threading::I_Condition*			m_pReadStarted;
    Threading::I_Condition*			m_pWriteStarted;

    Threading::I_Thread*			m_pReadThread;
    Threading::I_Thread*			m_pWriteThread;

    volatile bool					m_readThreadStarted;
    Threading::I_Mutex*				m_pReadThreadStartGuard;

    volatile bool					m_writeThreadStarted;
    Threading::I_Mutex*				m_pWriteThreadStartGuard;

    /// The wakeup client socket used to wake up the write thread
    /// in the case of shutting down or a write request.
    /// The POSIX implementation uses a socket pair, whereas
    /// the Win32 implementation uses two sockets.
    /// 
    /// The server side of this socket pair is maintained as a local
    /// variable in the write thread.
    SOCKET							m_wakeupClientSocket;

    /// Loopback Port
    /// This is the port used as the loopback port to wake up the
    /// write thread.
    unsigned short					m_loopbackPort;

    /// This queue is used to send write requests to the write thread.
    waiting_writers_queue_type      m_waitingWriters;

    /// This queue is used to send new connections to the read thread.
    waiting_connections_queue_type  m_waitingConnections;

    /// This queue is used to send new listening sockets to the read thread.
    /// When the read thread spins it will take this list and append it to
    /// the master list of listening sockets.  This implementation might be
    /// a bit of overkill since quite likely only one socket will be used 
    /// for listening... but hey, we're being flexible, right?
    waiting_listeners_queue_type	m_waitingListeners;

    /// Registered connection listeners.
    connection_listeners_type		m_connectionListeners;

    /// Listener for the "read ready" event.
    /// The "read ready" event is dispatched when a connection
    /// has data for reading.
    I_NetworkReadReadyListener*		m_pReadListener;

    /// Listener for the "write ready" event.
    /// The "write ready" event is dispatched when a connection
    /// has requested to write and the connection is ready
    /// for writing.
    I_NetworkWriteReadyListener*	m_pWriteListener;

    /// Map of SOCKET to Connections
    connection_map_type             m_connections;

    /// Guard for m_connections
    /// @todo Make this be a SpinLock
    IndieZen::Threading::I_Mutex*   m_pConnectionsGuard;
    /// @}

};  // interface BasicNetworkService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Networking
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_BASIC_NETWORKING_BASIC_NETWORK_SERVICE_HPP_INCLUDED
