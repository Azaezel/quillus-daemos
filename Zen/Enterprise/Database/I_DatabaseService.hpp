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
#ifndef ZEN_DATABASE_I_DATABASE_SERVICE_HPP_INCLUDED
#define ZEN_DATABASE_I_DATABASE_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Database {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_DatabaseConnection;

class DATABASE_DLL_LINK I_DatabaseService
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef std::string                                         index_type;

    typedef Zen::Memory::managed_ptr<I_DatabaseService>         pDatabaseService_type;
    typedef Zen::Memory::managed_weak_ptr<I_DatabaseService>    wpDatabaseService_type;
    typedef Zen::Memory::managed_ptr<I_DatabaseConnection>      pDatabaseConnection_type;

    typedef Zen::Event::Event<wpDatabaseService_type>           ServiceEvent_type;
    typedef Zen::Event::Event<pDatabaseConnection_type>         ConnectionEvent_type;

    typedef const std::map<std::string, std::string>            config_type;
    /// @}

    /// @name I_DatabaseService interface.
    /// @{
public:
    /// Connect to a database.
    /// The connection is maintained until the pDatabaseConnection_type 
    /// goes out of scope or reset is called.
    /// @param _name the name of the connection.  This is useful for maintaining
    ///             and distinguising among multiple connections.
    /// @param _config has service specific configuration information for
    ///             items such as user-id, password, etc.  Plugin implementers
    ///             should attempt to use standardized parameters to reduce
    ///             confusion and to make it easier to switch among database
    ///             types.
    /// @param _asynchronous If false, the call is synchronous, if true, the call is 
    ///             asynchronous.  Either way, the database connection is returned
    ///             using the onConnectedEvent.
    virtual pDatabaseConnection_type connect(const std::string& _name, config_type& _config, bool _asynchronous = false) = 0;
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

    /// Fired when a datbase connection completes
    ConnectionEvent_type onConnectedEvent;

    /// Fired when a database connection fails
    ConnectionEvent_type onConnectionFailedEvent;

    /// Fired when a database connection becomes disconnected,
    /// either as requested or due to an error.
    ConnectionEvent_type onDisconnectedEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_DatabaseService();
    virtual ~I_DatabaseService();
    /// @}

};  // interface I_DatabaseService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Database
namespace Memory 
{
    /// I_DatabaseService is managed by a factory
    template<>
    struct is_managed_by_factory<Database::I_DatabaseService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_DATABASE_I_DATABASE_SERVICE_HPP_INCLUDED
