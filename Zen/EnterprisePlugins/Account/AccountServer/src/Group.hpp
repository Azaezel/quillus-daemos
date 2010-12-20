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
#ifndef ZEN_ENTERPRISE_ACCOUNT_SERVER_GROUP_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_SERVER_GROUP_HPP_INCLUDED

#include "../I_Group.hpp"

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class AccountService;

/// Group.
/// This represents a stateful group.
class Group
:	public Zen::Enterprise::Account::Client::I_Group
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

	/// @name I_Group implemenation
	/// @{
public:
    virtual const GroupId& getGroupId() const;
	/// @}

	/// @name Group implementation
	/// @{
public:
    int scriptGetGroupState();
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
	/// @}

    /// @name Inner classes
    /// @{
public:
    class NativeGroupId
    :   public GroupId::I_NativeGroupId
    {
        /// @name Friend declarations
        /// @{
    private:
        friend class Group;
        /// @}

        /// @name I_NativeGroupId implementation
        /// @{
    public:
        virtual bool operator==(const GroupId::I_NativeGroupId& _otherId) const;
        virtual bool operator!=(const GroupId::I_NativeGroupId& _otherId) const;
        virtual bool operator< (const GroupId::I_NativeGroupId& _otherId) const;
        virtual I_NativeGroupId* clone() const;
        virtual std::string toString() const;
        virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
        virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
        /// @}

        /// @name NativeGroupId implementation
        /// @{
    public:
        boost::uint64_t getRawGroupId() const { return m_nativeGroupId; }
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 NativeGroupId() : m_nativeGroupId(0) {}
                 NativeGroupId(boost::uint64_t const _id) : m_nativeGroupId(_id) {}
        virtual ~NativeGroupId() {}
        /// @}

        /// @name Member Variables
        /// @{
    private:
        boost::uint64_t m_nativeGroupId;
        /// @}

    };  // class NativeGroupId
    /// @}

	/// @name Static Methods
	/// @{
public:
	static void registerScriptModule(Zen::Scripting::script_module& _module);
	/// @}

	/// @name 'Structors
	/// @{
public:
             Group();
             Group(const Group& _group);
	virtual ~Group();
	/// @}

	/// @name Member Variables
	/// @{
private:
	GroupId				    m_groupId;

	pScriptWrapper_type			m_pScriptObject;
	/// @}

};	// class Group

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Server
}	// namespace Account
}	// namespace Enterprise
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_SERVER_GROUP_HPP_INCLUDED
