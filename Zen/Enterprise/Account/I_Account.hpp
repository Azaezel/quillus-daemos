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
#ifndef ZEN_ENTERPRISE_ACCOUNT_I_ACCOUNT_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_I_ACCOUNT_HPP_INCLUDED

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

class ACCOUNT_DLL_LINK I_Account
:   public Zen::Memory::managed_self_ref<I_Account>
,   public Zen::Scripting::I_ScriptableType
{
	/// @name Types
	/// @{
public:
    struct AccountId;

    typedef Zen::Memory::managed_ptr<I_Account>                 pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<I_Account>          ScriptObjectReference_type;
	/// @}

	/// @name I_ScriptableType implementation
	/// @{
public:
	virtual const std::string& getScriptTypeName();
	/// @}

	/// @name I_Account interface
	/// @{
public:
    virtual const Enterprise::Account::I_Account::AccountId& getAccountId() const = 0;
	/// @}

    /// @name Inner Classes
    /// @{
public:
    struct AccountId
    {
        /// @name Inner Interfaces
        /// @{
    public:
        class I_NativeAccountId
        {
            /// @name Types
            /// @{
        public:
            /// @}

            /// @name I_NativeAccountId interface
            /// @{
        public:
            virtual bool operator==(const I_NativeAccountId& _otherId) const = 0;
            virtual bool operator!=(const I_NativeAccountId& _otherId) const = 0;
            virtual bool operator< (const I_NativeAccountId& _otherId) const = 0;
            virtual I_NativeAccountId* clone() const = 0;
            virtual std::string toString() const = 0;
            virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version) = 0;
            virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version) = 0;
            /// @}

            /// @name 'Structors
            /// @{
        public:
                     I_NativeAccountId() {}
            virtual ~I_NativeAccountId() {}
            /// @}

        };  // interface I_NativeAccountId
        /// @}

        /// @name AccountId implementation
        /// @{
    public:
        AccountId& operator= (const AccountId& _otherId)       {delete m_pNativeAccountId; m_pNativeAccountId = _otherId.m_pNativeAccountId->clone(); return *this;}
        bool       operator==(const AccountId& _otherId) const {return (m_pNativeAccountId == _otherId.m_pNativeAccountId) || ( m_pNativeAccountId &&  _otherId.m_pNativeAccountId && (*m_pNativeAccountId == *_otherId.m_pNativeAccountId));}
        bool       operator!=(const AccountId& _otherId) const {return (m_pNativeAccountId != _otherId.m_pNativeAccountId) && (!m_pNativeAccountId || !_otherId.m_pNativeAccountId || (*m_pNativeAccountId != *_otherId.m_pNativeAccountId));}
        bool       operator< (const AccountId& _otherId) const {return (m_pNativeAccountId <  _otherId.m_pNativeAccountId);}
        operator std::string ()                          const {return m_pNativeAccountId ? m_pNativeAccountId->toString() : "";}
        std::string toString ()                          const {return m_pNativeAccountId ? m_pNativeAccountId->toString() : "";}
        
        void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version) 
        {
            _archive & *m_pNativeAccountId; 
        }

        void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
        {
            _archive & *m_pNativeAccountId;
        }
        /// @}

        /// @name 'Structors
        /// @{
    public:
         AccountId(I_NativeAccountId* const _accountId = NULL) : m_pNativeAccountId(_accountId)                           {}
         AccountId(const AccountId& _otherId)                  : m_pNativeAccountId(_otherId.m_pNativeAccountId->clone()) {}
        ~AccountId()                                                                                                      {delete m_pNativeAccountId;}
        /// @}

        /// @name Member variables
        /// @{
    public:
        I_NativeAccountId* m_pNativeAccountId;
        /// @}

    };  // struct AccountId
    /// @}
    
    /// @name 'Structors
	/// @{
protected:
			 I_Account();
             I_Account(const I_Account& _account);
	virtual ~I_Account();
	/// @}

};	// interface I_Account

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Account
}	// namespace Enterprise
namespace Memory {
	// I_Account is managed by factory
	template<>
	struct is_managed_by_factory<Zen::Enterprise::Account::I_Account>
	:	public boost::true_type{};
}	// namespace Memory
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_I_ACCOUNT_HPP_INCLUDED
