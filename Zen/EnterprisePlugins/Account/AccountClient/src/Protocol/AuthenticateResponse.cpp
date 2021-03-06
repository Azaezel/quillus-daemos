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

#include "Message.hpp"
#include "AuthenticateResponse.hpp"
#include "MessageFactory.hpp"

#include <Zen/Enterprise/Networking/I_Endpoint.hpp>

#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>
#include <Zen/Enterprise/AppServer/I_MessageHeader.hpp>
#include <Zen/Enterprise/AppServer/I_MessageFactory.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_NumericTypeMessageRegistry.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace Account {
namespace Protocol {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AuthenticateResponse::pMessageType_type AuthenticateResponse::sm_pType;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_MessageRegistry* AuthenticateResponse::sm_pMessageRegistry = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AuthenticateResponse::AuthenticateResponse(pEndpoint_type _pSourceEndpoint,
                             pEndpoint_type _pDestinationEndpoint,
                             pResourceLocation_type _pSrcLocation,
                             pResourceLocation_type _pDestLocation,
                             boost::uint64_t _requestMessageId)
:   Response(AuthenticateResponse::createMessageHeader(getNewMessageId(), _requestMessageId, _pSrcLocation->toString(), _pDestLocation->toString()), _pSourceEndpoint, _pDestinationEndpoint)
,   m_account()
,   m_authenticated(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AuthenticateResponse::AuthenticateResponse(pMessageHeader_type _pMessageHeader,
                             pEndpoint_type _pSourceEndpoint,
                             pEndpoint_type _pDestinationEndpoint)
:   Response(_pMessageHeader, _pSourceEndpoint, _pDestinationEndpoint)
,   m_account()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AuthenticateResponse::~AuthenticateResponse()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AuthenticateResponse::serialize(pMessageHeader_type _pHeader, boost::archive::polymorphic_iarchive& _archive, const int _version)
{
    _archive & m_authenticated;
    _archive & m_account;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AuthenticateResponse::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
    _archive & m_authenticated;
    _archive & m_account;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Enterprise::Account::I_Account&
AuthenticateResponse::getAccount() const
{
    return m_account;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AuthenticateResponse::setAccount(const Enterprise::Account::I_Account& _account)
{
    const Enterprise::Account::Client::Account* pAccount =
        dynamic_cast<const Enterprise::Account::Client::Account*>(&_account);

    if (pAccount != NULL)
    {
        m_account = *pAccount;
    }
    else
    {
        // TODO Throw exception?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
AuthenticateResponse::getAuthenticated() const
{
    return m_authenticated;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AuthenticateResponse::setAuthenticated(const bool& _authenticated)
{
    m_authenticated = _authenticated;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AuthenticateResponse::registerMessage(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
{
    // Register messages.  Not keeping a managed pointer because it's really a singleton.
    sm_pMessageRegistry = _appServer.getMessageRegistry().get();

    Zen::Enterprise::AppServer::I_NumericTypeMessageRegistry*
        pMessageRegistry = dynamic_cast<Zen::Enterprise::AppServer::I_NumericTypeMessageRegistry*>
            (sm_pMessageRegistry);

    if (!sm_pMessageFactory.isValid())
    {
        sm_pMessageFactory = pMessageFactory_type(new MessageFactory, destroyMessageFactory);
    }

    if (pMessageRegistry != NULL)
    {
        sm_pType = pMessageRegistry->getMessageType(type);
        sm_pType->setMessageFactory(sm_pMessageFactory);
        sm_pMessageRegistry->registerMessageType(sm_pType);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AuthenticateResponse::pMessageHeader_type
AuthenticateResponse::createMessageHeader(boost::uint64_t _messageId, boost::uint64_t _requestId, const std::string& _srcLocation, const std::string& _destLocation)
{
    return sm_pMessageRegistry->createMessageHeader(sm_pType, _messageId,_requestId,_srcLocation,_destLocation);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
AuthenticateResponse::destroy(wpResponse_type _wpResponse)
{
    AuthenticateResponse* pResponse = dynamic_cast<AuthenticateResponse*>(_wpResponse.get());

    if( pResponse != NULL )
    {
        delete pResponse;
    }
    else
    {
        throw Zen::Utility::runtime_exception("AuthenticateResponse::destroy() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AuthenticateResponse::pMessageType_type
AuthenticateResponse::getStaticMessageType()
{
    return sm_pType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Protocol
}   // namespace Account
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
