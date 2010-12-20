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

#include "Session.hpp"

#include "SessionService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>

#include <boost/serialization/string.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Session {
namespace Server {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::Session()
:   m_pEndpoint()
,   m_pLocation()
,   m_sessionId(new NativeSessionId())
,   m_sessionState(Session::INITIALIZED)
,   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::Session(const Session& _session)
:   m_pEndpoint(_session.m_pEndpoint)
,   m_pLocation(_session.m_pLocation)
,   m_sessionId(_session.m_sessionId)
,   m_sessionState(_session.m_sessionState)
,   m_pScriptObject(NULL)
,   I_Session(_session)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::Session(pEndpoint_type _pDestination)
:	m_pEndpoint(_pDestination)
,   m_pLocation()
,	m_sessionId(new NativeSessionId())
,	m_sessionState(I_Session::INITIALIZED)
,	m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::Session(pEndpoint_type _pDestination, pResourceLocation_type _pLocation)
:	m_pEndpoint(_pDestination)
,   m_pLocation(_pLocation)
,	m_sessionId(new NativeSessionId())
,	m_sessionState(I_Session::INITIALIZED)
,	m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::~Session()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::SessionState_type
Session::getSessionState() const
{
	return m_sessionState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
Session::scriptGetSessionState()
{
	return static_cast<int>(getSessionState());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Session::pEndpoint_type
Session::getEndpoint() const
{
	return m_pEndpoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Session::pResourceLocation_type
Session::getLocation() const
{
    return m_pLocation;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Session::SessionId&
Session::getSessionId() const
{
    return m_sessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    std::string location;
    _archive & m_sessionState;
    _archive & location;
    _archive & m_sessionId;

    m_pLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton()
        .createLocation(location);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & m_sessionState;
    _archive & m_pLocation->toString();
    _archive & m_sessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::script_module* sm_pScriptModule = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::registerScriptModule(Zen::Scripting::script_module& _module)
{
    sm_pScriptModule = &_module;

    sm_pScriptModule->addType<Session>("Session", "Session")
        .addMethod("getSessionState", &Session::scriptGetSessionState)
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
Session::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(sm_pScriptModule->getScriptModule(),
            sm_pScriptModule->getScriptModule()->getScriptType(getScriptTypeName()),
            getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Session::NativeSessionId::operator==(const SessionId::I_NativeSessionId& _otherId) const
{
    const NativeSessionId* const pNativeSessionId = dynamic_cast<const NativeSessionId*>(&_otherId);
    return (pNativeSessionId != NULL) && (m_nativeSessionId == pNativeSessionId->m_nativeSessionId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Session::NativeSessionId::operator!=(const SessionId::I_NativeSessionId& _otherId) const
{
    const NativeSessionId* const pNativeSessionId = dynamic_cast<const NativeSessionId*>(&_otherId);
    return (pNativeSessionId == NULL) && (m_nativeSessionId != pNativeSessionId->m_nativeSessionId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Session::NativeSessionId::operator <(const SessionId::I_NativeSessionId& _otherId) const
{
    const NativeSessionId* const pNativeSessionId = dynamic_cast<const NativeSessionId*>(&_otherId);
    assert(pNativeSessionId != NULL);
    return (m_nativeSessionId < pNativeSessionId->m_nativeSessionId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Session::SessionId::I_NativeSessionId*
Session::NativeSessionId::clone() const
{
    return new Session::NativeSessionId(m_nativeSessionId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
Session::NativeSessionId::toString() const
{
    std::ostringstream oStream;
    oStream << m_nativeSessionId;
    return oStream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::NativeSessionId::serialize(boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    _archive & m_nativeSessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Session::NativeSessionId::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & m_nativeSessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}	// namespace Server
}	// namespace Session
}	// namespace Enterprise
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
