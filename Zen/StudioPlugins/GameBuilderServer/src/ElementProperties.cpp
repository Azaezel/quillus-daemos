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
#include "ElementProperties.hpp"
#include "GameObjectElement.hpp"
#include "GameObjectTypeDocument.hpp"
#include "Project.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertiesDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ElementProperties::ElementProperties(GameObjectElement& _element)
:   Properties()
,   m_element(_element)
,   m_persistence(*this, "Static")
,   m_replication(*this)
,   m_factory(*this, "", "Factory Extension Point")
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ElementProperties::~ElementProperties()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ElementProperties::getPropertiesName() const
{
    return m_element.getElementName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesController&
ElementProperties::getController()
{
    // Like DocumentProperties?
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ElementProperties::notifyListener(pPropertiesListener_type _pListener)
{
    // TODO Don't hard-code this.

    m_persistence.notifyAdd(_pListener);
    notifyPropertyValueModified(&m_persistence);

    m_replication.notifyAdd(_pListener);
    notifyPropertyValueModified(&m_replication);

    m_factory.notifyAdd(_pListener);
    notifyPropertyValueModified(&m_factory);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ElementProperties::loadProperty(pPropertyDomainObject_type _pDomainObject)
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
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ElementProperties::saveChildren(pDatabaseConnection_type _pDBConn)
{
    m_persistence.setPropertiesId(getPropertiesId());
    m_persistence.insert(_pDBConn);

    m_replication.setPropertiesId(getPropertiesId());
    m_replication.insert(_pDBConn);

    m_factory.setPropertiesId(getPropertiesId());
    m_factory.insert(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project&
ElementProperties::getProject()
{
    GameObjectTypeDocument* pDocument = dynamic_cast<GameObjectTypeDocument*>(&m_element.getDocument());
    if (pDocument)
    {
        return *pDocument->getProject();
    }
    else
    {
        throw Zen::Utility::runtime_exception("ElementPRoperties::getProject(): Error, wrong document type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
