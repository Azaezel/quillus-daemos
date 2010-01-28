//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ProtocolServiceFactory.hpp"
#include "HyperTextTransportProtocolService.hpp"
#include "UserDatagramProtocolService.hpp"
#include "TransmissionControlProtocolService.hpp"
#include "XML/XMLProtocolService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory::ProtocolServiceFactory()
{
    m_factoryFunctions.insert(std::make_pair<std::string, FactoryFunction_type>
            ("http", createHttp));
    m_factoryFunctions.insert(std::make_pair<std::string, FactoryFunction_type>
            ("udp", createUdp));
    m_factoryFunctions.insert(std::make_pair<std::string, FactoryFunction_type>
            ("xml", createXML));
    m_factoryFunctions.insert(std::make_pair<std::string, FactoryFunction_type>
            ("tcp-binary", createTCP));

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory::~ProtocolServiceFactory()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory::pProtocolService_type
ProtocolServiceFactory::createHttp(I_ApplicationServer& _server)
{
    HyperTextTransportProtocolService* pRawService = new HyperTextTransportProtocolService(_server);

    pProtocolService_type pProtocolService(pRawService, destroyHttp);

    // TODO Need self reference?

    return pProtocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProtocolServiceFactory::destroyHttp(wpProtocolService_type _pProtocolService)
{
    HyperTextTransportProtocolService* pRaw = dynamic_cast<HyperTextTransportProtocolService*>(_pProtocolService.get());

    if (pRaw)
    {
        delete pRaw;
    }
    else
    {
        throw Utility::runtime_exception("ProtocolServiceFactory::destroyHttp(): Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory::pProtocolService_type
ProtocolServiceFactory::createUdp(I_ApplicationServer& _server)
{
    UDP::UserDatagramProtocolService* pRawService = new UDP::UserDatagramProtocolService(_server);

    pProtocolService_type pProtocolService(pRawService, destroyUdp);

    wpProtocolService_type pWeakPtr(pProtocolService);

    pRawService->setSelfReference(pWeakPtr);

    return pProtocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProtocolServiceFactory::destroyUdp(wpProtocolService_type _pProtocolService)
{
    UDP::UserDatagramProtocolService* pRawService = 
        dynamic_cast<UDP::UserDatagramProtocolService*>(_pProtocolService.get());

    if( pRawService != NULL )
    {
        delete pRawService;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::Enterprise::AppServer::ProtocolServiceFactory::destroyUdp() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory::pProtocolService_type
ProtocolServiceFactory::createTCP(I_ApplicationServer& _server)
{
    TransmissionControlProtocolService* pRawService = new TransmissionControlProtocolService(_server);

    pProtocolService_type pProtocolService(pRawService, destroyTCP);

    wpProtocolService_type pWeakPtr(pProtocolService);

    pRawService->setSelfReference(pWeakPtr);

    return pProtocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProtocolServiceFactory::destroyTCP(wpProtocolService_type _pProtocolService)
{
    TransmissionControlProtocolService* pRawService = 
        dynamic_cast<TransmissionControlProtocolService*>(_pProtocolService.get());

    if( pRawService != NULL )
    {
        delete pRawService;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::Enterprise::AppServer::ProtocolServiceFactory::destroyTCP() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory::pProtocolService_type
ProtocolServiceFactory::createXML(I_ApplicationServer& _server)
{
    XML::XMLProtocolService* pRawService = new XML::XMLProtocolService(_server);

    pProtocolService_type pProtocolService(pRawService, destroyUdp);

    return pProtocolService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProtocolServiceFactory::destroyXML(wpProtocolService_type _pProtocolService)
{
    XML::XMLProtocolService* pRawService = 
        dynamic_cast<XML::XMLProtocolService*>(_pProtocolService.get());

    if( pRawService != NULL )
    {
        delete pRawService;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::Enterprise::AppServer::ProtocolServiceFactory::destroyXML() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory::pProtocolService_type
ProtocolServiceFactory::create(I_ApplicationServer& _server, const std::string& _type)
{
    FactoryFunctions_type::iterator iter = m_factoryFunctions.find(_type);
    if (iter != m_factoryFunctions.end())
    {
        return iter->second(_server);
    }
    return pProtocolService_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProtocolServiceFactory&
ProtocolServiceFactory::getSingleton()
{
    static ProtocolServiceFactory sm_ProtocolServiceFactory;

    return sm_ProtocolServiceFactory;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
