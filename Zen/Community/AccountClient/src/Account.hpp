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
#ifndef ZEN_COMMUNITY_ACCOUNTCLIENT_ACCOUNT_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTCLIENT_ACCOUNT_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Community/AccountCommon/I_Account.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_AccountService;
        class I_Group;
    }   // namespace Common
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

/// Account.
/// This represents a stateful account.
/// @note Should this be scriptable or should Common::I_Account be scriptable?
///     For now I think it should be here, since the server side currently
///     is not scriptable.
class Account
:   public Common::I_Account
,   public Scripting::I_ScriptableType
,   public Memory::managed_self_ref<Common::I_Account>
{
    /// @name Types
    /// @{
public:
    typedef Account*                                        pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<Account>        ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;
    typedef ScriptWrapper_type*                             pScriptWrapper_type;

    typedef Memory::managed_ptr<Common::I_Group>            pGroup_type;
    typedef std::set<pGroup_type>                           Groups_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_Account implementation
    /// @{
public:
    virtual boost::uint64_t getAccountId();
    virtual const std::string getUsername();
    virtual const std::string getPassword();
    virtual void getGroups(I_GroupVisitor& _visitor) const;
    /// @}

    /// @name Account implementation
    /// @{
public:
    void setAccountId(boost::uint64_t _accountId);
    void setUsername(const std::string& _username);
    void setPassword(const std::string& _password);
    void addGroup(const Common::I_Group& _group);
    static void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class AccountModelProxy;
             Account(AccountService& _parent);
public:
    virtual ~Account();
    /// @}

    /// @name Member variables
    /// @{
private:
    AccountService&             m_parent;

    boost::uint64_t             m_accountId;
    std::string                 m_username;
    std::string                 m_password;

    Groups_type                 m_groups;

    pScriptWrapper_type         m_pScriptObject;
    /// @}

};  // class Account

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTCLIENT_ACCOUNT_HPP_INCLUDED
