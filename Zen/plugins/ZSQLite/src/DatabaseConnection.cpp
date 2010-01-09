//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "DatabaseConnection.hpp"
#include "DatabaseTransaction.hpp"
#include "sqlite3.h"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>

#include <sstream>
#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZSQLite {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseConnection::DatabaseConnection(pDatabase_type _pDatabase, const std::string& _name, sqlite3* _pConnection)
:   m_pDatabase(_pDatabase)
,   m_name(_name)
,   m_pConnection(_pConnection)
,   m_pTransaction()
{
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
    if(!m_pTransaction.expired())
    {
        throw Utility::runtime_exception("Nested transactions are not supported.");
    }
    else
    {
        DatabaseTransaction* pRawDatabaseTransaction = 
            new DatabaseTransaction(this->m_pConnection);

        pDatabaseTransaction_type pTransaction(pRawDatabaseTransaction,
            boost::bind(&DatabaseConnection::onDestroyTransaction,this,_1));

        wpDatabaseTransaction_type pWeakTransaction(pTransaction);
        m_pTransaction = pWeakTransaction;

        return pTransaction;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::commitTransaction(pDatabaseTransaction_type _pDatabaseTransaction)
{
    _pDatabaseTransaction->commit();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseConnection::rollbackTransaction(pDatabaseTransaction_type _pDatabaseTransaction)
{
    _pDatabaseTransaction->rollback();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
void
DatabaseConnection::onDestroyConnection(wpSQLITEConnection_type _pSQLITEConnection)
{
    sqlite3_close(m_pConnection);
    m_pConnection = NULL;
}
#endif

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
        throw Utility::runtime_exception("Zen::Database::DatabaseConnection::onDestroyTransaction() : _wpDatabaseTransaction is not a valid DatabaseTransaction.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
DatabaseConnection::escapeString(const std::string& _string)
{
    std::string str(_string);

    // Escape the project name
    std::string searchString("\'"); 
    std::string replaceString("\'\'");

    std::string::size_type pos = 0;
    while ((pos = _string.find(searchString, pos)) != std::string::npos) 
    {
        str.replace(pos, searchString.size(), replaceString);
        pos++;
    }

    searchString = "\"\"";
    pos = 0;

    while ((pos = _string.find(searchString, pos)) != std::string::npos) 
    {
        str.replace(pos, searchString.size(), replaceString);
        pos++;
    }

    return str;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZSQLite
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
