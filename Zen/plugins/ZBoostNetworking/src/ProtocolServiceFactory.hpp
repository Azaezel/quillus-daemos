//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_ENTERPRISE_APPSERVER_PROTOCOL_SERVICE_FACTORY_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_PROTOCOL_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Enterprise/AppServer/I_ProtocolServiceFactory.hpp>

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <boost/function.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ProtocolServiceFactory
:   public I_ProtocolServiceFactory
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<I_ProtocolService>  wpProtocolService_type;
    /// @}

    /// @name I_ProtocolServiceFactory implementation
    /// @{
public:
    virtual pProtocolService_type create(I_ApplicationServer& _server, const std::string& _type);
    /// @}

    /// @name Factory Methods
    /// @{
private:
    static pProtocolService_type    createHttp(I_ApplicationServer& _server);
    static void                     destroyHttp(wpProtocolService_type _pProtocolService);

    static pProtocolService_type    createUdp(I_ApplicationServer& _server);
    static void                     destroyUdp(wpProtocolService_type _pProtocolService);

    static pProtocolService_type    createTCP(I_ApplicationServer& _server);
    static void                     destroyTCP(wpProtocolService_type _pProtocolService);

    static pProtocolService_type    createXML(I_ApplicationServer& _server);
    static void                     destroyXML(wpProtocolService_type _pProtocolService);
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get the singleton
    /// @return Reference to the singleton instance.
    static ProtocolServiceFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ProtocolServiceFactory();
    virtual ~ProtocolServiceFactory();
    /// @}

    /// @name Member Variables
    /// @{
private:
    typedef boost::function<pProtocolService_type(I_ApplicationServer&)>    FactoryFunction_type;
    typedef std::map<std::string, FactoryFunction_type> FactoryFunctions_type;

    FactoryFunctions_type m_factoryFunctions;
    /// @}

};  // class ProtocolServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_PROTOCOL_SERVICE_FACTORY_HPP_INCLUDED
