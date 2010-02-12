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
#ifndef ZEN_ENGINE_SERVER_BASIC_COMMAND_HANDLER_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_BASIC_COMMAND_HANDLER_HPP_INCLUDED

#include <Zen/Engine/Server/I_CommandHandler.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Server/I_CommandLineParseEvent.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBasicCommandLineParser {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Zen::Engine::Server::I_ConsoleStream;

class BasicCommandHandler
:   public Zen::Engine::Server::I_CommandHandler
,   public Zen::Memory::managed_self_ref<Zen::Engine::Server::I_CommandHandler>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Engine::Server::I_ConsoleStream>   pConsoleStream_type;
    /// @}

    /// @name I_CommandHandler implementation
    /// @{
public:
    virtual const std::string& getCommand() const;
    virtual parameterList_type getParameters() const;
    virtual const std::string& getUsage() const;
    virtual void handleCommand(iterator_type _start, const iterator_type _end);
    virtual void handleParameter(iterator_type _start, const iterator_type _end);
    virtual void done(iterator_type _start, const iterator_type _end, const pConsoleStream_type* _pConsoleStream);
    virtual std::string execute(const pConsoleStream_type& _pConsoleStream);
    /// @}

    /// @name BasicCommandHandler interface
    /// @{
public:
    /// @}

    /// @name Events
    /// @{
public:
    /// @}
    
    /// @name 'Structors
    /// @{
public:
    BasicCommandHandler(const std::string& _command, function_type _function, const std::string& _usage);
    virtual ~BasicCommandHandler();
    /// @}

    /// @name Member Variables
    /// @{
private:
    std::string                 m_command;
    std::string                 m_usage;
    function_type               m_function;
    parameterList_type          m_parameterList;
    /// @}

};  // interface I_ConsoleStream


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBasicCommandLineParser
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_BASIC_COMMAND_HANDLER_HPP_INCLUDED
