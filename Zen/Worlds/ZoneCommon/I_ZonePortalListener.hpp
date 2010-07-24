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
#ifndef ZEN_WORLDS_ZONE_I_ZONE_PORTAL_LISTENER_HPP_INCLUDED
#define ZEN_WORLDS_ZONE_I_ZONE_PORTAL_LISTENER_HPP_INCLUDED

#include "Configuration.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
namespace Zone {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Zone Portal Listener interface.
/// Implement this interface and pass an instance of the implementation to
/// I_ZoneSession::subscribe() in order to receive zone portal notifications.
/// Zone portal events occur when a player or camera approaches a portal, and
/// when a player or camera enters or leaves a zone via a portal.
/// @see I_ZoneSession::subscribe()
class ZONECOMMON_DLL_LINK I_ZonePortalListener
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ZonePortalListener interface
    /// @{
public:
    /// This method is called when a portal enters the scope of a controllable object.
    /// When this listener is subscribed to a I_ZoneSession, the session object will 
    /// use a I_ZonePortalScopator to scope controllable objects to portals.  This method
    /// is called when a controllable object moves within the scope of a portal.
    /// @param _controllable The controllable object entering the portal scope.
    /// @param _portal The portal that is being scoped.
    /// @note Scope is defined by the I_ZonePortalScopator and may vary from implementation to 
    ///     implementation.  Generally "scope" is defined as "within the object's view" although
    ///     other conditions, data and factors can be taken into consideration depending
    ///     on the type of simulation.
    virtual void onPortalEnterScope(I_ZoneControllable& _controllable, I_ZonePortal& _portal) = 0;

    /// This method is called when a portal leaves the scope of a controllable object.
    /// When this listener is subscribed to a I_ZoneSession, the session object will 
    /// use a I_ZonePortalScopator to scope controllable objects to portals.  This method
    /// is called when a controllable object leaves the scope of a portal.
    /// @param _controllable The controllable object leaving the portal scope.
    /// @param _portal The portal that is being scoped.
    /// @note Scope is defined by the I_ZonePortalScopator and may vary from implementation to 
    ///     implementation.  Generally "scope" is defined as "within the object's view" although
    ///     other conditions, data and factors can be taken into consideration depending
    ///     on the type of simulation.
    virtual void onPortalLeaveScope(I_ZoneControllable& _controllable, I_ZonePortal& _portal) = 0;
    
    /// This method is called when a controllable object transitions through a portal.
    /// @param _controllable The controllable object transitioning through the portla.
    /// @param _portal The portal through which the contollable object is transitioning.
    ///     Use this portal to get the zone from which and to which the controllable object
    ///     is transitioning.
    virtual void onTransitionPortal(I_ZoneControllable& _controllable, I_ZonePotal& _portal) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ZonePortalListener();
    virtual ~I_ZonePortalListener();
    /// @}

};  // interface I_ZonePortalListener

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Zone
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_WORLDS_ZONE_I_ZONE_PORTAL_LISTENER_HPP_INCLUDED
