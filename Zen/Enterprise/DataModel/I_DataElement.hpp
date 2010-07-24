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
#ifndef ZEN_DATAMODEL_I_DATA_ELEMENT_HPP_INCLUDED
#define ZEN_DATAMODEL_I_DATA_ELEMENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/any.hpp>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace DataModel {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class DATAMODEL_DLL_LINK I_DataElement
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    enum UnderlyingType
    {
        NULL_TYPE,
        STRING,
        REAL,
        INTEGER,
        UNKNOWN
    };

    typedef Memory::managed_ptr<I_DataElement>              pElement_type;
    /// @}

    /// @name I_DataElement interface.
    /// @{
public:
    /// Get the underlying data type.
    virtual UnderlyingType getUnderlyingType() = 0;

    /// @return true if this data element is a NULL value.
    virtual bool isNull() = 0;

    /// @return true if this data element was modified after
    ///     the last database retrieval or after setDirty(false)
    ///     was called.
    virtual bool isDirty() = 0;

    /// Set the dirty flag for this element.
    virtual void setDirty(bool _dirty) = 0;

    /// @return the data element value as a boost::any
    virtual boost::any getAnyValue() const = 0;
    virtual void setAnyValue(const boost::any& _value) = 0;

    virtual I_DataElement& operator=(const boost::any& _value) = 0;

    /// @return the data element value as a string.
    virtual std::string getStringValue() const = 0;
    virtual I_DataElement& operator=(const std::string& _value) = 0;
    virtual operator std::string() = 0;

    /// @return the data element value as a Math::Real.
    virtual Math::Real getRealValue() const = 0;

    virtual boost::int64_t getInt64Value() const = 0;
    virtual void setInt64Value(boost::int64_t _value) = 0;
    virtual I_DataElement& operator=(boost::int64_t _value) = 0;
    virtual operator boost::int64_t() = 0;

    /// @}

    /// @name Static factory method
    /// @{
public:
    static pElement_type create();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DataElement();
    virtual ~I_DataElement();
    /// @}

};  // interface I_DataElement

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace DataModel
}   // namespace Enterprise
namespace Memory {
    // I_DataElement is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::DataModel::I_DataElement> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATAMODEL_I_DATA_ELEMENT_HPP_INCLUDED
