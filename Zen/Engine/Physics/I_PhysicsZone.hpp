//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_PHYSICS_I_PHYSICS_ZONE_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_I_PHYSICS_ZONE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Ray.hpp>
#include <Zen/Core/Math/Matrix4.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <boost/noncopyable.hpp>

#include <string>
#include <map>
#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_CollisionShape;
class I_PhysicsActor;
class I_PhysicsMaterial;
class I_PhysicsJoint;

/// Physics Zone.
/// Physics simulations are separated by zones.  Each zone is it's own physics
/// simulation.  Physics actors (I_PhysicsActor) exists in only one zone, and
/// actors can only interact with other actors in the same zone.
class PHYSICS_DLL_LINK I_PhysicsZone
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef std::map<std::string, std::string>              config_type;

    typedef Memory::managed_ptr<I_PhysicsZone>             pScriptObject_type;
    typedef Scripting::ObjectReference<I_PhysicsZone>      ScriptObjectReference_type;

    typedef Memory::managed_ptr<I_PhysicsZone>              pPhysicsZone_type;
    typedef Memory::managed_weak_ptr<I_PhysicsZone>         wpPhysicsZone_type;
    typedef Event::Event<wpPhysicsZone_type>                PhysicsZoneEvent_type;

    typedef Memory::managed_ptr<I_PhysicsActor>             pPhysicsActor_type;
    typedef Memory::managed_ptr<I_CollisionShape>           pCollisionShape_type;

    typedef Memory::managed_ptr<I_PhysicsMaterial>          pPhysicsMaterial_type;
    typedef Memory::managed_weak_ptr<I_PhysicsMaterial>     wpPhysicsMaterial_type;

    typedef Memory::managed_ptr<I_PhysicsJoint>             pPhysicsJoint_type;


    typedef std::vector<pPhysicsMaterial_type>              materials_vector_type;
    /// @}

    /// @name I_PhysicsZone Inner Classes
    /// @{
public:
    struct I_CollisionVisitor
    {
        /// @return true to continue testing, return false to
        /// indicate that no more collisions should be tested.
        virtual bool visit(pPhysicsActor_type _Shape, Math::Vector3 _normal, Math::Real _distance) = 0;

        /// @return true if this physics actor should be tested.
        virtual bool filter(pPhysicsActor_type _filterType) = 0;
    };
    /// @}

    /// @name I_PhysicsZone interface
    /// @{
public:
    /// Set the physics simulation for this zone.
	virtual void stepSimulation(double _elapsedTime) = 0;

    /// Set the default gravity.
    /// @todo TR - Does this actually work?  If not then possibly should remove it.
    virtual void setGravity(const Math::Vector3& _grav) = 0;

    /// Create a Physics Actor.
    /// @todo Should more than one actor type be supported here?
    virtual pPhysicsActor_type createActor() = 0;

    /// Create a Physics Material
    /// @param _default True if this material should be the default material used
    ///     by the physics engine.
    virtual pPhysicsMaterial_type createMaterial(bool _default = false) = 0;

    /// @todo HACK!  Egads!  Don't do it this way as it breaks encapsulation.  Use a visitor.
    virtual materials_vector_type getAllMaterials() = 0;

    /// @todo Is this used?  It seems rather hackish to me.
    virtual int getDefaultMaterialID() = 0;

    /// Create a Physics Joint
    virtual pPhysicsJoint_type createJoint() = 0;

    /// Perform a ray cast on scene objects.
    virtual void rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor) = 0;

    /// Create a Null collision shape.
    /// A Null collision shape is a shape that does not collide with anything.
    virtual pCollisionShape_type createNullShape(void) = 0;

    /// Create a Box collision shape.
    /// A Box physics shape is a collision shape that is defined by a box.
    virtual pCollisionShape_type createBoxShape(Zen::Math::Real _dx = 1.0f, Zen::Math::Real _dy = 1.0f, Zen::Math::Real _dz = 1.0f) = 0;

    /// Create an Ovoid collision shape.
    /// A Box physics shape is a collision shape that is defined by an ovoid.
    virtual pCollisionShape_type createOvoidShape(Zen::Math::Real _radiusX = 1.0f, Zen::Math::Real _radiusY = 1.0f, Zen::Math::Real _radiusZ = 1.0f) = 0;

    /// Create an Capsule collision shape.
    /// A Capsule physics shape is a collision shape that is defined by a capsule.
    /// What is a capsule?
    virtual pCollisionShape_type createCapsuleShape(Zen::Math::Real _radius = 1.0f, Zen::Math::Real _height = 1.0f) = 0;

    /// @todo I don't like this interface.  If we're loading from a file, we should go through
    ///     a resource manager.
    virtual pCollisionShape_type createHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize) = 0;

    /// @todo I don't like this interface.  If we're loading from a file, we should go through
    ///     a resource manager.
    virtual pCollisionShape_type createHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform) = 0;

    /// @todo I don't like this interface.  If we're loading from a file, we should go through
    ///     a resource manager.
    virtual pCollisionShape_type createConvexHullShape(std::string _filename) = 0;

    /// @todo I don't like this interface.  If we're loading from a file, we should go through
    ///     a resource manager.
    virtual pCollisionShape_type createTreeCollisionShape(std::string _filename) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    PhysicsZoneEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PhysicsZone();
    virtual ~I_PhysicsZone();
    /// @}
};  // interface I_PhysicsZone

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
namespace Memory
{
    /// I_PhysicsZone is managed by a I_PhysicsService
    template<>
    struct is_managed_by_factory<Zen::Engine::Physics::I_PhysicsZone> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_I_PHYSICS_ZONE_HPP_INCLUDED
