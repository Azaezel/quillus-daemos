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
#ifndef ZEN_ENTERPRISE_ACCOUNT_CLIENT_ACCOUNT_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_CLIENT_ACCOUNT_HPP_INCLUDED

#include "../I_Account.hpp"

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Account.
/// This represents a login account.
class Account
:   public Zen::Enterprise::Account::Client::I_Account
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

    /// @name I_Account implementation
    /// @{
public:
    virtual const Enterprise::Account::I_Account::AccountId& getAccountId() const;
    virtual const std::string& getUserId() const;
    /// @}

    /// @name Account implementation
    /// @{
public:
    void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
    void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
    /// @}

    /// @name Inner classes
    /// @{
public:
    class NativeAccountId
    :   public AccountId::I_NativeAccountId
    {
        /// @name Friend declarations
        /// @{
    private:
        friend class Account;
        /// @}

        /// @name I_NativeAccountId implementation
        /// @{
    public:
        virtual bool operator==(const AccountId::I_NativeAccountId& _otherId) const;
        virtual bool operator!=(const AccountId::I_NativeAccountId& _otherId) const;
        virtual bool operator< (const AccountId::I_NativeAccountId& _otherId) const;
        virtual I_NativeAccountId* clone() const;
        virtual std::string toString() const;
        virtual void serialize(boost::archive::polymorphic_iarchive& _archive, const int _version);
        virtual void serialize(boost::archive::polymorphic_oarchive& _archive, const int _version);
        /// @}

        /// @name NativeAccountId implementation
        /// @{
    public:
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 NativeAccountId() : m_nativeAccountId(0) {}
                 NativeAccountId(boost::uint64_t const _id) : m_nativeAccountId(_id) {}
        virtual ~NativeAccountId() {}
        /// @}

        /// @name Member Variables
        /// @{
    private:
        boost::uint64_t m_nativeAccountId;
        /// @}

    };  // class NativeAccountId
    /// @}

	/// @name Static Methods
	/// @{
public:
	static void registerScriptModule(Zen::Scripting::script_module& _module);
	/// @}

    /// @name 'Structors
    /// @{
public:
             Account();
             Account(const Account& _account);
    virtual ~Account();
    /// @}

    /// @name Member Variables
    /// @{
private:
    AccountId               m_accountId;
    std::string             m_userId;

	pScriptWrapper_type		m_pScriptObject;
    /// @}

};  // class Account

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_CLIENT_ACCOUNT_HPP_INCLUDED
