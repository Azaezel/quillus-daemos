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
#ifndef ZEN_WORLDS_COMMON_I_WORLD_EVENT_QUEUE_HPP_INCLUDED
#define ZEN_WORLDS_COMMON_I_WORLD_EVENT_QUEUE_HPP_INCLUDED

#include "Configuration.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
namespace Common {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_WorldEvent;

/// World Event Queue.
/// This is a bidirectional queue of all of the events being received from
/// and sent to the world service.
class WORLDCOMMON_DLL_LINK I_WorldEventQueue
{
    /// @name Types
    /// @{
public:
    enum DeliveryRule
    {
        /// Unreliable delivery.  If the event is dropped, the event is not
        /// resent.
        UNRELIABLE = 0,

        /// Reliable delivery.  If the event is dropped, it is resent until
        /// the recipient acknowledges the event or until the session is
        /// closed.
        RELIABLE = 1,

        /// Guaranteed delivery.  If the event is dropped, it is resent until
        /// the recipient acknowledges the event.  The event is persisted, so
        /// even if the session is closed, the event will be resent after the 
        /// session is re-established.
        GUARANTEED = 2,

        /// Ordered delivery.  This is "or'd" with RELIABLE or GUARANTEED
        /// to make the even fire in the same order as other RELIABLE or
        /// GUARANTEED events.
        /// This bitmask does not have any affect on UNRELIABLE events.
        ORDERED = 4
    };

    typedef Memory::managed_ptr<I_WorldEvent>           pWorldEvent_type;
    /// @}

    /// @name I_WorldEventQueue interface
    /// @{
public:
    /// @return true if this queue has any incoming events.
    virtual bool hasIncomingEvent() = 0;

    /// Get the next event in the queue.  This method will block until an
    /// event is received.  If you need to poll instead of block, be sure
    /// to call hasIncomingEvent() first.
    virtual void pWorldEvent_type popEvent() = 0;

    /// Push an outgoing event.
    virtual void pushEvent(pWorldEvent_type _pEvent, DeliveryRule _deliveryRule = UNRELIABLE) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WorldEventQueue();
    virtual ~I_WorldEventQueue();
    /// @}

};  // interface I_WorldEventQueue

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Common
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ZEN_WORLDS_COMMON_I_WORLD_EVENT_QUEUE_HPP_INCLUDED
