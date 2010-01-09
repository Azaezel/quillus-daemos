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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_DATABASE_I_DATABASE_COLUMN_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_COLUMN_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <typeinfo>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DatabaseConnection;
class I_DatabaseResult;

class DATABASE_DLL_LINK I_DatabaseColumn
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseColumn>          pDatabaseColumn_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseColumn>     wpDatabaseColumn_type;
    typedef Zen::Event::Event<wpDatabaseColumn_type>            databaseColumnEvent_type;
    /// @}

    /// @name I_DatabaseColummn interface
    /// @{
public:
    virtual const std::string& getFieldName() const = 0;

    /// Get the sequence number in which this column was
    /// returned in the result set.  The first field is
    /// field sequnce 0, the next field is sequence 1, etc.
    /// @todo TR - Matt, do you really need this on the public interface?
    ///     I'm not sure it belongs.
    //virtual int getFieldSequence() const = 0;

    /// Get the type of this field.
    /// @todo Define the types
    virtual const std::type_info& getType() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    databaseColumnEvent_type    onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseColumn();
    virtual ~I_DatabaseColumn();
    /// @}

};  // interface I_DatabaseColummn

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory {
    // I_DatabaseColumn is managed by I_DatabaseResult
    template<>
    struct is_managed_by_factory<Zen::Database::I_DatabaseColumn> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_DATABASE_I_DATABASE_COLUMN_HPP_INCLUDED
