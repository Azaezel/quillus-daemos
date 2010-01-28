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
#ifndef ZEN_COMMUNITY_CHAT_CLIENT_CHAT_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_CHAT_CLIENT_CHAT_SERVICE_HPP_INCLUDED

#include <Zen/Community/ChatCommon/I_ChatService.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
    namespace Enterprise {
        namespace AppServer {
            class I_ApplicationServer;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Community {
namespace Chat {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ChatService
:   public Common::I_ChatService
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>     pEndpoint_type;
    typedef std::map<unsigned int, pResponseHandler_type>   Handlers_type;
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual Threading::I_Condition* prepareToStart(Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Threading::I_Condition* prepareToStop();
    virtual void stop();
    /// @}

    /// @name I_RequestHandler implementation
    /// @{
public:
    virtual void handleRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    /// @}

    /// @name I_ApplicationService implementation
    /// @{
public:
    virtual Zen::Enterprise::AppServer::I_ApplicationServer& getApplicationServer();
    virtual void handleMessage(pMessage_type _pMessage);
    /// @}

    /// @name I_ChatService implementation
    /// @{
public:
    virtual pFutureChatSession_type connect(pSession_type _pSession);
    /// @}

    /// @name ChatService implementation
    /// @{
private:
    void requestChatSession(pEndpoint_type _pDestinationEndpoint,
                            pSession_type _pSession,
                            pResponseHandler_type _pResponseHandler);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ChatServiceFactory;
             ChatService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    virtual ~ChatService();
    /// @}

    /// @name Member variables
    /// @{
private:
    Zen::Enterprise::AppServer::I_ApplicationServer&    m_appServer;
    Zen::Threading::ThreadPool*                         m_pThreadPool;

    /// Map from MessageID() to the response handler
    Handlers_type                                       m_responseHandlers;
    Zen::Threading::I_Mutex*                            m_pHandlersMutex;
    /// @}

};  // class ChatService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Chat
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHAT_CLIENT_CHAT_SERVICE_HPP_INCLUDED
