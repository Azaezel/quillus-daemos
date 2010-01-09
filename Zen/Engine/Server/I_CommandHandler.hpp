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
#ifndef ZEN_ENGINE_SERVER_I_COMMAND_HANDLER_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_I_COMMAND_HANDLER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_ConsoleStream;

/// I_CommandHandler interface
class GAMESERVER_DLL_LINK I_CommandHandler
{
    /// @name Types
    /// @{
public:
    typedef std::string::const_iterator                         iterator_type;
    typedef Zen::Memory::managed_ptr<I_CommandHandler>          pCommandHandler_type;
    typedef Zen::Memory::managed_weak_ptr<I_CommandHandler>     wpCommandHandler_type;
    typedef Zen::Event::Event<wpCommandHandler_type>            commandHandlerEvent_type;

    typedef Zen::Memory::managed_ptr<I_ConsoleStream>           pConsoleStream_type;

    typedef std::list<std::string>                                                  parameterList_type;
    typedef boost::function<std::string(const pConsoleStream_type&, const parameterList_type&)>    function_type;
    /// @}

    /// @name I_CommandHandler interface
    /// @{
public:
    virtual const std::string& getCommand() const = 0;
    virtual parameterList_type getParameters() const = 0;
    virtual const std::string& getUsage() const = 0;
    virtual void handleCommand(iterator_type _start, const iterator_type _end) = 0;
    virtual void handleParameter(iterator_type _start, const iterator_type _end) = 0;
    virtual void done(iterator_type _start, const iterator_type _end, const pConsoleStream_type* _pConsoleStream) = 0;
    virtual std::string execute(const pConsoleStream_type& _pConsoleStream) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    commandHandlerEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CommandHandler();
    virtual ~I_CommandHandler();
    /// @}

};  // interface I_ConsoleStream

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Engine
namespace Memory {
    // I_CommandHandler is managed by a factory
    template<>
    struct is_managed_by_factory<Engine::Server::I_CommandHandler> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_I_COMMAND_HANDLER_HPP_INCLUDED
