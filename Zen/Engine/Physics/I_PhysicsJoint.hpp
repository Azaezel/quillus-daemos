//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_PHYSICS_I_PHYSICS_JOINT_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_I_PHYSICS_JOINT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Math {
        class Vector3;
    }   // namespace Math
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_PhysicsZone;
class I_PhysicsActor;

class PHYSICS_DLL_LINK I_PhysicsJoint
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_PhysicsJoint>            pPhysicsJoint_type;
    typedef Memory::managed_weak_ptr<I_PhysicsJoint>       wpPhysicsJoint_type;
    typedef Event::Event<wpPhysicsJoint_type>              physicsJointEvent_type;
    typedef Memory::managed_ptr<I_PhysicsActor>            pPhysicsActor_type;
    typedef Memory::managed_weak_ptr<I_PhysicsActor>       wpPhysicsActor_type;
    /// @}

    /// @name I_PhysicsJoint interface
    /// @{
public:
    virtual void attachActor(pPhysicsActor_type _shape) = 0;
    virtual void initUpVectorJoint(const Math::Vector3& _upVector) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    physicsJointEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PhysicsJoint();
    virtual ~I_PhysicsJoint();
    /// @}

};  // interface I_PhysicsJoint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
namespace Memory
{
    /// I_PhysicsJoint is managed by a I_PhysicsZone
    template<>
    struct is_managed_by_factory<Zen::Engine::Physics::I_PhysicsJoint> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_I_PHYSICS_JOINT_HPP_INCLUDED
