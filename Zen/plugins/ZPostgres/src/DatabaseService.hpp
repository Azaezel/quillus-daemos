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
#ifndef INDIEZEN_ZPOSTGRES_DATABASE_SERVICE_HPP_INCLUDED
#define INDIEZEN_ZPOSTGRES_DATABASE_SERVICE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPostgres {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class DatabaseService
:   public Database::I_DatabaseService
,   public Memory::managed_self_ref<Database::I_DatabaseService>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Database::I_DatabaseConnection>     wpDatabaseConnection_type;
    typedef Memory::managed_weak_ptr<Database::I_DatabaseService>        wpDatabaseService_type;

    typedef std::map<std::string,  wpDatabaseConnection_type>                   connections_types;
    /// @}

    /// @name I_DatabaseService implementation
    /// @{
public:
    virtual void connect(const std::string& _name, Configuration_type& _config, bool _asynchronous = false);
    /// @}

    /// @name Event handlers
    /// @{
protected:
    void onDestroy(wpDatabaseConnection_type _pConnection);
    /// @}

    /// @name 'Structors
    /// @{
public:
             DatabaseService(Configuration_type _config);
    virtual ~DatabaseService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Threading::I_Mutex*         m_pConnectionsGuard;
    connections_types           m_namedConnections;
    /// @}

};  // class DatabaseService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPostgres
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_ZPOSTGRES_DATABASE_SERVICE_HPP_INCLUDED
