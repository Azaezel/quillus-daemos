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
#ifndef GAME_BUILDER_PERSISTENCE_PROPERTY_HPP_INCLUDED
#define GAME_BUILDER_PERSISTENCE_PROPERTY_HPP_INCLUDED

#include "Property.hpp"

#include <Zen/Studio/WorkbenchCommon/I_EnumProperty.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Persistence Property.
class PersistenceProperty
:   public Property
,   public Zen::Studio::Workbench::I_EnumProperty
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Property implementation
    /// @{
public:
    virtual const std::string& getName() const { return Property::getName(); }
    virtual const std::string& getFullName() const { return Property::getFullName(); }
    virtual const std::string& getValue() const { return Property::getValue(); }
    virtual PropertyType getType() const;
    virtual Zen::Studio::Workbench::I_Property* getParent() { return Property::getParent(); }
    virtual Zen::Studio::Workbench::I_PropertiesPublisher& getPublisher() { return Property::getPublisher(); }
    /// @}

    /// @name I_EnumProperty implementation
    /// @{
public:
    virtual void getEnums(I_EnumVisitor& _visitor);
    /// @}

    /// @name Property overrides
    /// @{
public:
    /// @}

    /// @name PersistenceProperty implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             PersistenceProperty(Properties& _properties, const std::string& _value, Property* _pParent = NULL);
    virtual ~PersistenceProperty();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class PersistenceProperty

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // GAME_BUILDER_PERSISTENCE_PROPERTY_HPP_INCLUDED
