//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "PhysicsWorld.hpp"
#include "PhysicsJoint.hpp"
#include "PhysicsMaterial.hpp"
#include "PhysicsShape.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>

#include <boost/bind.hpp>
#include <Zen/Core/Math/Vector4.hpp>

//#include <dVector.h> //no likey. recheck

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPhysX {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::PhysicsZone()
:   m_pScriptModule(Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
{
    m_pPhysicsSDK = PhysicsService::getSingletonPtr()->getPhysicsSDKPtr();
    NxSceneDesc sceneDesc;

    sceneDesc.gravity = NxVec3(m_vGravity.m_x,m_vGravity.m_y,m_vGravity.m_z);
    sceneDesc.dynamicStructure = NX_PRUNING_DYNAMIC_AABB_TREE;

	if (/*set_use_hardware &&*/ m_pPhysicsSDK->getHWVersion() != NX_HW_VERSION_NONE) //physX licence agreement requires hardware usage suport for noncomercial use. 
    {
		sceneDesc.simType = NX_SIMULATION_HW;
	}
	else
    {
		sceneDesc.simType = NX_SIMULATION_SW;
	}

    gPhysicsSDK->createScene(sceneDesc);

    m_bDefaultMaterialCreated = false;
    //NewtonSetFrictionModel(m_pZone, 1);
    //NewtonSetSolverModel(m_pZone, 2);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::~PhysicsZone()
{
    m_pPhysicsSDK->releaseScene(m_pZone);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
PhysicsZone::getZonePtr(void)
{
    return m_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
PhysicsZone::getDefaultMaterialID()
{
    return NewtonMaterialGetDefaultGroupID(m_pZone);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setZoneSize(const Math::Vector3& _min, const Math::Vector3& _max)
{
	// set the size of the world
    NewtonSetWorldSize(m_pZone, _min.m_array, _max.m_array); 
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setGravity(const Math::Vector3& _grav)
{
    // note that Newton does not support a global gravity value, instead it must
    // be applied as an additional downward force in any force callback functions.
    // this value is held here simply as a storage place for now.
    // TODO - use this value in a gravity force calculation, or pass it to another object which does.
    m_vGravity = _grav;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type 
PhysicsZone::createShape()
{
    CollisionShape* pRawShape = new CollisionShape(getSelfReference());
    pCollisionShape_type pShape = pCollisionShape_type(pRawShape, boost::bind(&PhysicsZone::onDestroyCollisionShape, this, _1));

    wpCollisionShape_type wpShape(pShape);
    pRawShape->setSelfReference(wpShape);

    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::vector<PhysicsZone::pPhysicsMaterial_type>
PhysicsZone::getAllMaterials()
{
    return m_materials;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsMaterial_type 
PhysicsZone::createMaterial(bool _default)
{
    PhysicsMaterial* pRawMaterial;

    if (m_bDefaultMaterialCreated)
    {
        // only one "default" material allowed
        if (_default)
        {
            // TODO - warning here, default material already created
        }
        pRawMaterial = new PhysicsMaterial(getSelfReference(), false);
    }
    else
    {
        m_bDefaultMaterialCreated = _default;
        pRawMaterial = new PhysicsMaterial(getSelfReference(), _default);
    }

    pPhysicsMaterial_type pMaterial = pPhysicsMaterial_type(pRawMaterial, boost::bind(&PhysicsZone::onDestroyPhysicsMaterial, this, _1));

    wpPhysicsMaterial_type wpMaterial(pMaterial);
    pRawMaterial->setSelfReference(wpMaterial);

    m_materials.push_back(pMaterial);

    return pMaterial;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsJoint_type
PhysicsZone::createJoint()
{
    PhysicsJoint* pRawJoint = new PhysicsJoint(getSelfReference());
    pPhysicsJoint_type pJoint = pPhysicsJoint_type(pRawJoint, boost::bind(&PhysicsZone::onDestroyPhysicsJoint, this, _1));

    wpPhysicsJoint_type wpJoint(pJoint);
    pRawJoint->setSelfReference(wpJoint);

    return pJoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::onDestroyCollisionShape(wpCollisionShape_type _wpCollisionShape)
{
    /// Fire the CollisionShape's onDestroyEvent
    _wpCollisionShape->onDestroyEvent(_wpCollisionShape);
    
    /// delete the CollisionShape pointer
    CollisionShape* pCollisionShape = dynamic_cast<CollisionShape*>(_wpCollisionShape.get());

    if (pCollisionShape)
    {
        delete pCollisionShape;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::onDestroyCollisionShape() : _wpCollisionShape is an invalid CollisionShape.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::onDestroyPhysicsMaterial(wpPhysicsMaterial_type _wpPhysicsMaterial)
{
    /// Fire the PhysicsMaterial's onDestroyEvent
    _wpPhysicsMaterial->onDestroyEvent(_wpPhysicsMaterial);
    
    /// delete the PhysicsMaterial pointer
    PhysicsMaterial* pPhysicsMaterial = dynamic_cast<PhysicsMaterial*>(_wpPhysicsMaterial.get());

    if (pPhysicsMaterial)
    {
        delete pPhysicsMaterial;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::onDestroyPhysicsMaterial() : _wpPhysicsMaterial is an invalid PhysicsMaterial.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::onDestroyPhysicsJoint(wpPhysicsJoint_type _wpPhysicsJoint)
{
    /// Fire the PhysicsJoint's onDestroyEvent
    _wpPhysicsJoint->onDestroyEvent(_wpPhysicsJoint);
    
    /// delete the PhysicsJoint pointer
    PhysicsJoint* pPhysicsJoint = dynamic_cast<PhysicsJoint*>(_wpPhysicsJoint.get());

    if (pPhysicsJoint)
    {
        delete pPhysicsJoint;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::onDestroyPhysicsJoint() : _wpPhysicsJoint is an invalid PhysicsJoint.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
PhysicsZone::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type
            (m_pScriptModule, m_pScriptModule->getScriptType(getScriptTypeName()), getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//see http://www.newtondynamics.com/wiki/index.php5?title=NewtonWorldRayCast
//for reference
void 
PhysicsZone::rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor)
{
    Math::Vector3 offset = _ray.getDirection();
    offset.normalize();
    offset *= _maxDistance;
    Math::Point3 endpoint = _ray.getOrigin() + offset;

    RayCastResult rayCastQuery(_maxDistance, _visitor);
    NewtonWorldRayCast(m_pZone, _ray.getOrigin().m_array, endpoint.m_array, rayCastFilter, &rayCastQuery, rayCastPrefilter);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//implement a ray cast filter
dFloat 
PhysicsZone::rayCastFilter(const NewtonBody* _pBody, const dFloat* _pNormal, int _collisionID, void* _pUserData, dFloat _intersectDistance)
{
    PhysicsZone::RayCastResult* pCastquery = static_cast<PhysicsZone::RayCastResult*>(_pUserData);
    CollisionShape* pRawCollisionShape = static_cast<CollisionShape*>(NewtonBodyGetUserData(_pBody));
    assert(pCastquery != NULL);
    assert(pRawCollisionShape != NULL);

    Math::Vector3 normal(
        static_cast<const Math::Real*>(_pNormal)
    );

    return (pCastquery->m_visitor.visit(
                pRawCollisionShape->getSelfReference().lock(), 
                normal, 
                (Math::Real)_intersectDistance * pCastquery->m_distance
            )) ? 1.0f : 0.0f;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//newton 1.53 Tutorial_06_UtilityFuntionality says:
// implement a ray cast pre-filter
unsigned int
PhysicsZone::rayCastPrefilter(const NewtonBody* _pBody, const NewtonCollision* _pCollision, void* _pUserData)
{
    PhysicsZone::RayCastResult* pCastquery = static_cast<PhysicsZone::RayCastResult*>(_pUserData);
    CollisionShape* pRawCollisionShape = static_cast<CollisionShape*>(NewtonBodyGetUserData(_pBody));
    assert(pCastquery != NULL);
    assert(pRawCollisionShape != NULL);

    Math::Point3 position;
    pRawCollisionShape->getPosition(position);

    return (pCastquery->m_visitor.filter(pRawCollisionShape->getSelfReference().lock())) ? 1 : 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
