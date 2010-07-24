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
#ifndef ZEN_DATABASE_I_DATABASE_MANAGER_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_DatabaseService;
class I_SchemaService;

class DATABASE_DLL_LINK I_DatabaseManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_DatabaseService>       pDatabaseService_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseService>  wpDatabaseService_type;

    typedef Zen::Memory::managed_ptr<I_SchemaService>         pSchemaService_type;
    typedef Zen::Memory::managed_weak_ptr<I_SchemaService>    wpSchemaService_type;

    typedef std::map<std::string, std::string>                config_type;
    /// @}

    /// @name I_DatabaseManager interface
    /// @{
public:

    /// Create a database service
    /// @param _type Type of database service.  This is generally lower case 
    ///             (mysql, postgres, sybase, etc)
    /// @param _config Configuration.  Additional parameters that are required 
    ///             by the database service initialization.
    /// @return Pointer to a database service
    virtual pDatabaseService_type createDatabaseService(const std::string& _type, config_type& _config) = 0;

    /// Create a schema service
    /// @param _type Type of schema service.  This is generally lower case 
    ///             (dbdesigner, mysql, postgres, sybase, etc)
    /// @param _config Configuration.  Additional parameters that are required 
    ///             by the schema service initialization.
    /// @return Pointer to a schema service
    virtual pSchemaService_type createSchemaService(const std::string& _type, config_type& _config) = 0;
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get the singleton
    /// @return Reference to the singleton instance.
    static I_DatabaseManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseManager();
    virtual ~I_DatabaseManager();
    /// @}

};  // interface I_DatabaseManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_MANAGER_HPP_INCLUDED
