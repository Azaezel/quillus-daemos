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
#ifndef ZEN_APPSERVER_I_NUMERIC_TYPE_MESSAGE_REGISTRY_HPP_INCLUDED
#define ZEN_APPSERVER_I_NUMERIC_TYPE_MESSAGE_REGISTRY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Enterprise/AppServer/I_MessageRegistry.hpp>
#include <Zen/Enterprise/AppServer/I_MessageType.hpp>

#include <boost/cstdint.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Message Registry that uses I_MessageType with a numeric
/// uint32_t implementation.
class APPSERVER_DLL_LINK I_NumericTypeMessageRegistry
:   public I_MessageRegistry
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_MessageType>         pMessageType_type;
    typedef Zen::Memory::managed_weak_ptr<I_MessageType>    wpMessageType_type;
    typedef Zen::Memory::managed_ptr<I_MessageRegistry>     pMessageRegistry_type;
    /// @}

    /// @name I_NumericTypeMessageRegistry interface.
    /// @{
public:
    virtual pMessageType_type getMessageType(boost::uint32_t _type) = 0;
    /// @}

    /// @name Inner types 
    /// @{
public:
    class I_NumericType
    :   public I_MessageType
    {
    public:
        virtual boost::uint32_t getType() const = 0;
    };  // class NumericType

    /// @}
    /// @name 'Structors
    /// @{
protected:
             I_NumericTypeMessageRegistry();
    virtual ~I_NumericTypeMessageRegistry();
    /// @}

};  // interface I_NumericTypeMessageRegistry

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
namespace Memory {
    // I_NumericTypeMessageRegistry is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Enterprise::AppServer::I_NumericTypeMessageRegistry> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_APPSERVER_I_NUMERIC_TYPE_MESSAGE_REGISTRY_HPP_INCLUDED
