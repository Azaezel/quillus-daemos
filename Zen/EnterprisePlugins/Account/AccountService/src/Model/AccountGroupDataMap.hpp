//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ENTERPRISE_ACCOUNT_MODEL_ACCOUNT_GROUP_DATA_MAP_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_MODEL_ACCOUNT_GROUP_DATA_MAP_HPP_INCLUDED

#include "../../Model/I_AccountGroupDataMap.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Model {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AccountGroupDataMap
:   public I_AccountGroupDataMap
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_AccountGroupDataMap implementation
    /// @{
public:
    virtual pDomainObject_type createNew();
    virtual pFutureDomainObject_type getByKey(boost::uint64_t _primaryKeyValue);
    virtual void deleteByKey(boost::uint64_t _primaryKeyValue);
    virtual pFutureDataCollection_type getAll();
    virtual Zen::Database::I_DatabaseTransaction::pFutureKey_type update(pDomainObject_type _pDomainObject);
    /// @}

    /// @name AccountGroupDataMap implementation
    /// @{
public:
    /// Escape the string to prevent SQL injection.
    std::string escapeString(const std::string& _value) const;

    /// Get a comma separated list of the fields in this table.
    static const std::string& getFieldNames();

    /// Get a comma separated ilst of the fields in this table
    /// sans the primary key (accountId)
    static const std::string& getNonPKFieldNames();

    /// Get the name of the table this class maps.
    static const std::string& getTableName();

    pDatabaseConnection_type getDatabaseConnection() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             AccountGroupDataMap(pDatabaseConnection_type _pDatabaseConnection);
    virtual ~AccountGroupDataMap();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pDatabaseConnection_type    m_pDatabaseConnection;
    /// @}

};  // class AccountGroupDataMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Model
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_MODEL_ACCOUNT_GROUP_DATA_MAP_HPP_INCLUDED
