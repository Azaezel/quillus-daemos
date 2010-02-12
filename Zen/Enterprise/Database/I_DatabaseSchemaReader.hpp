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
#ifndef ZEN_DATABASE_I_DATABASE_SCHEMA_READER_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_SCHEMA_READER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>
#include <boost/shared_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_DatabaseSchema;
class I_MutableDatabaseSchema;

class DATABASE_DLL_LINK I_DatabaseSchemaReader
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseSchema>        pDatabaseSchema_type;
    typedef Zen::Memory::managed_ptr<I_MutableDatabaseSchema> pMutableDatabaseSchema_type;
    typedef I_DatabaseConnection::pDatabaseTransaction_type     pDatabaseTransaction_type;
    /// @}

    /// @name I_DatabaseSchemaReader interface
    /// @{
public:

    /// Read a database schema from the schema source.
    /// @return Pointer to a database schema.
    virtual pDatabaseSchema_type read() = 0;


    /// Get a mutable database schema.
    /// @param _pDatabaseTransaction Pointer to an instance of I_DatabaseTransaction
    /// @return Pointer to an instance of I_MutableDatabaseSchema
    virtual pMutableDatabaseSchema_type getSchema(pDatabaseTransaction_type _pDatabaseSchema) = 0;

    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseSchemaReader();
    virtual ~I_DatabaseSchemaReader();
    /// @}

};  // interface I_DatabaseSchemaReader

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory {
    /// I_DatabaseSchemaReader is managed by a factory
    template<>
    struct is_managed_by_factory<Database::I_DatabaseSchemaReader> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_SCHEMA_READER_HPP_INCLUDED
