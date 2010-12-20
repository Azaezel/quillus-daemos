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
#ifndef ZEN_ENTERPRISE_ACCOUNT_PROTOCOL_I_AUTHENTICATE_RESPONSE_HPP_INCLUDED
#define ZEN_ENTERPRISE_ACCOUNT_PROTOCOL_I_AUTHENTICATE_RESPONSE_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Enterprise/Account/I_Account.hpp>

#include <Zen/Enterprise/AppServer/I_Response.hpp>

#include <boost/serialization/string.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ACCOUNT_CLIENT_DLL_LINK I_AuthenticateResponse
:   public virtual Zen::Enterprise::AppServer::I_Response
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Enterprise::AppServer::I_Response>    pResponse_type;
    /// @}
    
    /// @name Getter / Setter methods
    /// @{
public:
    /// Get the account element.
    virtual const Account::I_Account& getAccount() const = 0;

    /// Set the account element.
    virtual void setAccount(const Account::I_Account& _account) = 0;

    /// Get the authenticated element.
    virtual bool getAuthenticated() const = 0;

    /// Set the authenticated element.
    virtual void setAuthenticated(const bool& _authenticated) = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static pResponse_type create(pEndpoint_type _pSourceEndpoint,
                                 pEndpoint_type _pDestinationEndpoint,
                                 pResourceLocation_type _pSrcLocation,
                                 pResourceLocation_type _pDestLocation,
                                 boost::uint64_t _requestMessageId);

    static pMessageType_type getStaticMessageType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_AuthenticateResponse();
    virtual ~I_AuthenticateResponse();
    /// @}

};  // interface I_AuthenticateResponse

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}   // namespace Account
}   // namespace Enterprise
namespace Memory {
    // I_AuthenticateResponse is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::Account::Protocol::I_AuthenticateResponse> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_ACCOUNT_PROTOCOL_I_AUTHENTICATE_RESPONSE_HPP_INCLUDED
