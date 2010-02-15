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
#ifndef ZEN_ENGINE_SERVER_BASIC_COMMAND_LINE_PARSER_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_BASIC_COMMAND_LINE_PARSER_SERVICE_HPP_INCLUDED

#include "BasicCommandLineParserServiceFactory.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Engine/Server/I_CommandHandler.hpp>
#include <Zen/Engine/Server/I_CommandLineParseEvent.hpp>
#include <Zen/Engine/Server/I_CommandLineParserService.hpp>

#include <boost/spirit/core.hpp>
#include <boost/spirit/dynamic.hpp>
#include <boost/spirit/utility/lists.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBasicCommandLineParser {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BasicCommandLineParserService
:   public Zen::Engine::Server::I_CommandLineParserService
,   public Zen::Memory::managed_self_ref<Zen::Engine::Server::I_CommandLineParserService>
{
    /// @name Friend declarations
    /// @{
    friend BasicCommandLineParserServiceFactory;
    /// @}

    /// @name Types
    /// @{
public:
    typedef std::string::const_iterator                                             iterator_type;
    typedef boost::spirit::scanner<iterator_type>                                   scanner_type;
    typedef boost::spirit::stored_rule<scanner_type>                                stored_rule_type;
    typedef Zen::Memory::managed_ptr<Zen::Engine::Server::I_CommandHandler>         pCommandHandler_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Server::I_CommandHandler>    wpCommandHandler_type;
    typedef std::map<std::string,pCommandHandler_type>                              commandHandlers_type;

    typedef Zen::Event::Event<pCommandHandler_type>                                 parseEvent_type;
    /// @}

    /// @name I_CommandLineParserService implementation
    /// @{
public:
    virtual void registerHandler(const std::string& _command, function_type _handler, const std::string& _usage);
    virtual void unregisterHandler(const std::string& _command);
    virtual void parse(Zen::Engine::Server::I_ConsoleInputEvent* _pConsoleInputEvent);
    /// @}

    /// @name BasicCommandLineParserService implementation
    /// @{
public:
    std::string helpFunction(const pConsoleStream_type& _pConsoleStream, const parameterList_type& _parameterList);
    std::string quitFunction(const pConsoleStream_type& _pConsoleStream, const parameterList_type& _parameterList);
    /// @}

    /// @name Event handlers
    /// @{
public:
    void onDestroyCommandHandler(wpCommandHandler_type _wpCommandHandler);
    /// @}

    /// @name 'Structors
    /// @{
public:
             BasicCommandLineParserService();
    virtual ~BasicCommandLineParserService();
    /// @}

    /// @name Visitors
    /// @{
private:

    /// @name Member variables
    /// @{
private:
    bool                                        m_firstCommand;
    stored_rule_type                            m_commands;
    std::map<std::string,pCommandHandler_type>  m_commandHandlers;
    Zen::Threading::I_Mutex*                    m_pCommandHandlersMutex;
    Zen::Memory::managed_ptr<Zen::Engine::Server::I_ConsoleStream>  m_pConsoleStream;
    /// @}

};  // class BasicCommandLineParserService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBasicCommandLineParser
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_BASIC_COMMAND_LINE_PARSER_SERVICE_HPP_INCLUDED
