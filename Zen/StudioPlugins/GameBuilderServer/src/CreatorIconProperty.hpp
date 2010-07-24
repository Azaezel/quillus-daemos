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
#ifndef GAME_BUILDER_CREATOR_ICON_PROPERTY_HPP_INCLUDED
#define GAME_BUILDER_CREATOR_ICON_PROPERTY_HPP_INCLUDED

#include "Property.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Creator Icon Property.
class CreatorIconProperty
:   public Property
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

    /// @name Property overrides
    /// @{
public:
    /// @}

    /// @name CreatorIconProperty implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             CreatorIconProperty(Properties& _properties, const std::string& _value, Property* _pParent = NULL);
    virtual ~CreatorIconProperty();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class CreatorIconProperty

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAME_BUILDER_CREATOR_ICON_PROPERTY_HPP_INCLUDED
