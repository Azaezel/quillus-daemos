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
#ifndef ZEN_COMMUNITY_ACCOUNTSERVER_GROUP_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTSERVER_GROUP_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Community/AccountCommon/I_Group.hpp>

#include <Zen/Community/AccountModel/I_GroupDataMap.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_AccountService;
        class I_Account;
    }   // namespace Common
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

/// Group.
/// This represents a group.
/// @note Should this be scriptable or should Common::I_Group be scriptable?
///     For now I think it should be here, since the server side currently
///     is not scriptable.
class Group
:   public Common::I_Group
,   public Memory::managed_self_ref<Common::I_Group>
{
    /// @name Types
    /// @{
public:
    typedef Community::Account::I_GroupDataMap::pGroupDomainObject_type pGroupDomainObject_type;

    typedef Memory::managed_ptr<Common::I_Account>                      pAccount_type;
    typedef std::set<pAccount_type>                                     Accounts_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_Group implementation
    /// @{
public:
    virtual boost::uint64_t getGroupId();
    virtual const std::string getGroupName();
    virtual const std::string getGroupDescription();
    virtual void getGroupMembers(I_AccountVisitor& _visitor) const;
    /// @}

    /// @name Group implementation
    /// @{
public:
    void setGroupId(boost::uint64_t _groupId);
    void setGroupName(const std::string& _groupName);
    void setGroupDescription(const std::string& _groupDescription);
    void addMember(Common::I_Account& _account);

    pGroupDomainObject_type getDO();

    static void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class GroupModel;
             Group(AccountService& _parent, pGroupDomainObject_type _pDO);
public:
    virtual ~Group();
    /// @}

    /// @name Member variables
    /// @{
private:
    AccountService&             m_parent;

    pGroupDomainObject_type     m_groupDO;

    Accounts_type               m_accounts;

    pScriptWrapper_type         m_pScriptObject;
    /// @}

};  // class Group

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTSERVER_GROUP_HPP_INCLUDED
