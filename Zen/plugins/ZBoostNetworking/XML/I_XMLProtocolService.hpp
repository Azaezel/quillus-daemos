//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENTERPRISE_APPSERVER_I_XML_PROTOCOL_SERVICE_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_I_XML_PROTOCOL_SERVICE_HPP_INCLUDED

#include "../Configuration.hpp"

#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
namespace XML {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ZBOOSTNETWORKING_DLL_LINK I_XMLProtocolService
:   public I_ProtocolService
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ResourceLocation>         pResourceLocation_type;
    /// @}

    /// @name I_XMLProtocolService interface
    /// @{
public:
    /// Register a partial URL with a destination service location.
    /// All HTTP requests that partially match this _url will be directed 
    /// to the application service that handles _pDestination.
    ///
    /// The first parameter is the string used to partially
    /// match the URL.  The second entry is the location used by
    /// the application server to dispatch the request to the correct
    /// application service.  The longest match will be used.
    ///
    /// Examples: 
    ///     _url = /       will match the HTTP requests / and /xyz
    ///     _url = /xyz    will match the HTTP requests /xyz
    /// 
    /// If there are more than one matches, the longest match will be used. 
    /// In the above example _url = /xyz will actually end up matching 
    /// HTTP request /xyz even though _url = / also matches.
    virtual void registerURL(const std::string& _url, pResourceLocation_type _pDestination) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_XMLProtocolService();
    virtual ~I_XMLProtocolService();
    /// @}

};  // interface I_XMLProtocolService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace XML
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_I_XML_PROTOCOL_SERVICE_HPP_INCLUDED
