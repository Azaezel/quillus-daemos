//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Worlds Framework
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
#ifndef ZEN_WORLDS_ZONE_I_ZONE_SERVICE_HPP_INCLUDED
#define ZEN_WORLDS_ZONE_I_ZONE_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
namespace Zone {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ZoneSession;
class I_ZoneId;
class I_WorldSession;

/// Zone Service interface.
/// I_ZoneService provides an interface for connecting to a zone, and
/// receiving object creation, deletion and replication events.
/// In multi-zoned games and virtual worlds there is one zone service per zone.
/// See this discussion for more details about this design.
/// http://www.indiezen.org/wiki/irclogs/2009/03/19/#UTC2009-03-19T14:27:25
/// @see I_ApplicationServer::getApplication() to get an instance of an 
///     implementation of this interface.
class ZONECOMMON_DLL_LINK I_ZoneService
:   public Enterprise::AppServer::I_ApplicationService
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_WorldSession>            pWorldSession_type;
    typedef Zen::Memory::managed_ptr<I_ZoneId>                  pZoneId_type;
    typedef Zen::Memory::managed_ptr<I_ZoneSession>             pZoneSession_type;
    typedef Zen::Event::future_return_value<pZoneSession_type>  FutureZoneSession_type;
	typedef Zen::Memory::managed_ptr<FutureZoneSession_type>    pFutureZoneSession_type;
    /// @}

    /// @name I_ZoneService interface
    /// @{
public:
    /// Connect to the zone server for the specified ZoneId.
    virtual pFutureZoneSession_type connect(pWorldSession_type _pWorldSession, pZoneId_type _pZoneId) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ZoneService();
    virtual ~I_ZoneService();
    /// @}

};  // interface I_ZoneService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zone
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_WORLDS_ZONE_I_ZONE_SERVICE_HPP_INCLUDED
