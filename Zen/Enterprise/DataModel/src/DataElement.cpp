//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "DataElement.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace DataModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::DataElement()
:   m_type(NULL_TYPE)
,   m_dirty(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::~DataElement()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::UnderlyingType
DataElement::getUnderlyingType()
{
    return m_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DataElement::isNull()
{
    return m_type == NULL_TYPE;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
DataElement::isDirty()
{
    return m_dirty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DataElement::setDirty(bool _dirty)
{
    m_dirty = _dirty;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::any
DataElement::getAnyValue() const
{
    return m_value;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DataElement::setAnyValue(const boost::any& _value)
{
    m_type = UNKNOWN;
    m_value = _value;
    setDirty(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_DataElement&
DataElement::operator=(const boost::any& _value)
{
    setAnyValue(_value);
    m_type = UNKNOWN;
    setDirty(true);
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
DataElement::getStringValue() const
{
    if (m_type == STRING)
    {
        return boost::any_cast<std::string>(m_value);
    }

    std::stringstream value;

    switch(m_type)
    {
    case NULL_TYPE:
        break;
    case INTEGER:
        value << boost::any_cast<boost::int64_t>(m_value);
        break;
    case REAL:
        value << boost::any_cast<Math::Real>(m_value);
        break;
    case UNKNOWN:
        if (m_value.type() == typeid(std::string))
        {
            return boost::any_cast<std::string>(m_value);
        }
        else if (m_value.type() == typeid(boost::int64_t))
        {
            value << boost::any_cast<boost::int64_t>(m_value);
            return value.str();
        }
        else if (m_value.type() == typeid(boost::uint64_t))
        {
            value << boost::any_cast<boost::uint64_t>(m_value);
            return value.str();
        }
        // No break here.  Error if type wasn't found.
    default:
        // TODO Implement type coercion
        throw Utility::runtime_exception("DataElement::getStringValue(): Error, not implemented");
    }

    return value.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_DataElement&
DataElement::operator=(const std::string& _value)
{
    m_value = _value;
    m_type = STRING;
    setDirty(true);
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::operator std::string()
{
    return getStringValue();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real
DataElement::getRealValue() const
{
    throw Utility::runtime_exception("DataElement::getRealValue(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::int64_t
DataElement::getInt64Value() const
{
    if (m_type == INTEGER)
    {
        return boost::any_cast<boost::int64_t>(m_value);
    }

    switch(m_type)
    {
    case UNKNOWN:
        if (m_value.type() == typeid(std::string))
        {
            std::string value = boost::any_cast<std::string>(m_value);
            return strtol(value.c_str(), NULL, 10);
        }
        else if (m_value.type() == typeid(boost::int64_t))
        {
            return boost::any_cast<boost::int64_t>(m_value);
        }
        else if (m_value.type() == typeid(boost::uint64_t))
        {
            return boost::any_cast<boost::uint64_t>(m_value);
        }
        break;
    }

    throw Utility::runtime_exception("DataElement::getInt64Value(): Error, not implemented");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
DataElement::setInt64Value(boost::int64_t _value)
{
    m_value = _value;
    m_type = INTEGER;
    setDirty(true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_DataElement&
DataElement::operator=(boost::int64_t _value)
{
    setInt64Value(_value);
    return *this;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
DataElement::operator boost::int64_t()
{
    return getInt64Value();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace DataModel
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
