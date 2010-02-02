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
//  Matthew Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_DATABASE_I_DATABASE_TRANSACTION_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_TRANSACTION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_DatabaseQuery;
class I_DatabaseStaticQuery;
class I_DatabaseDynamicQuery;
class I_DatabaseCommand;
class I_DatabaseResult;

class DATABASE_DLL_LINK I_DatabaseTransaction
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseTransaction>         pDatabaseTransaction_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseTransaction>    wpDatabaseTransaction_type;
    typedef Zen::Event::Event<wpDatabaseTransaction_type>           databaseTransactionEvent_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseQuery>               pQuery_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseStaticQuery>         pStaticQuery_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseStaticQuery>    wpStaticQuery_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseDynamicQuery>        pDynamicQuery_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseDynamicQuery>   wpDynamicQuery_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseCommand>             pDatabaseCommand_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseCommand>        wpDatabaseCommand_type;

    class I_QueryResultHandler;
    typedef Memory::managed_ptr<I_QueryResultHandler>               pQueryResultHandler_type;

    class I_CommandResultHandler;
    typedef Memory::managed_ptr<I_CommandResultHandler>             pCommandResultHandler_type;

    typedef boost::uint64_t                                         Key_type;
    typedef Event::future_return_value<Key_type>                    FutureKey_type;
    typedef Memory::managed_ptr<FutureKey_type>                     pFutureKey_type;
    /// @}

    /// @name I_DatabaseTransaction interface.
    /// @{
public:
    /// @brief Create a static query.
    ///
    /// A static query is a query that has no substitution parameters.
    virtual pStaticQuery_type createStaticQuery() = 0;

    /// @todo Implement.
    /// Dynamic queries are currently not supported, but eventually
    /// they will be used to create queries with parameters used for
    /// substituion within the query.
    virtual pDynamicQuery_type createDynamicQuery() = 0;

    /// @brief Execute a query.
    ///
    /// Dynamic queries must be prepared before they can be executed.
    /// Static queries can be executed without needing to be prepared.
    /// This method is normally used when there is a result set, such as
    /// in a select.
    virtual void executeQuery(pQuery_type _pQuery, pQueryResultHandler_type _pQueryResultHandler) = 0;

    /// Execute a query that has no results.
    /// This is generally used for updates but it can be used for inserts 
    /// and other queries that don't return results.
    virtual void executeQuery(pQuery_type _pQuery) = 0;

    /// Execute a query that is an insert statement.
    /// This will return a future value that is the auto-increment 
    /// primary key value.
    virtual pFutureKey_type executeInsert(pQuery_type _pQuery) = 0;

    /// @brief Create a command.
    ///
    /// Commands are generally things such as "EXEC" for executing a stored
    /// procedure.  They can but don't necessarily return a result set.
    virtual pDatabaseCommand_type createCommand() = 0;

    /// @brief Execute a command.
    ///
    /// The I_CommandResultHandler will be invoked when the
    /// result is ready (at least partially) for reading.  If the command
    /// returns more than one result set, the result handler will be invoked
    /// more than one time.  When no more results are left, the result handler
    /// will go out of scope, potentially destroying it.
    virtual void executeCommand(pDatabaseCommand_type _pCommand, pCommandResultHandler_type _pCommandResultHandler) = 0;

    /// @return true if this transaction has been committed or rolled back.
    virtual bool isFinalized() const = 0;

    /// @return true if this transaction has not been committed and has not been rolled back.
    virtual bool isActive() const = 0;

    /// Commit this database transaction
    virtual void commit() = 0;

    /// Roll back this database transaction.
    virtual void rollback() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    databaseTransactionEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseTransaction();
    virtual ~I_DatabaseTransaction();
    /// @}

    /// @name Handlers
    /// @{
public:
    class DATABASE_DLL_LINK I_QueryResultHandler
    {
        /// @name Types
        /// @{
    public:
        typedef Zen::Memory::managed_ptr<I_DatabaseResult>      pDatabaseResult_type;
        typedef Zen::Memory::managed_ptr<I_DatabaseTransaction> pDatabaseTransaction_type;
        /// @}

        /// @name I_DatabaseTransaction::QueryResultHandler interface
        /// @{
    public:
        virtual void handleResult(pDatabaseResult_type _pResult) = 0;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 I_QueryResultHandler();
        virtual ~I_QueryResultHandler();
        /// @}

    };  // interface I_QueryResultHandler

    class DATABASE_DLL_LINK I_CommandResultHandler
    {
        /// @name Types
        /// @{
    public:
        typedef Zen::Memory::managed_ptr<I_DatabaseCommand> pDatabaseCommand_type;
        /// @}

        /// @name I_DatabaseTransaction::CommandResultHandler interface
        /// @{
    public:
        virtual void handleCommand(pDatabaseCommand_type _pCommand) = 0;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 I_CommandResultHandler();
        virtual ~I_CommandResultHandler();
        /// @}

    };  // interface I_CommandResultHandler
    /// @}

};  // interface I_DatabaseTransaction

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory 
{
    /// I_DatabaseTransaction is managed by a factory
    template<>
    struct is_managed_by_factory<Database::I_DatabaseTransaction> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_TRANSACTION_HPP_INCLUDED
