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
#ifndef ZEN_DATABASE_I_SCHEMA_SERVICE_HPP_INCLUDED
#define ZEN_DATABASE_I_SCHEMA_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>

#include <boost/noncopyable.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DatabaseSchemaReader;
class I_DatabaseSchemaWriter;

class DATABASE_DLL_LINK I_SchemaService
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::string                                         index_type;

    typedef I_DatabaseConnection::pDatabaseTransaction_type     pDatabaseTransaction_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseSchemaReader>  pSchemaReader_type;
    typedef Zen::Memory::managed_ptr<I_DatabaseSchemaWriter>  pSchemaWriter_type;

    typedef Zen::Memory::managed_ptr<I_SchemaService>         pSchemaService_type;
    typedef Zen::Memory::managed_weak_ptr<I_SchemaService>    wpSchemaService_type;
    typedef Zen::Event::Event<wpSchemaService_type>          ServiceEvent_type;

    typedef const std::map<std::string, std::string>&           Configuration_type;
    /// @}

    /// @name I_DatabaseService interface.
    /// @{
public:
    /// Create a database schema reader.
    /// @return Pointer to a database schema reader
    virtual pSchemaReader_type createSchemaReader() = 0;

    /// Create a database schema writer.
    /// @return Pointer to a database schema writer
    virtual pSchemaWriter_type createSchemaWriter() = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    /// Fired immediately before this object is destroyed.
    /// The payload is about to be destroyed, so do not keep a reference of it around.
    ServiceEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SchemaService();
    virtual ~I_SchemaService();
    /// @}

};  // interface I_SchemaService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory {
    /// I_DatabaseService is managed by a factory
    template<>
    struct is_managed_by_factory<Database::I_SchemaService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_SCHEMA_SERVICE_HPP_INCLUDED
