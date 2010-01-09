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
#ifndef ZEN_DATABASE_DATABASE_MANAGER_HPP_INCLUDED
#define ZEN_DATABASE_DATABASE_MANAGER_HPP_INCLUDED

#include "../I_DatabaseManager.hpp"

#include "../I_DatabaseServiceFactory.hpp"
#include "../I_DatabaseService.hpp"

#include "../I_SchemaServiceFactory.hpp"
#include "../I_SchemaService.hpp"

#include <Zen/Core/Plugins/ServiceCache.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DatabaseManager
:   public I_DatabaseManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Plugins::ServiceCache<I_DatabaseService, I_DatabaseServiceFactory>  database_service_cache_type;
    typedef Zen::Plugins::ServiceCache<I_SchemaService, I_SchemaServiceFactory>      schema_service_cache_type;
    /// @}

    /// @name I_DatabaseManager implementation
    /// @{
public:
    virtual pDatabaseService_type createDatabaseService(const std::string& _type, config_type& _config);
    virtual pSchemaService_type createSchemaService(const std::string& _type, config_type& _config);
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseManager();
    virtual ~DatabaseManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    database_service_cache_type m_databaseServiceCache;
    schema_service_cache_type   m_schemaServiceCache;
    /// @}

};  // class DatabaseManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_MANAGER_HPP_INCLUDED
