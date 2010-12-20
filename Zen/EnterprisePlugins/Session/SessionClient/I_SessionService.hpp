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
#ifndef ZEN_ENTERPRISE_SESSION_CLIENT_I_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_CLIENT_I_SESSION_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/Session/I_SessionService.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
    class I_Session;
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Session Service interface.
/// @see I_ApplicationServer::getApplication() to get an instance of an
///     implementation of this interface.
class SESSION_CLIENT_DLL_LINK I_SessionService
:	public Zen::Enterprise::Session::I_SessionService
{

	/// @name Types
	/// @{
public:
	typedef Zen::Memory::managed_ptr<Zen::Networking::I_Endpoint>	pEndpoint_type;
	/// @}

	/// @name I_SessionService interface
	/// @{
public:
	/// Request a login.
	/// When the session is connected (or rejected) the session event is
	/// fired.
	/// @see getSessionEvent().
	virtual void requestLogin(pEndpoint_type _pDestinationEndpoint,
                              pResourceLocation_type _pDestLocation,
							  const std::string& _name,
							  const std::string& _password) = 0;

    /// Request a logout.
    /// When the session is disconnected, the session event is fired.
    /// @see getSessionEvent().
    virtual void requestLogout(Enterprise::Session::I_Session& _session) = 0;
	/// @}

	/// @name 'Structors
	/// @{
protected:
			 I_SessionService();
	virtual ~I_SessionService();
	/// @}

};	// interface I_SessionService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Client
}	// namespace Session
}	// namespace Enterprise
namespace Memory {
	/// I_SessionService is managed by factory
	template<>
    struct is_managed_by_factory<Zen::Enterprise::Session::Client::I_SessionService>
	:	public boost::true_type{};
}	// namespace Memory
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_CLIENT_I_SESSION_SERVICE_HPP_INCLUDED
