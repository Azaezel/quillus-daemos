//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Property.hpp"
#include "Properties.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDataCollection.hpp>

#include <sstream>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Property::Property(Properties& _properties, const std::string& _name, const std::string& _value, PropertyType _type, Property* _pParent)
:   m_properties(_properties)
,   m_name(_name)
,   m_value(_value)
,   m_type(_type)
,   m_canChange(true)
,   m_pParent(_pParent)
,   m_propertyId(0)
,   m_propertiesId(0)
,   m_pChildrenGuard(Zen::Threading::MutexFactory::create())
{
    if (m_pParent != NULL)
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
    return m_type;
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
    m_pPropertyDO->getName() = getFullName();
    m_pPropertyDO->getValue() = getValue();
    /// TODO Do we need to persist property type to the DB?
    /// TODO Do we need to persist property mutability to the DB?

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

    m_fullyQualifiedName = (const std::string&)m_pPropertyDO->getName();

    std::string fullName = std::string("\\") + m_fullyQualifiedName;
    std::string::iterator foundIter = fullName.begin();
    std::string::iterator iter = fullName.begin();
    while (iter != fullName.end())
    {
        const std::string delimiter("\\");
        foundIter = iter + 1;
        iter++;
        iter = std::find_first_of(
            iter, 
            fullName.end(), 
            delimiter.begin(), 
            delimiter.end()
        );
    }

    m_name = std::string(foundIter, fullName.end());

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
    /// TODO Maybe support some sort of rule that can be defined using 
    /// meta-data for verification checking of a value. I.E. a regex for
    /// checking if a numeric property is being changed to a numeric or not
    return m_canChange;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::setValue(const std::string& _newValue)
{
    m_value = _newValue;

    // Quite likely, this property is a new property and doesn't exist
    // in the database.
    if (!m_pPropertyDO.isValid())
    {
        // This property doesn't exist in the databae, so put it there.
        insert(getProperties().getProject().getDatabaseConnection());
    }

    m_pPropertyDO->getValue() = getValue();

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
    if (m_fullyQualifiedName == _pDomainObject->getName().getStringValue())
    {
        setPropertyDO(_pDomainObject);
        return true;
    }

    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Studio::Workbench::I_Property*
Property::getPropertyByName(const std::string& _name) const
{
    if (_name == getName())
    {
        return this;
    }
    else if (getType() == CATEGORY_TYPE)
    {
        // Parse the name using "\" as a delimiter.
        const std::string delimiter("\\");
        std::string::const_iterator iter = std::find_first_of(
            _name.begin(), 
            _name.end(), 
            delimiter.begin(), 
            delimiter.end()
        );

        if (iter != _name.end())
        {
            std::string firstPart(_name.begin(), iter);

            if (firstPart != getName())
            {
                // TODO Throw error?
                return NULL;
            }
        }

        std::string::const_iterator iter2 = std::find_first_of(
            iter + 1,
            _name.end(),
            delimiter.begin(),
            delimiter.end()
        );

        PropertyChildren_type::const_iterator pChild =
            m_children.find(std::string(iter + 1, iter2));

        if (pChild != m_children.end())
        {
            return pChild->second->getPropertyByName(
                std::string(iter + 1, _name.end())
            );
        }

        // Child name wasn't in map.
        return NULL;
    }

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties&
Property::getProperties()
{
    return m_properties;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Property::addChild(const std::string& _name, Property* _pChild)
{
    Zen::Threading::CriticalSection lock(m_pChildrenGuard);

    m_children[_name] = _pChild;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
