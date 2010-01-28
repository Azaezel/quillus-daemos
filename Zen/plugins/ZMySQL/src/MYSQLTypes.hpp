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

#ifdef WIN32
#include <windows.h>
#endif // WIN32

#ifndef ZEN_ZMYSQL_MYSQL_TYPES_HPP_INCLUDED
#define ZEN_ZMYSQL_MYSQL_TYPES_HPP_INCLUDED

#include <mysql.h>
#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Memory {
    /// MYSQL is managed by I_DatabaseService
    template<>
    struct is_managed_by_factory<MYSQL> : public boost::true_type{};
    /// MYSQL_RES is managed by I_DatabaseTransaction
    template<>
    struct is_managed_by_factory<MYSQL_RES> : public boost::true_type{};
    /// MYSQL_ROW is managed by I_DatabaseResult
    template<>
    struct is_managed_by_factory<MYSQL_ROW> : public boost::true_type{};
    /// MYSQL_FIELD is managed by I_DatabaseResult and I_DatabaseRow
    template<>
    struct is_managed_by_factory<MYSQL_FIELD> : public boost::true_type{};
}   // namespace Memory
namespace ZMySQL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getDoubleType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToDouble(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getFloatType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToFloat(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getCharType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToChar(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getIntType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToInt(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getLongType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToLong(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getLongLongType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToLongLong(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getStringType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToString(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getBoolType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToBool(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getVoidType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToVoid(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::type_info& getBlobType();
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const boost::any convertToBlob(const std::string& _value);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class DatabaseTypes
{
public:
    std::map<int,const std::type_info& (__cdecl *) (void)> m_typeMap;
    std::map< std::string, boost::function< const boost::any ( const std::string& ) > > m_conversions;
public:
    DatabaseTypes::DatabaseTypes()
    {
        m_typeMap[MYSQL_TYPE_DECIMAL]       = &getDoubleType;
        m_typeMap[MYSQL_TYPE_TINY]          = &getCharType;
        m_typeMap[MYSQL_TYPE_SHORT]         = &getIntType;
        m_typeMap[MYSQL_TYPE_LONG]          = &getLongType;
        m_typeMap[MYSQL_TYPE_FLOAT]         = &getFloatType;
        m_typeMap[MYSQL_TYPE_DOUBLE]        = &getDoubleType;
        m_typeMap[MYSQL_TYPE_NULL]          = &getVoidType;
        m_typeMap[MYSQL_TYPE_TIMESTAMP]     = &getStringType;
        m_typeMap[MYSQL_TYPE_LONGLONG]      = &getLongLongType;
        m_typeMap[MYSQL_TYPE_INT24]         = &getLongType;
        m_typeMap[MYSQL_TYPE_DATE]          = &getStringType;
        m_typeMap[MYSQL_TYPE_TIME]          = &getStringType;
        m_typeMap[MYSQL_TYPE_DATETIME]      = &getStringType;
        m_typeMap[MYSQL_TYPE_YEAR]          = &getStringType;
        m_typeMap[MYSQL_TYPE_NEWDATE]       = &getStringType;
        m_typeMap[MYSQL_TYPE_VARCHAR]       = &getStringType;
        m_typeMap[MYSQL_TYPE_BIT]           = &getBoolType;
        m_typeMap[MYSQL_TYPE_NEWDECIMAL]    = &getDoubleType;
        m_typeMap[MYSQL_TYPE_ENUM]          = &getIntType;
        m_typeMap[MYSQL_TYPE_SET]           = &getLongLongType;
        m_typeMap[MYSQL_TYPE_TINY_BLOB]     = &getBlobType;
        m_typeMap[MYSQL_TYPE_MEDIUM_BLOB]   = &getBlobType;
        m_typeMap[MYSQL_TYPE_LONG_BLOB]     = &getBlobType;
        m_typeMap[MYSQL_TYPE_BLOB]          = &getBlobType;
        m_typeMap[MYSQL_TYPE_VAR_STRING]    = &getStringType;
        m_typeMap[MYSQL_TYPE_STRING]        = &getStringType;
        m_typeMap[MYSQL_TYPE_GEOMETRY]      = &getBlobType;

        m_conversions[std::string(getDoubleType().name())]      = &convertToDouble;
        m_conversions[std::string(getFloatType().name())]       = &convertToFloat;
        m_conversions[std::string(getCharType().name())]        = &convertToChar;
        m_conversions[std::string(getIntType().name())]         = &convertToInt;
        m_conversions[std::string(getLongType().name())]        = &convertToLong;
        m_conversions[std::string(getLongLongType().name())]    = &convertToLongLong;
        m_conversions[std::string(getStringType().name())]      = &convertToString;
        m_conversions[std::string(getBoolType().name())]        = &convertToBool;
        m_conversions[std::string(getVoidType().name())]        = &convertToVoid;
        m_conversions[std::string(getBlobType().name())]        = &convertToBlob;
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZMySQL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZMYSQL_MYSQL_TYPES_HPP_INCLUDED
