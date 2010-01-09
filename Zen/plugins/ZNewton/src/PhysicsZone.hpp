//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C)        2008 Walt Collins
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
#ifndef ZEN_ZNEWTON_PHYSICS_ZONE_HPP_INCLUDED
#define ZEN_ZNEWTON_PHYSICS_ZONE_HPP_INCLUDED

// TR - Newton.h should include stdlib.h but it doesn't.
#include <stdlib.h>
#include <Newton.h>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>
#include <vector>

#include "CollisionShape.hpp"
#include "PhysicsActor.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZNewton {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PhysicsService;

class PhysicsZone
:   public Engine::Physics::I_PhysicsZone
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsZone>
{
    /// @name Types
    /// @{
public:
    typedef unsigned short U16;
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsZone>       wpPhysicsZone_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule>                 pScriptModule_type;

    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsActor>           pPhysicsActor_type;
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsActor>      wpPhysicsActor_type;

    typedef Memory::managed_ptr<Zen::Engine::Physics::I_CollisionShape>         pCollisionShape_type;
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_CollisionShape>    wpCollisionShape_type;

    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsMaterial>        pPhysicsMaterial_type;
    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsJoint>           pPhysicsJoint_type;
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsMaterial>   wpPhysicsMaterial_type;
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsJoint>      wpPhysicsJoint_type;

    typedef std::vector<pPhysicsMaterial_type>                                  materials_vector_type;
    /// @}

    /// @name I_PhysicsZone implementation
    /// @{
public:
    virtual int getDefaultMaterialID();
    virtual void setZoneSize(const Math::Vector3& _min, const Math::Vector3& _max);
    virtual void setGravity(const Math::Vector3& _grav);
    virtual pPhysicsActor_type createActor();
    virtual pPhysicsMaterial_type createMaterial(bool _default);
    virtual pPhysicsJoint_type createJoint();
    virtual materials_vector_type getAllMaterials();
    /// @see http://www.indiezen.org/wiki/irclogs/2009/08/12/#UTC2009-08-12T17:28:49
    virtual void rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor);

    virtual pCollisionShape_type createNullShape(void);
    virtual pCollisionShape_type createBoxShape(Zen::Math::Real _dx = 1.0f, Zen::Math::Real _dy = 1.0f, Zen::Math::Real _dz = 1.0f);
    virtual pCollisionShape_type createOvoidShape(Zen::Math::Real _radiusX = 1.0f, Zen::Math::Real _radiusY = 1.0f, Zen::Math::Real _radiusZ = 1.0f);
    virtual pCollisionShape_type createCapsuleShape(Zen::Math::Real _radius = 1.0f, Zen::Math::Real _height = 1.0f);

    //TODO - Break These Out into a proper resource - bjr
    virtual pCollisionShape_type createConvexHullShape(std::string _filename);
    virtual pCollisionShape_type createTreeCollisionShape(std::string _filename);
    virtual pCollisionShape_type createHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize);
    virtual pCollisionShape_type createHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform);

    void destroyCollisionShape(wpCollisionShape_type _wpCollisionShape);

    void stepSimulation(double _elapsedTime);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name PhysicsZone implementation
    /// @{
public:
    void onDestroyActor(wpPhysicsActor_type _wpPhysicsActor);
    void onDestroyPhysicsJoint(wpPhysicsJoint_type _wpPhysicsJoint);
    void onDestroyPhysicsMaterial(wpPhysicsMaterial_type _wpPhysicsMaterial);
    /// @}

    /// @ For Internal Newton usage
    /// @{
    public:
        virtual NewtonWorld* getZonePtr();
        virtual void setZonePtr(NewtonWorld* _pZone);
    /// @}


    /// @name Inner structs
    /// @{
private:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct RayCastResult
    {
        I_CollisionVisitor& m_visitor;
        Math::Real          m_distance;

        RayCastResult(Math::Real _distance, I_CollisionVisitor& _visitor)
        :   m_distance(_distance)
        ,   m_visitor(_visitor)
        {
        }
    };
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name Static methods
    /// @{
public:
private:
    static dFloat rayCastFilter(const NewtonBody* _pBody, const dFloat* _pNormal, int _collisionId, void* _pUserData, dFloat _intersectDistance);
    static unsigned int rayCastPrefilter(const NewtonBody* _pBody, const NewtonCollision* _pCollision, void* _pUserData);
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
    NewtonWorld*                        m_pZone;
    Math::Vector3                       m_vGravity;
    bool                                m_bDefaultMaterialCreated;
    materials_vector_type               m_materials;

    pScriptModule_type                  m_pScriptModule;
    ScriptObjectReference_type*         m_pScriptObject;
    /// @}

};  // class PhysicsZone

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZNEWTON_PHYSICS_ZONE_HPP_INCLUDED
