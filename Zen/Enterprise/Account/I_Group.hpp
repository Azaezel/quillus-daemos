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
#ifndef ZEN_ENTERPRISE_ACCOUNT_I_GROUP_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_I_GROUP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ACCOUNT_DLL_LINK I_Group
:   public Zen::Memory::managed_self_ref<I_Group>
,   public Zen::Scripting::I_ScriptableType
{
	/// @name Types
	/// @{
public:
    struct GroupId;

    typedef Zen::Memory::managed_ptr<I_Group>               pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<I_Group>        ScriptObjectReference_type;
	/// @}

	/// @name I_ScriptableType implementation
	/// @{
public:
	virtual const std::string& getScriptTypeName();
	/// @}

	/// @name I_Group interface
	/// @{
public:
    virtual const GroupId& getGroupId() const = 0;
	/// @}

    /// @name Inner Classes
    /// @{
public:
    struct GroupId
    {
        /// @name Inner Interfaces
        /// @{
    public:
        class I_NativeGroupId
        {
            /// @name Types
            /// @{
        public:
            /// @}

            /// @name I_NativeGroupId interface
            /// @{
        public:
            virtual bool operator==(const I_NativeGroupId& _otherId) const = 0;
            virtual bool operator!=(const I_NativeGroupId& _otherId) const = 0;
            virtual bool operator< (const I_NativeGroupId& _otherId) const = 0;
            virtual I_NativeGroupId* clone() const = 0;
            virtual std::string toString() const = 0;
            virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version) = 0;
            virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
            /// @}

            /// @name 'Structors
            /// @{
        public:
                     I_NativeGroupId() {}
            virtual ~I_NativeGroupId() {}
            /// @}

        };  // interface I_NativeGroupId
        /// @}

        /// @name GroupId implementation
        /// @{
    public:
        GroupId&   operator= (const GroupId& _otherId)       {delete m_pNativeGroupId; m_pNativeGroupId = _otherId.m_pNativeGroupId->clone(); return *this;}
        bool       operator==(const GroupId& _otherId) const {return (m_pNativeGroupId == _otherId.m_pNativeGroupId) || ( m_pNativeGroupId &&  _otherId.m_pNativeGroupId && (*m_pNativeGroupId == *_otherId.m_pNativeGroupId));}
        bool       operator!=(const GroupId& _otherId) const {return (m_pNativeGroupId != _otherId.m_pNativeGroupId) && (!m_pNativeGroupId || !_otherId.m_pNativeGroupId || (*m_pNativeGroupId != *_otherId.m_pNativeGroupId));}
        bool       operator< (const GroupId& _otherId) const {return (m_pNativeGroupId <  _otherId.m_pNativeGroupId);}
        operator std::string ()                          const {return m_pNativeGroupId ? m_pNativeGroupId->toString() : "";}
        std::string toString ()                          const {return m_pNativeGroupId ? m_pNativeGroupId->toString() : "";}
        
        void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version) 
        {
            _archive & *m_pNativeGroupId; 
        }

        void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
        {
            _archive & *m_pNativeGroupId;
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
         GroupId(I_NativeGroupId* const _groupId = NULL) : m_pNativeGroupId(_groupId)                           {}
         GroupId(const GroupId& _otherId)                : m_pNativeGroupId(_otherId.m_pNativeGroupId->clone()) {}
        ~GroupId()                                                                                              {delete m_pNativeGroupId;}
        /// @}

        /// @name Member variables
        /// @{
    public:
        I_NativeGroupId* m_pNativeGroupId;
        /// @}

    };  // struct GroupId
    /// @}
    
	/// @name 'Structors
	/// @{
protected:
			 I_Group();
             I_Group(const I_Group& _group);
	virtual ~I_Group();
	/// @}

};	// interface I_Group

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Account
}	// namespace Enterprise
namespace Memory {
	// I_Group is managed by factory
	template<>
	struct is_managed_by_factory<Zen::Enterprise::Account::I_Group>
	:	public boost::true_type{};
}	// namespace Memory
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_I_GROUP_HPP_INCLUDED
