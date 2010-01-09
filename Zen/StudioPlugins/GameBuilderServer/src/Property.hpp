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
#ifndef GAME_BUILDER_PROPERTY_HPP_INCLUDED
#define GAME_BUILDER_PROPERTY_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_Property.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertyDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Game Object Type Property.
/// @note Does the element property need to be different from a document property?
///         For now I'm going to assume not.
class Property
:   public virtual Zen::Studio::Workbench::I_Property
,   public Zen::Database::I_Persistable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_PropertiesListener*       pPropertiesListener_type;

    typedef Zen::Studio::Workbench::I_PropertyDataMap::pPropertyDataMap_type                pPropertyDataMap_type;
    typedef Zen::Studio::Workbench::I_PropertyDataMap::pPropertyDomainObject_type           pPropertyDomainObject_type;
    /// @}

    /// @name I_Property implementation
    /// @{
public:
    virtual const std::string& getName() const;
    virtual const std::string& getFullName() const;
    virtual const std::string& getValue() const;
    virtual PropertyType getType() const;
    virtual Zen::Studio::Workbench::I_Property* getParent();
    virtual Zen::Studio::Workbench::I_PropertiesPublisher& getPublisher();
    /// @}

    /// @name I_Persistable implementation
    /// @{
public:
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name Property implementation
    /// @{
public:
    /// Notify that the property has been added.
    /// This method gives the property an opportunity of adding
    /// child properties.
    virtual void notifyAdd(pPropertiesListener_type _pListener);

    /// @return true if this property can change to _newValue.
    virtual bool canChange(const std::string& _newValue);

    /// Set the value of the property.
    /// The property must notify m_publisher if the value 
    /// is actually modified.  m_publisher will notify all of the
    /// listeners.
    virtual void setValue(const std::string& _newValue);

    virtual void setPropertiesId(boost::uint64_t _propertiesId);

    /// Try to load a property.
    /// @return true if the property was loaded or false if the DO is
    ///         not targeting this property.  This is generally based on
    ///         the name of the property.
    virtual bool tryLoadProperty(pPropertyDomainObject_type _pDomainObject);

private:
    /// Called by load() to set the m_pPropertyDO.
    void setPropertyDO(pPropertyDomainObject_type _pPropertyDO);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Property(Zen::Studio::Workbench::I_PropertiesPublisher& _publisher, const std::string& _name, const std::string& _value, Property* _pParent = NULL);
    virtual ~Property();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pPropertyDomainObject_type  m_pPropertyDO;

    Zen::Studio::Workbench::I_PropertiesPublisher&  m_publisher;
    std::string         m_name;
    std::string         m_value;
    Property*           m_pParent;
    std::string         m_fullyQualifiedName;

    boost::uint64_t     m_propertyId;
    boost::uint64_t     m_propertiesId;
    /// @}

};  // class Property

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // GAME_BUILDER_PROPERTY_HPP_INCLUDED
