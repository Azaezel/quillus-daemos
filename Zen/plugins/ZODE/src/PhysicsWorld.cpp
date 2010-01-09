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
//refnotes: http://docs.taoframework.com/Tao.Ode/Tao.Ode.Ode.dHashSpaceCreate.html
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::PhysicsZone()
:   m_pScriptModule(Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
{

    m_bDefaultMaterialCreated = false;

	m_pZone = dWorldCreate();
	m_pSpace = dHashSpaceCreate (0); //Create a multi-resolution hash table space. If space is nonzero, insert the new space into that space. 
    m_vGravity = Math::Vector3(0.0f, -9.8f, 0.0f);
    setGravity(m_vGravity);

    //NewtonSetFrictionModel(m_pZone, 1);
    //NewtonSetSolverModel(m_pZone, 2); suspect there's an equivalent I haven't seen yet... further research needed
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::~PhysicsZone()
{
    dWorldDestroy(m_pZone);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
PhysicsZone::getZonePtr(void)
{
    return m_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
PhysicsZone::getSpacePtr(void)
{
    return m_pSpace;
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

    m_vGravity = _grav;
    dWorldSetGravity(m_vGravity);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsShape_type 
PhysicsZone::createShape()
{
    PhysicsShape* pRawShape = new PhysicsShape(getSelfReference());
    pPhysicsShape_type pShape = pPhysicsShape_type(pRawShape, boost::bind(&PhysicsZone::onDestroyPhysicsShape, this, _1));

    wpPhysicsShape_type wpShape(pShape);
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
PhysicsZone::onDestroyPhysicsShape(wpPhysicsShape_type _wpPhysicsShape)
{
    /// Fire the PhysicsShape's onDestroyEvent
    _wpPhysicsShape->onDestroyEvent(_wpPhysicsShape);
    
    /// delete the PhysicsShape pointer
    PhysicsShape* pPhysicsShape = dynamic_cast<PhysicsShape*>(_wpPhysicsShape.get());

    if (pPhysicsShape)
    {
        delete pPhysicsShape;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZODE::PhysicsZone::onDestroyPhysicsShape() : _wpPhysicsShape is an invalid PhysicsShape.");
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
        throw Zen::Utility::runtime_exception("Zen::ZODE::PhysicsZone::onDestroyPhysicsMaterial() : _wpPhysicsMaterial is an invalid PhysicsMaterial.");
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
        throw Zen::Utility::runtime_exception("Zen::ZODE::PhysicsZone::onDestroyPhysicsJoint() : _wpPhysicsJoint is an invalid PhysicsJoint.");
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
    PhysicsShape* pRawPhysicsShape = static_cast<PhysicsShape*>(NewtonBodyGetUserData(_pBody));
    assert(pCastquery != NULL);
    assert(pRawPhysicsShape != NULL);

    Math::Vector3 normal(
        static_cast<const Math::Real*>(_pNormal)
    );

    return (pCastquery->m_visitor.visit(
                pRawPhysicsShape->getSelfReference().lock(), 
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
    PhysicsShape* pRawPhysicsShape = static_cast<PhysicsShape*>(NewtonBodyGetUserData(_pBody));
    assert(pCastquery != NULL);
    assert(pRawPhysicsShape != NULL);

    Math::Point3 position;
    pRawPhysicsShape->getPosition(position);

    return (pCastquery->m_visitor.filter(pRawPhysicsShape->getSelfReference().lock())) ? 1 : 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
