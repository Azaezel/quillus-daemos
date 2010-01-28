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
#ifndef GAMEBUILDER_I_GAME_OBJECT_ELEMENT_HPP_INCLUDED
#define GAMEBUILDER_I_GAME_OBJECT_ELEMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_GameObjectTypeDocument;

/// Game Object Element interface.
/// A game object element is an element within a game object type.
class GAMEBUILDERCOMMON_DLL_LINK I_GameObjectElement
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_GameObjectElement interface
    /// @{
public:
    /// Get the document to which this element belongs.
    virtual I_GameObjectTypeDocument& getDocument() const = 0;

    virtual bool isElementNameMutable() const = 0;
    virtual void setElementName(const std::string& _value) = 0;
    virtual const std::string& getElementName() const = 0;

    virtual bool isDataTypeMutable() const = 0;
    virtual void setDataType(const std::string& _type) = 0;
    virtual const std::string& getDataType() const = 0;

    virtual bool isDefaultValueMutable() const = 0;
    virtual void setDefaultValue(const std::string& _value) = 0;
    virtual const std::string& getDefaultValue() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameObjectElement();
    virtual ~I_GameObjectElement();
    /// @}

};  // interface I_GameObjectElement

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // GAMEBUILDER_I_GAME_OBJECT_ELEMENT_HPP_INCLUDED
