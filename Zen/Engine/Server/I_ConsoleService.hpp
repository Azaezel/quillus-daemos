//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_SERVER_I_CONSOLE_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_I_CONSOLE_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/function.hpp>

#include <list>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ConsoleStream;
class I_CommandHandler;
class I_CommandLineParserService;

/// I_ConsoleService interface
class GAMESERVER_DLL_LINK I_ConsoleService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                                         index_type;

    typedef Zen::Memory::managed_ptr<I_CommandLineParserService>                pCommandLineParserService_type;

    typedef Memory::managed_ptr<I_ConsoleService>                               pService_type;
    typedef Memory::managed_weak_ptr<I_ConsoleService>                          wpService_type;
    typedef Event::Event<wpService_type>                                        serviceEvent_type;

    typedef Memory::managed_ptr<I_ConsoleStream>                                pConsoleStream_type;

    typedef boost::function<std::string(const pConsoleStream_type&, const std::list<std::string>& _parameters)>   handler_type;
    /// @}

    /// @name I_ConsoleService interface
    /// @{
public:
    /// Register a console command handler.
    virtual void registerCommandHandler(const std::string& _command, handler_type, const std::string& _usage) = 0;

    /// Broadcast output to all Console output devices
    virtual void broadcastOutput(const std::string& _output) = 0;

    /// Register a console stream to be used by this console.
    virtual void registerStream(pConsoleStream_type& _pStream) = 0;

    /// De register a console stream to be used by this console.
    virtual void unregisterStream(pConsoleStream_type& _pStream) = 0;

    virtual void attachCommandLineParser(pCommandLineParserService_type _pCommandLineParserService) = 0;

    virtual pConsoleStream_type createStream() = 0;

    virtual bool isActive() = 0;

    virtual void setActive(bool _isActive) = 0;
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
    serviceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ConsoleService();
    virtual ~I_ConsoleService();
    /// @}

};  // interface I_ConsoleService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Engine
namespace Memory {
    // I_ConsoleService is managed by I_ConsoleServiceFactory
    template<>
    struct is_managed_by_factory<Engine::Server::I_ConsoleService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_I_CONSOLE_SERVICE_HPP_INCLUDED
