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
#ifndef ZEN_COMMUNITY_CHATCLIENT_CHAT_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_CHATCLIENT_CHAT_SERVICE_HPP_INCLUDED

#include "ChannelModelProxy.hpp"

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include <Zen/Community/ChatCommon/I_ChatService.hpp>

#include <boost/cstdint.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Threading {
        class I_Mutex;
    }   // namespace Threading
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Community {
    namespace Common {
        class I_ChannelView;
    }   // namespace Common
    namespace Protocol {
        class I_SubscribeChannelModelRequest;
        class I_UnsubscribeChannelModelRequest;
        class I_CreateChannelRequest;
        class I_ReadChannelRequest;
        class I_UpdateChannelRequest;
        class I_DeleteChannelRequest;
    }   // namespace Protocol
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ChatService
:   public Zen::Enterprise::AppServer::scriptable_generic_service
        <Zen::Community::Common::I_ChatService, ChatService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Common::I_ChannelView>             pChannelView_type;
    /// @}

    /// @name I_StartupShutdownParticipant implementation
    /// @{
public:
    virtual void setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config);
    virtual Zen::Threading::I_Condition* prepareToStart(Zen::Threading::ThreadPool& _threadPool);
    virtual void start();
    virtual Zen::Threading::I_Condition* prepareToStop();
    virtual void stop();
    /// @}

    /// @name I_ScriptableType
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ScriptableService implementation
    /// @{
public:
    virtual void registerScriptEngine(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name I_ChatService implementation
    /// @{
public:
    virtual pFutureChannelModel_type createChannelModel();
    virtual pFutureChannelController_type createChannelController(Common::I_ChannelModel& _model);

    virtual boost::uint32_t getAccessFlags() const;
    virtual void setAccessFlags(boost::uint32_t _accessFlags);
    virtual const Common::I_Account& getOwner() const;
    virtual void setOwner(const Common::I_Account& _owner);
    virtual const Common::I_Group& getGroup() const;
    virtual void setGroup(const Common::I_Group& _group);
    /// @}

    /// @name ChatService implementation
    /// @{
public:
    const std::string& getDatabaseConnectionName() const;

    void handleSubscribeChannelModelResponse(pResponse_type _pResponse, Protocol::I_SubscribeChannelModelRequest& _request, ChannelModelProxy::ProxySubscription* _pProxySubscription);
    void handleUnsubscribeChannelModelResponse(pResponse_type _pResponse, Protocol::I_UnsubscribeChannelModelRequest& _request, pChannelView_type _pChannelView);

    void handleCreateChannelResponse(pResponse_type _pResponse, Protocol::I_CreateChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel);
    void handleReadChannelResponse(pResponse_type _pResponse, Protocol::I_ReadChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel);
    void handleUpdateChannelResponse(pResponse_type _pResponse, Protocol::I_UpdateChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel);
    void handleDeleteChannelResponse(pResponse_type _pResponse, Protocol::I_DeleteChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel);

    void handleChannelModelUpdateMessage(pMessage_type _pMessage);

private:
    pScriptModule_type getScriptModule();
    /// @}

    /// @name Static methods
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ChatServiceFactory;
             ChatService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    virtual ~ChatService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pScriptEngine_type                                  m_pScriptEngine;
    Zen::Scripting::script_module*                      m_pScriptModule;
    Scripting::I_ObjectReference*                       m_pScriptObject;
    const Zen::Plugins::I_ConfigurationElement*         m_pDatabaseConfig;

    pChannelModel_type                                  m_pChannelModel;
    pChannelController_type                             m_pChannelController;
    /// @}

};  // class ChatService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHATCLIENT_CHAT_SERVICE_HPP_INCLUDED
