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
#include "AccountService.hpp"
#include "AccountModelProxy.hpp"
#include "AccountController.hpp"
#include "GroupModelProxy.hpp"
#include "GroupController.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_Event.hpp>
#include <Zen/Core/Event/I_Action.hpp>
#include <Zen/Core/Event/I_ActionMap.hpp>
#include <Zen/Core/Event/I_Connection.hpp>

#include <Zen/Community/AccountProtocol/I_AccountProtocolManager.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeAccountModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeAccountModelResponse.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeAccountModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeAccountModelResponse.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeGroupModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeGroupModelResponse.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeGroupModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeGroupModelResponse.hpp>
#include <Zen/Community/AccountProtocol/I_AccountModelUpdateMessage.hpp>
#include <Zen/Community/AccountProtocol/I_GroupModelUpdateMessage.hpp>
#include <Zen/Community/AccountProtocol/I_CreateAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_CreateAccountResponse.hpp>
#include <Zen/Community/AccountProtocol/I_ReadAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_ReadAccountResponse.hpp>
#include <Zen/Community/AccountProtocol/I_UpdateAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UpdateAccountResponse.hpp>
#include <Zen/Community/AccountProtocol/I_DeleteAccountRequest.hpp>
#include <Zen/Community/AccountProtocol/I_DeleteAccountResponse.hpp>
#include <Zen/Community/AccountProtocol/I_CreateGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_CreateGroupResponse.hpp>
#include <Zen/Community/AccountProtocol/I_ReadGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_ReadGroupResponse.hpp>
#include <Zen/Community/AccountProtocol/I_UpdateGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UpdateGroupResponse.hpp>
#include <Zen/Community/AccountProtocol/I_DeleteGroupRequest.hpp>
#include <Zen/Community/AccountProtocol/I_DeleteGroupResponse.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::AccountService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service <Zen::Community::Common::I_AccountService, AccountService>(_appServer)
,   m_pAccountModel()
,   m_pAccountController()
,   m_pGroupModel()
,   m_pGroupController()
,   m_pScriptObject(NULL)
,   m_pScriptModule(NULL)
,   m_pScriptEngine()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::~AccountService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
AccountService::getScriptTypeName()
{
    static std::string sm_name("AccountClient");
    return sm_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
AccountService::getScriptObject()
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
AccountService::pScriptModule_type
AccountService::getScriptModule()
{
    return m_pScriptModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::registerScriptEngine(pScriptEngine_type _pScriptEngine)
{
    m_pScriptEngine = _pScriptEngine;

    // TODO change this so the Community module can be shared
    m_pScriptModule = new Zen::Scripting::script_module(_pScriptEngine, "Community");

    m_pScriptModule->addType<AccountService>(getScriptTypeName(), "Account Client Service")
        .createGlobalObject("accountClient", this)
    ;

    m_pScriptModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
AccountService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Community::Account::Protocol::I_AccountProtocolManager::getSingleton().install(getApplicationServer());

    registerMessageHandler(Zen::Community::Account::Protocol::I_AccountModelUpdateMessage::getStaticMessageType(),
        boost::bind(&AccountService::handleAccountModelUpdateMessage, this, _1));

    registerMessageHandler(Zen::Community::Account::Protocol::I_GroupModelUpdateMessage::getStaticMessageType(),
        boost::bind(&AccountService::handleGroupModelUpdateMessage, this, _1));

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::start()
{
    // Allow the super class to start up.
    super::start();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
AccountService::prepareToStop()
{
    return super::prepareToStop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::stop()
{
    super::stop();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::destroyAccountModel(wpAccountModel_type _pModel)
{
    AccountModelProxy* pModel =
        dynamic_cast<AccountModelProxy*>(_pModel.get());

    if( pModel != NULL )
    {
        delete pModel;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Common::I_Account&
AccountService::createAccount(const std::string& _userName, const std::string& _resourceName)
{
	throw Zen::Utility::runtime_exception("AccountService::createAccount(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Common::I_Group&
AccountService::createGroup(const std::string& _groupName)
{
	throw Zen::Utility::runtime_exception("AccountService::createGroup(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Common::I_PermissibleResource&
AccountService::createPermissibleResource(const std::string& _resourceName)
{
	throw Zen::Utility::runtime_exception("AccountService::createPermissibleResource(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::setResourceOwner(Common::I_PermissibleResource& _resource, Common::I_Account& _owner)
{
	throw Zen::Utility::runtime_exception("AccountService::setResourceOwner(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::pFutureAccountModel_type 
AccountService::createAccountModel()
{
    /// TODO Make this async
    pFutureAccountModel_type pModel(new FutureAccountModel_type);

    AccountModelProxy* pRawModel = new AccountModelProxy(*this);

    m_pAccountModel = pAccountModel_type(pRawModel, &destroyAccountModel);
    pModel->setValue(m_pAccountModel);

    createAccountController(*pRawModel);

    return pModel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::destroyAccountController(wpAccountController_type _pController)
{
    AccountController* pController =
        dynamic_cast<AccountController*>(_pController.get());

    if( pController != NULL )
    {
        delete pController;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::pFutureAccountController_type 
AccountService::createAccountController(Common::I_AccountModel& _model)
{
    pFutureAccountController_type pController(
        new FutureAccountController_type
    );

    if( !m_pAccountController.isValid() )
    {
        m_pAccountController = pAccountController_type(
            new AccountController(
                *this,
                _model
            ),
            &destroyAccountController
        );
    }

    pController->setValue(m_pAccountController);

    return pController;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::destroyGroupModel(wpGroupModel_type _pModel)
{
    GroupModelProxy* pModel =
        dynamic_cast<GroupModelProxy*>(_pModel.get());

    if( pModel != NULL )
    {
        delete pModel;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::pFutureGroupModel_type 
AccountService::createGroupModel()
{
    pFutureGroupModel_type pModel(new FutureGroupModel_type);

    GroupModelProxy* pRawModel = new GroupModelProxy(*this);

    m_pGroupModel = pGroupModel_type(pRawModel, &destroyGroupModel);
    pModel->setValue(m_pGroupModel);
    
    createGroupController(*pRawModel);

    return pModel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::destroyGroupController(wpGroupController_type _pController)
{
    GroupController* pController =
        dynamic_cast<GroupController*>(_pController.get());

    if( pController != NULL )
    {
        delete pController;
    }
    else
    {
        // TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::pFutureGroupController_type 
AccountService::createGroupController(Common::I_GroupModel& _model)
{
    pFutureGroupController_type pController(
        new FutureGroupController_type
    );

    if( m_pGroupController == NULL )
    {
        m_pGroupController = pGroupController_type(
            new GroupController(
                *this,
                _model
            ),
            &destroyGroupController
        );
    }

    pController->setValue(m_pGroupController);

    return pController;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint32_t 
AccountService::getAccessFlags() const
{
    throw Zen::Utility::runtime_exception("AccountService::getAccessFlags() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::setAccessFlags(boost::uint32_t _accessFlags)
{
    throw Zen::Utility::runtime_exception("AccountService::setAccessFlags() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Common::I_Account&
AccountService::getOwner() const
{
    throw Zen::Utility::runtime_exception("AccountService::getOwner() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::setOwner(const Common::I_Account& _owner)
{
    throw Zen::Utility::runtime_exception("AccountService::setOwner() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Common::I_Group&
AccountService::getGroup() const
{
    throw Zen::Utility::runtime_exception("AccountService::getGroup() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::setGroup(const Common::I_Group& _group)
{
    throw Zen::Utility::runtime_exception("AccountService::setGroup() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleSubscribeAccountModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_SubscribeAccountModelRequest& _request, AccountModelProxy::ProxySubscription* _pProxySubscription)
{
    // _pView is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_SubscribeAccountModelResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_SubscribeAccountModelResponse*>(_pResponse.get());

    /// TODO Get the proxyId from the response and update _pView with it.
    _pProxySubscription->m_proxyId = pResponse->getProxyId();

    typedef Memory::managed_ptr<AccountModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy update it's list of views and fire the Account event.
    pModelProxy->handleSubscription(_pProxySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUnsubscribeAccountModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UnsubscribeAccountModelRequest& _request, pAccountView_type _pAccountView)
{
    // _pView is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_UnsubscribeAccountModelResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_UnsubscribeAccountModelResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<AccountModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy update it's list of views and fire the Account event.
    pModelProxy->handleUnsubscribe(_pAccountView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleSubscribeGroupModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_SubscribeGroupModelRequest& _request, GroupModelProxy::ProxySubscription* _pProxySubscription)
{
    // _pView is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_SubscribeGroupModelResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_SubscribeGroupModelResponse*>(_pResponse.get());

    /// TODO Get the proxyId from the response and update _pView with it.
    _pProxySubscription->m_proxyId = pResponse->getProxyId();

    typedef Memory::managed_ptr<GroupModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pGroupModel.as<pModelProxy_type>();

    // Let the model proxy update it's list of views and fire the Account event.
    pModelProxy->handleSubscription(_pProxySubscription);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUnsubscribeGroupModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UnsubscribeGroupModelRequest& _request, pGroupView_type _pGroupView)
{
    // _pView is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_UnsubscribeGroupModelResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_UnsubscribeGroupModelResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<GroupModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pGroupModel.as<pModelProxy_type>();

    // Let the model proxy update it's list of views and fire the Account event.
    pModelProxy->handleUnsubscribe(_pGroupView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleCreateAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_CreateAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount)
{
    // _pAccount is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_CreateAccountResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_CreateAccountResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<AccountModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Account event.
    pModelProxy->handleCreateAccount(_pProxyAccount);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleReadAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_ReadAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount)
{
    // _pAccount is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_ReadAccountResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_ReadAccountResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<AccountModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Account event.
    pModelProxy->handleReadAccount(_pProxyAccount);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUpdateAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UpdateAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount)
{
    // _pAccount is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_UpdateAccountResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_UpdateAccountResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<AccountModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Account event.
    pModelProxy->handleUpdateAccount(_pProxyAccount);
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleDeleteAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_DeleteAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount)
{
    // _pAccount is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_DeleteAccountResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_DeleteAccountResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<AccountModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Account event.
    pModelProxy->handleDeleteAccount(_pProxyAccount);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleCreateGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_CreateGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup)
{
    // _pGroup is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_CreateGroupResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_CreateGroupResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<GroupModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pGroupModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Group event.
    pModelProxy->handleCreateGroup(_pProxyGroup);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleReadGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_ReadGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup)
{
    // _pGroup is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_ReadGroupResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_ReadGroupResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<GroupModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pGroupModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Group event.
    pModelProxy->handleReadGroup(_pProxyGroup);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUpdateGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UpdateGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup)
{
    // _pGroup is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_UpdateGroupResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_UpdateGroupResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<GroupModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pGroupModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Group event.
    pModelProxy->handleUpdateGroup(_pProxyGroup);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleDeleteGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_DeleteGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup)
{
    // _pGroup is the payload associated with the original _request.
    // Handle this response.

    Zen::Community::Account::Protocol::I_DeleteGroupResponse* pResponse =
        dynamic_cast<Zen::Community::Account::Protocol::I_DeleteGroupResponse*>(_pResponse.get());

    typedef Memory::managed_ptr<GroupModelProxy>  pModelProxy_type;
    pModelProxy_type pModelProxy = m_pGroupModel.as<pModelProxy_type>();

    // Let the model proxy update and fire the Group event.
    pModelProxy->handleDeleteGroup(_pProxyGroup);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::handleAccountModelUpdateMessage(pMessage_type _pMessage)
{
    Zen::Community::Account::Protocol::I_AccountModelUpdateMessage* pMessage =
        dynamic_cast<Zen::Community::Account::Protocol::I_AccountModelUpdateMessage*>(_pMessage.get());

    typedef Memory::managed_ptr<AccountModelProxy> pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy process the model update message and fire the Account event.
    pModelProxy->handleUpdateNotification(_pMessage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::handleGroupModelUpdateMessage(pMessage_type _pMessage)
{
    Zen::Community::Account::Protocol::I_GroupModelUpdateMessage* pMessage =
        dynamic_cast<Zen::Community::Account::Protocol::I_GroupModelUpdateMessage*>(_pMessage.get());

    typedef Memory::managed_ptr<GroupModelProxy> pModelProxy_type;
    pModelProxy_type pModelProxy = m_pAccountModel.as<pModelProxy_type>();

    // Let the model proxy process the model update message and fire the Group event.
    pModelProxy->handleUpdateNotification(_pMessage);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
