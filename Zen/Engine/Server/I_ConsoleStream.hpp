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
#ifndef ZEN_ENGINE_SERVER_I_CONSOLE_STREAM_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_I_CONSOLE_STREAM_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ConsoleInputEvent;

/// I_ConsoleStream interface
class GAMESERVER_DLL_LINK I_ConsoleStream
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_ConsoleStream>       pConsoleStream_type;
    typedef Zen::Memory::managed_weak_ptr<I_ConsoleStream>  wpConsoleStream_type;
    typedef Zen::Event::Event<wpConsoleStream_type>         consoleStreamEvent_type;

    typedef Zen::Event::Event<I_ConsoleInputEvent*>  console_input_event;
    /// @}

    /// @name I_ConsoleStream interface
    /// @{
public:
    /// Output a string
    virtual I_ConsoleStream& operator<<(const std::string& _output) = 0;

    virtual void run() = 0;
    virtual void stop() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    consoleStreamEvent_type onDestroyEvent;
    console_input_event     onConsoleInputEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ConsoleStream();
    virtual ~I_ConsoleStream();
    /// @}

};  // interface I_ConsoleStream

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Engine
namespace Memory {
    // I_ConsoleStream is managed by a factory
    template<>
    struct is_managed_by_factory<Engine::Server::I_ConsoleStream> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_I_CONSOLE_STREAM_HPP_INCLUDED
