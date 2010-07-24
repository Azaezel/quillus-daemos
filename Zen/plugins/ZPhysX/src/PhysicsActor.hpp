//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_ZPHYSX_PHYSICS_ACTOR_HPP_INCLUDED
#define ZEN_ZPHYSX_PHYSICS_ACTOR_HPP_INCLUDED

#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Physics/include/NxPhysics.h>
#include <boost/shared_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPhysX {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsActor
:   public Physics::I_PhysicsActor
{
    /// @name Types
    /// @{
public:
    typedef NxActor*                                    nxActor_ptr_type;
    typedef boost::shared_ptr<NxActorDesc>              nxActorDesc_ptr_type;
    typedef boost::shared_ptr<NxShapeDesc>              nxShapeDesc_ptr_type;
    typedef boost::shared_ptr<NxBodyDesc>               nxBodyDesc_ptr_type;
    typedef NxScene*                                    nxScene_ptr_type;
    /// @}

    /// @name I_PhysicsActor implementation
    /// @{
public:
    virtual void setSceneNode(scene_node_ptr_type _pSceneNode);

#if 0
    virtual void setAngularDamping(const Math::Real _angularDamping);
    virtual void setAngularMomentum(const Math::Vector3& _angularMomentum);
    virtual void setAngularVelocity(const Math::Vector3& _angularVelocity);
    virtual void setLinearDamping(const Math::Real _linearDamping);
    virtual void setLinearMomentum(const Math::Vector3& _linearMomentum);
    virtual void setLinearVelocity(const Math::Vector3& _linearVelocity);
    virtual void setOrientation(const Math::Quaternion4& _orientation);
    virtual void setPosition(const Math::Vector3& _position);
    virtual void setMass(const Math::Real _mass);
    virtual void setMassSpaceInertiaTensor(const Math::Vector3& _inertiaTensor);
    virtual void setMassOrientation(const Math::Quaternion4& _massOrientation);
    virtual void setMassPosition(const Math::Vector3& _massPosition);
    virtual void addForce(const Math::Vector3& _force);
    virtual void addTorque(const Math::Vector3& _torque);
    virtual Math::Real getAngularDamping() const;
    virtual Math::Vector3& getAngularMomentum() const;
    virtual Math::Vector3& getAngularVelocity() const;
    virtual Math::Real getLinearDamping() const;
    virtual Math::Vector3& getLinearMomentum() const;
    virtual Math::Vector3& getLinearVelocity() const;
    virtual Math::Quaternion4& getOrientation() const;
    virtual Math::Vector3& getPosition() const;
    virtual Math::Real getMass() const;
    virtual Math::Vector3& getMassSpaceInertiaTensor() const;
    virtual Math::Quaternion4& getMassOrientation() const;
    virtual Math::Vector3& getMassPosition() const;
#endif
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsActor(nxScene_ptr_type _pScene);
    virtual ~PhysicsActor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    nxActor_ptr_type m_pPhysXActor;
    nxActorDesc_ptr_type m_pPhysXActorDesc;
    nxShapeDesc_ptr_type m_pPhysXShapeDesc;
    nxBodyDesc_ptr_type m_pPhysXBodyDesc;
    scene_node_ptr_type m_pSceneNode;
    /// @}

};  // class PhysicsActor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPhysX
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZPHYSX_PHYSICS_ACTOR_HPP_INCLUDED
