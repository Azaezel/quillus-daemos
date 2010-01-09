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
#include "DocumentProperties.hpp"
#include "GameObjectType.hpp"
#include "Project.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertiesDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DocumentProperties::DocumentProperties(GameObjectType& _document)
:   Properties()
,   m_document(_document)
,   m_persistence(*this, "Static")
,   m_replication(*this)
,   m_factory(*this, "", "Factory Extension Point")
,   m_creator(*this)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DocumentProperties::~DocumentProperties()
{
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
DocumentProperties::getPropertiesName() const
{
    return m_document.getDocument()->getShortName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesController&
DocumentProperties::getController()
{
    return *this;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DocumentProperties::cellPropertiesChangeEachRow() const
{
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DocumentProperties::cellPropertiesChangeEachColumn() const
{
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DocumentProperties::notifyListener(pPropertiesListener_type _pListener)
{
    // TODO Don't hard-code this.

    m_persistence.notifyAdd(_pListener);

    m_replication.notifyAdd(_pListener);

    m_factory.notifyAdd(_pListener);

    m_creator.notifyAdd(_pListener);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DocumentProperties::loadProperty(pPropertyDomainObject_type _pDomainObject)
{
    // Probably could optimize with a map.  Probably should if loading properties
    // is too slow.
    if (m_persistence.tryLoadProperty(_pDomainObject))
    {
        return;
    }

    if (m_replication.tryLoadProperty(_pDomainObject))
    {
        return;
    }

    if (m_factory.tryLoadProperty(_pDomainObject))
    {
        return;
    }

    if (m_creator.tryLoadProperty(_pDomainObject))
    {
        return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DocumentProperties::saveChildren(pDatabaseConnection_type _pDBConn)
{
    m_persistence.setPropertiesId(getPropertiesId());
    m_persistence.insert(_pDBConn);

    m_replication.setPropertiesId(getPropertiesId());
    m_replication.insert(_pDBConn);

    m_factory.setPropertiesId(getPropertiesId());
    m_factory.insert(_pDBConn);

    m_creator.setPropertiesId(getPropertiesId());
    m_creator.insert(_pDBConn);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project&
DocumentProperties::getProject()
{
    return m_document.getProject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
