//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ENGINE_SERVER_I_SERVER_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_I_SERVER_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ConsoleService;

/// @brief Basic game server interface
///
/// A game server can be any kind of server, from a standalone FPS game server
/// to individual compoenent servers for an MMO game such as a chat server,
/// master server, etc.  Note that this is the actual server service, not the
/// remote proxy service.
/// @see Community::I_LobbyService
/// @see GameCore
class GAMESERVER_DLL_LINK I_ServerService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                         index_type;

    typedef Zen::Memory::managed_ptr<I_ServerService>         pServerService_type;
    typedef Zen::Memory::managed_weak_ptr<I_ServerService>    wpServerService_type;

    typedef Zen::Event::Event<wpServerService_type>          ServiceEvent_type;
    /// @}

    /// @name I_ServerService interface
    /// @{
public:
    /// Attach a console to the game server.
    /// The game server will register all of it's commands
    /// to the console.
    /// Once the console is attached, call detachConsole() to 
    /// detach it.
    virtual void attachConsole(I_ConsoleService& _console) = 0;

    /// Detach a console from the game server.
    /// The game server will unregister all of its console commands
    /// before detaching.
    virtual void detachConsole(I_ConsoleService& _console) = 0;

    /// Initialize the server service
    virtual void initialize() = 0;

    virtual void run() = 0;
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
    /// Fired just before this object is destroyed.
    /// The object will be destroyed, so do not promote the weak pointer to
    /// a full managed pointer.
    ServiceEvent_type       onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ServerService();
    virtual ~I_ServerService();
    /// @}

};  // interface I_ServerService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Engine
namespace Memory 
{
    /// I_ServerService is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Server::I_ServerService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_I_SERVER_SERVICE_HPP_INCLUDED
