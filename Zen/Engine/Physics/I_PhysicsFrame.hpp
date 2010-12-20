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
#ifndef ZEN_ENGINE_PHYSICS_I_PHYSICS_FRAME_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_I_PHYSICS_FRAME_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_PhysicsFrameListener;

/// Physics Frame.
/// A physics frame is a frame in the physics simulation.  A single frame
/// may be several physics simulation steps depending on the physics
/// engine being used.
class PHYSICS_DLL_LINK I_PhysicsFrame
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_PhysicsFrame>         pPhysicsFrame_type;
    typedef Memory::managed_weak_ptr<I_PhysicsFrame>    wpPhysicsFrame_type;
    typedef I_PhysicsFrameListener*                     pPhysicsFrameListener_type;
    /// @}

    /// @name I_PhysicsFrame interface
    /// @{
public:
    /// Register a listener.
    virtual void registerListener(pPhysicsFrameListener_type _pPhysicsFrameListener) = 0;

    /// Step through a physics frame lasting the specified duration.
    /// @param _elapsedTime amount of time that the frame represents in
    ///     seconds.
    virtual void stepFrame(double _elapsedTime) = 0;

    /// Set the step size for the physics simulation.
    virtual void setStepSize(double _stepSize) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PhysicsFrame();
    virtual ~I_PhysicsFrame();
    /// @}
};  // interface I_PhysicsFrame

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
namespace Memory
{
    /// I_PhysicsZone is managed by a I_PhysicsService
    template<>
    struct is_managed_by_factory<Zen::Engine::Physics::I_PhysicsFrame> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_I_PHYSICS_ZONE_HPP_INCLUDED
