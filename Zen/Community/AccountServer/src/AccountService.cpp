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
#include "AccountModel.hpp"
#include "AccountController.hpp"
#include "Account.hpp"
#include "GroupModel.hpp"
#include "GroupController.hpp"
#include "Group.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Enterprise/AppServer/I_Request.hpp>
#include <Zen/Enterprise/AppServer/I_ResponseHandler.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

#include <Zen/Enterprise/Database/I_DatabaseService.hpp>

#include <Zen/Community/AccountProtocol/I_AccountProtocolManager.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeAccountModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeAccountModelResponse.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeAccountModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeAccountModelResponse.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeGroupModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_SubscribeGroupModelResponse.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeGroupModelRequest.hpp>
#include <Zen/Community/AccountProtocol/I_UnsubscribeGroupModelResponse.hpp>
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

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AccountService::AccountService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
:   Zen::Enterprise::AppServer::scriptable_generic_service<Zen::Community::Common::I_AccountService, AccountService>(_appServer)
,   m_pScriptObject(NULL)
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
    static std::string sm_name("AccountServer");
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
    static Zen::Scripting::script_module module(_pScriptEngine, "Community");

    m_pScriptModule = &module;

    module.addType<AccountService>(getScriptTypeName(), "Account Server Service")
        .createGlobalObject("accountServer", this)
    ;

    module.activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::setConfiguration(const Zen::Plugins::I_ConfigurationElement& _config)
{
    super::setConfiguration(_config);

    /// TODO Get database name from configuration data? -- mgray 02/22/2010
    m_pDatabaseConfig = _config.getChild("database");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
AccountService::prepareToStart(Zen::Threading::ThreadPool& _threadPool)
{
    Zen::Community::Protocol::I_AccountProtocolManager::getSingleton().install(getApplicationServer());

    registerRequestHandler(Protocol::I_SubscribeAccountModelRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleSubscribeAccountRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_UnsubscribeAccountModelRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleUnsubscribeAccountRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_SubscribeGroupModelRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleSubscribeGroupRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_UnsubscribeGroupModelRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleUnsubscribeGroupRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_CreateAccountRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleCreateAccountRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_ReadAccountRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleReadAccountRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_UpdateAccountRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleUpdateAccountRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_DeleteAccountRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleDeleteAccountRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_CreateGroupRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleCreateGroupRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_ReadGroupRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleReadGroupRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_UpdateGroupRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleUpdateGroupRequest, this, _1, _2));

    registerRequestHandler(Protocol::I_DeleteGroupRequest::getStaticMessageType(),
        boost::bind(&AccountService::handleDeleteGroupRequest, this, _1, _2));

    //getApplicationServer().registerDefaultAccountService(getSelfReference().lock());

    return super::prepareToStart(_threadPool);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AccountService::start()
{
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
    AccountModel* pModel =
        dynamic_cast<AccountModel*>(_pModel.get());

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

    AccountModel* pRawModel = new AccountModel(*this);

    m_pAccountModel = pAccountModel_type(pRawModel, &destroyAccountModel);
    pModel->setValue(m_pAccountModel);
    
    pRawModel->loadDataModel();

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

    if( m_pAccountController == NULL )
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
    GroupModel* pModel =
        dynamic_cast<GroupModel*>(_pModel.get());

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

    GroupModel* pRawModel = new GroupModel(*this);

    m_pGroupModel = pGroupModel_type(pRawModel, &destroyGroupModel);
    pModel->setValue(m_pGroupModel);
    
    pRawModel->loadDataModel();

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
const std::string&
AccountService::getDatabaseConnectionName() const
{
    return m_pDatabaseConfig->getAttribute("connection");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleSubscribeAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_SubscribeAccountModelRequest* pRequest =
        dynamic_cast<Protocol::I_SubscribeAccountModelRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_SubscribeAccountModelResponse> pResponse(
            Protocol::I_SubscribeAccountModelResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        /// TODO Create an AccountViewProxy here and store it somewhere.
        /// Insert the new view into the m_views collection, update the
        /// proxyId in the response.

        // Handle the response
        _pResponseHandler->handleResponse(
            pResponse.as<Protocol::I_SubscribeAccountModelResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUnsubscribeAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_UnsubscribeAccountModelRequest* pRequest =
        dynamic_cast<Protocol::I_UnsubscribeAccountModelRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_UnsubscribeAccountModelResponse> pResponse(
            Protocol::I_UnsubscribeAccountModelResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        /// TODO Get the request proxyId and use it to index the view proxy
        /// that this unsubscribe request is associated with. If it exist,
        /// remove and destroy the view proxy.

        // Handle the response
        _pResponseHandler->handleResponse(
            pResponse.as<Protocol::I_UnsubscribeAccountModelResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleSubscribeGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_SubscribeGroupModelRequest* pRequest =
        dynamic_cast<Protocol::I_SubscribeGroupModelRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_SubscribeGroupModelResponse> pResponse(
            Protocol::I_SubscribeGroupModelResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        /// TODO Create a GroupViewProxy here and store it somewhere.
        /// Insert the new view into the m_views collection, update the
        /// proxyId in the response.

        // Handle the response
        _pResponseHandler->handleResponse(
            pResponse.as<Protocol::I_SubscribeGroupModelResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUnsubscribeGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_UnsubscribeGroupModelRequest* pRequest =
        dynamic_cast<Protocol::I_UnsubscribeGroupModelRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_UnsubscribeGroupModelResponse> pResponse(
            Protocol::I_UnsubscribeGroupModelResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        /// TODO Get the request proxyId and use it to index the view proxy
        /// that this unsubscribe request is associated with. If it exist,
        /// remove and destroy the view proxy.

        // Handle the response
        _pResponseHandler->handleResponse(
            pResponse.as<Protocol::I_UnsubscribeGroupModelResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleCreateAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_CreateAccountRequest* pRequest =
        dynamic_cast<Protocol::I_CreateAccountRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_CreateAccountResponse> pResponse(
            Protocol::I_CreateAccountResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        // Create a new account and return the accountId in
        // the response.
        throw Zen::Utility::runtime_exception("AccountService::handleCreateAccountRequest(): TODO Finish.");
//        Common::I_Session& session(
//            getApplicationServer().getDefaultSessionService()
//                ->getSession(pRequest->getSessionId())
//        );
//
//        typedef Memory::managed_ptr<Common::I_Account>  pAccount_type;
//        pAccount_type pAccount = m_pAccountController->createAccount(
//            session,
//            pRequest->getUsername(),
//            pRequest->getPassword()
//        );
//
//        pResponse->setAccountId(pAccount->getAccountId());
//
//        // Handle the response
//        _pResponseHandler->handleResponse(
//            pResponse.as<Protocol::I_CreateAccountResponse::pResponse_type>()
//        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleReadAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_ReadAccountRequest* pRequest =
        dynamic_cast<Protocol::I_ReadAccountRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_ReadAccountResponse> pResponse(
            Protocol::I_ReadAccountResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        // Get an existing account and return it in
        // the response.
        throw Zen::Utility::runtime_exception("AccountService::handleReadAccountRequest(): TODO Finish.");
//        Common::I_Session& session(
//            getApplicationServer().getDefaultSessionService()
//                ->getSession(pRequest->getSessionId())
//        );
//
//        typedef Memory::managed_ptr<Common::I_Account>  pAccount_type;
//        pAccount_type pAccount = m_pAccountController->getAccount(
//            session,
//            pRequest->getUsername()
//        );
//
//        typedef Memory::managed_ptr<Account>    pConcreteAccount_type;
//        pConcreteAccount_type pConcreteAccount = pAccount.as<pConcreteAccount_type>();
//
//        pResponse->setAccountId(pAccount->getAccountId());
//        pResponse->setUsername(pAccount->getUsername());
//        pResponse->setPassword(pConcreteAccount->getPassword());
//
//        // Handle the response
//        _pResponseHandler->handleResponse(
//            pResponse.as<Protocol::I_ReadAccountResponse::pResponse_type>()
//        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUpdateAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_UpdateAccountRequest* pRequest =
        dynamic_cast<Protocol::I_UpdateAccountRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_UpdateAccountResponse> pResponse(
            Protocol::I_UpdateAccountResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        // Update an existing account and return it in
        // the response.
        throw Zen::Utility::runtime_exception("AccountService::handleUpdateAccountRequest(): TODO Finish.");
//        Common::I_Session& session(
//            getApplicationServer().getDefaultSessionService()
//                ->getSession(pRequest->getSessionId())
//        );
//
//        typedef Memory::managed_ptr<Common::I_Account>  pAccount_type;
//        pAccount_type pAccount = m_pAccountController->getAccount(
//            session,
//            pRequest->getUsername()
//        );
//
//        typedef Memory::managed_ptr<Account>    pConcreteAccount_type;
//        pConcreteAccount_type pConcreteAccount = pAccount.as<pConcreteAccount_type>();
//
//        pConcreteAccount->setUsername(pRequest->getUsername());
//        pConcreteAccount->setPassword(pRequest->getPassword());
//
//        m_pAccountController->updateAccount(
//            session,
//            *pAccount
//        );
//
//        pResponse->setAccountId(pAccount->getAccountId());
//        pResponse->setUsername(pAccount->getUsername());
//        pResponse->setPassword(pConcreteAccount->getPassword());
//
//        // Handle the response
//        _pResponseHandler->handleResponse(
//            pResponse.as<Protocol::I_UpdateAccountResponse::pResponse_type>()
//        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleDeleteAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_DeleteAccountRequest* pRequest =
        dynamic_cast<Protocol::I_DeleteAccountRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_DeleteAccountResponse> pResponse(
            Protocol::I_DeleteAccountResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        /// TODO Delete an existing account.
        throw Zen::Utility::runtime_exception("AccountService::handleDeleteAccountRequest() : Error, this operation has not yet been supported in AccountModel.");

        // Handle the response
        _pResponseHandler->handleResponse(
            pResponse.as<Protocol::I_DeleteAccountResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleCreateGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_CreateGroupRequest* pRequest =
        dynamic_cast<Protocol::I_CreateGroupRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_CreateGroupResponse> pResponse(
            Protocol::I_CreateGroupResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        // Create a new group and return the groupId in
        // the response.
        throw Zen::Utility::runtime_exception("AccountService::handleCreateGroupRequest(): TODO Finish.");
//
//        Common::I_Session& session(
//            getApplicationServer().getDefaultSessionService()
//                ->getSession(pRequest->getSessionId())
//        );
//
//        typedef Memory::managed_ptr<Common::I_Group>  pGroup_type;
//        pGroup_type pGroup = m_pGroupController->createGroup(
//            session,
//            pRequest->getGroupName(),
//            pRequest->getGroupDescription()
//        );
//
//        pResponse->setGroupId(pGroup->getGroupId());
//
//        // Handle the response
//        _pResponseHandler->handleResponse(
//            pResponse.as<Protocol::I_CreateGroupResponse::pResponse_type>()
//        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleReadGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_ReadGroupRequest* pRequest =
        dynamic_cast<Protocol::I_ReadGroupRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_ReadGroupResponse> pResponse(
            Protocol::I_ReadGroupResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        // Get an existing group and return it in
        // the response.
        throw Zen::Utility::runtime_exception("AccountService::handleReadGroupRequest(): TODO Finish.");
//        Common::I_Session& session(
//            getApplicationServer().getDefaultSessionService()
//                ->getSession(pRequest->getSessionId())
//        );
//
//        typedef Memory::managed_ptr<Common::I_Group>  pGroup_type;
//        pGroup_type pGroup = m_pGroupController->getGroup(
//            session,
//            pRequest->getGroupName()
//        );
//
//        pResponse->setGroupId(pGroup->getGroupId());
//        pResponse->setGroupName(pGroup->getGroupName());
//        pResponse->setGroupDescription(pGroup->getGroupDescription());
//
//        // Handle the response
//        _pResponseHandler->handleResponse(
//            pResponse.as<Protocol::I_ReadGroupResponse::pResponse_type>()
//        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleUpdateGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_UpdateGroupRequest* pRequest =
        dynamic_cast<Protocol::I_UpdateGroupRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_UpdateGroupResponse> pResponse(
            Protocol::I_UpdateGroupResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        /// TODO Update an existing group and return it in
        /// the response.
        throw Zen::Utility::runtime_exception("AccountService::handleUpdateGroupRequest(): TODO Finish.");
//        Common::I_Session& session(
//            getApplicationServer().getDefaultSessionService()
//                ->getSession(pRequest->getSessionId())
//        );
//
//        typedef Memory::managed_ptr<Common::I_Group>  pGroup_type;
//        pGroup_type pGroup = m_pGroupController->getGroup(
//            session,
//            pRequest->getGroupName()
//        );
//
//        typedef Memory::managed_ptr<Group>    pConcreteGroup_type;
//        pConcreteGroup_type pConcreteGroup = pGroup.as<pConcreteGroup_type>();
//
//        pConcreteGroup->setGroupName(pRequest->getGroupName());
//        pConcreteGroup->setGroupDescription(pRequest->getGroupDescription());
//
//        m_pGroupController->updateGroup(
//            session,
//            *pGroup
//        );
//
//        pResponse->setGroupId(pGroup->getGroupId());
//        pResponse->setGroupName(pGroup->getGroupName());
//        pResponse->setGroupDescription(pGroup->getGroupDescription());
//
//        // Handle the response
//        _pResponseHandler->handleResponse(
//            pResponse.as<Protocol::I_UpdateGroupResponse::pResponse_type>()
//        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AccountService::handleDeleteGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler)
{
    Protocol::I_DeleteGroupRequest* pRequest =
        dynamic_cast<Protocol::I_DeleteGroupRequest*>(_pRequest.get());

    if( pRequest != NULL )
    {
        // Create response
        Zen::Memory::managed_ptr<Protocol::I_DeleteGroupResponse> pResponse(
            Protocol::I_DeleteGroupResponse::create(
                pRequest->getDestinationEndpoint(),
                pRequest->getSourceEndpoint(),
                pRequest->getMessageId()
            )
        );

        /// TODO Delete an existing group.

        // Handle the response
        _pResponseHandler->handleResponse(
            pResponse.as<Protocol::I_DeleteGroupResponse::pResponse_type>()
        );
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
