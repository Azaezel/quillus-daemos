//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
#ifndef ZEN_COMMUNITY_SESSIONCOMMON_I_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONCOMMON_I_SESSION_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Networking {
        class I_Endpoint;
    }   // namespace Networking
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Session;

/// Session Service interface.
/// See this discussion for more details about this design.
/// http://www.indiezen.org/wiki/irclogs/2009/03/19/#UTC2009-03-19T14:27:25
/// @see I_ApplicationServer::getApplication() to get an instance of an 
///     implementation of this interface.
class SESSIONCOMMON_DLL_LINK I_SessionService
:   public Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Networking::I_Endpoint>     pEndpoint_type;

	typedef Memory::managed_ptr<I_SessionService>				pService_type;

    typedef Memory::managed_ptr<I_Session>                  pSession_type;
    typedef Event::Event<pSession_type>                     SessionEvent_type;
    typedef Event::future_return_value<pSession_type>       FutureSession_type;
    typedef Memory::managed_ptr<FutureSession_type>         pFutureSession_type;
    /// @}

    /// @name I_SessionService interface
    /// @{
public:
    /// Request a login
    virtual pFutureSession_type requestSession(pEndpoint_type _pDestinationEndpoint, 
                              const std::string& _name, 
                              const std::string& _password) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// This event will fire when the session status changes.
    SessionEvent_type onSessionStatusChange;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SessionService();
public:
    virtual ~I_SessionService();
    /// @}

};  // interface I_SessionService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONCOMMON_I_SESSION_SERVICE_HPP_INCLUDED
