//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAME_BUILDER_REPLICATION_GROUP_PROPERTY_HPP_INCLUDED
#define GAME_BUILDER_REPLICATION_GROUP_PROPERTY_HPP_INCLUDED

#include "PropertyGroup.hpp"

#include "ReplicationTypeProperty.hpp"
#include "ReplicationDirectionProperty.hpp"
#include "ExtensionPointProperty.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Replication Property Group.
class ReplicationPropertyGroup
:   public PropertyGroup
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Property implementation
    /// @{
public:
    /// @}

    /// @name I_Persistable overrides
    /// @{
public:
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name PropertyGroup implementation
    /// @{
public:
    /// @}

    /// @name Property overrides
    /// @{
public:
    virtual void notifyAdd(pPropertiesListener_type _pListener);
    virtual void setPropertiesId(boost::uint64_t _propertiesId);
    virtual bool tryLoadProperty(pPropertyDomainObject_type _pDomainObject);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ReplicationPropertyGroup(Properties& _properties, Property* _pParent = NULL);
    virtual ~ReplicationPropertyGroup();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ReplicationTypeProperty         m_replicationType;
    ReplicationDirectionProperty    m_replicationDirection;
    ExtensionPointProperty          m_scopator;
    /// @}

};  // class ReplicationPropertyGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // GAME_BUILDER_REPLICATION_GROUP_PROPERTY_HPP_INCLUDED
