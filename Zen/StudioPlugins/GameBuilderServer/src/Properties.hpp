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
#ifndef GAME_BUILDER_PROPERTIES_HPP_INCLUDED
#define GAME_BUILDER_PROPERTIES_HPP_INCLUDED

#include "Property.hpp"
#include "PersistenceProperty.hpp"
#include "ReplicationPropertyGroup.hpp"
#include "ExtensionPointProperty.hpp"
#include "CreatorPropertyGroup.hpp"

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <Zen/Studio/WorkbenchCommon/I_PropertiesController.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertiesDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDataMap.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Properties base class.
/// This is the base implementation for a collection of properties.
class Properties
:   public virtual Zen::Studio::Workbench::I_PropertiesPublisher
,   public Zen::Database::I_Persistable
,   public Zen::Studio::Workbench::I_PropertiesController
{
    /// @name Types
    /// @{
public:
    typedef std::set<pPropertiesListener_type>  Listeners_type;
    typedef Zen::Studio::Workbench::I_PropertiesDataMap::pPropertiesDataMap_type            pPropertiesDataMap_type;
    typedef Zen::Studio::Workbench::I_PropertiesDataMap::pPropertiesDomainObject_type       pPropertiesDomainObject_type;
    typedef Zen::Studio::Workbench::I_PropertyDataMap::pPropertyDomainObject_type           pPropertyDomainObject_type;
    /// @}

    /// @name I_PropertiesPublisher implementation
    /// @{
public:
    virtual Zen::Studio::Workbench::I_PropertiesPublisher::pSubscription_type subscribe(pPropertiesListener_type _pListener);
    virtual Zen::Studio::Workbench::I_PropertiesController& getController();
    /// @}

    /// @name I_PropertiesController implementation
    /// @{
public:
    virtual bool canChangeProperty(Zen::Studio::Workbench::I_Property& _property, const std::string& _newValue);
    virtual void setPropertyValue(Zen::Studio::Workbench::I_Property& _property, const std::string& _newValue);
    /// @}

    /// @name I_Persistable implementation
    /// @{
public:
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name Properties overridable implementation
    ///     Override these in your derived class to create a custom
    ///     Properties class.
    /// @{
protected:
    /// Notify a single listener of the current property status.
    virtual void notifyListener(pPropertiesListener_type _pListener) = 0;

    /// Called by load() to load a child property that has already
    /// been ready from the database.
    virtual void loadProperty(pPropertyDomainObject_type _pDomainObject) = 0;

    /// Called by save() and insert() to save the child property
    /// domain objects to the database.
    virtual void saveChildren(pDatabaseConnection_type _pDBConn) = 0;

    /// Get the project.
    virtual Zen::Studio::Workbench::I_Project& getProject() = 0;
    /// @}

    /// @name Properties implementation
    /// @{
public:
    /// This should be treated as private.
    /// It's called automatically by Subscription_type when the last reference
    /// to the pSubscription_type goes out of scope.
    void unSubscribe(pPropertiesListener_type _pListener);

    boost::uint64_t getPropertiesId() const;
    void setPropertiesId(boost::uint64_t _id);

protected:
    friend class Property;
    /// Notify all of the listeners that a property value has been modified.
    void notifyPropertyValueModified(Property* _pProperty);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class GameObjectType;
             Properties();
    virtual ~Properties();
    /// @}

    /// @name Member Variables
    /// @{
private:
    boost::uint64_t                 m_propertiesId;
    pPropertiesDomainObject_type    m_pPropertiesDO;

    /// Guard for m_listeners.
    Zen::Threading::I_Mutex*        m_pListenersGuard;

    Listeners_type                  m_listeners;
    /// @}

};  // class Properties

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // GAME_BUILDER_PROPERTIES_HPP_INCLUDED
