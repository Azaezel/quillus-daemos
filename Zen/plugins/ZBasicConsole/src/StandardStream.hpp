//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ENGINE_SERVER_DEFAULT_CONSOLE_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_DEFAULT_CONSOLE_HPP_INCLUDED

#include <Zen/Engine/Server/I_ConsoleStream.hpp>
#include <Zen/Engine/Server/I_ConsoleService.hpp>

#include <Zen/Core/Threading/I_Runnable.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBasicConsole {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class StandardStream
:   public Zen::Engine::Server::I_ConsoleStream
,   public Zen::Memory::managed_self_ref<Zen::Engine::Server::I_ConsoleStream>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Engine::Server::I_ConsoleService>          pConsoleService_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Server::I_ConsoleService>     wpConsoleService_type;

    typedef Zen::Event::Event<Zen::Engine::Server::I_ConsoleInputEvent*>  console_input_event;
    /// @}

    /// @name I_ConsoleStream implementation
    /// @{
public:
    /// Output a string
    virtual Zen::Engine::Server::I_ConsoleStream& operator<<(const std::string& _output);
    virtual void run();
    virtual void stop();
    /// @}

    /// @name Events
    /// @{
public:
    ///console_input_event     onConsoleInputEvent;
    /// @}
    
    /// @name StandardStream interface
    /// @{
public:
    /// @}

    /// @name Member Variables
    /// @{
private:
    volatile bool m_shuttingDown;
    /// @}

    /// @name 'Structors
    /// @{
public:
             StandardStream(const pConsoleService_type& _pConsoleService);
    virtual ~StandardStream();
    /// @}

    /// @name Member variables
    /// @{
    wpConsoleService_type   m_wpConsoleService;
    /// @}

};  // interface I_ConsoleStream


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBasicConsole
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_DEFAULT_CONSOLE_HPP_INCLUDED
