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
#ifndef ZEN_ENGINE_SERVER_I_COMMAND_LINE_PARSER_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_SERVER_I_COMMAND_LINE_PARSER_SERVICE_HPP_INCLUDED

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

class I_CommandLineParseEvent;
class I_ConsoleInputEvent;
class I_CommandHandler;
class I_ConsoleStream;

/// I_ConsoleService interface
class GAMESERVER_DLL_LINK I_CommandLineParserService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                                 index_type;
    typedef Memory::managed_ptr<I_CommandLineParserService>             pService_type;
    typedef Memory::managed_weak_ptr<I_CommandLineParserService>        wpService_type;
    typedef Event::Event<wpService_type>                                serviceEvent_type;

    typedef Zen::Memory::managed_ptr<I_ConsoleStream>                   pConsoleStream_type;

    typedef std::list<std::string>                                      parameterList_type;
    typedef boost::function<std::string(const pConsoleStream_type&, const parameterList_type&)>     function_type;
    /// @}

    /// @name I_CommandLineParserService interface
    /// @{
public:
    virtual void registerHandler(const std::string& _command, function_type _handler, const std::string& _usage) = 0;
    virtual void unregisterHandler(const std::string& _command) = 0;
    virtual void parse(I_ConsoleInputEvent* _pConsoleInputEvent) = 0;
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
             I_CommandLineParserService();
    virtual ~I_CommandLineParserService();
    /// @}

};  // interface I_CommandLineParserService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Engine
namespace Memory {
    // I_CommandLineParserService is managed by I_CommandLineParserServiceFactory
    template<>
    struct is_managed_by_factory<Engine::Server::I_CommandLineParserService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SERVER_I_COMMAND_LINE_PARSER_SERVICE_HPP_INCLUDED
