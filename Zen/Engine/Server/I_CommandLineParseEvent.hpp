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
#ifndef ZEN_ENGINE_SERVER_I_COMMAND_LINE_PARSE_EVENT_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_I_COMMAND_LINE_PARSE_EVENT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Engine/Server/I_ConsoleStream.hpp>
#include <Zen/Engine/Server/I_CommandHandler.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// I_CommandLineParseEvent interface
class GAMESERVER_DLL_LINK I_CommandLineParseEvent
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_CommandLineParseEvent>       pCommandLineParseEvent_type;
    typedef Zen::Memory::managed_weak_ptr<I_CommandLineParseEvent>  wpCommandLineParseEvent_type;
    typedef Zen::Event::Event<wpCommandLineParseEvent_type>         destroyEvent_type;

    typedef Zen::Memory::managed_ptr<I_ConsoleStream>               pConsoleStream_type;
    typedef Zen::Memory::managed_ptr<I_CommandHandler>              pCommandHandler_type;
    /// @}

    /// @name I_CommandLineParseEvent interface
    /// @{
public:
    virtual pConsoleStream_type getConsoleStream() const = 0;
    virtual void setConsoleStream(const pConsoleStream_type& _pConsoleStream) = 0;

    virtual pCommandHandler_type getCommandHandler() const = 0;
    virtual void setCommandHandler(const pCommandHandler_type& _pCommandHandler) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    destroyEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CommandLineParseEvent();
    virtual ~I_CommandLineParseEvent();
    /// @}

    /// @name Member variables
    /// @{
private:
    pConsoleStream_type     m_pConsoleStream;
    pCommandHandler_type    m_pCommandHandler;
    /// @}

};  // interface I_CommandLineParseEvent

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Engine
namespace Memory {
    // I_CommandLineParseEvent is managed by a factory
    template<>
    struct is_managed_by_factory<Engine::Server::I_CommandLineParseEvent> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_I_COMMAND_LINE_PARSE_EVENT_HPP_INCLUDED
