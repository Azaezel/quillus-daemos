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

#include "DatabaseRow.hpp"
#include "DatabaseColumn.hpp"

#include <boost/bind.hpp>

extern Zen::ZMySQL::DatabaseTypes gDatabaseTypes;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseRow::DatabaseRow(pMYSQLResult_type _pMYSQLResult, pMYSQLRow_type _pMYSQLRow)
:   m_pMYSQLResult(_pMYSQLResult)
,   m_pMYSQLRow(_pMYSQLRow)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DatabaseRow::~DatabaseRow()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseRow::getFields(I_FieldVisitor& _visitor) const
{
    unsigned int numFields = mysql_num_fields(this->m_pMYSQLResult.get());
    unsigned int i;

    _visitor.begin();

    for( i = 0 ; i < numFields ; i++ )
    {
        MYSQL_FIELD* pRawMYSQLField = mysql_fetch_field_direct(this->m_pMYSQLResult.get(), i);

        if( pRawMYSQLField )
        {
            pMYSQLField_type pMYSQLField(pRawMYSQLField,
                boost::bind(&DatabaseRow::onDestroyMYSQLField, this, _1));

            DatabaseColumn databaseColumn(this->m_pMYSQLResult,pMYSQLField,i);

            std::string typeName(databaseColumn.getType().name());

            if( (*(this->m_pMYSQLRow))[i] != NULL )
            {
                _visitor.visit(databaseColumn, gDatabaseTypes.m_conversions.find(typeName)->second(std::string((*(this->m_pMYSQLRow))[i])));
            }
        }
        else
        {
            throw std::exception("Zen::ZMySQL::DatabaseRow::getColumns() : Invalid MYSQL_FIELD pointer.");
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DatabaseRow::onDestroyMYSQLField(wpMYSQLField_type _wpMYSQLField) const
{
    /// TODO : Implement
    /// Need to find out if we need to delete MYSQL_FIELD or not
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
