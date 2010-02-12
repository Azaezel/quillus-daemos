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

#include "DatabaseResult.hpp"
#include "DatabaseColumn.hpp"
#include "DatabaseRow.hpp"

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseResult::DatabaseResult(pMYSQLResult_type _pMYSQLResult)
:   m_pMYSQLResult(_pMYSQLResult)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseResult::~DatabaseResult()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::getColumns(I_ColumnVisitor& _visitor)
{
    unsigned int numFields;
    unsigned int i;

    numFields = mysql_num_fields(this->m_pMYSQLResult.get());

    _visitor.begin();

    for( i = 0 ; i < numFields ; i++ )
    {
        MYSQL_FIELD* pRawMYSQLField = mysql_fetch_field_direct(this->m_pMYSQLResult.get(), i);

        if( pRawMYSQLField )
        {
            pMYSQLField_type pMYSQLField(pRawMYSQLField,
                boost::bind(&DatabaseResult::onDestroyMYSQLField,this,_1));

            DatabaseColumn* pRawDatabaseColumn = new DatabaseColumn(this->m_pMYSQLResult,pMYSQLField,i);

            pDatabaseColumn_type pDatabaseColumn(pRawDatabaseColumn,
                boost::bind(&DatabaseResult::onDestroyDatabaseColumn,this,_1));

            _visitor.visit(*pDatabaseColumn);
        }
        else
        {
            throw std::exception("Zen::ZMySQL::DatabaseResult::getColumns() : Invalid MYSQL_FIELD pointer.");
        }
    }
    
    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::getResults(I_ResultVisitor& _visitor)
{
    //unsigned long long numRows;
    //unsigned long long i;

    //numRows = mysql_num_rows(this->m_pMYSQLResult.get());

    _visitor.begin();

    MYSQL_ROW mysqlRow;
    while( mysqlRow = mysql_fetch_row(this->m_pMYSQLResult.get()) )
    //for( i = 0 ; i < numRows ; i++ )
    {
        MYSQL_ROW* pRawMYSQLRow = new MYSQL_ROW(mysqlRow);
        //*pRawMYSQLRow = mysql_fetch_row(this->m_pMYSQLResult.get());

        if( *pRawMYSQLRow )
        {
            pMYSQLRow_type pMYSQLRow(pRawMYSQLRow,
                boost::bind(&DatabaseResult::onDestroyMYSQLRow,this,_1));

            DatabaseRow* pRawDatabaseRow = new DatabaseRow(this->m_pMYSQLResult,pMYSQLRow);

#if 0
            pDatabaseRow_type pDatabaseRow(pRawDatabaseRow,
                boost::bind(&DatabaseResult::onDestroyDatabaseRow,this,_1));
#endif

            _visitor.visit(*pRawDatabaseRow);
        }
        else
        {
            delete pRawMYSQLRow;
            throw std::exception("Zen::ZMySQL::DatabaseResult::getResults() : Invalid MYSQL_ROW.");
        }
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::onDestroyMYSQLField(wpMYSQLField_type _wpMYSQLField)
{
    /// TODO : Implement
    /// Need to find out if we need to delete MYSQL_FIELD or not
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::onDestroyMYSQLRow(wpMYSQLRow_type _wpMYSQLRow)
{
    /// TODO : Implement
    /// Need to find out if we need to delete MYSQL_ROW or not
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseResult::onDestroyDatabaseColumn(wpDatabaseColumn_type _wpDatabaseColumn)
{
    /// Fire the DatabaseColumn's onDestroyEvent
    _wpDatabaseColumn->onDestroyEvent(_wpDatabaseColumn);

    /// Delete the DatabaseColumn
    DatabaseColumn* pDatabaseColumn = dynamic_cast<DatabaseColumn*>(_wpDatabaseColumn.get());

    if( pDatabaseColumn )
    {
        delete pDatabaseColumn;
    }
    else
    {
        throw std::exception("Zen::ZMySQL::DatabaseResult::onDestroyDatabaseColumn() : _wpDatabaseColumn is an invalid DatabaseColumn.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
void
DatabaseResult::onDestroyDatabaseRow(wpDatabaseRow_type _wpDatabaseRow)
{
    /// Fire the DatabaseRow's onDestroyEvent
    _wpDatabaseRow->onDestroyEvent(_wpDatabaseRow);

    /// Delete the DatabaseRow
    DatabaseRow* pDatabaseRow = dynamic_cast<DatabaseRow*>(_wpDatabaseRow.get());

    if( pDatabaseRow )
    {
        delete pDatabaseRow;
    }
    else
    {
        throw std::exception("Zen::ZMySQL::DatabaseResult::onDestroyDatabaseRow() : _wpDatabaseRow is an invalid DatabaseRow.");
    }
}
#endif 
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
