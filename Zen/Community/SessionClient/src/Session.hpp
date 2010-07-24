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

#include <Zen/Core/Scripting.hpp>

#include <Zen/Community/SessionCommon/I_Session.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
    namespace Common {
        class I_SessionService;
    }   // namespace Common
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class SessionService;

/// Session.
/// This represents a stateful session.
/// @note Should this be scriptable or should Common::I_Session be scriptable?
///     For now I think it should be here, since the server side currently
///     is not scriptable.
class Session
:   public Common::I_Session
,   public Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Session*                                        pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<Session>        ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;
    typedef ScriptWrapper_type*                             pScriptWrapper_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_Session implementation
    /// @{
public:
    virtual SessionState_type getSessionState() const;
    virtual boost::uint32_t getSessionId() const;
    virtual const pEndpoint_type getEndpoint() const;
    //virtual pFutureAttribute_type getAttribute(const std::string& _key) const;
    /// @}

    /// @name Session implementation
    /// @{
public:
    void setSessionId(boost::uint32_t _sessionId);
    void setSessionState(SessionState_type _sessionState);
    static void registerScriptModule(Zen::Scripting::script_module& _module);
    int scriptGetSessionState();
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

    pScriptWrapper_type         m_pScriptObject;
    /// @}

};  // class Session

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_SESSIONCLIENT_SESSION_HPP_INCLUDED
