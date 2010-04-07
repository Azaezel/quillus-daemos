//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ZODE_PHYSICS_ZONE_HPP_INCLUDED
#define ZEN_ZODE_PHYSICS_ZONE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Engine/Physics/I_PhysicsZone.hpp>

#include <ode/ode.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PhysicsService;

class PhysicsZone
:   public Engine::Physics::I_PhysicsZone
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsZone>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule>                 pScriptModule_type;

    typedef std::map<int, pPhysicsMaterial_type>                                Materials_type;
    /// @}

    /// @name I_PhysicsZone implementation
    /// @{
public:
	virtual void stepSimulation(double _elapsedTime);
    virtual void setBoundary(const Math::Vector3& _min, const Math::Vector3& _max);
    virtual void setGravity(const Math::Vector3& _grav);
    virtual pPhysicsActor_type createActor();
    virtual pPhysicsMaterial_type createMaterial(bool _default = false);
    virtual materials_vector_type getAllMaterials();
    virtual int getDefaultMaterialID();
    virtual pPhysicsJoint_type createJoint();
    virtual void rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor);
    virtual pCollisionShape_type createNullShape(void);
    virtual pCollisionShape_type createBoxShape(Zen::Math::Real _dx = 1.0f, Zen::Math::Real _dy = 1.0f, Zen::Math::Real _dz = 1.0f);
    virtual pCollisionShape_type createOvoidShape(Zen::Math::Real _radiusX = 1.0f, Zen::Math::Real _radiusY = 1.0f, Zen::Math::Real _radiusZ = 1.0f);
    virtual pCollisionShape_type createCapsuleShape(Zen::Math::Real _radius = 1.0f, Zen::Math::Real _height = 1.0f);
    virtual pCollisionShape_type createHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize);
    virtual pCollisionShape_type createHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform);
    virtual pCollisionShape_type createConvexHullShape(std::string _filename);
    virtual pCollisionShape_type createTreeCollisionShape(std::string _filename);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name PhysicsZone implementation
    /// @{
public:
    dSpaceID getSpaceId();
    /// @}

    /// @name Static methods
    /// @{
public:
    static void destroyMaterial(wpPhysicsMaterial_type _pMaterial);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsZone();
    virtual ~PhysicsZone();
    /// @}

    /// @name Member Variables
    /// @{
private:
    dWorldID                            m_worldId;
    dSpaceID                            m_spaceId;

    Materials_type                      m_materials;

    pScriptModule_type                  m_pScriptModule;
    ScriptObjectReference_type*         m_pScriptObject;
    /// @}

};  // class PhysicsZone

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_PHYSICS_ZONE_HPP_INCLUDED
