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
#ifndef ZEN_ENTERPRISE_SESSION_SERVER_I_SESSION_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_SESSION_SERVER_I_SESSION_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/EnterprisePlugins/Session/SessionClient/I_SessionService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Session;

/// Session Service interface.
/// @see I_ApplicationServer::getApplication() to get an instance of an
///     implementation of this interface.
class SESSION_SERVER_DLL_LINK I_SessionService
:   public Zen::Enterprise::Session::Client::I_SessionService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Server::I_Session>     pServerSession_type;
    /// @}

    /// @name I_SessionService interface
    /// @{
public:
    /// Subscribe to a session service.
    /// If an existing subscription exists, it is replaced with the one obtained by this method.
    virtual void subscribe(Enterprise::Session::I_Session& _session, pEndpoint_type _pEndpoint, pResourceLocation_type _pLocation) = 0;

    /// Remotely terminate a session.
    /// This service must be in possession of a valid subscription before calling this method.
    /// @see subscribe().
    virtual void terminateSession(pServerSession_type _pSession) = 0;
    /// @}

    /// @name 'Structors
    /// @{
public:
             I_SessionService();
    virtual ~I_SessionService();
    /// @}

};  // interface I_SessionService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Server
}   // namespace Session
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_SESSION_SERVER_I_SESSION_SERVICE_HPP_INCLUDED
