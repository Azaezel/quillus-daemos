//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_COMMUNITY_CHATCOMMON_CHAT_COMMAND_CONTROLLER_HPP_INCLUDED
#define ZEN_COMMUNITY_CHATCOMMON_CHAT_COMMAND_CONTROLLER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ChatCommand;
class I_ChatCommandModel;
class I_ChatService;
class I_Session;

class CHATCOMMON_DLL_LINK I_ChatCommandController
:   public boost::noncopyable
{
    /// @name Forward declarations
    /// @{
public:
    struct I_ChatCommandVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ChatCommand>                          pChatCommand_type;
    typedef Event::future_return_value<pChatCommand_type>               FutureChatCommand_type;
    typedef Memory::managed_ptr<FutureChatCommand_type>                 pFutureChatCommand_type;
    /// @}

    /// @name I_ChatCommandController interface
    /// @{
public:
    /// Get the chat service
    virtual I_ChatService& getChatService() const = 0;

    /// Create a new chat command
    virtual pFutureChatCommand_type createChatCommand(I_Session& _session, const std::string& _name, const std::string& _description) = 0;

    /// Get an existing chat command
    virtual pFutureChatCommand_type getChatCommand(I_Session& _session, const std::string& _name) = 0;

    /// Update an existing chat command
    virtual void updateChatCommand(I_Session& _session, Common::I_ChatCommand& _chatCommand) = 0;

    /// Remove an existing chat command
    virtual void removeChatCommand(I_Session& _session, Common::I_ChatCommand& _chatCommand) = 0;

    /// Visit all existing chat commands
    virtual void getChatCommands(I_Session& _session, I_ChatCommandVisitor& _visitor) = 0;
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct I_ChatCommandVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_ChatCommand& _chatCommand) = 0;
        virtual void end() = 0;
    };  // interface I_ChatCommandVisitor
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatCommandController();
    virtual ~I_ChatCommandController();
    /// @}

};  // interface I_ChatCommandController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
	/// I_ChatCommandController is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_ChatCommandController> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHATCOMMON_CHAT_COMMAND_CONTROLLER_HPP_INCLUDED
