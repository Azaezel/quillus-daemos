//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "DatabaseConnection.hpp"
#include "DatabaseTransaction.hpp"

#include <boost/bind.hpp>

#include <sstream>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::DatabaseConnection(pDatabase_type _pDatabase, const std::string& _name, config_type& _config)
:   m_pDatabase(_pDatabase)
,   m_name(_name)
{
    // TODO clean up parsing of config and check for errors
    /// Get Hostname
    std::string host;
    {
        config_type::const_iterator iter = _config.find("host");
        if( iter != _config.end() )
        {
            host = iter->second;
        }
        else
        {
            host = "localhost";
        }
    }

    /// Get user
    std::string user;
    {
        config_type::const_iterator iter = _config.find("user");
        if( iter != _config.end() )
        {
            user = iter->second;
        }
        else
        {
            user = "root";
        }
    }

    /// Get password
    std::string passwd;
    {
        config_type::const_iterator iter = _config.find("passwd");
        if( iter != _config.end() )
        {
            passwd = iter->second;
        }
        else
        {
            passwd = "";
        }
    }

    /// Get database name
    std::string db;
    {
        config_type::const_iterator iter = _config.find("db");
        if( iter != _config.end() )
        {
            db = iter->second;
        }
        else
        {
            db = "mysql";
        }
    }

    /// Get database port
    unsigned int port;
    {
        config_type::const_iterator iter = _config.find("port");
        if( iter != _config.end() )
        {
            port = atoi( iter->second.c_str() );
        }
        else
        {
            port = 3306;
        }
    }

    //std::string socket( _config.find("socket")->second );

    // TODO create a readable table of clientFlags and update this statement accordingly
    //unsigned long clientFlag( atoi(_config.find("clientFlag")->second.c_str()) );

    // Create a MYSQL instance
    MYSQL* pMYSQL = mysql_init(NULL);
    if( pMYSQL == NULL )
    {
        std::string msg("Zen::Database::DatabaseConnection::DatabaseConnection() : Could not create MYSQL instance -- ");
        msg = msg + mysql_error(pMYSQL);
        throw std::exception(msg.c_str());
    }

    // Create a MYSQL Connection
    MYSQL* pRawConnection = mysql_real_connect(pMYSQL,
                                               host.c_str(),
                                               user.c_str(),
                                               passwd.c_str(),
                                               db.c_str(),
                                               port,
                                               //socket.c_str(),
                                               //clientFlag);
                                               NULL,
                                               0);

    if( pRawConnection == NULL )
    {
        std::string msg("Zen::Database::DatabaseConnection::DatabaseConnection() : Could not create MYSQL instance -- ");
        msg = msg + mysql_error(pMYSQL);
        mysql_close(pMYSQL);
        throw std::exception(msg.c_str());
    }

    if( mysql_autocommit(pRawConnection, false) )
    {
        std::string msg("Zen::Database::DatabaseConnection::DatabaseConnection() : Error occurred disabling autocommit -- ");
        msg = msg + mysql_error(pMYSQL);
        mysql_close(pMYSQL);
        throw std::exception(msg.c_str());
    }

    m_pConnection = pMYSQLConnection_type(pRawConnection,
        boost::bind(&DatabaseConnection::onDestroyConnection,this,_1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::~DatabaseConnection()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
DatabaseConnection::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::pDatabaseTransaction_type
DatabaseConnection::beginTransaction()
{
    if( m_pTransaction.isValid() )
    {
        DatabaseTransaction* pRawDatabaseTransaction =
            m_pTransaction.as<Zen::Memory::managed_ptr<DatabaseTransaction> >().get();
        
        if( pRawDatabaseTransaction->isActive() )
        {
            pRawDatabaseTransaction->rollback();
        }

        pRawDatabaseTransaction->reset();
    }
    else
    {
        DatabaseTransaction* pRawDatabaseTransaction = 
            new DatabaseTransaction(this->m_pConnection);

        pDatabaseTransaction_type pTransaction(pRawDatabaseTransaction,
            boost::bind(&DatabaseConnection::onDestroyTransaction,this,_1));

        this->m_pTransaction = pTransaction;
    }

    return this->m_pTransaction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::commitTransaction(pDatabaseTransaction_type _pDatabaseTransaction)
{
    if( (_pDatabaseTransaction == this->m_pTransaction) && _pDatabaseTransaction->isActive() )
    {
        /// Commit the transaction
        _pDatabaseTransaction.as<Zen::Memory::managed_ptr<DatabaseTransaction> >()->commit();
    }
    else
    {
        throw std::exception("Zen::ZMySQL::DatabaseConnection::commitTransaction() : Attempting to commit an inactive transaction.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::rollbackTransaction(pDatabaseTransaction_type _pDatabaseTransaction)
{
    if( (_pDatabaseTransaction == this->m_pTransaction) && _pDatabaseTransaction->isActive() )
    {
        /// Rollback the transaction
        _pDatabaseTransaction.as<Zen::Memory::managed_ptr<DatabaseTransaction> >()->rollback();
    }
    else
    {
        throw std::exception("Zen::ZMySQL::DatabaseConnection::rollbackTransaction() : Attempting to rollback an inactive transaction.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::onDestroyConnection(wpMYSQLConnection_type _pMYSQLConnection)
{
    /// Delete the MYSQL Connection
    mysql_close(_pMYSQLConnection.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::onDestroyTransaction(wpDatabaseTransaction_type _wpDatabaseTransaction)
{
    /// Fire the DatabaseTransaction's onDestroyEvent
    _wpDatabaseTransaction->onDestroyEvent(_wpDatabaseTransaction);

    /// Delete the DatabaseTransaction
    DatabaseTransaction* pDatabaseTransaction = 
        dynamic_cast<DatabaseTransaction*>(_wpDatabaseTransaction.get());

    if( pDatabaseTransaction )
    {
        delete pDatabaseTransaction;
    }
    else
    {
        throw std::exception("Zen::Database::DatabaseConnection::onDestroyTransaction() : _wpDatabaseTransaction is not a valid DatabaseTransaction.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
