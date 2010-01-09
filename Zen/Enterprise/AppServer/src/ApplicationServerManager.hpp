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
#ifndef ZEN_APPSERVER_APPLICATION_SERVER_MANAGER_HPP_INCLUDED
#define ZEN_APPSERVER_APPLICATION_SERVER_MANAGER_HPP_INCLUDED
#include "../I_ApplicationServerManager.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ResourceLocation;

class ApplicationServerManager
:   public I_ApplicationServerManager
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ProtocolService>          pProtocolService_type;
    typedef Memory::managed_ptr<I_ApplicationService>       pApplicationService_type;
    typedef Memory::managed_ptr<I_ResourceLocation>         pResourceLocation_type;
    typedef Memory::managed_weak_ptr<I_ResourceLocation>    wpResourceLocation_type;
    /// @}

    /// @name I_ApplicationServerManager implementation.
    /// @{
public:
    virtual pProtocolService_type createProtocolService(I_ApplicationServer& _server, const std::string& _type);
    virtual pApplicationService_type createApplicationService(I_ApplicationServer& _server, const std::string& _type);
    virtual pResourceLocation_type createLocation(const std::string& _location);
    /// @}

    /// @name ApplicationServerManager implementation
    /// @{
private:
    void destroyLocation(wpResourceLocation_type _location);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class I_ApplicationServerManager;
             ApplicationServerManager();
    virtual ~ApplicationServerManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    typedef std::map<std::string, wpResourceLocation_type>   ResourceLocations_type;

    /// Guard for m_resourceLocations
    Threading::I_Mutex*                                 m_pResourceLocationGuard;
    /// Map of resource locations from the string representation to the actual object
    ResourceLocations_type                              m_resourceLocations;
    /// @}

};  // interface ApplicationServerManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_APPLICATION_SERVER_MANAGER_HPP_INCLUDED
