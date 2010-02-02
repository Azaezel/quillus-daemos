//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_COMMUNITY_SESSIONCLIENT_SESSION_HPP_INCLUDED
#define ZEN_COMMUNITY_SESSIONCLIENT_SESSION_HPP_INCLUDED

#include <Zen/Community/SessionCommon/I_Session.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_SessionService;
        class I_SessionListener;
    }   // namespace Common
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class SessionService;

/// Session Session
class Session
:   Common::I_Session
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Common::I_SessionListener>      pSessionListener_type;
    /// @}

    /// @name I_Session implementation
    /// @{
public:
    virtual SessionState_type getSessionState() const;
    virtual boost::int32_t getSessionId() const;
    virtual const pEndpoint_type getEndpoint() const;
    //virtual pFutureAttribute_type getAttribute(const std::string& _key) const;
    /// @}

    /// @name Session implementation
    /// @{
public:
    void setSessionId(boost::int32_t _sessionId);
    void setSessionState(SessionState_type _sessionState);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class SessionService;
             Session(SessionService& _parent, pEndpoint_type _pDestination);
    virtual ~Session();
    /// @}

    /// @name Member variables
    /// @{
private:
    SessionService&             m_parent;
    /// Destination endpoint
    pEndpoint_type              m_pEndpoint;

    boost::int32_t              m_sessionId;
    SessionState_type           m_sessionState;
    /// @}

};  // class Session

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONCLIENT_SESSION_HPP_INCLUDED
