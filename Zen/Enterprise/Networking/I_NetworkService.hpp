//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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
#ifndef ZEN_NETWORKING_I_NETWORK_SERVICE_HPP_INCLUDED
#define ZEN_NETWORKING_I_NETWORK_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Networking {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_NetworkAddress;
class I_NetworkConnection;
class I_NetworkConnectionListener;
class I_NetworkReadReadyListener;
class I_NetworkWriteReadyListener;

/// Basic network service interface
class NETWORKING_DLL_LINK I_NetworkService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_NetworkConnection>     pNetworkConnection_type;
    /// @}

    /// @name I_NetworkService interface
    /// @{
public:
    /// Connect to the specified network address
    ///
    /// A valid I_NetworkConnection is always returned if _address is valid, but
    /// it's possible that the returned object is not connected.  If the
    /// I_NetworkService implementation implements an asynchronous connect method
    /// then the logical / physical connection might occur at a later time.
    ///
    /// @return Network connection or NULL if _address is invalid.
    /// @see I_NetworkConnection
    virtual pNetworkConnection_type connect(const I_NetworkAddress& _address) = 0;

    /// Listens on the specified network address
    virtual void listen(const I_NetworkAddress& _address) = 0;

    /// Registers a network connection listener
    virtual void registerConnectionListener(I_NetworkConnectionListener& _listener) = 0;

    /// Unregisters a network connection listener
    virtual void unregisterConnectionListener(I_NetworkConnectionListener& _listener) = 0;

    /// Registers the one Read Ready Listener
    /// If a listener has already been registered then it will be superceeded.
    /// If NULL is specified for _pListener then the previous listener will be 
    //  unregistered.
    virtual void setReadReadyListener(I_NetworkReadReadyListener* const _pListener) = 0;

    /// Registers the one Write Ready Listener
    /// If a listener has already been registered then it will be superceeded.
    /// If NULL is specified for _pListener then the previous listener will be 
    ///  unregistered.
    virtual void setWriteReadyListener(I_NetworkWriteReadyListener* const _pListener) = 0;

	/// At the network level, a write must be requested before writing.
	/// I_NetworkWriteReadyListener::onWriteReady will indicate that the connection
	/// is ready for writing.
	virtual void requestWrite(pNetworkConnection_type const _pConnection) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_NetworkService();
    virtual ~I_NetworkService();
    /// @}

};  // interface I_NetworkService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Networking
namespace Memory 
{
    /// I_NetworkService is managed by a factory
    template<>
    struct is_managed_by_factory<Networking::I_NetworkService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_NETWORKING_I_NETWORK_SERVICE_HPP_INCLUDED
