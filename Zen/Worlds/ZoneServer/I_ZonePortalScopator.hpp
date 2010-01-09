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
#ifndef ZEN_WORLDS_ZONE_I_ZONE_PORTAL_SCOPATOR_HPP_INCLUDED
#define ZEN_WORLDS_ZONE_I_ZONE_PORTAL_SCOPATOR_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
namespace Zone {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ZoneControllable;
class I_ZonePortal;

/// Zone Portal Scopator interface.
/// Implement this interface and pass an instance of the implementation to
/// I_ZonePortalServerService::setScopator().  Your implementation should
/// monitor the movement of dynamic objects and call (???) I_ZoneP
class ZONESERVER_DLL_LINK I_ZonePortalScopator
{
    /// @name Types
    /// @{
public:
    struct ZonePortalEventPayload;
    typedef Zen::Event::Event<ZonePortalEventPayload&>      ZonePortalEvent_type;
    /// @}

    /// @name I_ZonePortalScopator interface
    /// @{
public:
    /// This handler is called when a zone controllable object is modified.
    /// Your implementation should check to see if the controllable object
    /// entered, left or approached a zone and fire the appropriate events.
    virtual void onControllableModified(I_ZoneControllable& _controllable) = 0;
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct ZonePortalEventPayload
    {
        /// The controllable associated with this event.
        I_ZoneControllable&         controllable;

        /// The portal associated with this event.
        I_ZonePortal&               portal;
    };
    /// @}

    ZonePortalEvent_type    PortalEnteredScopeEvent;
    ZonePortalEvent_type    PortalLeftScopeEvent;
    ZonePortalEvent_type    ControllableEnteredZoneEvent;
    ZonePortalEvent_type    ControllableLeftZoneEvent;

    /// @name 'Structors
    /// @{
protected:
             I_ZonePortalScopator();
    virtual ~I_ZonePortalScopator();
    /// @}

};  // interface I_ZonePortalScopator

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zone
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_WORLDS_ZONE_I_ZONE_PORTAL_SCOPATOR_HPP_INCLUDED
