//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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
#ifndef ZEN_ENGINE_SERVER_BASIC_CONSOLE_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_BASIC_CONSOLE_SERVICE_HPP_INCLUDED

#include "BasicConsoleServiceFactory.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Event/Connection.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Engine/Server/I_CommandLineParseEvent.hpp>
#include <Zen/Engine/Server/I_ConsoleService.hpp>
#include <Zen/Engine/Server/I_ConsoleStream.hpp>
#include <Zen/Engine/Server/I_CommandLineParserService.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBasicConsole {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BasicConsoleService
:   public Zen::Engine::Server::I_ConsoleService
,   public Zen::Memory::managed_self_ref<Zen::Engine::Server::I_ConsoleService>
{
    /// @name Friend declarations
    /// @{
    friend BasicConsoleServiceFactory;
    /// @}

    /// @name Types
    /// @{
public:
    typedef std::map<pConsoleStream_type,Threading::I_Thread*>                  consoleStreams_type;

    typedef Zen::Event::Connection<Zen::Engine::Server::I_ConsoleInputEvent*>*  pConsoleInputEventConnection_type;
    typedef std::map<pConsoleStream_type,pConsoleInputEventConnection_type>     consoleStreamInputEventConnections_type;

    typedef std::map<std::string,handler_type>                                  commandHandlers_type;

    typedef Zen::Memory::managed_ptr<Zen::Engine::Server::I_CommandHandler>     pCommandHandler_type;
    typedef Zen::Event::Event<pCommandHandler_type>                             parseEvent_type;

    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Server::I_ConsoleStream> wpConsoleStream_type;
    /// @}

    /// @name I_ConsoleService implementation
    /// @{
public:
    /// Register a console command handler.
    virtual void registerCommandHandler(const std::string& _command, handler_type, const std::string& _usage);

    /// Broadcast output to all Console output devices
    virtual void broadcastOutput(const std::string& _output);

    /// Register a console stream to be used by this console.
    virtual void registerStream(pConsoleStream_type& _stream);

    /// De register a console stream to be used by this console.
    virtual void unregisterStream(pConsoleStream_type& _stream);

    virtual void attachCommandLineParser(pCommandLineParserService_type _pCommandLineParserService);

    virtual pConsoleStream_type createStream();

    virtual bool isActive();

    virtual void setActive(bool _isActive);
    /// @}

    /// @name BasicConsoleService implementation
    /// @{
public:
    void onInputEvent(Zen::Engine::Server::I_ConsoleInputEvent* _pConsoleInputEvent);
    void onConsoleStreamDestroy(wpConsoleStream_type _wpConsoleStream);
    /// @}

    /// @name 'Structors
    /// @{
public:
             BasicConsoleService();
    virtual ~BasicConsoleService();
    /// @}

    /// @name Member variables
    /// @{
private:
    Threading::I_Mutex*                     m_pConsoleStreamsGuard;
    consoleStreams_type                     m_consoleStreams;
    consoleStreamInputEventConnections_type m_consoleStreamInputEventConnections;

    pCommandLineParserService_type          m_pCommandLineParser;

    bool                                    m_isActive;

    /// @}

};  // interface I_ConsoleService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBasicConsoleService
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_I_CONSOLE_SERVICE_HPP_INCLUDED
