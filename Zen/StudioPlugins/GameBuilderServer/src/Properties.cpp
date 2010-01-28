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
#include "Properties.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

#include <Zen/Studio/WorkbenchCommon/subscription.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertyDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertiesDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDataCollection.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties::Properties()
:   m_pListenersGuard(Zen::Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties::~Properties()
{
    Zen::Threading::MutexFactory::destroy(m_pListenersGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Zen::Studio::Workbench::subscription<Properties*, Properties::pPropertiesListener_type>    Subscription_type;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Zen::Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription_type* pSubscription = dynamic_cast<Subscription_type*>(_wpSubscription.get());

    if (pSubscription)
    {
        delete pSubscription;
        return;
    }

    // Else error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Properties::pSubscription_type
Properties::subscribe(pPropertiesListener_type _pListener)
{
    Zen::Threading::CriticalSection lock(m_pListenersGuard);

    m_listeners.insert(_pListener);

    notifyListener(_pListener);
    return pSubscription_type(new Subscription_type(this, _pListener), &destroySubscription);
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
    : public Zen::Studio::Workbench::I_PropertyDataCollection::I_CollectionVisitor
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
        Properties& m_parent;
    };

    PropertyVisitor visitor(*this);

    pPropertyDM->getByPropertiesId(m_propertiesId)->getValue()->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Properties::save(pDatabaseConnection_type _pDBConn)
{
    // Probably a worthless step, but maybe the key changed?
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
    
    for(Listeners_type::iterator iter = m_listeners.begin(); iter != m_listeners.end(); iter++)
    {
        (*iter)->onValueModified(*this, *_pProperty);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
