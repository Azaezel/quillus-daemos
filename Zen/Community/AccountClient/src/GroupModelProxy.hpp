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
#ifndef ZEN_COMMUNITY_CLIENT_GROUP_MODEL_PROXY_HPP_INCLUDED
#define ZEN_COMMUNITY_CLIENT_GROUP_MODEL_PROXY_HPP_INCLUDED

#include <Zen/Community/AccountCommon/I_GroupModel.hpp>
#include <Zen/Community/AccountCommon/I_GroupController.hpp>
#include <Zen/Community/AccountCommon/I_GroupView.hpp>
#include <Zen/Community/AccountCommon/I_Group.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/future_return_value.hpp>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Enterprise {
        namespace AppServer {
            class I_Message;
        }   // namespace AppServer
    }   // namespace Enterprise
namespace Community {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

class GroupModelProxy
:   public Common::I_GroupModel
{
    /// @name Forward declarations
    /// @{
public:
    struct ProxySubscription;
    struct ProxyGroup;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_Group>        pGroup_type;
    typedef Memory::managed_weak_ptr<Common::I_Group>   wpGroup_type;

    typedef std::map<std::string, pGroup_type>          Groups_type;

    typedef std::map<boost::uint64_t, pGroupView_type>  GroupViews_type;

    typedef Event::future_return_value<pGroup_type>     FutureGroup_type;
    typedef Memory::managed_ptr<FutureGroup_type>       pFutureGroup_type;
    /// @}

    /// @name I_GroupModel implementation
    /// @{
public:
    virtual pFutureSubscription_type subscribe(Common::I_Session& _session, pGroupView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual Event::I_Event& getGroupEvent();
    /// @}

    /// @name GroupModelProxy implementation
    /// @{
public:
    pFutureGroup_type createGroup(Common::I_Session& _session, const std::string& _name, const std::string& _description);
    pFutureGroup_type getGroup(Common::I_Session& _session, const std::string& _name);
    void updateGroup(Common::I_Session& _session, Common::I_Group& _group);
    void removeGroup(Common::I_Session& _session, Common::I_Group& _group);

    void getGroups(Common::I_Session& _session, Common::I_GroupController::I_GroupVisitor& _visitor);

    /// Unsubscribe a view from the model.
    void unSubscribe(Common::I_Session& _session, pGroupView_type _pView);

    /// Handle a remote subscription.
    /// TODO Should the view be a managed_ptr<>?
    void handleSubscription(ProxySubscription* _pProxySubscription);

    /// Handle a remote unsubscribe.
    /// TODO Should the view be a managed_ptr<>?
    void handleUnsubscribe(pSubscription_type _pSubscription);

    /// Handle an group creation
    void handleCreateGroup(ProxyGroup* _pProxyGroup);

    /// Handle an group read
    void handleReadGroup(ProxyGroup* _pProxyGroup);

    /// Handle an group update
    void handleUpdateGroup(ProxyGroup* _pProxyGroup);

    /// Handle an group deletion
    void handleDeleteGroup(ProxyGroup* _pProxyGroup);

    /// Handle a remote update notification.
    typedef Memory::managed_ptr<Enterprise::AppServer::I_Message>   pMessage_type;
    void handleUpdateNotification(pMessage_type _pMessage);
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyGroup(wpGroup_type _wpGroup);
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct ProxySubscription
    {
        pFutureSubscription_type    m_pFutureSubscription;
        pGroupView_type             m_pView;
        Common::I_Session&          m_session;
        boost::uint64_t             m_proxyId;

        ProxySubscription(Common::I_Session& _session) : m_pFutureSubscription(), m_pView(), m_session(_session), m_proxyId(0) {}
    };  // struct ProxySubscription

    struct ProxyGroup
    {
        pFutureGroup_type           m_pFutureGroup;
        pGroup_type                 m_pGroup;

        ProxyGroup() : m_pFutureGroup(), m_pGroup() {}
    };  // struct ProxyGroup
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit GroupModelProxy(AccountService& _service);
    virtual ~GroupModelProxy();
    /// @}

    /// @name Member variables
    /// @{
private:
    AccountService&                     m_service;
    Groups_type                         m_groups;

    /// Views subscribed to this model.
    GroupViews_type                     m_views;

    /// Guard for m_views.
    Threading::I_Mutex*                 m_pViewsGuard;

    /// Guard for groups
    Threading::I_Mutex*                 m_pGroupsGuard;
    /// @}

};  // class GroupModelProxy

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CLIENT_GROUP_MODEL_PROXY_HPP_INCLUDED
