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
#ifndef ZEN_COMMUNITY_SESSION_ACCOUNT_DOMAIN_OBJECT_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSION_ACCOUNT_DOMAIN_OBJECT_HPP_INCLUDED

#include "../I_AccountDomainObject.hpp"

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Session {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


class AccountDomainObject
:	public I_AccountDomainObject
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, pElement_type>    FieldElementMap_type;
    /// @}

    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the AccountId element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setAccountId().
    virtual ElementReference_type getAccountId() const;
    
    /// Set the AccountId element value.
    /// This method does not directly update the database.  
    /// You must use the I_AccountDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setAccountId(const boost::any& _value);

    /// Get the User element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setUser().
    virtual ElementReference_type getUser() const;
    
    /// Set the User element value.
    /// This method does not directly update the database.  
    /// You must use the I_AccountDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setUser(const boost::any& _value);

    /// Get the Password element.
    /// Use this return value to check to see if the column is NULL in the database,
    /// or you can use it to modify the value instead of calling setPassword().
    virtual ElementReference_type getPassword() const;
    
    /// Set the Password element value.
    /// This method does not directly update the database.  
    /// You must use the I_AccountDataMap::update() method for the modifications
    /// to be persisted.
    virtual void setPassword(const boost::any& _value);
    
    virtual Zen::Enterprise::DataModel::I_DataElement& getElement(const std::string& _fieldName);
    /// @}

    /// @name Member Variables
    /// @{
private:
    pElement_type               m_pAccountId;
    pElement_type               m_pUser;
    pElement_type               m_pPassword;

    FieldElementMap_type            m_elements;
    /// @}

    /// @name 'Structors
    /// @{
public:
             AccountDomainObject();
    virtual ~AccountDomainObject();
    /// @}

};  // class AccountDomainObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Community
}   // namespace Session
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_COMMUNITY_SESSION_ACCOUNT_DOMAIN_OBJECT_HPP_INCLUDED
