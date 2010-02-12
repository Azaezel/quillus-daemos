//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#ifndef INDIEZEN_BASIC_NETWORKING_BASIC_NETWORK_ADDRESS_HPP_INCLUDED
#define INDIEZEN_BASIC_NETWORKING_BASIC_NETWORK_ADDRESS_HPP_INCLUDED

#include <IndieZen/framework/Networking/I_NetworkAddress.hpp>

// NOTE: I'm not quite sure this is correct.  It's possible that
// differences exist among non Windows platforms, so instead
// of "else everything else", it's possible to require
// "else Linux" and "else Solaris" and others.
#ifdef _WIN32
#include <Winsock2.h>
#include <Ws2tcpip.h>
#else
#include <sys/types.h>
#include <netinet/in.h>   /* sockaddr_in structure */
#include <sys/socket.h>   /* SOCK_STREAM */
#endif

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace Networking {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BasicNetworkAddress
:   public I_NetworkAddress
{
    /// @name I_NetworkAddress implementation
    /// @{
public:
    /// @}

	/// @name BasicNetworkAddress implementation
	/// @{
public:
    const sockaddr* getSocketAddress() const;
    int getSize() const;
    bool valid() const;
    std::string asString() const;
	/// @}

    /// @name 'Structors
    /// @{
public:
             BasicNetworkAddress();
             BasicNetworkAddress(unsigned short _port);
             BasicNetworkAddress(unsigned long _address, unsigned short _port);
    virtual ~BasicNetworkAddress();
    /// @}

	/// @name Member Variables
	/// @{
private:
    bool		m_isValid;    //< True if valid
    sockaddr_in m_sockaddr;
	/// @}

};  // interface BasicNetworkAddress

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Networking
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_BASIC_NETWORKING_BASIC_NETWORK_SERVICE_HPP_INCLUDED
