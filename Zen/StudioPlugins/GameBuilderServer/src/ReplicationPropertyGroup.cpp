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
#include "ReplicationPropertyGroup.hpp"

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ReplicationPropertyGroup::ReplicationPropertyGroup(Properties& _properties, Property* _pParent)
:   PropertyGroup(_properties, "Replication", _pParent)
,   m_replicationType(_properties, "", this)
,   m_replicationDirection(_properties, "", this)
,   m_scopator(_properties, "", "Scopator Extension Point", this)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ReplicationPropertyGroup::~ReplicationPropertyGroup()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ReplicationPropertyGroup::insert(pDatabaseConnection_type _pDBConn)
{
    Property::insert(_pDBConn);
    m_replicationType.insert(_pDBConn);
    m_replicationDirection.insert(_pDBConn);
    m_scopator.insert(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ReplicationPropertyGroup::load(pDatabaseConnection_type _pDBConn)
{
    Property::load(_pDBConn);
    m_replicationType.load(_pDBConn);
    m_replicationDirection.load(_pDBConn);
    m_scopator.load(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ReplicationPropertyGroup::save(pDatabaseConnection_type _pDBConn)
{
    Property::save(_pDBConn);
    m_replicationType.save(_pDBConn);
    m_replicationDirection.save(_pDBConn);
    m_scopator.save(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ReplicationPropertyGroup::remove(pDatabaseConnection_type _pDBConn)
{
    Property::remove(_pDBConn);
    m_replicationType.remove(_pDBConn);
    m_replicationDirection.remove(_pDBConn);
    m_scopator.remove(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ReplicationPropertyGroup::notifyAdd(pPropertiesListener_type _pListener)
{
    _pListener->onAddProperty(getPublisher(), *this);

    m_replicationType.notifyAdd(_pListener);
    m_replicationDirection.notifyAdd(_pListener);
    m_scopator.notifyAdd(_pListener);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ReplicationPropertyGroup::setPropertiesId(boost::uint64_t _propertiesId)
{
    Property::setPropertiesId(_propertiesId);
    m_replicationType.setPropertiesId(_propertiesId);
    m_replicationDirection.setPropertiesId(_propertiesId);
    m_scopator.setPropertiesId(_propertiesId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ReplicationPropertyGroup::tryLoadProperty(pPropertyDomainObject_type _pDomainObject)
{
    return (Property::tryLoadProperty(_pDomainObject) ||
        m_replicationType.tryLoadProperty(_pDomainObject) ||
        m_replicationDirection.tryLoadProperty(_pDomainObject) ||
        m_scopator.tryLoadProperty(_pDomainObject));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
