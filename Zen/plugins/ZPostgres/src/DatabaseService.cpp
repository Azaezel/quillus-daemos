//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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

#include "DatabaseService.hpp"
#include "DatabaseConnection.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <boost/bind.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseService::DatabaseService(Configuration_type _config)
{
    m_pConnectionsGuard = Threading::MutexFactory::create();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseService::~DatabaseService()
{
    Threading::MutexFactory::destroy(m_pConnectionsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseService::connect(const std::string &_name, Configuration_type& _config, bool _asynchronous)
{
    Threading::CriticalSection lock(m_pConnectionsGuard);

    connections_types::iterator iter = m_namedConnections.find(_name);
    if (iter != m_namedConnections.end())
    {
        pDatabaseConnection_type pConn(iter->second);

        onConnectedEvent(pConn);
        return;
    }

    DatabaseConnection* pRawPointer = new DatabaseConnection(getSelfReference().lock(), _name, _config);

    pDatabaseConnection_type pConn(pRawPointer, boost::bind(&DatabaseService::onDestroy, this, _1));

    wpDatabaseConnection_type pWeakConn(pConn);
    m_namedConnections[_name] = pWeakConn;

    pRawPointer->setSelfReference(pWeakConn);

    onConnectedEvent(pConn);
    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseService::onDestroy(wpDatabaseConnection_type _pConnection)
{
    Threading::CriticalSection lock(m_pConnectionsGuard);

    connections_types::iterator iter = m_namedConnections.find(_pConnection->getName());
    if (iter != m_namedConnections.end())
    {
        // This is safe since the only place that this can transition
        // from expired to non-expired is in connect() where m_pConnectionsGuard
        // is acquired.
        if (iter->second.expired())
        {
            m_namedConnections.erase(iter);

            DatabaseConnection* pConn = dynamic_cast<DatabaseConnection*>(_pConnection.get());

            pConn->onDestroyEvent(_pConnection);
            delete pConn;
        }
        else
        {
            // Not expired, so we are here for no reason
            // We can get here during race conditions where one shared_ptr
            // is being deleted while connect() is being called and connect()
            // beat onDestroy() to the lock(m_pConnectionsGuard).
            return;
        }
    }
    else
    {
        // TODO Severe error, should never get here
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
