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
#include "Properties.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertyDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDataCollection.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertiesDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertiesDomainObject.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties::Properties(Zen::Studio::Workbench::I_ExplorerNode::UserData& _document)
:   m_pListenersGuard(Zen::Threading::MutexFactory::create())
,   m_pPropertyMapGuard(Zen::Threading::MutexFactory::create())
,   m_document(_document)
{
    m_document.registerProperties(*this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties::~Properties()
{
    {
        Zen::Threading::CriticalSection lock(m_pPropertyMapGuard);

        for (PropertyMap_type::iterator iter = m_propertyMap.begin();
             iter != m_propertyMap.end(); ++iter)
        {
            delete iter->second;
        }
        m_propertyMap.clear();
    }

    Zen::Threading::MutexFactory::destroy(m_pPropertyMapGuard);
    Zen::Threading::MutexFactory::destroy(m_pListenersGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Properties::getPropertiesName() const
{
    return m_document.getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Zen::Studio::Workbench::subscription<Properties*, Properties::pPropertiesListener_type> Subscription_type;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Zen::Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription_type* pSubscription =
        dynamic_cast<Subscription_type*>(_wpSubscription.get());

    if (pSubscription != NULL)
    {
        delete pSubscription;
        return;
    }

    /// TODO Error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties::pSubscription_type
Properties::subscribe(pPropertiesListener_type _pListener)
{
    Zen::Threading::CriticalSection lock(m_pListenersGuard);

    m_listeners.insert(_pListener);

    notifyListener(_pListener);
    return pSubscription_type(
        new Subscription_type(this, _pListener),
        &destroySubscription
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::unSubscribe(pPropertiesListener_type _pListener)
{
    Zen::Threading::CriticalSection lock(m_pListenersGuard);
    m_listeners.erase(_pListener);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_PropertiesController&
Properties::getController()
{
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Studio::Workbench::I_Property*
Properties::getPropertyByName(const std::string& _name) const
{
    Zen::Threading::CriticalSection lock(m_pPropertyMapGuard);

    PropertyMap_type::const_iterator propertyIter = m_propertyMap.find(_name);
    if (propertyIter != m_propertyMap.end())
    {
        return propertyIter->second;
    }
    else
    {
        return NULL;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Properties::canChangeProperty(Zen::Studio::Workbench::I_Property& _property, const std::string& _newValue)
{
    Property* const pProperty = dynamic_cast<Property*>(&_property);

    assert(pProperty != NULL);

    return pProperty->canChange(_newValue);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::setPropertyValue(Zen::Studio::Workbench::I_Property& _property, const std::string& _newValue)
{
    Property* const pProperty = dynamic_cast<Property*>(&_property);

    assert(pProperty != NULL);

    pProperty->setValue(_newValue);

    // TODO Validate referential integrity among the different properties.

    // Save the property value.
    pProperty->save(getProject().getDatabaseConnection());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::insert(pDatabaseConnection_type _pDBConn)
{
    pPropertiesDataMap_type pPropertiesDM =
        Zen::Studio::Workbench::I_PropertiesDataMap::create(_pDBConn);

    m_pPropertiesDO = pPropertiesDM->createNew();

    pPropertiesDM->update(m_pPropertiesDO);

    m_propertiesId = m_pPropertiesDO->getPropertiesId();

    saveChildren(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::load(pDatabaseConnection_type _pDBConn)
{
    // Load the child Property collection, but don't bother loading Properties.
    Zen::Studio::Workbench::I_PropertyDataMap::pPropertyDataMap_type pPropertyDM =
        Zen::Studio::Workbench::I_PropertyDataMap::create(_pDBConn);

    struct PropertyVisitor
    :   public Zen::Studio::Workbench::I_PropertyDataCollection::I_CollectionVisitor
    {
        virtual void begin()
        {
        }

        virtual void visit(pPropertyDomainObject_type _pDomainObject)
        {
            m_parent.loadProperty(_pDomainObject);
        }

        virtual void end()
        {
        }

        PropertyVisitor(Properties& _parent)
        :   m_parent(_parent)
        {
        }

    private:
        Properties&     m_parent;
    };  // struct PropertyVisitor

    PropertyVisitor visitor(*this);

    pPropertyDM->getByPropertiesId(m_propertiesId)->getValue()->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::save(pDatabaseConnection_type _pDBConn)
{
    pPropertiesDataMap_type pPropertiesDM =
        Zen::Studio::Workbench::I_PropertiesDataMap::create(_pDBConn);

    m_pPropertiesDO->setPropertiesId(m_propertiesId);

    pPropertiesDM->update(m_pPropertiesDO);

    saveChildren(_pDBConn);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::remove(pDatabaseConnection_type _pDBConn)
{
    pPropertiesDataMap_type pPropertiesDM =
        Zen::Studio::Workbench::I_PropertiesDataMap::create(_pDBConn);

    pPropertiesDM->deleteByKey(m_propertiesId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint64_t
Properties::getPropertiesId() const
{
    return m_propertiesId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::setPropertiesId(boost::uint64_t _id)
{
    m_propertiesId = _id;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::notifyPropertyValueModified(Property* _pProperty)
{
    // Notify listeners of the modification
    Zen::Threading::CriticalSection lock(m_pListenersGuard);

    for (Listeners_type::iterator iter = m_listeners.begin();
         iter != m_listeners.end(); ++iter)
    {
        (*iter)->onValueModified(*this, *_pProperty);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::notifyListener(pPropertiesListener_type _pListener)
{
    // Notify listeners of addition
    Zen::Threading::CriticalSection lock(m_pPropertyMapGuard);

    for (PropertyMap_type::iterator iter = m_propertyMap.begin();
         iter != m_propertyMap.end(); ++iter)
    {
        iter->second->notifyAdd(_pListener);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::loadProperty(pPropertyDomainObject_type _pDomainObject)
{
    Zen::Threading::CriticalSection lock(m_pPropertyMapGuard);

    PropertyMap_type::iterator iter = m_propertyMap.find(_pDomainObject->getName());
    if (iter != m_propertyMap.end())
    {
        if (!iter->second->tryLoadProperty(_pDomainObject))
        {
            /// TODO Throw exception?
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::saveChildren(pDatabaseConnection_type _pDBConn)
{
    Zen::Threading::CriticalSection lock(m_pPropertyMapGuard);

    for (PropertyMap_type::iterator iter = m_propertyMap.begin();
         iter != m_propertyMap.end(); ++iter)
    {
        iter->second->setPropertiesId(getPropertiesId());
        iter->second->insert(_pDBConn);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::registerProperty(Property* _pProperty)
{
    Zen::Threading::CriticalSection lock(m_pPropertyMapGuard);

    PropertyMap_type::iterator iter = m_propertyMap.find(_pProperty->getFullName());
    if (iter == m_propertyMap.end())
    {
        m_propertyMap[_pProperty->getFullName()] = _pProperty;
        return;
    }

    /// Delete this property if one already exists.
    delete _pProperty;

    /// TODO Throw error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_Project&
Properties::getProject()
{
    return *m_document.getNode()->getProject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
