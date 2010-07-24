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
#ifndef ZEN_COMMUNITY_ACCOUNTCLIENT_ACCOUNT_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTCLIENT_ACCOUNT_SERVICE_HPP_INCLUDED

#include "AccountModelProxy.hpp"
#include "GroupModelProxy.hpp"

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <Zen/Community/AccountCommon/I_AccountService.hpp>

#include <boost/cstdint.hpp>

#include <set>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Account {
        namespace Protocol {
            class I_SubscribeAccountModelRequest;
            class I_UnsubscribeAccountModelRequest;
            class I_SubscribeGroupModelRequest;
            class I_UnsubscribeGroupModelRequest;
            class I_CreateAccountRequest;
            class I_ReadAccountRequest;
            class I_UpdateAccountRequest;
            class I_DeleteAccountRequest;
            class I_CreateGroupRequest;
            class I_ReadGroupRequest;
            class I_UpdateGroupRequest;
            class I_DeleteGroupRequest;
        }   // namespace Protocol
    }   // namespace Account
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Account;

class AccountService
:   public Zen::Enterprise::AppServer::scriptable_generic_service
        <Zen::Community::Common::I_AccountService, AccountService>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;
    typedef Event::future_return_value<pSubscription_type>              FutureSubscription_type;
    typedef Memory::managed_ptr<FutureSubscription_type>                pFutureSubscription_type;

    typedef Zen::Memory::managed_ptr<Common::I_AccountView>             pAccountView_type;
    typedef Zen::Memory::managed_ptr<Common::I_GroupView>               pGroupView_type;
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

    /// @name I_AccountService implementation
    /// @{
public:
    virtual Common::I_Account& createAccount(const std::string& _userName, const std::string& _password);
    virtual Common::I_Group& createGroup(const std::string& _groupName);
    virtual Common::I_PermissibleResource& createPermissibleResource(const std::string& _resourceName);
    virtual void setResourceOwner(Common::I_PermissibleResource& _resource, Common::I_Account& _owner);
    virtual pFutureAccountModel_type createAccountModel();
    virtual pFutureAccountController_type createAccountController(Common::I_AccountModel& _model);
    virtual pFutureGroupModel_type createGroupModel();
    virtual pFutureGroupController_type createGroupController(Common::I_GroupModel& _model);
    virtual boost::uint32_t getAccessFlags() const;
    virtual void setAccessFlags(boost::uint32_t _accessFlags);
    virtual const Common::I_Account& getOwner() const;
    virtual void setOwner(const Common::I_Account& _owner);
    virtual const Common::I_Group& getGroup() const;
    virtual void setGroup(const Common::I_Group& _group);
    /// @}

    /// @name AccountService implementation
    /// @{
public:
    pScriptModule_type getScriptModule();

    void handleSubscribeAccountModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_SubscribeAccountModelRequest& _request, AccountModelProxy::ProxySubscription* _pProxySubscription);
    void handleUnsubscribeAccountModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UnsubscribeAccountModelRequest& _request, pAccountView_type _pAccountView);
    void handleSubscribeGroupModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_SubscribeGroupModelRequest& _request, GroupModelProxy::ProxySubscription* _pProxySubscription);
    void handleUnsubscribeGroupModelResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UnsubscribeGroupModelRequest& _request, pGroupView_type _pGroupView);

    void handleCreateAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_CreateAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount);
    void handleReadAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_ReadAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount);
    void handleUpdateAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UpdateAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount);
    void handleDeleteAccountResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_DeleteAccountRequest& _request, AccountModelProxy::ProxyAccount* _pProxyAccount);

    void handleCreateGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_CreateGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup);
    void handleReadGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_ReadGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup);
    void handleUpdateGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_UpdateGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup);
    void handleDeleteGroupResponse(pResponse_type _pResponse, Zen::Community::Account::Protocol::I_DeleteGroupRequest& _request, GroupModelProxy::ProxyGroup* _pProxyGroup);

    void handleAccountModelUpdateMessage(pMessage_type _pMessage);

    void handleGroupModelUpdateMessage(pMessage_type _pMessage);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyAccountModel(wpAccountModel_type _pModel);
    static void destroyAccountController(wpAccountController_type _pController);

    static void destroyGroupModel(wpGroupModel_type _pModel);
    static void destroyGroupController(wpGroupController_type _pController);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class AccountServiceFactory;
             AccountService(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer);
    virtual ~AccountService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pScriptEngine_type                                  m_pScriptEngine;
    Zen::Scripting::script_module*                      m_pScriptModule;
    Scripting::I_ObjectReference*                       m_pScriptObject;

    pAccountModel_type                                  m_pAccountModel;
    pAccountController_type                             m_pAccountController;

    pGroupModel_type                                    m_pGroupModel;
    pGroupController_type                               m_pGroupController;
    /// @}

};  // class AccountService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTCLIENT_ACCOUNT_SERVICE_HPP_INCLUDED
