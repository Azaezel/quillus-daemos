//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
#ifndef ZEN_ENTERPRISE_ACCOUNT_SERVER_ACCOUNT_GROUP_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_SERVER_ACCOUNT_GROUP_HPP_INCLUDED

#include "../I_AccountGroup.hpp"

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// AccountGroup.
/// This represents a group to which an account belongs.
class AccountGroup
:   public Zen::Enterprise::Account::Server::I_AccountGroup
{
	/// @name Types
	/// @{
public:
	typedef ScriptObjectReference_type						ScriptWrapper_type;
	typedef ScriptWrapper_type*								pScriptWrapper_type;
	/// @}

	/// @name I_ScriptableType implementation
	/// @{
public:
	virtual Zen::Scripting::I_ObjectReference* getScriptObject();
	/// @}

    /// @name I_AccountGroup implementation
    /// @{
public:
    /// @}

    /// @name AccountGroup implementation
    /// @{
public:
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

	/// @name Static Methods
	/// @{
public:
	static void registerScriptModule(Zen::Scripting::script_module& _module);
	/// @}

    /// @name 'Structors
    /// @{
public:
             AccountGroup();
             AccountGroup(const AccountGroup& _accountGroup);
    virtual ~AccountGroup();
    /// @}

    /// @name Member Variables
    /// @{
private:
	pScriptWrapper_type		m_pScriptObject;
    /// @}

};  // class AccountGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_SERVER_ACCOUNT_GROUP_HPP_INCLUDED
