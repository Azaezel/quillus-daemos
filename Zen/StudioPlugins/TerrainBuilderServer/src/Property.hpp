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
#ifndef TERRAIN_BUILDER_PROPERTY_HPP_INCLUDED
#define TERRAIN_BUILDER_PROPERTY_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_Property.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesListener.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>

#include <Zen/Enterprise/Database/I_Persistable.hpp>

#include <Zen/Studio/WorkbenchModel/I_PropertyDataMap.hpp>
#include <Zen/Studio/WorkbenchModel/I_PropertyDomainObject.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
}   // namespace Zen
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Properties;

class Property
:   public virtual Zen::Studio::Workbench::I_Property
,   public Zen::Database::I_Persistable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::I_PropertiesListener*                           pPropertiesListener_type;
    typedef Zen::Studio::Workbench::I_PropertyDataMap::pPropertyDataMap_type        pPropertyDataMap_type;
    typedef Zen::Studio::Workbench::I_PropertyDataMap::pPropertyDomainObject_type   pPropertyDomainObject_type;
    typedef std::map<std::string, Property*>                                        PropertyChildren_type;
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
    void notifyAdd(pPropertiesListener_type _pListener);

    /// @return true if this property can change to _newValue.
    bool canChange(const std::string& _newValue);

    /// Set the value of the property.
    /// The property must notify m_publisher if the value 
    /// is actually modified. m_publisher will notify all of the
    /// listeners.
    void setValue(const std::string& _newValue);

    void setPropertiesId(boost::uint64_t _propertiesId);

    /// Try to load a property.
    /// @return true if the property was loaded or false if the DO is
    ///     not targeting this property. This is generally based on 
    ///     the name of the property.
    bool tryLoadProperty(pPropertyDomainObject_type _pDomainObject);

    /// Get a property by name.
    /// For a property, the name must match the value returned by getName(),
    /// otherwise NULL is returned.
    const Zen::Studio::Workbench::I_Property* getPropertyByName(const std::string& _name) const;

    /// Get the parent Properties that contains this property.
    Properties& getProperties();

    void addChild(const std::string& _name, Property* _pChild);

private:
    /// Called by load() to set the m_pPropertyDO.
    void setPropertyDO(pPropertyDomainObject_type _pPropertyDO);
    /// @}

    /// @name 'Structors
    /// @{
public:
             Property(Properties& _property, const std::string& _name, const std::string& _value, PropertyType _type, Property* _pParent = NULL);
    virtual ~Property();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pPropertyDomainObject_type      m_pPropertyDO;

    Properties&                     m_properties;
    std::string                     m_name;
    std::string                     m_value;
    PropertyType                    m_type;
    bool                            m_canChange;

    Property*                       m_pParent;
    std::string                     m_fullyQualifiedName;

    boost::uint64_t                 m_propertyId;
    boost::uint64_t                 m_propertiesId;

    Zen::Threading::I_Mutex*        m_pChildrenGuard;
    PropertyChildren_type           m_children;
    /// @}

};  // class Property

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_PROPERTY_HPP_INCLUDED
