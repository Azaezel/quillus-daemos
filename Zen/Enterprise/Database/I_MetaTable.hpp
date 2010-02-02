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
#ifndef ZEN_DATABASE_I_META_TABLE_HPP_INCLUDED
#define ZEN_DATABASE_I_META_TABLE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/shared_ptr.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_DatabaseSchema;
class I_MetaField;
class I_MetaFieldIterator;
class I_MetaAttribute;
class I_MetaAttributeIterator;
class I_MetaKey;

class DATABASE_DLL_LINK I_MetaTable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_MetaFieldIterator>     pMetaFieldIterator_type;
    typedef Zen::Memory::managed_ptr<I_MetaAttributeIterator> pMetaAttributeIterator_type;

    typedef boost::shared_ptr<I_MetaAttribute>      meta_attribute_ptr_type;
    typedef boost::shared_ptr<I_MetaField>          meta_field_ptr_type;
    /// @}

    /// @name I_MetaTable interface.
    /// @{
public:

    /// Get table fields.
    /// @return Pointer to an iterator referencing the first element in a collection of fields.
    virtual pMetaFieldIterator_type getFields() const = 0;

    /// Set table fields.
    /// @param _fields Pointer to an iterator referencing the first element in a collection of fields.
    virtual void setFields(const pMetaFieldIterator_type _fields) = 0;

    /// Add a field to the table.
    /// @param _metaField Pointer to a field.
    /// @return Pointer to an iterator referencing the added field.
    virtual pMetaFieldIterator_type addField(const meta_field_ptr_type& _metaField) const = 0;

    /// Drop a field from the table.
    /// @param _fieldName Field name.
    virtual void dropField(const std::string& _fieldName) = 0;

    /// Drop a field from the table.
    /// @param _metaField Pointer to an iterator referencing a field.
    virtual void dropField(pMetaFieldIterator_type _metaField) = 0;

    /// Create a key for the table.
    /// @param _metaKey Table key.
    virtual void createKey(const I_MetaKey& _metaKey) = 0;

    /// Drop a key from the table.
    virtual void dropKey() = 0;

    /// Get the table name.
    /// @return Table name.
    virtual std::string& getName() const = 0;

    /// Set the table name.
    /// @param _name Table name.
    virtual void setName(const std::string& _name) = 0;

    /// Get the owning database schema.
    /// @return Reference to a database schema.
    virtual I_DatabaseSchema& getDatabase() const = 0;

    /// Set the owning database schema.
    /// @param _database Reference to a database schema.
    virtual void setDatabase(const I_DatabaseSchema& _database) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_MetaTable();
    virtual ~I_MetaTable();
    /// @}

};  // class I_MetaTable

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_META_TABLE_HPP_INCLUDED
