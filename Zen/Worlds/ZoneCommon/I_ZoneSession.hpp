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
#ifndef ZEN_WORLDS_ZONE_I_ZONE_SESSION_HPP_INCLUDED
#define ZEN_WORLDS_ZONE_I_ZONE_SESSION_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
namespace Zone {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ZonePortalListener;
class I_ZoneObjectListener;

/// Zone Session.
class ZONECOMMON_DLL_LINK I_ZoneSession
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Spaces::ObjectModel::I_Subscription>    pSubscription_type;
    typedef Memory::managed_ptr<I_ZonePortalListener>                   pZonePortalListener_type;
    typedef Memory::managed_ptr<I_ZoneObjectListener>                   pZoneObjectListener_type;
    /// @}

    /// @name I_ZoneSession interface
    /// @{
public:
    /// Subscribe to zone portal events.
    /// Zone portal events are events that occur when the player (or player's
    /// controlled object or camera) approaches a new zone, enters a zone
    /// and leaves a zone.
    virtual pSubscription_type subscribe(pZonePortalListener_type _pListener) = 0;

    /// Subscribe to zone object events.
    /// Zone object events are events that occur when a zone object is created,
    /// destroyed, or modified.
    virtual pSubscription_type subscribe(pZoneObjectListener_type _pListener) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ZoneSession();
    virtual ~I_ZoneSession();
    /// @}

};  // interface I_ZoneSession

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zone
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_WORLDS_ZONE_I_ZONE_SESSION_HPP_INCLUDED
