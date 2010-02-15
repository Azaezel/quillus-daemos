//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
//  Matthew Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_DATABASE_I_META_FIELD_HPP_INCLUDED
#define ZEN_DATABASE_I_META_FIELD_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_DatabaseSchema;
class I_MetaTable;
class I_MetaAttribute;
class I_MetaAttributeIterator;

class DATABASE_DLL_LINK I_MetaField
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_MetaAttributeIterator> pMetaAttributeIterator_type;

    typedef boost::shared_ptr<I_MetaAttribute>    meta_attribute_ptr_type;
    /// @}

    /// @name I_MetaField Interface
    /// @{
public:

    /// Set the field attributes.
    /// @param _metaAttributes Pointer to an iterator referencing the first element in a collection of attributes.
    virtual void setAttributes(const pMetaAttributeIterator_type _metaAttributes) = 0;

    /// Get the field attributes.
    /// @return Pointer to an iterator referencing the first element in a collection of attributes.
    virtual pMetaAttributeIterator_type getAttributes() const = 0;

    /// Create a field attribute.
    /// @param _name Attribute name.
    /// @param _value Attribute value.
    /// @return Pointer to an iterator referencing the created attribute.
    virtual pMetaAttributeIterator_type createAttribute(const std::string& _name, boost::any _value) const = 0;

    /// Drop a field attribute.
    /// @param _name Attribute name.
    virtual void dropAttribute(const std::string& _name) = 0;

    /// Set field name.
    /// @param _name Field name.
    virtual void setName(const std::string& _name) = 0;

    /// Get field name.
    /// @return Field name.
    virtual std::string& getName() const = 0;

    /// Get owning table.
    /// @return Reference to a table.
    virtual I_MetaTable& getTable() const = 0;

    /// Set owning table.
    /// @param Reference to a table.
    virtual void setTable(const I_MetaTable& _metaTable) = 0;

    /// Get owning database schema.
    /// @return Reference to a database schema.
    virtual I_DatabaseSchema& getDatabase() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MetaField();
    virtual ~I_MetaField();
    /// @}

};  // class I_MetaField

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_META_FIELD_HPP_INCLUDED
