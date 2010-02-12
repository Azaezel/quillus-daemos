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

#include "MYSQLTypes.hpp"

#ifndef ZEN_ZMYSQL_DATABASE_RESULT_HPP_INCLUDED
#define ZEN_ZMYSQL_DATABASE_RESULT_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Enterprise/Database/I_DatabaseResult.hpp>
#include <Zen/Enterprise/Database/I_DatabaseColumn.hpp>
#include <Zen/Enterprise/Database/I_DatabaseRow.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DatabaseResult
:   public Zen::Database::I_DatabaseResult
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<MYSQL_FIELD>                           pMYSQLField_type;
    typedef Zen::Memory::managed_weak_ptr<MYSQL_FIELD>                      wpMYSQLField_type;

    typedef Zen::Memory::managed_ptr<Zen::Database::I_DatabaseColumn>       pDatabaseColumn_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Database::I_DatabaseColumn>  wpDatabaseColumn_type;

    typedef Zen::Memory::managed_ptr<MYSQL_ROW>                             pMYSQLRow_type;
    typedef Zen::Memory::managed_weak_ptr<MYSQL_ROW>                        wpMYSQLRow_type;

    typedef Zen::Memory::managed_ptr<Zen::Database::I_DatabaseRow>          pDatabaseRow_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Database::I_DatabaseRow>     wpDatabaseRow_type;

    typedef Zen::Memory::managed_ptr<MYSQL_RES>                             pMYSQLResult_type;
    /// @}

    /// @name Zen::Database::I_DatabaseResult implementation
    /// @{
public:
    virtual void getResults(I_ResultVisitor& _visitor);
    virtual void getColumns(I_ColumnVisitor& _visitor);
    /// @}

    /// @name Zen::ZMySQL::DatabaseResult implementation
    /// @{
public:
    /// @}

    /// @name Event handlers
    /// @{
public:
    void onDestroyMYSQLField(wpMYSQLField_type _wpMYSQLField);
    void onDestroyMYSQLRow(wpMYSQLRow_type _wpMYSQLRow);
    void onDestroyDatabaseColumn(wpDatabaseColumn_type _wpDatabaseColumn);
#if 0
    void onDestroyDatabaseRow(wpDatabaseRow_type _wpDatabaseRow);
#endif
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseResult(pMYSQLResult_type _pMYSQLResult);
    virtual ~DatabaseResult();
    /// @}

    /// @name Member variables
    /// @{
private:
    pMYSQLResult_type   m_pMYSQLResult;
    /// @}

};  // class DatabaseResult

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZMYSQL_DATABASE_RESULT_HPP_INCLUDED
