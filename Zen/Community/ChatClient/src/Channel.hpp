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
#ifndef ZEN_COMMUNITY_CHAT_CLIENT_CHANNEL_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_CLIENT_CHANNEL_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Community/ChatCommon/I_Channel.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_ChatService;
    }   // namespace Common
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ChatService;

/// Channel.
/// This represents a stateful channel.
/// @note Should this be scriptable or should Common::I_Channel be scriptable?
///     For now I think it should be here, since the server side currently
///     is not scriptable.
class Channel
:   public Common::I_Channel
,   public Scripting::I_ScriptableType
,   public Memory::managed_self_ref<Channel>
{
    /// @name Types
    /// @{
public:
	typedef Memory::managed_ptr<Channel>                    pChannel_type;
    typedef pChannel_type                                   pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<Channel>        ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;
    typedef ScriptWrapper_type*                             pScriptWrapper_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_PermissibleResource implementation
    /// @{
private:
    virtual boost::uint32_t getAccessFlags() const;
    virtual void setAccessFlags(boost::uint32_t _accessFlags);
    virtual const Common::I_Account& getOwner() const;
    virtual void setOwner(const Common::I_Account& _owner);
    virtual const Common::I_Group& getGroup() const;
    virtual void setGroup(const Common::I_Group& _group);
    /// @}

    /// @name I_Channel implementation
    /// @{
public:
    virtual boost::uint64_t getChannelId();
    virtual const std::string getChannelName();
    virtual const std::string getChannelDescription();
    virtual pFutureChannelMemberModel_type createChannelMemberModel();
    virtual pFutureChannelMemberController_type createChannelMemberController(Common::I_ChannelMemberModel& _model);
    /// @}

    /// @name Channel implementation
    /// @{
public:
    void setChannelId(boost::uint64_t _channelId);
    void setChannelName(const std::string& _name);
    void setChannelDescription(const std::string& _description);
    static void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ChannelModelProxy;
             Channel(ChatService& _parent);
public:
    virtual ~Channel();
    /// @}

    /// @name Member variables
    /// @{
private:
    ChatService&                m_parent;

    boost::uint64_t             m_channelId;
    std::string                 m_name;
    std::string                 m_description;

    pScriptWrapper_type         m_pScriptObject;
    /// @}

};  // class Channel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_CLIENT_CHANNEL_HPP_INCLUDED
