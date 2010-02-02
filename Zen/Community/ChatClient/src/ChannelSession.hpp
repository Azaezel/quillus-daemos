//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
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
#ifndef ZEN_COMMUNITY_CHAT_CLIENT_CHANNEL_SESSION_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_CLIENT_CHANNEL_SESSION_HPP_INCLUDED

#include <Zen/Community/ChatCommon/I_ChannelSession.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Chat {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ChannelSession
:   public Common::I_ChannelSession
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Object implementation
    /// @{
public:
    virtual pObjectCollection_type getChildObjects(pObjectType_type _pType, pFilter_type _pFilter);
    /// @}

    /// @name I_ChannelSession implementation
    /// @{
public:
    virtual ChannelSessionState_type getState() const;
    virtual pChannel_type getChannel() const;
    virtual pChatSession_type getChatSession() const;
    virtual pSubscription_type getMessages(pView_type _pView, pFilter_type _pFilter);
    virtual pSubscription_type getUsers(pView_type _pView, pFilter_type _pFilter);
    virtual void leaveChannel();
    /// @}

    /// @name ChannelSession implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             ChannelSession();
    virtual ~ChannelSession();
    /// @}

    /// @name Member variables
    /// @{
private:
    /// @}

};  // class ChannelSession

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Chat
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_CLIENT_CHANNEL_SESSION_HPP_INCLUDED
