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
//	Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "LoginResponse.hpp"
#include "MessageFactory.hpp"

#include <Zen/Enterprise/AppServer/I_MessageHeader.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>
#include <Zen/Enterprise/AppServer/I_MessageRegistry.hpp>
#include <Zen/Enterprise/AppServer/I_NumericTypeMessageRegistry.hpp>

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <boost/serialization/string.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LoginResponse::pMessageType_type LoginResponse::sm_pType;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_MessageRegistry* LoginResponse::sm_pMessageRegistry = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LoginResponse::LoginResponse(pEndpoint_type _pSourceEndpoint,
                             pEndpoint_type _pDestinationEndpoint)
:   Message(createMessageHeader(), _pSourceEndpoint, _pDestinationEndpoint)
,   m_response(FAIL)
,   m_loginMessage()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LoginResponse::LoginResponse(pMessageHeader_type _pMessageHeader,
                             pEndpoint_type _pSourceEndpoint,
                             pEndpoint_type _pDestinationEndpoint)
:   Message(_pMessageHeader, _pSourceEndpoint, _pDestinationEndpoint)
,   m_response(FAIL)
,   m_loginMessage()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LoginResponse::~LoginResponse()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::serialize(pMessageHeader_type _pMessageHeader, boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    _archive & m_messageID;
    _archive & m_response;
    _archive & m_loginMessage;
    _archive & m_sessionId;
    _archive & m_sessionState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & m_messageID;
    _archive & m_response;
    _archive & m_loginMessage;
    _archive & m_sessionId;
    _archive & m_sessionState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
unsigned int
LoginResponse::getRequestMessageId() const
{
    return m_messageID;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LoginResponse::NumericResponse_type
LoginResponse::getResponse() const
{
    return (NumericResponse_type)m_response;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
LoginResponse::getMessage() const
{
    return m_loginMessage;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::int32_t
LoginResponse::getSessionId() const
{
    return m_sessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Common::I_Session::SessionState_type
LoginResponse::getSessionState() const
{
    return m_sessionState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::setRequestMessageId(unsigned int _messageId)
{
    m_messageID = _messageId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::setResponse(NumericResponse_type _response)
{
    m_response = _response;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::setMessage(const std::string& _message)
{
    m_loginMessage = _message;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::setSessionId(boost::int32_t _sessionId)
{
    m_sessionId = _sessionId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::setSessionState(Common::I_Session::SessionState_type _sessionState)
{
    m_sessionState = _sessionState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::registerMessage(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
{
    // Register message. Not keeping a managed pointer because it's really a singleton
    sm_pMessageRegistry = _appServer.getMessageRegistry().get();

    Zen::Enterprise::AppServer::I_NumericTypeMessageRegistry*
        pMessageRegistry = dynamic_cast<Zen::Enterprise::AppServer::I_NumericTypeMessageRegistry*>
            (sm_pMessageRegistry);

    if( !sm_pMessageFactory.isValid() )
    {
        sm_pMessageFactory = pMessageFactory_type(new MessageFactory, destroyMessageFactory);
    }

    if( pMessageRegistry != NULL )
    {
        sm_pType = pMessageRegistry->getMessageType(type);
        sm_pType->setMessageFactory(sm_pMessageFactory);
        sm_pMessageRegistry->registerMessageType(sm_pType);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LoginResponse::pMessageHeader_type
LoginResponse::createMessageHeader()
{
    return sm_pMessageRegistry->createMessageHeader(sm_pType);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
LoginResponse::destroy(wpResponse_type _wpResponse)
{
    LoginResponse* pResponse = dynamic_cast<LoginResponse*>(_wpResponse.get());

    if( pResponse != NULL )
    {
        delete pResponse;
    }
    else
    {
        throw Zen::Utility::runtime_exception("LoginResponse::destroy() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Message::pResourceLocation_type  sm_pResourceLocation;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
LoginResponse::pResourceLocation_type
LoginResponse::getDestinationLocation()
{
    if(!sm_pResourceLocation.isValid())
    {
        sm_pResourceLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/loginClient");
    }
    return sm_pResourceLocation;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}	// namespace Community
}	// namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
