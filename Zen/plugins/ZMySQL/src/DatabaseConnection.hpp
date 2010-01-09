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

#include "MYSQLTypes.hpp"

#ifndef ZEN_ZMYSQL_DATABASE_CONNECTION_HPP_INCLUDED
#define ZEN_ZMYSQL_DATABASE_CONNECTION_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class DatabaseService;

class DatabaseConnection
: public Database::I_DatabaseConnection
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<MYSQL>                                  pMYSQLConnection_type;
    typedef Memory::managed_weak_ptr<MYSQL>                             wpMYSQLConnection_type;

    typedef Database::I_DatabaseService::config_type                    config_type;
    typedef Memory::managed_ptr<Database::I_DatabaseService>            pDatabase_type;
    typedef Memory::managed_weak_ptr<Database::I_DatabaseConnection>    self_reference_type;
    /// @}

    /// @name I_DatabaseService implementation
    /// @{
public:
    virtual const std::string& getName() const;
    virtual pDatabaseTransaction_type beginTransaction();
    virtual void commitTransaction(pDatabaseTransaction_type _pDatabaseTransaction);
    virtual void rollbackTransaction(pDatabaseTransaction_type _pDatabaseTransaction);
    /// @}

    /// @name DatabaseConnection implementation
    /// @{
public:
    /// Give the connection an opportunity to keep a weak self-reference
    void setSelfReference(self_reference_type _pReference);

    /// @name Event Handlers
    /// @{
    void onDestroyConnection(wpMYSQLConnection_type _pMYSQLConnection);
    void onDestroyTransaction(wpDatabaseTransaction_type _wpDatabaseTransaction);
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseConnection(pDatabase_type _pDatabase, const std::string& _name, config_type& _config);
    virtual ~DatabaseConnection();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Reference to the database for this connection.  This reference
    /// prevents the database object from being deleted while it has
    /// open connections.
    pDatabase_type              m_pDatabase;

    std::string                 m_name;

    pMYSQLConnection_type       m_pConnection;

    self_reference_type         m_pSelfReference;

    pDatabaseTransaction_type   m_pTransaction;
    /// @}

};  // class DatabaseConnection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZMYSQL_DATABASE_CONNECTION_HPP_INCLUDED
