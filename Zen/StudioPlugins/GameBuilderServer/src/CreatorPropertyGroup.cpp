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
#include "CreatorPropertyGroup.hpp"

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CreatorPropertyGroup::CreatorPropertyGroup(Properties& _properties, Property* _pParent)
:   PropertyGroup(_properties, "Creator", _pParent)
,   m_creatorCategory(_properties, "", this)
,   m_creatorIcon(_properties, "", this)
{
    addChild(m_creatorCategory.getName(), &m_creatorCategory);
    addChild(m_creatorIcon.getName(), &m_creatorIcon);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CreatorPropertyGroup::~CreatorPropertyGroup()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CreatorPropertyGroup::insert(pDatabaseConnection_type _pDBConn)
{
    // TODO This could be handled by ProperyGroup since it has a list of the children now.
    Property::insert(_pDBConn);
    m_creatorCategory.insert(_pDBConn);
    m_creatorIcon.insert(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CreatorPropertyGroup::load(pDatabaseConnection_type _pDBConn)
{
    // TODO This could be handled by ProperyGroup since it has a list of the children now.
    Property::load(_pDBConn);
    m_creatorCategory.load(_pDBConn);
    m_creatorIcon.load(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CreatorPropertyGroup::save(pDatabaseConnection_type _pDBConn)
{
    // TODO This could be handled by ProperyGroup since it has a list of the children now.

    Property::save(_pDBConn);
    m_creatorCategory.save(_pDBConn);
    m_creatorIcon.save(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CreatorPropertyGroup::remove(pDatabaseConnection_type _pDBConn)
{
    // TODO This could be handled by ProperyGroup since it has a list of the children now.

    Property::remove(_pDBConn);
    m_creatorCategory.remove(_pDBConn);
    m_creatorIcon.remove(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CreatorPropertyGroup::notifyAdd(pPropertiesListener_type _pListener)
{
    // TODO This could be handled by ProperyGroup since it has a list of the children now.
    _pListener->onAddProperty(getPublisher(), *this);

    m_creatorCategory.notifyAdd(_pListener);
    m_creatorIcon.notifyAdd(_pListener);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CreatorPropertyGroup::setPropertiesId(boost::uint64_t _propertiesId)
{
    // TODO This could be handled by ProperyGroup since it has a list of the children now.
    Property::setPropertiesId(_propertiesId);
    m_creatorCategory.setPropertiesId(_propertiesId);
    m_creatorIcon.setPropertiesId(_propertiesId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CreatorPropertyGroup::tryLoadProperty(pPropertyDomainObject_type _pDomainObject)
{
    // TODO This could be handled by ProperyGroup since it has a list of the children now.

    return (Property::tryLoadProperty(_pDomainObject) ||
        m_creatorCategory.tryLoadProperty(_pDomainObject) ||
        m_creatorIcon.tryLoadProperty(_pDomainObject));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
