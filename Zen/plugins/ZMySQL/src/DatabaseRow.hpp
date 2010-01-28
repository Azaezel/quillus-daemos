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

#ifndef ZEN_ZMYSQL_DATABASE_ROW_HPP_INCLUDED
#define ZEN_ZMYSQL_DATABASE_ROW_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Enterprise/Database/I_DatabaseRow.hpp>
#include <Zen/Enterprise/Database/I_DatabaseColumn.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DatabaseRow
:   public Zen::Database::I_DatabaseRow
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Database::I_DatabaseColumn>   pDatabaseColumn_type;
    typedef Zen::Memory::managed_ptr<MYSQL_RES>                         pMYSQLResult_type;
    typedef Zen::Memory::managed_ptr<MYSQL_FIELD>                       pMYSQLField_type;
    typedef Zen::Memory::managed_weak_ptr<MYSQL_FIELD>                  wpMYSQLField_type;
    typedef Zen::Memory::managed_ptr<MYSQL_ROW>                         pMYSQLRow_type;
    /// @}

    /// @name Zen::Database::I_DatabaseRow implementation
    /// @{
public:
    virtual void getFields(I_FieldVisitor& _visitor) const;
    /// @}

    /// @name Zen::ZMySQL::DatabaseRow implementation
    /// @{
public:
    /// @}

    /// @name Event handlers
    /// @{
public:
    void onDestroyMYSQLField(wpMYSQLField_type _wpMYSQLField) const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseRow(pMYSQLResult_type _pMYSQLResult, pMYSQLRow_type _pMYSQLRow);
    virtual ~DatabaseRow();
    /// @}

    /// @name Member variables
    /// @{
private:
    pMYSQLResult_type   m_pMYSQLResult;
    pMYSQLRow_type      m_pMYSQLRow;

};  // class DatabaseRow

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZMYSQL_DATABASE_ROW_HPP_INCLUDED
