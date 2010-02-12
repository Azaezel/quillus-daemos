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
#ifndef ZEN_ENTERPRISE_APPSERVER_APPLICATION_SERVICE_FACTORY_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_APPLICATION_SERVICE_FACTORY_HPP_INCLUDED

#include <Zen/Enterprise/AppServer/I_ApplicationServiceFactory.hpp>

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <boost/function.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ApplicationServiceFactory
:   public I_ApplicationServiceFactory
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<I_ApplicationService>  wpApplicationService_type;
    /// @}

    /// @name I_ProtocolServiceFactory implementation
    /// @{
public:
    virtual pApplicationService_type create(I_ApplicationServer& _server, const std::string& _name);
    /// @}

    /// @name Factory Methods
    /// @{
private:
    static pApplicationService_type createHello(I_ApplicationServer& _server);
    static void                     destroyHello(wpApplicationService_type _pApplicationService);
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get the singleton
    /// @return Reference to the singleton instance.
    static ApplicationServiceFactory& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ApplicationServiceFactory();
    virtual ~ApplicationServiceFactory();
    /// @}

    /// @name Member Variables
    /// @{
private:
    typedef boost::function<pApplicationService_type(I_ApplicationServer&)> FactoryFunction_type;
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
