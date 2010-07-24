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
#ifndef ZEN_COMMUNITY_ACCOUNTCOMMON_I_GROUP_HPP_INCLUDED
#define ZEN_COMMUNITY_ACCOUNTCOMMON_I_GROUP_HPP_INCLUDED

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
class I_Account;

/// I_Group
class ACCOUNTCOMMON_DLL_LINK I_Group
: 	public Scripting::I_ScriptableType
{
    /// @name Forward declarations
    /// @{
public:
    struct I_AccountVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Group>                    pGroup_type;
    typedef pGroup_type                                     pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<I_Group>        ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;
    typedef ScriptWrapper_type*                             pScriptWrapper_type;
	/// @}

    /// @name I_Group interface
    /// @{
public:
    /// Get the group identifier.
    virtual boost::uint64_t getGroupId() = 0;

    /// Get the group name
    virtual const std::string getGroupName() = 0;

    /// Get the group description
    virtual const std::string getGroupDescription() = 0;

    /// Visit the group members
    virtual void getGroupMembers(I_AccountVisitor& _visitor) const = 0;
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct I_AccountVisitor
    {
        virtual void begin() = 0;
        virtual void visit(const I_Account& _account) = 0;
        virtual void end() = 0;
    };  // struct I_AccountVisitor
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Group();
    virtual ~I_Group();
    /// @}

};  // interface I_Group

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
namespace Memory {
	/// I_Group is managed by factory
	template<>
	struct is_managed_by_factory<Community::Common::I_Group> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_ACCOUNTCOMMON_I_GROUP_HPP_INCLUDED
