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

#include "GroupModel.hpp"
#include "Group.hpp"
#include "AccountService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Event/I_EventManager.hpp>
#include <Zen/Core/Event/I_EventService.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

#include <Zen/Community/AccountCommon/subscription.hpp>
#include <Zen/Community/AccountCommon/I_Group.hpp>
#include <Zen/Community/AccountCommon/I_GroupView.hpp>

#include <Zen/Community/AccountModel/I_GroupDomainObject.hpp>
#include <Zen/Community/AccountModel/I_GroupDataMap.hpp>
#include <Zen/Community/AccountModel/I_GroupDataCollection.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModel::GroupModel(AccountService& _service)
:   m_service(_service)
,   m_pViewsGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModel::~GroupModel()
{
    Threading::MutexFactory::destroy(m_pViewsGuard);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef Common::subscription<GroupModel*, GroupModel::pGroupView_type> Subscription;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
destroySubscription(Memory::managed_weak_ptr<Zen::Spaces::ObjectModel::I_Subscription> _wpSubscription)
{
    Subscription* pSubscription =
        dynamic_cast<Subscription*>(
            _wpSubscription.get()
        );

    if( pSubscription != NULL )
    {
        delete pSubscription;
    }
    else
    {
        /// TODO Exception?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModel::pFutureSubscription_type
GroupModel::subscribe(Common::I_Session& _session, pGroupView_type _pView, pFilter_type _pFilter)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make async
    pFutureSubscription_type pReturnValue(new FutureSubscription_type());

    Threading::CriticalSection lock(m_pViewsGuard);

    m_views.insert(_pView);

    // This view needs to be notified of all of the existing model.
    for(Groups_type::iterator iter = m_groups.begin(); iter != m_groups.end(); iter++)
    {
        // Notify the view of an account
        _pView->onAddGroup(*iter->second.get());
    }

    pSubscription_type pSubscription(
        new Subscription(
            _session,
            this,
            _pView
        ),
        &destroySubscription
    );

    pReturnValue->setValue(pSubscription);

    return pReturnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Event::I_Event& 
GroupModel::getGroupEvent()
{
    return Zen::Event::I_EventManager::getSingleton().create("eventService")
        ->createEvent("Account::Server::GroupEvent");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::destroyGroup(wpGroup_type _wpGroup)
{
    Group* pRaw = 
        dynamic_cast<Group*>(_wpGroup.get());

    if( pRaw != NULL )
    {
        delete pRaw;
    }
    else
    {
        /// TODO Error?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModel::pFutureGroup_type 
GroupModel::createGroup(Common::I_Session& _session, const std::string& _name, const std::string& _description)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make this async
    pFutureGroup_type returnValue(new FutureGroup_type);

    // Get the data map.
    Community::Account::I_GroupDataMap::pGroupDataMap_type pGroupDM =
        Community::Account::I_GroupDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    // Create a new domain object.
    Community::Account::I_GroupDataMap::pGroupDomainObject_type pGroupDO =
        pGroupDM->createNew();

    // Populate the values.
    pGroupDO->getGroupName()        = _name;
    pGroupDO->getGroupDescription() = _description;

    // Update the object (and in this case, insert it).
    boost::uint64_t newGroupId      = pGroupDM->update(pGroupDO)->getValue();
    pGroupDO->getGroupId()          = newGroupId;

    Group* pRawGroup = new Group(m_service, pGroupDO);

    pGroup_type pGroup(
        pRawGroup,
        &destroyGroup
    );

    m_groups[_name] = pGroup;

    onAddGroup(pGroup);

    returnValue->setValue(pGroup);

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GroupModel::pFutureGroup_type 
GroupModel::getGroup(Common::I_Session& _session, const std::string& _name)
{
    /// TODO Check session (i.e. session user) against permissions.

    /// TODO Make this async
    pFutureGroup_type returnValue(new FutureGroup_type);

    pGroup_type pGroup;
    Groups_type::iterator iter = m_groups.find(_name);
    if( iter != m_groups.end() )
    {
        pGroup = iter->second;
    }

    returnValue->setValue(pGroup);

    return returnValue;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::updateGroup(Common::I_Session& _session, Common::I_Group& _group)
{
    /// TODO Check session (i.e. session user) against permissions.

    // Get the data map.
    Community::Account::I_GroupDataMap::pGroupDataMap_type pGroupDM =
        Community::Account::I_GroupDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    Group* pRawGroup = dynamic_cast<Group*>(&_group);

    // Update the object.
    pGroupDM->update(pRawGroup->getDO())->getValue();

    onUpdateGroup(pRawGroup->getSelfReference().lock());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::removeGroup(Common::I_Session& _session, Common::I_Group& _group)
{
    /// TODO Check session (i.e. session user) against permissions.

    throw Zen::Utility::runtime_exception("GroupModel::removeGroup() : Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
GroupModel::getGroups(Common::I_Session& _session, Common::I_GroupController::I_GroupVisitor& _visitor)
{
    /// TODO Check session (i.e. session user) against permissions.

    _visitor.begin();

    Groups_type::iterator iter = m_groups.begin();
    while( iter != m_groups.end() )
    {
        _visitor.visit(*iter->second);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::unSubscribe(Common::I_Session& _session, pGroupView_type _pView)
{
    Threading::CriticalSection lock(m_pViewsGuard);
    m_views.erase(_pView);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::onAddGroup(pGroup_type _pGroup)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    GroupViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(GroupViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(GroupViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onAddGroup(*_pGroup);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::onUpdateGroup(pGroup_type _pGroup)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    GroupViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(GroupViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(GroupViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onUpdateGroup(*_pGroup);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::onRemoveGroup(pGroup_type _pGroup)
{
    // Create a temporary list of views, otherwise we'll get into
    // a situation where nested locks might occur.
    GroupViews_type tmpViews;

    {
        Threading::CriticalSection lock(m_pViewsGuard);
        for(GroupViews_type::iterator iter = m_views.begin(); iter != m_views.end(); iter++)
        {
            tmpViews.insert(*iter);
        }
    }
    
    for(GroupViews_type::iterator iter = tmpViews.begin(); iter != tmpViews.end(); iter++)
    {
        (*iter)->onRemoveGroup(*_pGroup);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GroupModel::loadDataModel()
{
    // Get the data map.
    Community::Account::I_GroupDataMap::pGroupDataMap_type pGroupDM =
        Community::Account::I_GroupDataMap::create(
            m_service.getApplicationServer().getDatabaseConnection(
                m_service.getDatabaseConnectionName()
            )
        );

    Community::Account::I_GroupDataMap::pFutureGroupDataCollection_type pGroupDC =
        pGroupDM->getAll();

    struct GroupVisitor
    :   public Community::Account::I_GroupDataCollection::I_CollectionVisitor
    {
        typedef Community::Account::I_GroupDataCollection::pGroupDomainObject_type  pGroupDomainObject_type;
        typedef Memory::managed_ptr<Common::I_Group>                                  pGroup_type;

        virtual void begin()
        {
        }

        virtual void visit(pGroupDomainObject_type _pDomainObject)
        {
            // Queue the account to be loaded.
        }

        virtual void end()
        {
            delete this;
        }

        GroupVisitor(AccountService& m_service)
        :   m_service(m_service)
        {
        }

    private:
        AccountService&   m_service;
    };  // struct GroupVisitor

    GroupVisitor visitor(m_service);
    pGroupDC->getValue()->getAll(visitor);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
