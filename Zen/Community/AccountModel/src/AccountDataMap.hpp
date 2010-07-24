//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Spaces
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// This is generated by the Zen Spaces Code Generator.  Do not modify!
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_COMMUNITY_ACCOUNT_ACCOUNT_DATA_MAP_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNT_ACCOUNT_DATA_MAP_HPP_INCLUDED

#include "../I_AccountDataMap.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Account {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class AccountDataMap
:	public I_AccountDataMap
{
    /// @name I_AccountDataMap implementation
    /// @{
public:
    virtual pAccountDomainObject_type createNew();
    virtual pFutureAccountDomainObject_type getByKey(boost::uint64_t _primaryKeyValue);
    virtual void deleteByKey(boost::uint64_t _primaryKeyValue);
    virtual pFutureAccountDataCollection_type getByUserPassword(const std::string& _user, const std::string& _password);
    virtual pFutureAccountDataCollection_type getAll();
    virtual Zen::Database::I_DatabaseTransaction::pFutureKey_type update(pAccountDomainObject_type _pAccountDomainObject);
    /// @}

    /// @name AccountDataMap implementation
    /// @{
public:
    /// Escape the string to prevent SQL injection
    std::string escapeString(const std::string& _value) const;

    /// Get a comma separated list of the fields in this table.
    static const std::string& getFieldNames();

    /// Get a comma separated list of the fields in this table
    /// sans the primary key (accountId).
    static const std::string& getNonPKFieldNames();
    
    /// Get the name of the table this class maps.
    static const std::string& getTableName();

	pDatabaseConnection_type getDatabaseConnection() const;
    /// @}

	/// @name Member Variables
	/// @{
public:
	pDatabaseConnection_type			m_pDatabaseConnection;
	/// @}

    /// @name 'Structors
    /// @{
public:
             AccountDataMap(pDatabaseConnection_type _pDatabaseConnection);
    virtual ~AccountDataMap();
    /// @}

};  // class AccountDataMap

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Community
}   // namespace Account
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_COMMUNITY_ACCOUNT_ACCOUNT_DATA_MAP_HPP_INCLUDED