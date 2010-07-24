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
#include "ChatService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Community/ChatProtocol/I_ChatProtocolManager.hpp>
#include <Zen/Community/ChatProtocol/I_SubscribeChannelModelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_SubscribeChannelModelResponse.hpp>
#include <Zen/Community/ChatProtocol/I_UnsubscribeChannelModelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_UnsubscribeChannelModelResponse.hpp>
#include <Zen/Community/ChatProtocol/I_CreateChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_CreateChannelResponse.hpp>
#include <Zen/Community/ChatProtocol/I_ReadChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_ReadChannelResponse.hpp>
#include <Zen/Community/ChatProtocol/I_UpdateChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_UpdateChannelResponse.hpp>
#include <Zen/Community/ChatProtocol/I_DeleteChannelRequest.hpp>
#include <Zen/Community/ChatProtocol/I_DeleteChannelResponse.hpp>
#include <Zen/Community/ChatProtocol/I_ChannelModelUpdateMessage.hpp>

#include <boost/bind.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::ChatService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service<Zen::Community::Common::I_ChatService, ChatService>(_appServer)
,   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::~ChatService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ChatService::getScriptTypeName()
{
    static std::string sm_name("ChatServer");
    return sm_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
ChatService::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(getScriptModule(),
            getScriptModule()->getScriptType(getScriptTypeName()),
            this
            );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::pScriptModule_type
ChatService::getScriptModule()
{
    return m_pScriptModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    m_pScriptEngine = _pScriptEngine;

    // TODO change this so the Community module can be shared
    static Zen::Scripting::script_module module(_pScriptEngine, "Community");

    m_pScriptModule = &module;

    module.addType<ChatService>(getScriptTypeName(), "Chat Server Service")
        .createGlobalObject("chatServer", this)
    ;

    module.activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);

    /// TODO Get database name from configuration data? -- mgray 02/22/2010
    m_pDatabaseConfig = _config.getChild("database");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
ChatService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Community::Protocol::I_ChatProtocolManager::getSingleton().install(getApplicationServer());

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::start()
{
    super::start();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
ChatService::prepareToStop()
{
    return super::prepareToStop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::stop()
{
    super::stop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::pFutureChannelModel_type 
ChatService::createChannelModel()
{
    throw Zen::Utility::runtime_exception("ChatService::createChannelModel() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ChatService::pFutureChannelController_type 
ChatService::createChannelController(Common::I_ChannelModel& _model)
{
    throw Zen::Utility::runtime_exception("ChatService::createChannelController() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint32_t 
ChatService::getAccessFlags() const
{
    throw Zen::Utility::runtime_exception("ChatService::getAccessFlags() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::setAccessFlags(boost::uint32_t _accessFlags)
{
    throw Zen::Utility::runtime_exception("ChatService::setAccessFlags() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Common::I_Account&
ChatService::getOwner() const
{
    throw Zen::Utility::runtime_exception("ChatService::getOwner() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::setOwner(const Common::I_Account& _owner)
{
    throw Zen::Utility::runtime_exception("ChatService::setOwner() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Common::I_Group&
ChatService::getGroup() const
{
    throw Zen::Utility::runtime_exception("ChatService::getGroup() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::setGroup(const Common::I_Group& _group)
{
    throw Zen::Utility::runtime_exception("ChatService::setGroup() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::handleSubscribeChannelModelResponse(pResponse_type _pResponse, Protocol::I_SubscribeChannelModelRequest& _request, ChannelModelProxy::ProxySubscription* _pProxySubscription)
{
    // _pView is the payload associated with the original _request.
    // Handle this response.

    Protocol::I_SubscribeChannelModelResponse* pResponse =
        dynamic_cast<Protocol::I_SubscribeChannelModelResponse*>(_pResponse.get());

    /// TODO Get the proxyId from the response and update _pView with it.
    _pProxySubscription->m_proxyId = pResponse->getProxyId();

    typedef Memory::managed_ptr<ChannelModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pChannelModel.as<pModelProxy_type>();

    // Let the model proxy update it's list of views and fire the Channel event.
    pModelProxy->handleSubscription(_pProxySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::handleUnsubscribeChannelModelResponse(pResponse_type _pResponse, Protocol::I_UnsubscribeChannelModelRequest& _request, pChannelView_type _pChannelView)
{
    // _pView is the payload associated with the original _request.
    // Handle this response.

    Protocol::I_UnsubscribeChannelModelResponse* pResponse =
        dynamic_cast<Protocol::I_UnsubscribeChannelModelResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<ChannelModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pChannelModel.as<pModelProxy_type>();

    // Let the model proxy update it's list of views and fire the Channel event.
    pModelProxy->handleUnsubscribe(_pChannelView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::handleCreateChannelResponse(pResponse_type _pResponse, Protocol::I_CreateChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel)
{
    // _pChannel is the payload associated with the original _request.
    // Handle this response.

    Protocol::I_CreateChannelResponse* pResponse =
        dynamic_cast<Protocol::I_CreateChannelResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<ChannelModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pChannelModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Channel event.
    pModelProxy->handleCreateChannel(_pProxyChannel);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::handleReadChannelResponse(pResponse_type _pResponse, Protocol::I_ReadChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel)
{
    // _pChannel is the payload associated with the original _request.
    // Handle this response.

    Protocol::I_ReadChannelResponse* pResponse =
        dynamic_cast<Protocol::I_ReadChannelResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<ChannelModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pChannelModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Channel event.
    pModelProxy->handleReadChannel(_pProxyChannel);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::handleUpdateChannelResponse(pResponse_type _pResponse, Protocol::I_UpdateChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel)
{
    // _pChannel is the payload associated with the original _request.
    // Handle this response.

    Protocol::I_UpdateChannelResponse* pResponse =
        dynamic_cast<Protocol::I_UpdateChannelResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<ChannelModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pChannelModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Channel event.
    pModelProxy->handleUpdateChannel(_pProxyChannel);
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
ChatService::handleDeleteChannelResponse(pResponse_type _pResponse, Protocol::I_DeleteChannelRequest& _request, ChannelModelProxy::ProxyChannel* _pProxyChannel)
{
    // _pChannel is the payload associated with the original _request.
    // Handle this response.

    Protocol::I_DeleteChannelResponse* pResponse =
        dynamic_cast<Protocol::I_DeleteChannelResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<ChannelModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pChannelModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Channel event.
    pModelProxy->handleDeleteChannel(_pProxyChannel);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ChatService::handleChannelModelUpdateMessage(pMessage_type _pMessage)
{
    Protocol::I_ChannelModelUpdateMessage* pMessage =
        dynamic_cast<Protocol::I_ChannelModelUpdateMessage*>(_pMessage.get());

    typedef Memory::managed_ptr<ChannelModelProxy> pModelProxy_type;
    pModelProxy_type pModelProxy = m_pChannelModel.as<pModelProxy_type>();

    // Let the model proxy process the model update message and fire the Channel event.
    pModelProxy->handleUpdateNotification(_pMessage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ChatService::getDatabaseConnectionName() const
{
    return m_pDatabaseConfig->getAttribute("connection");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
