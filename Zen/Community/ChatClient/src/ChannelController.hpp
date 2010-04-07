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
#ifndef ZEN_COMMUNITY_CLIENT_CHANNEL_CONTROLLER_HPP_INCLUDED
#define ZEN_COMMUNITY_CLIENT_CHANNEL_CONTROLLER_HPP_INCLUDED

#include <Zen/Community/ChatCommon/I_ChannelController.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ChatService;
class ChannelModelProxy;

class ChannelController
:   public Common::I_ChannelController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ChannelController implementation
    /// @{
public:
    virtual Common::I_ChatService& getChatService() const;
    virtual pFutureChannel_type createChannel(Common::I_Session& _session, const std::string& _name, const std::string& _description);
    virtual pFutureChannel_type getChannel(Common::I_Session& _session, const std::string& _name);
    virtual void updateChannel(Common::I_Session& _session, Common::I_Channel& _channel);
    virtual void removeChannel(Common::I_Session& _session, Common::I_Channel& _channel);
    virtual void getChannels(Common::I_Session& _session, I_ChannelVisitor& _visitor);
    /// @}

    /// @name ChannelController implementation
    /// @{
public:
    ChannelModelProxy& getModel() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit ChannelController(ChatService& _service, Common::I_ChannelModel& _model);
    virtual ~ChannelController();
    /// @}

    /// @name Member variables
    /// @{
private:
    ChatService&                    m_service;
    ChannelModelProxy&              m_model;
    /// @}

};  // class ChannelController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CLIENT_CHANNEL_CONTROLLER_HPP_INCLUDED
