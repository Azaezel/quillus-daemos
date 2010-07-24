//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Community Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_COMMUNITY_CHATCOMMON_I_CHAT_SERVICE_HPP_INCLUDED
#define ZEN_COMMUNITY_CHATCOMMON_I_CHAT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

#include <boost/cstdint.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Community {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Channel;
class I_ChannelModel;
class I_ChannelController;
class I_Account;
class I_Group;

/// Chat Service interface.
/// See this discussion for more details about this design.
/// http://www.indiezen.org/wiki/irclogs/2009/03/19/#UTC2009-03-19T14:27:25
/// @see I_ApplicationServer::getApplication() to get an instance of an 
///     implementation of this interface.
class CHATCOMMON_DLL_LINK I_ChatService
:   public Enterprise::AppServer::I_ApplicationService
{
    /// @name Forward declarations
    /// @{
public:
    /// @}

    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_ChatService>                      pService_type;

    typedef Memory::managed_ptr<I_ChannelModel>                     pChannelModel_type;
    typedef Memory::managed_weak_ptr<I_ChannelModel>                wpChannelModel_type;
    typedef Event::future_return_value<pChannelModel_type>          FutureChannelModel_type;
    typedef Memory::managed_ptr<FutureChannelModel_type>            pFutureChannelModel_type;

    typedef Memory::managed_ptr<I_ChannelController>                pChannelController_type;
    typedef Memory::managed_weak_ptr<I_ChannelController>           wpChannelController_type;
    typedef Event::future_return_value<pChannelController_type>     FutureChannelController_type;
    typedef Memory::managed_ptr<FutureChannelController_type>       pFutureChannelController_type;
    /// @}

    /// @name I_ChatService interface
    /// @{
public:
    virtual pFutureChannelModel_type createChannelModel() = 0;

    virtual pFutureChannelController_type createChannelController(I_ChannelModel& _model) = 0;

    /// TODO These should be propagated to I_PermissibleResource
    /// and I_ChatService should be made to extend I_PermissibleResource
    virtual boost::uint32_t getAccessFlags() const = 0;                 // TODO Abstract access flags to an I_Permissions interface?
    virtual void setAccessFlags(boost::uint32_t _accessFlags) = 0;      // TODO Abstract access flags to an I_Permissions interface?
    virtual const I_Account& getOwner() const = 0;
    virtual void setOwner(const I_Account& _owner) = 0;
    virtual const I_Group& getGroup() const = 0;
    virtual void setGroup(const I_Group& _group) = 0;
    /// @}

    /// @name Inner structs
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ChatService();
public:
    virtual ~I_ChatService();
    /// @}

};  // interface I_ChatService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Community
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_COMMUNITY_CHATCOMMON_I_CHAT_SERVICE_HPP_INCLUDED
