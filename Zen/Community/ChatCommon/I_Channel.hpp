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
#ifndef ZEN_COMMUNITY_CHATCOMMON_I_CHANNEL_HPP_INCLUDED
#define ZEN_COMMUNITY_CHATCOMMON_I_CHANNEL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Community/AccountCommon/I_PermissibleResource.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Session;
class I_ChannelMemberModel;         // This will be a non-persisted model
class I_ChannelMemberController;

/// I_Channel
class CHATCOMMON_DLL_LINK I_Channel
:   public I_PermissibleResource
{
    /// @name Types
    /// @{
public:
//    typedef Memory::managed_ptr<I_Channel>                              pChannel_type;
//
//    typedef pChannel_type                                               pScriptObject_type;
//    typedef Zen::Scripting::ObjectReference<I_Channel>                  ScriptObjectReference_type;
//    typedef ScriptObjectReference_type                                  ScriptWrapper_type;
//    typedef ScriptWrapper_type*                                         pScriptWrapper_type;

    typedef Memory::managed_ptr<I_ChannelMemberModel>                   pChannelMemberModel_type;
    typedef Memory::managed_weak_ptr<I_ChannelMemberModel>              wpChannelMemberModel_type;
    typedef Event::future_return_value<pChannelMemberModel_type>        FutureChannelMemberModel_type;
    typedef Memory::managed_ptr<FutureChannelMemberModel_type>          pFutureChannelMemberModel_type;

    typedef Memory::managed_ptr<I_ChannelMemberController>              pChannelMemberController_type;
    typedef Memory::managed_weak_ptr<I_ChannelMemberController>         wpChannelMemberController_type;
    typedef Event::future_return_value<pChannelMemberController_type>   FutureChannelMemberController_type;
    typedef Memory::managed_ptr<FutureChannelMemberController_type>     pFutureChannelMemberController_type;
    /// @}

    /// @name I_Channel interface
    /// @{
public:
    virtual boost::uint64_t getChannelId() = 0;

    virtual const std::string getChannelName() = 0;

    virtual const std::string getChannelDescription() = 0;

    virtual pFutureChannelMemberModel_type createChannelMemberModel() = 0;

    virtual pFutureChannelMemberController_type createChannelMemberController(I_ChannelMemberModel& _model) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Channel();
    virtual ~I_Channel();
    /// @}

};  // interface I_Channel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
	/// I_Channel is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_Channel> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHATCOMMON_I_CHANNEL_HPP_INCLUDED
