//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ENGINE_PHYSICS_I_PHYSICS_FRAME_LISTENER_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_I_PHYSICS_FRAME_LISTENER_HPP_INCLUDED

#include "Configuration.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Physics Frame Listener.
/// Listens to a physics frame, which steps through a physics simulation.
class PHYSICS_DLL_LINK I_PhysicsFrameListener
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_PhysicsFrameListener interface
    /// @{
public:
    /// Called when the frame simulation is about to begin.  Implementations
    /// should use this to initialize frame related data.
    virtual void beginFrame() = 0;

    /// Set the physics simulation for this zone.
    virtual void stepSimulation(double _elapsedTime) = 0;

    /// Called when the frame is complete.  Implementations should use this
    /// for cleaning up after the frame.
    virtual void endFrame() = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PhysicsFrameListener();
    virtual ~I_PhysicsFrameListener();
    /// @}
};  // interface I_PhysicsFrameListener

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_I_PHYSICS_FRAME_LISTENER_HPP_INCLUDED
