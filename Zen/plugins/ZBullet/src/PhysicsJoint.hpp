//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2009 Brian Roberts
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
#ifndef ZEN_ZBULLET_PHYSICS_JOINT_HPP_INCLUDED
#define ZEN_ZBULLET_PHYSICS_JOINT_HPP_INCLUDED

// TR - Newton.h should include stdlib.h but it doesn't.
#include <stdlib.h>
#include <btBulletDynamicsCommon.h>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Physics/I_PhysicsJoint.hpp>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBullet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsZone;
class PhysicsActor;
class CollisionShape;

class PhysicsJoint
:   public Engine::Physics::I_PhysicsJoint
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsJoint>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsActor>       pPhysicsActor_type;
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsZone>   wpPhysicsZone_type;
    /// @}

    /// @name I_PhysicsJoint implementation
    /// @{
public:
    virtual void attachActor(pPhysicsActor_type _shape);
    virtual void initUpVectorJoint(const Math::Vector3& _upVector);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsJoint(wpPhysicsZone_type _zone);
    virtual ~PhysicsJoint();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pPhysicsActor_type m_pShape;
    wpPhysicsZone_type m_pZone;
    btGeneric6DofConstraint* m_pConstraint;
    /// @}

};  // class PhysicsJoint

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZBULLET_PHYSICS_JOINT_HPP_INCLUDED
