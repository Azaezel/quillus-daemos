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
#ifndef ZEN_COMMUNITY_SERVER_GROUP_MODEL_HPP_INCLUDED
#define ZEN_COMMUNITY_SERVER_GROUP_MODEL_HPP_INCLUDED

#include <Zen/Community/AccountCommon/I_GroupModel.hpp>
#include <Zen/Community/AccountCommon/I_GroupController.hpp>
#include <Zen/Community/AccountCommon/I_Group.hpp>
#include <Zen/Community/AccountCommon/subscription.hpp>

#include <Zen/Community/AccountModel/I_GroupDataMap.hpp>
#include <Zen/Community/AccountModel/I_GroupDataCollection.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/future_return_value.hpp>

#include <string>
#include <map>
#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

class GroupModel
:   public Common::I_GroupModel
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_Group>        pGroup_type;
    typedef Memory::managed_weak_ptr<Common::I_Group>   wpGroup_type;

    typedef std::map<std::string, pGroup_type>          Groups_type;

    typedef std::set<pGroupView_type>                   GroupViews_type;

    typedef Event::future_return_value<pGroup_type>     FutureGroup_type;
    typedef Memory::managed_ptr<FutureGroup_type>       pFutureGroup_type;
    /// @}

    /// @name I_GroupModel implementation
    /// @{
public:
    virtual pFutureSubscription_type subscribe(pEndpoint_type _pDestinationEndpoint, pGroupView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual Event::I_Event& getGroupEvent();
    /// @}

    /// @name GroupModel implementation
    /// @{
public:
    pFutureGroup_type createGroup(Common::I_Session& _session, const std::string& _name, const std::string& _description);
    pFutureGroup_type getGroup(Common::I_Session& _session, const std::string& _name);
    void updateGroup(Common::I_Session& _session, Common::I_Group& _group);
    void removeGroup(Common::I_Session& _session, Common::I_Group& _group);
    void getGroups(Common::I_Session& _session, Common::I_GroupController::I_GroupVisitor& _visitor);

    /// Unsubscribe a view from the model.
    typedef Common::subscription<GroupModel*, pGroupView_type> Subscription;
    void unSubscribe(Subscription* _pSubscription);

    /// Fire the onAddGroup notification for all subscribed views.
    void onAddGroup(pGroup_type _pGroup);

    /// Fire the onUpdateGroup notification for all subscribed views.
    void onUpdateGroup(pGroup_type _pGroup);

    /// Fire the onRemoveGroup notification for all subscribed views.
    void onRemoveGroup(pGroup_type _pGroup);

    /// Load the data from the database.
    void loadDataModel();
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyGroup(wpGroup_type _wpGroup);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class AccountService;
    explicit GroupModel(AccountService& _service);
    virtual ~GroupModel();
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
    /// @}

};  // class GroupModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SERVER_GROUP_MODEL_HPP_INCLUDED
