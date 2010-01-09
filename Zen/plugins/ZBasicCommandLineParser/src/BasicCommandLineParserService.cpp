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

#include "BasicCommandLineParserService.hpp"
#include "BasicCommandHandler.hpp"

#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>
#include <Zen/Engine/Server/I_ConsoleInputEvent.hpp>

#include <boost/spirit/core.hpp>
#include <boost/spirit/dynamic.hpp>
#include <boost/spirit/utility/lists.hpp>
#include <boost/bind.hpp>

#include <iostream>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBasicCommandLineParser {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BasicCommandLineParserService::BasicCommandLineParserService()
:   m_firstCommand(true)
{
    this->m_pCommandHandlersMutex = Zen::Threading::MutexFactory::create();
    this->m_commands = boost::spirit::nothing_p;
    this->registerHandler("help",
        boost::bind(&BasicCommandLineParserService::helpFunction,this,_1,_2),
        "Returns the usage of a command.");
    this->registerHandler("quit",
        boost::bind(&BasicCommandLineParserService::quitFunction,this,_1,_2),
        "Terminates the console stream.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BasicCommandLineParserService::~BasicCommandLineParserService()
{
    Zen::Threading::MutexFactory::destroy(this->m_pCommandHandlersMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BasicCommandLineParserService::registerHandler(const std::string& _command, function_type _handler, const std::string& _usage)
{
    BasicCommandHandler* pRawCommandHandler =
        new BasicCommandHandler(_command,_handler,_usage);

    pCommandHandler_type pCommandHandler(pRawCommandHandler,
        boost::bind(&BasicCommandLineParserService::onDestroyCommandHandler,this,_1));

    wpCommandHandler_type pWeakPtr(pCommandHandler);

    pRawCommandHandler->setSelfReference(pWeakPtr);

    {
        Zen::Threading::CriticalSection guard(this->m_pCommandHandlersMutex);

        this->m_commandHandlers[_command] = pCommandHandler;

        this->m_commands = this->m_commands.copy() |
            
            boost::spirit::str_p(pCommandHandler->getCommand().c_str()) [boost::bind(&Zen::Engine::Server::I_CommandHandler::handleCommand, pCommandHandler.get(), _1, _2)]
                >>  *boost::spirit::space_p
                >>  !boost::spirit::list_p(
                        (
                            *boost::spirit::anychar_p
                        )                                               [boost::bind(&Zen::Engine::Server::I_CommandHandler::handleParameter, pCommandHandler.get(), _1, _2)]
                        ,boost::spirit::space_p
                    )
                >>  boost::spirit::end_p                                [boost::bind(&Zen::Engine::Server::I_CommandHandler::done, pCommandHandler.get(), _1, _2, &this->m_pConsoleStream)]
        ;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BasicCommandLineParserService::unregisterHandler(const std::string& _command)
{
    // TODO : Implement
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BasicCommandLineParserService::parse(Zen::Engine::Server::I_ConsoleInputEvent* _pConsoleInputEvent)
{
    const std::string input( _pConsoleInputEvent->getInputString() );

    this->m_pConsoleStream = _pConsoleInputEvent->getConsoleStream();

    boost::spirit::parse_info<iterator_type> result = 
        boost::spirit::parse(input.begin(), input.end(),this->m_commands);

    if (result.full)
    {
        return;
    }
    else
    {
        // TODO We can use result to display a better error, but it's not easy
        std::cout << "Syntax Error" << std::endl;
        return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
BasicCommandLineParserService::helpFunction(const pConsoleStream_type& _pConsoleStream, const parameterList_type& _parameterList)
{
    std::string result("");
    parameterList_type::const_iterator pCommand;
    
    for( pCommand = _parameterList.begin();
        pCommand != _parameterList.end();
        pCommand++ )
    {
        commandHandlers_type::iterator pHandler = this->m_commandHandlers.find(*pCommand);
        if( pHandler != this->m_commandHandlers.end() )
        {
            result = result + *pCommand + std::string(": ") + pHandler->second->getUsage() + std::string("\r\n");
        }
        else
        {
            result = result + *pCommand + std::string(": Invalid command.") + std::string("\r\n");
        }
    }

    return result;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
BasicCommandLineParserService::quitFunction(const pConsoleStream_type& _pConsoleStream, const parameterList_type& _parameterList)
{
    std::string result("Quitting.");
    
    _pConsoleStream->stop();

    return result;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BasicCommandLineParserService::onDestroyCommandHandler(wpCommandHandler_type _wpCommandHandler)
{
    /// Fire the CommandHandler's onDestroyEvent
    _wpCommandHandler->onDestroyEvent(_wpCommandHandler);

    /// Delete the CommandHandler
    BasicCommandHandler* pCommandHandler =
        dynamic_cast<BasicCommandHandler*>(_wpCommandHandler.get());

    if( pCommandHandler )
    {
        delete pCommandHandler;
    }
    else
    {
        throw std::exception("Zen::Server::BasicCommandLineParserService::onDestroyCommandHandler() : _wpCommandHandler is an invalid BasicCommandHandler.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBasicCommandLineParser
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
