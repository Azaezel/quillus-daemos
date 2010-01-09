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
#ifndef GAME_BUILDER_PROPERTY_GROUP_HPP_INCLUDED
#define GAME_BUILDER_PROPERTY_GROUP_HPP_INCLUDED

#include "Property.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Property Group.
class PropertyGroup
:   public Property
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, Property*>                 PropertyChildren_type;
    /// @}

    /// @name I_Property implementation
    /// @{
public:
    virtual PropertyType getType() const;
    /// @}

    /// @name Property override
    /// @{
public:
    virtual const Zen::Studio::Workbench::I_Property* getPropertyByName(const std::string& _fullName);
    /// @}

    /// @name PropertyGroup implementation
    /// @{
public:
    void addChild(const std::string& _name, Property* _pChild);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PropertyGroup(Properties& _properties, const std::string& _name, Property* _pParent = NULL);
    virtual ~PropertyGroup();
    /// @}

    /// @name Member Variables
    /// @{
private:
    PropertyChildren_type                   m_children;
    /// @}

};  // class PropertyGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // GAME_BUILDER_PROPERTY_GROUP_HPP_INCLUDED
