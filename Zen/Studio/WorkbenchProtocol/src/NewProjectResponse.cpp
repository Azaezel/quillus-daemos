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
// This is generated by the Zen Protocol Code Generator.  Do not modify!
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "Message.hpp"        
#include "NewProjectResponse.hpp"
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
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NewProjectResponse::pMessageType_type NewProjectResponse::sm_pType;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Enterprise::AppServer::I_MessageRegistry* NewProjectResponse::sm_pMessageRegistry = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NewProjectResponse::NewProjectResponse(pEndpoint_type _pSourceEndpoint,
                           pEndpoint_type _pDestinationEndpoint, boost::uint64_t _requestMessageId)
:   Response(NewProjectResponse::createMessageHeader(getNewMessageId(), _requestMessageId), _pSourceEndpoint, _pDestinationEndpoint)        
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NewProjectResponse::NewProjectResponse(pMessageHeader_type _pMessageHeader,
                             pEndpoint_type _pSourceEndpoint,
                             pEndpoint_type _pDestinationEndpoint)            
:   Response(_pMessageHeader, _pSourceEndpoint, _pDestinationEndpoint) 
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NewProjectResponse::~NewProjectResponse()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NewProjectResponse::serialize(pMessageHeader_type _pHeader, boost::archive::polymorphic_iarchive& _archive, const int _version)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NewProjectResponse::serialize(boost::archive::polymorphic_oarchive& _archive, const int _version)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NewProjectResponse::registerMessage(Zen::Enterprise::AppServer::I_ApplicationServer& _appServer)
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
NewProjectResponse::pMessageHeader_type
NewProjectResponse::createMessageHeader(boost::uint64_t _messageId, boost::uint64_t _requestId)
{
    return sm_pMessageRegistry->createMessageHeader(sm_pType, _messageId, _requestId, "", "");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NewProjectResponse::destroy(wpResponse_type _wpResponse)
{
    NewProjectResponse* pResponse = dynamic_cast<NewProjectResponse*>(_wpResponse.get());

    if( pResponse != NULL )
    {
        delete pResponse;
    }
    else
    {
        throw Zen::Utility::runtime_exception("NewProjectResponse::destroy() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Message::pResourceLocation_type  sm_pResourceLocation;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NewProjectResponse::pResourceLocation_type
NewProjectResponse::getDestinationLocation()
{
    if(!sm_pResourceLocation.isValid())
    {
        sm_pResourceLocation = Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton().createLocation("/workbenchClient");
    }
    return sm_pResourceLocation;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NewProjectResponse::pMessageType_type
NewProjectResponse::getStaticMessageType()
{
    return sm_pType;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zen
}   // namespace Studio
}   // namespace Workbench
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
