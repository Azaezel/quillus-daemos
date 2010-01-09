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
#include "PropertyGroup.hpp"

#include <sstream>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PropertyGroup::PropertyGroup(Properties& _properties, const std::string& _name, Property* _pParent)
:   Property(_properties, _name, "", _pParent)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PropertyGroup::~PropertyGroup()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Property::PropertyType
PropertyGroup::getType() const
{
    return CATEGORY_TYPE;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Studio::Workbench::I_Property*
PropertyGroup::getPropertyByName(const std::string& _fullName)
{
    if (_fullName == getName())
    {
        std::cout << "PropertyGroup::getPropertyByName(): Found it... it's me!" << std::endl;
        return this;
    }
    else
    {
        // Parse the name using "\" as a delimiter.

        const std::string delimiter("\\");
        std::string::const_iterator iter = std::find_first_of(_fullName.begin(), _fullName.end(), delimiter.begin(), delimiter.end());

        // If the delimiter was found...
        if (iter != _fullName.end())
        {
            // Check to make sure the first part is equal to getName(); if it's
            // not then something went wrong.
            std::string firstPart(_fullName.begin(), iter);

            if (firstPart != getName())
            {
                std::cout << "PropertyGroup::getPropertyByName(): Oops, something went wrong." << std::endl;
                return NULL;
            }

            std::string::const_iterator iter2 = std::find_first_of(iter + 1, _fullName.end(), delimiter.begin(), delimiter.end());

            PropertyChildren_type::iterator pChild = m_children.find(std::string(iter + 1, iter2));
            if (pChild != m_children.end())
            {
                return pChild->second->getPropertyByName(std::string(iter + 1, _fullName.end()));
            }
            else
            {
                std::cout << "Child name wasn't in map" << std::endl;
            }
        }
    }

    std::cout << "PropertyGroup::getPropertyByName(): Ooops, didn't find it." << std::endl;
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PropertyGroup::addChild(const std::string& _name, Property* _pChild)
{
    m_children[_name] = _pChild;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
