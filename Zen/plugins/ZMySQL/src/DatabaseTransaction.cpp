//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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

#include "DatabaseTransaction.hpp"
#include "DatabaseStaticQuery.hpp"
#include "DatabaseDynamicQuery.hpp"
#include "DatabaseCommand.hpp"
#include "DatabaseResult.hpp"

#include <Zen/Enterprise/Database/I_DatabaseQuery.hpp>
#include <Zen/Enterprise/Database/I_DatabaseStaticQuery.hpp>
#include <Zen/Enterprise/Database/I_DatabaseDynamicQuery.hpp>
#include <Zen/Enterprise/Database/I_DatabaseCommand.hpp>

#include <boost/bind.hpp>
#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    DatabaseTransaction::DatabaseTransaction(DatabaseConnection::pMYSQLConnection_type _pConnection)
:   m_pConnection(_pConnection)
,   m_isCommitted(false)
,   m_isActive(true)
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::~DatabaseTransaction()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::pStaticQuery_type 
DatabaseTransaction::createStaticQuery()
{
    DatabaseStaticQuery* pRawStaticQuery = new DatabaseStaticQuery();

    pStaticQuery_type pStaticQuery(pRawStaticQuery,
        boost::bind(&DatabaseTransaction::onDestroyStaticQuery,this,_1));

    return pStaticQuery;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::pDynamicQuery_type 
DatabaseTransaction::createDynamicQuery()
{
    DatabaseDynamicQuery* pRawDynamicQuery = new DatabaseDynamicQuery();

    pDynamicQuery_type pDynamicQuery(pRawDynamicQuery,
        boost::bind(&DatabaseTransaction::onDestroyDynamicQuery,this,_1));

    return pDynamicQuery;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
DatabaseTransaction::executeQuery(pQuery_type _pQuery, pQueryResultHandler_type _pQueryResultHandler)
{
    // Cast the query to a DatabaseQuery object
    DatabaseQuery* pRawQuery = dynamic_cast<DatabaseQuery*>(_pQuery.get());

    if( pRawQuery )
    {
        if( mysql_real_query(this->m_pConnection.get(),
                             pRawQuery->getQuery().c_str(),
                             (unsigned long)(pRawQuery->getQuery().length())) )
        {
            std::stringstream msgStream;
            msgStream << "Zen::Database::DatabaseConnection::DatabaseConnection() : MYSQL Query error -- ";
            msgStream << mysql_error(this->m_pConnection.get());
            throw std::exception(msgStream.str().c_str());
        }

        MYSQL_RES* pRawMYSQLResult = mysql_use_result(this->m_pConnection.get());

        if( pRawMYSQLResult == NULL )
        {
            std::string error(mysql_error(this->m_pConnection.get()));
            if(error.length() > 0)
            {
                std::stringstream msgStream;
                msgStream << "Zen::ZMySQL::DatabaseTransaction::executeQuery() : MySQL Result Retrieval error -- ";
                msgStream << error;
                throw std::exception(msgStream.str().c_str());
            }
            else
            {
                return;
            }
        }

        pMYSQLResult_type pMYSQLResult(pRawMYSQLResult,
            boost::bind(&DatabaseTransaction::onDestroyMYSQLResult,this,_1));

        DatabaseResult* pRawDatabaseResult = new DatabaseResult(pMYSQLResult);

        pDatabaseResult_type pDatabaseResult(pRawDatabaseResult,
            boost::bind(&DatabaseTransaction::onDestroyDatabaseResult,this,_1));

        _pQueryResultHandler->handleResult(pDatabaseResult);
    }
    else
    {
        throw std::exception("Zen::ZMySQL::DatabaseTransaction::executeQuery() : _pQuery is not a valid DatabaseQuery.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseTransaction::pDatabaseCommand_type 
DatabaseTransaction::createCommand()
{
    DatabaseCommand* pRawDatabaseCommand = new DatabaseCommand();

    pDatabaseCommand_type pDatabaseCommand(pRawDatabaseCommand,
        boost::bind(&DatabaseTransaction::onDestroyDatabaseCommand,this,_1));

    return pDatabaseCommand;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
DatabaseTransaction::executeCommand(pDatabaseCommand_type _pCommand, pCommandResultHandler_type _pCommandResultHandler)
{
    // TODO Implement
    throw std::exception("Zen::ZMySQL::DatabaseTransaction::executeCommand() : Not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
DatabaseTransaction::commit()
{
    if( mysql_commit(this->m_pConnection.get()) )
    {
        throw std::exception("Zen::ZMySQL::DatabaseTransaction::commit() : Error committing transaction.");
    }
    this->m_isCommitted = true;
    this->m_isActive = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
DatabaseTransaction::rollback()
{
    if( mysql_rollback(this->m_pConnection.get()) )
    {
        throw std::exception("Zen::ZMySQL::DatabaseTransaction::rollback() : Error rolling back transaction.");
    }
    this->m_isCommitted = false;
    this->m_isActive = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
DatabaseTransaction::onDestroyStaticQuery(wpStaticQuery_type _wpStaticQuery)
{
    /// Fire the DatabaseStaticQuery's onDestroyEvent
    _wpStaticQuery->onDestroyEvent(_wpStaticQuery);

    /// Delete the DatabaseStaticQuery
    DatabaseStaticQuery* pStaticQuery = 
        dynamic_cast<DatabaseStaticQuery*>(_wpStaticQuery.get());

    if( pStaticQuery )
    {
        delete pStaticQuery;
    }
    else
    {
        throw std::exception("Zen::Database::DatabaseTransaction::onDestroyStaticQuery() : _wpStaticQuery is not a valid DatabaseStaticQuery.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::reset()
{
    if( this->m_isActive = true )
    {
        this->rollback();
    }

    this->m_isActive = true;
    this->m_isCommitted = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DatabaseTransaction::isFinalized() const
{
    return !this->m_isActive;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DatabaseTransaction::isActive() const
{
    return this->m_isActive;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
DatabaseTransaction::onDestroyDynamicQuery(wpDynamicQuery_type _wpDynamicQuery)
{
    /// Fire the DatabaseDynamicQuery's onDestroyEvent
    _wpDynamicQuery->onDestroyEvent(_wpDynamicQuery);

    /// Delete the DatabaseDynamicQuery
    DatabaseDynamicQuery* pDynamicQuery = 
        dynamic_cast<DatabaseDynamicQuery*>(_wpDynamicQuery.get());

    if( pDynamicQuery )
    {
        delete pDynamicQuery;
    }
    else
    {
        throw std::exception("Zen::Database::DatabaseTransaction::onDestroyDynamicQuery() : _wpDynamicQuery is not a valid DatabaseDynamicQuery.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
DatabaseTransaction::onDestroyDatabaseCommand(wpDatabaseCommand_type _wpDatabaseCommand)
{
    /// Fire the DatabaseCommand's onDestroyEvent
    _wpDatabaseCommand->onDestroyEvent(_wpDatabaseCommand);

    /// Delete the DatabaseCommand
    DatabaseCommand* pDatabaseCommand =
        dynamic_cast<DatabaseCommand*>(_wpDatabaseCommand.get());

    if( pDatabaseCommand )
    {
        delete pDatabaseCommand;
    }
    else
    {
        throw std::exception("Zen::Database::DatabaseTransaction::onDestroyDatabaseCommand() : _wpDatabaseCommand is not a valid DatabaseCommand.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::onDestroyMYSQLResult(wpMYSQLResult_type _wpMYSQLResult)
{
    mysql_free_result(_wpMYSQLResult.get());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseTransaction::onDestroyDatabaseResult(wpDatabaseResult_type _wpDatabaseResult)
{
    /// Fire the DatabaseResult's onDestroyEvent
    _wpDatabaseResult->onDestroyEvent(_wpDatabaseResult);

    /// Delete the DatabaseResult
    DatabaseResult* pDatabaseResult =
        dynamic_cast<DatabaseResult*>(_wpDatabaseResult.get());

    if( pDatabaseResult )
    {
        delete pDatabaseResult;
    }
    else
    {
        throw std::exception("Zen::Database::DatabaseTransaction::onDestroyDatabaseResult() : _wpDatabaseResult is not a valid DatabaseResult.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
