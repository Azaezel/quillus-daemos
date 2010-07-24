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
#ifndef ZEN_DATABASE_I_DATABASE_SCHEMA_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_SCHEMA_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/shared_ptr.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_MetaTable;
class I_MetaTableIterator;
class I_MetaView;
class I_MetaViewIterator;
class I_MetaFieldIterator;

class DATABASE_DLL_LINK I_DatabaseSchema
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_MetaTableIterator> pMetaTableIterator_type;
    typedef Zen::Memory::managed_ptr<I_MetaViewIterator>  pMetaViewIterator_type;

    typedef boost::shared_ptr<I_MetaTable>          meta_table_ptr_type;
    typedef boost::shared_ptr<I_MetaView>           meta_view_ptr_type;
    /// @}

    /// @name I_DatabaseSchema interface
    /// @{
public:

    /// Get the tables in the database schema.
    /// @return Pointer to an iterator referencing the beginning element in a collection of tables.
    virtual pMetaTableIterator_type getTables() const = 0;

    /// Set the tables in the database schema.
    /// @param _tables Pointer to an iterator referencing the beginning element in a collection of tables.
    virtual void setTables(const pMetaTableIterator_type _tables) = 0;

    /// Add a table to the database schema.
    /// @param _table Pointer to a table.
    /// @return Pointer to an iterator referencing the added table.
    virtual pMetaTableIterator_type addTable(const meta_table_ptr_type& _table) const = 0;

    /// Drop a table from this schema.
    /// @param _tableName Name of the table to drop.
    virtual void dropTable(const std::string& _tableName) = 0;

    /// Drop a table from the database schema.
    /// @param _table Pointer to an iterator referencing the table to drop.
    virtual void dropTable(const pMetaTableIterator_type _table) = 0;

    /// Get the views in the database schema.
    /// @return Pointer to an iterator referencing the beginning element in a collection of views.
    virtual pMetaViewIterator_type getViews() const = 0;

    /// Set the views in the database schema.
    /// @param _views Pointer to an iterator referencing the beginning element in a collection of views.
    virtual void setViews(const pMetaTableIterator_type _views) = 0;

    /// Add a view to the database schema.
    /// @param _view Pointer to a view.
    /// @return Pointer to an iterator referencing the added view.
    virtual pMetaViewIterator_type addView(const meta_view_ptr_type& _view) const = 0;

    /// Drop a view from the database schema.
    /// @param _viewName Name of the view to drop.
    virtual void dropView(const std::string& _viewName) = 0;

    /// Drop a view from the database schema.
    /// @param _view Pointer to an iterator referencing the view to drop.
    virtual void dropView(const pMetaViewIterator_type _view) = 0;

    /// Set the name of the database schema.
    /// @param _name Name of the database schema.
    virtual void setName(const std::string& _name) = 0;

    /// Get the name of the database schema.
    /// @return Name of the database schema.
    virtual std::string& getName() const = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseSchema();
    virtual ~I_DatabaseSchema();
    /// @}

};  // class I_DatabaseSchema

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_SCHEMA_HPP_INCLUDED
