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
#ifndef ZEN_COMMUNITY_SERVER_GROUP_CONTROLLER_HPP_INCLUDED
#define ZEN_COMMUNITY_SERVER_GROUP_CONTROLLER_HPP_INCLUDED

#include <Zen/Community/AccountCommon/I_GroupController.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;
class GroupModel;

class GroupController
:   public Common::I_GroupController
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_GroupController implementation
    /// @{
public:
    virtual Common::I_AccountService& getAccountService() const;
    virtual pFutureGroup_type createGroup(Common::I_Session& _session, const std::string& _name, const std::string& _description);
    virtual pFutureGroup_type getGroup(Common::I_Session& _session, const std::string& _name);
    virtual void updateGroup(Common::I_Session& _session, Common::I_Group& _group);
    virtual void removeGroup(Common::I_Session& _session, Common::I_Group& _group);
    virtual void getGroups(Common::I_Session& _session, I_GroupVisitor& _visitor);
    /// @}

    /// @name GroupController implementation
    /// @{
public:
    GroupModel& getModel() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit GroupController(AccountService& _service, Common::I_GroupModel& _model);
    virtual ~GroupController();
    /// @}

    /// @name Member variables
    /// @{
private:
    AccountService&                 m_service;
    GroupModel&                   m_model;
    /// @}

};  // class GroupController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SERVER_GROUP_CONTROLLER_HPP_INCLUDED
