//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
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
#include "Property.hpp"
#include "DocumentProperties.hpp"
#include "ElementProperties.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDataCollection.hpp>

#include <sstream>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Property::Property(Properties& _properties, const std::string& _name, const std::string& _value, Property* _pParent)
:   m_properties(_properties)
,   m_name(_name)
,   m_value(_value)
,   m_pParent(_pParent)
,   m_propertyId(0)
,   m_propertiesId(0)
{
    if (m_pParent)
    {
        std::stringstream name;
        name << m_pParent->getName() << "\\" << m_name;
        m_fullyQualifiedName = name.str();
    }
    else
    {
        m_fullyQualifiedName = m_name;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Property::~Property()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Property::getName() const
{
    return m_name;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Property::getFullName() const
{
    return m_fullyQualifiedName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Property::getValue() const
{
    return m_value;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Property::PropertyType
Property::getType() const
{
    return STRING_TYPE;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Property*
Property::getParent()
{
    return m_pParent;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesPublisher&
Property::getPublisher()
{
    return m_properties;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::insert(pDatabaseConnection_type _pDBConn)
{
    pPropertyDataMap_type pPropertyDM =
        Zen::Studio::Workbench::I_PropertyDataMap::create(_pDBConn);

    m_pPropertyDO = pPropertyDM->createNew();

    m_pPropertyDO->getPropertiesId() = m_propertiesId;
    m_pPropertyDO->getName() = getName();
    m_pPropertyDO->getValue() = getValue();

    pPropertyDM->update(m_pPropertyDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::load(pDatabaseConnection_type _pDBConn)
{
    pPropertyDataMap_type pPropertyDM =
        Zen::Studio::Workbench::I_PropertyDataMap::create(_pDBConn);

    setPropertyDO(pPropertyDM->getByKey(m_propertyId));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::save(pDatabaseConnection_type _pDBConn)
{
    pPropertyDataMap_type pPropertyDM =
        Zen::Studio::Workbench::I_PropertyDataMap::create(_pDBConn);

    pPropertyDM->update(m_pPropertyDO);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::remove(pDatabaseConnection_type _pDBConn)
{
    pPropertyDataMap_type pPropertyDM =
        Zen::Studio::Workbench::I_PropertyDataMap::create(_pDBConn);

    pPropertyDM->deleteByKey(m_pPropertyDO->getPropertyId());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::setPropertyDO(pPropertyDomainObject_type _pPropertyDO)
{
    m_pPropertyDO = _pPropertyDO;

    m_name = (const std::string&)m_pPropertyDO->getName();
    setValue((const std::string&)m_pPropertyDO->getValue());
    m_propertiesId = m_pPropertyDO->getPropertiesId();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::notifyAdd(pPropertiesListener_type _pListener)
{
    _pListener->onAddProperty(getPublisher(), *this);
    _pListener->onValueModified(getPublisher(), *this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Property::canChange(const std::string& _newValue)
{
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::setValue(const std::string& _newValue)
{
    m_value = _newValue;

    // Quite likely this property is a new property and doesn't exist
    // in the database.
    if (!m_pPropertyDO.isValid())
    {
        // This property doesn't exist in the database, so put it there.
        insert(getProperties().getProject().getDatabaseConnection());
    }

    m_pPropertyDO->getValue() = getValue();

    std::cout << "Property::setValue(): notifyPropertyValueModified" << std::endl;
    getProperties().notifyPropertyValueModified(this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::setPropertiesId(boost::uint64_t _propertiesId)
{
    m_propertiesId = _propertiesId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Property::tryLoadProperty(pPropertyDomainObject_type _pDomainObject)
{
    if(m_name == _pDomainObject->getName().getStringValue())
    {
        setPropertyDO(_pDomainObject);
        return true;
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Studio::Workbench::I_Property*
Property::getPropertyByName(const std::string& _name)
{
    if (m_name == _name)
    {
        return this;
    }
    else
    {
        return NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties&
Property::getProperties()
{
    return m_properties;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
