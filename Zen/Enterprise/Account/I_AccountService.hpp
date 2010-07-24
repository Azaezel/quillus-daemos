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
#ifndef ZEN_ENTERPRISE_ACCOUNT_I_ACCOUNT_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_I_ACCOUNT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Event/I_Event.hpp>

#include <Zen/Enterprise/Account/I_Account.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ACCOUNT_DLL_LINK I_AccountService
:   public Zen::Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_AccountService>      pService_type;
    /// @}

    /// @name I_AccountService interface
    /// @{
public:
    /// Get the authentication event.
    /// The payload is an I_Account.
    virtual Zen::Event::I_Event& getAuthenticationEvent() = 0;
    /// @}

    /// @name Inner structs
    /// @{
public:
    struct AuthenticationPayload
    {
        boost::uint64_t     m_id;
        bool                m_authenticated;
        const I_Account&    m_account;

        AuthenticationPayload(boost::uint64_t _id, bool _authenticated, const I_Account& _account) : m_id(_id), m_authenticated(_authenticated), m_account(_account)
        {}
    };  // struct AuthenticationPayload
    /// @|

    /// @name 'Structors
    /// @{
protected:
             I_AccountService();
    virtual ~I_AccountService();
    /// @}

};  // interface I_AccountService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Account
}   // namespace Enterprise
namespace Memory {
	/// I_AccountService is managed by factory
	template<>
	struct is_managed_by_factory<Zen::Enterprise::Account::I_AccountService>
	:	public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_I_ACCOUNT_SERVICE_HPP_INCLUDED
