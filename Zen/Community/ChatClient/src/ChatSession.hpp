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
#ifndef ZEN_COMMUNITY_CHAT_CLIENT_CHAT_SESSION_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_CLIENT_CHAT_SESSION_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Community/ChatCommon/I_ChatSession.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
    namespace Enterprise {
        namespace AppServer {
            class I_ResponseHandler;
            class I_Request;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Community {
namespace Chat {
    namespace Common {
        class I_Channels;
        class I_ChatService;
    }   // namespace Common
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ChatSession
:   public Common::I_ChatSession
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_ChatSession>                      pChatSession_type;
    typedef Memory::managed_weak_ptr<Common::I_ChatSession>                 wpChatSession_type;

    typedef Memory::managed_ptr<Networking::I_Endpoint>                     pEndpoint_type;

    typedef Memory::managed_ptr<Enterprise::AppServer::I_ResponseHandler>   pResponseHandler_type;
    typedef Memory::managed_ptr<Enterprise::AppServer::I_Request>           pRequest_type;

    typedef Memory::managed_ptr<Common::I_Channels>                         pChannels_type;
    typedef Memory::managed_ptr<Common::I_ChatService>                      pService_type;
    /// @}

    /// @name I_Object implementation
    /// @{
public:
    virtual pObjectCollection_type getChildObjects(pObjectType_type _pType, pFilter_type _pFilter);
    /// @}

    /// @name I_ChatSesssion implementation
    /// @{
public:
    virtual ChatSessionState_type getState() const;
    virtual pSession_type getLoginSession() const;
    virtual void disconnect();
    virtual pFutureChannels_type getChannels();
    /// @}

    /// @name ChatSession implementation
    /// @{
public:
private:
    void requestChannels(pEndpoint_type _pDestinationEndpoint,
                         pChatSession_type _pChatSession,
                         pResponseHandler_type _pResponseHandler);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroy(wpChatSession_type _wpChatSession);
    /// @}
    
    /// @name 'Structors
    /// @{
public:
             ChatSession(pService_type _pService, pSession_type _pSession);
    virtual ~ChatSession();
    /// @}

    /// @name Member variables
    /// @{
private:
    pService_type           m_pService;

    ChatSessionState_type   m_state;

    pSession_type           m_pLoginSession;
    /// @}

};  // class ChatSession

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Chat
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_CLIENT_CHAT_SESSION_HPP_INCLUDED
