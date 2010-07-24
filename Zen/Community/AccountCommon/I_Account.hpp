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
#ifndef ZEN_COMMUNITY_ACCOUNTCOMMON_I_ACCOUNT_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTCOMMON_I_ACCOUNT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Group;

/// Account game object type.
/// For now lets not worry about the base class, if there is one, but
/// quite likely there will be one.
class ACCOUNTCOMMON_DLL_LINK I_Account
//:   Zen::Scripting::I_ScriptableType
{
    /// @name Forward declarations
    /// @{
public:
    struct I_GroupVisitor;
    /// @}

    /// @name Types
    /// @{
public:
//    typedef Memory::managed_ptr<Common::I_Account>              pAccount_type;
//    typedef pAccount_type                                       pScriptObject_type;
//    typedef Zen::Scripting::ObjectReference<Common::I_Account>  ScriptObjectReference_type;
//    typedef ScriptObjectReference_type                          ScriptWrapper_type;
//    typedef ScriptWrapper_type*                                 pScriptWrapper_type;
	/// @}

    /// @name I_Account interface
    /// @{
public:
    /// Get the account identifier.
    virtual boost::uint64_t getAccountId() = 0;

    /// Get the account username
    virtual const std::string getUsername() = 0;

    /// Get the account password.
    virtual const std::string getPassword() = 0;

    /// Visit the account groups
    virtual void getGroups(I_GroupVisitor& _visitor) const = 0;
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct I_GroupVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_Group& _group) = 0;
        virtual void end() = 0;
    };  // struct I_GroupVisitor
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Account();
    virtual ~I_Account();
    /// @}

};  // interface I_Account

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
	/// I_Account is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_Account> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTCOMMON_I_ACCOUNT_HPP_INCLUDED
