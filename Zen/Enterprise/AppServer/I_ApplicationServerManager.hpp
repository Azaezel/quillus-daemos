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
#ifndef ZEN_APPSERVER_I_APPLICATION_SERVER_MANAGER_HPP_INCLUDED
#define ZEN_APPSERVER_I_APPLICATION_SERVER_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ProtocolService;
class I_ApplicationService;
class I_ResourceLocation;
class I_ApplicationServer;

class APPSERVER_DLL_LINK I_ApplicationServerManager
:   boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ProtocolService>          pProtocolService_type;
    typedef Memory::managed_ptr<I_ApplicationService>       pApplicationService_type;
    typedef Memory::managed_ptr<I_ResourceLocation>         pResourceLocation_type;
    /// @}

    /// @name I_ApplicationServerManager interface.
    /// @{
public:
    /// Create a protocol service of the specified type.
    virtual pProtocolService_type createProtocolService(I_ApplicationServer& _server, const std::string& _type) = 0;

    /// Create an Application Service
    virtual pApplicationService_type createApplicationService(I_ApplicationServer& _server, const std::string& _type) = 0;

    /// Create a resource location
    virtual pResourceLocation_type createLocation(const std::string& _location) = 0;
    /// @}

    /// @name Static Instance
    /// @{
public:
    /// Get the singleton
    /// @return Reference to the singleton instance.
    static I_ApplicationServerManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ApplicationServerManager();
    virtual ~I_ApplicationServerManager();
    /// @}

};  // interface I_ApplicationServerManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_I_APPLICATION_SERVER_MANAGER_HPP_INCLUDED
