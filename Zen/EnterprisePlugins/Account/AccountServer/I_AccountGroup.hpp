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
#ifndef ZEN_ENTERPRISE_ACCOUNT_SERVER_I_ACCOUNT_GROUP_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_SERVER_I_ACCOUNT_GROUP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// AccountGroup
class ACCOUNT_SERVER_DLL_LINK I_AccountGroup
:   public Zen::Memory::managed_self_ref<I_AccountGroup>
,   public Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_AccountGroup>        pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<I_AccountGroup> ScriptObjectReference_type;
    /// @}

	/// @name I_ScriptableType implementation
	/// @{
public:
	virtual const std::string& getScriptTypeName();
	/// @}

    /// @name I_AccountGroup interface
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_AccountGroup();
             I_AccountGroup(const I_AccountGroup& _accountGroup);
    virtual ~I_AccountGroup();
    /// @}

};  // interface I_AccountGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Account
}   // namespace Enterprise
namespace Memory {
	// I_Account is managed by factory
	template<>
	struct is_managed_by_factory<Zen::Enterprise::Account::Server::I_AccountGroup>
	:	public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_SERVER_I_ACCOUNT_GROUP_HPP_INCLUDED
