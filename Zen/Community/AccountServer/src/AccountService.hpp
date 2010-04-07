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
#ifndef ZEN_COMMUNITY_ACCOUNTSERVER_ACCOUNT_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTSERVER_ACCOUNT_SERVICE_HPP_INCLUDED

#include <Zen/Enterprise/AppServer/scriptable_generic_service.hpp>

#include <Zen/Community/AccountCommon/I_AccountService.hpp>

#include <Zen/Enterprise/Database/I_DatabaseManager.hpp>
#include <Zen/Enterprise/Database/I_DatabaseConnection.hpp>

#include <Zen/Community/AccountModel/I_AccountDataMap.hpp>
#include <Zen/Community/AccountModel/I_AccountDomainObject.hpp>
#include <Zen/Community/AccountModel/I_AccountDataCollection.hpp>

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
    }   // namespace Common
    namespace Account {
        class I_AccountDomainObject;
    }   // namespace Account
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Account;

class AccountService
:   public Zen::Enterprise::AppServer::scriptable_generic_service
        <Zen::Community::Common::I_AccountService, AccountService>
{
    /// @name Types
    /// @{
public:
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
    const std::string& getDatabaseConnectionName() const;
    
    void handleSubscribeAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleUnsubscribeAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleSubscribeGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleUnsubscribeGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    
    void handleCreateAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleReadAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleUpdateAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleDeleteAccountRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);

    void handleCreateGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleReadGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleUpdateGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
    void handleDeleteGroupRequest(pRequest_type _pRequest, pResponseHandler_type _pResponseHandler);
private:
    pScriptModule_type getScriptModule();
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
    const Zen::Plugins::I_ConfigurationElement*         m_pDatabaseConfig;

    pAccountModel_type                                  m_pAccountModel;
    pAccountController_type                             m_pAccountController;

    pGroupModel_type                                    m_pGroupModel;
    pGroupController_type                               m_pGroupController;
    /// @}

};  // class AccountService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTSERVER_ACCOUNT_SERVICE_HPP_INCLUDED
