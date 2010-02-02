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

#ifndef ZEN_ZMYSQL_DATABASE_COLUMN_HPP_INCLUDED
#define ZEN_ZMYSQL_DATABASE_COLUMN_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Enterprise/Database/I_DatabaseColumn.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DatabaseColumn
:   public Zen::Database::I_DatabaseColumn
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<MYSQL_RES>         pMYSQLResult_type;
    typedef Zen::Memory::managed_ptr<MYSQL_FIELD>       pMYSQLField_type;
    /// @}

    /// @name Zen::Database::I_DatabaseColumn implementation
    /// @{
public:
    virtual const std::string& getFieldName() const;
#if 0   // deprecated
    virtual int getFieldSequence();
#endif
    virtual const std::type_info& getType() const;
    /// @}

    /// @name Zen::ZMySQL::DatabaseColumn implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseColumn(pMYSQLResult_type _pMYSQLResult, pMYSQLField_type _pMYSQLField, unsigned int _seqNo);
    virtual ~DatabaseColumn();
    /// @}

    /// @name Member variables
    /// @{
private:
    pMYSQLResult_type   m_pMYSQLResult;
    pMYSQLField_type    m_pMYSQLField;
#if 0
    unsigned int        m_seqNo;
#endif
    const std::string   m_fieldName;
    /// @}

};  // class DatabaseColumn

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZMYSQL_DATABASE_COLUMN_HPP_INCLUDED
