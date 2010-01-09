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
//  Matthew Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_DATABASE_I_DATABASE_CONNECTION_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_CONNECTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/Database/I_DatabaseTransaction.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Plugins/I_Service.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_MutableDatabaseSchema;
class I_ImmutableDatabaseSchema;

class DATABASE_DLL_LINK I_DatabaseConnection
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseTransaction>         pDatabaseTransaction_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseTransaction>    wpDatabaseTransaction_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseConnection>          pDatabaseConnection_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseConnection>     wpDatabaseConnection_type;
    typedef Zen::Event::Event<wpDatabaseConnection_type>            ConnectionEvent_type;
    /// @}

    /// @name I_DatabaseConnection interface.
    /// @{
public:
    /// Get the name of this connection.
    /// All database connections are named.  This is useful for maintaining and
    /// distinguishing multiple database connections.
    /// It's recommended that you colorize database connections if at all possible.
    /// For more information on colorizing 
    virtual const std::string& getName() const = 0;

    /// Begin a database transaction.
    /// Most database implementations do not support more than one transaction at a 
    /// time per connection.  Leaky abstraction:  Because of this limitation, 
    /// most plugins will throw an exception if you attempt to create a new transaction
    /// before the previous one goes out of scope.  Take care to write your code to
    /// accomodate this limitation.
    /// @return Pointer to an instance of I_DatabaseTransaction
    /// @throws Utility::runtime_exception if beginning a transaction and a transaction
    ///     has already started and nested transactions are not supported by the underlying
    ///     database system.
    virtual pDatabaseTransaction_type beginTransaction() = 0;

    /// Commit a database transaction
    /// @param _pDatabaseTransaction Pointer to an instance of I_DatabaseTransaction
    /// @deprecated Use the commit() method on the I_DatabaseTransaction.
    virtual void commitTransaction(pDatabaseTransaction_type _pDatabaseTransaction) = 0;

    /// Rollback a database transaction
    /// @param _pDatabaseTransaction Pointer to an instance of I_DatabaseTransaction
    /// @deprecated Use the rollback() method on the I_DatabaseTransaction
    virtual void rollbackTransaction(pDatabaseTransaction_type _pDatabaseTransaction) = 0;

    /// Escape a string so as to avoid SQL injection.
    virtual std::string escapeString(const std::string& _string) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// Fired immediately before this object is destroyed.
    /// The payload is about to be destroyed, so do not keep a reference of it around.
    ConnectionEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseConnection();
    virtual ~I_DatabaseConnection();
    /// @}

};  // interface I_DatabaseConnection

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory {
    /// I_DatabaseService is managed by a factory
    template<>
    struct is_managed_by_factory<Database::I_DatabaseConnection> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_CONNECTION_HPP_INCLUDED
