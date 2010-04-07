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
#ifndef ZEN_COMMUNITY_CHATCOMMON_CHAT_COMMAND_VIEW_HPP_INCLUDED
#define ZEN_COMMUNITY_CHATCOMMON_CHAT_COMMAND_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ChatCommand;

/// ChatCommand View.
/// Implement this interface and use I_ChatCommandModel::subscribe()
/// to be notified of updates to the ChatCommand Model.
class CHATCOMMON_DLL_LINK I_ChatCommandView
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ChatCommandView interface
    /// @{
public:
    /// Notification of when a channel is added to the model.
    virtual void onAddChatCommand(I_ChatCommand& _chatCommand) = 0;

    /// Notification of when a channel is removed from the model.
    virtual void onRemoveChatCommand(I_ChatCommand& _chatCommand) = 0;

    /// Notification of when a channel is updated in the model.
    virtual void onUpdateChatCommand(I_ChatCommand& _chatCommand) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatCommandView();
    virtual ~I_ChatCommandView();
    /// @}

};  // interface I_ChatCommandView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
    /// I_ChatCommandView is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_ChatCommandView> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHATCOMMON_CHAT_COMMAND_VIEW_HPP_INCLUDED
