//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
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
#include "PhysicsMaterial.hpp"
#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZNewton {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsMaterial::PhysicsMaterial(wpPhysicsZone_type _zone, bool _default)
:   m_pZone(_zone)
{
    m_pNewtonZone = (NewtonWorld*)m_pZone->getZonePtr();
    m_defaultGroupID = NewtonMaterialGetDefaultGroupID(m_pNewtonZone);

    if (_default)
    {
        m_id = m_defaultGroupID;
    }
    else
    {
        m_id = NewtonMaterialCreateGroupID(m_pNewtonZone);
    }

    setCollidable(true);
    setAdvancedCollisionPrediction(false);
    setRestitution(0.25f);
    m_staticFriction = 1.0f;
    setDynamicFriction(0.7f);
	setCallbacks();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsMaterial::~PhysicsMaterial()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
PhysicsMaterial::getMaterialID()
{
    return m_id;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
PhysicsMaterial::getDefaultMaterialID()
{
    return m_defaultGroupID;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real 
PhysicsMaterial::getDynamicFriction()
{
    return m_dynamicFriction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real 
PhysicsMaterial::getStaticFriction()
{
    return m_staticFriction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real 
PhysicsMaterial::getRestitution()
{
    return m_restitution;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
PhysicsMaterial::getAdvancedCollisionPrediction()
{
    return m_bAdvancedCollisionPrediction;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
PhysicsMaterial::getCollidable()
{
    return m_bCollidable;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setDynamicFriction(Math::Real _dynamicFriction)
{
    // Newton friction coefficients are normally between 0 and 1
    m_dynamicFriction = _dynamicFriction;
    if (m_dynamicFriction < 0.01f)
        m_dynamicFriction = 0.01f;
    if (m_staticFriction < m_dynamicFriction + 0.01f)
        m_staticFriction = m_dynamicFriction + 0.01f;

    materials_vector_type materials = m_pZone->getAllMaterials();
    materials_vector_type::const_iterator iter = materials.begin();
    while (iter != materials.end())
    {
        pPhysicsMaterial_type pMaterial = *iter;

        NewtonMaterialSetDefaultFriction(m_pNewtonZone, pMaterial->getMaterialID(), m_id,
            (dFloat)((m_staticFriction + pMaterial->getStaticFriction()) / 2.0f),
            (dFloat)((m_dynamicFriction + pMaterial->getDynamicFriction()) / 2.0f));

        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setMatMatrixDynamicFriction(pPhysicsMaterial_type _material, Math::Real _dynamicFriction)
{
        NewtonMaterialSetDefaultFriction(m_pNewtonZone, getMaterialID(), m_id, getStaticFriction(), _dynamicFriction);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setStaticFriction(Math::Real _staticFriction)
{
    // Newton friction coefficients are normally between 0 and 1
    m_staticFriction = _staticFriction;
    if (m_staticFriction < 0.02f)
        m_staticFriction = 0.02f;
    if (m_dynamicFriction > m_staticFriction - 0.01f)
        m_dynamicFriction = m_staticFriction - 0.01f;

    materials_vector_type materials = m_pZone->getAllMaterials();
    materials_vector_type::const_iterator iter = materials.begin();
    while (iter != materials.end())
    {
        pPhysicsMaterial_type pMaterial = *iter;

        NewtonMaterialSetDefaultFriction(m_pNewtonZone, pMaterial->getMaterialID(), m_id,
            (dFloat)((m_staticFriction + pMaterial->getStaticFriction()) / 2.0f),
            (dFloat)((m_dynamicFriction + pMaterial->getDynamicFriction()) / 2.0f));

        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setMatMatrixStaticFriction(pPhysicsMaterial_type _material, Math::Real _staticFriction)
{
    NewtonMaterialSetDefaultFriction(m_pNewtonZone, getMaterialID(), m_id, _staticFriction, getDynamicFriction());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setRestitution(Math::Real _restitution)
{
    m_restitution = _restitution;

    materials_vector_type materials = m_pZone->getAllMaterials();
    materials_vector_type::const_iterator iter = materials.begin();
    while (iter != materials.end())
    {
        pPhysicsMaterial_type pMaterial = *iter;

        NewtonMaterialSetDefaultElasticity(m_pNewtonZone, pMaterial->getMaterialID(), m_id,
            (dFloat)((m_restitution + pMaterial->getRestitution()) / 2.0f));

        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setMatMatrixRestitution(pPhysicsMaterial_type _material, Math::Real _restitution)
{
        NewtonMaterialSetDefaultElasticity(m_pNewtonZone, getMaterialID(), m_id, _restitution);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setAdvancedCollisionPrediction(bool _mode)
{
    m_bAdvancedCollisionPrediction = _mode;

    materials_vector_type materials = m_pZone->getAllMaterials();
    materials_vector_type::const_iterator iter = materials.begin();
    while (iter != materials.end())
    {
        pPhysicsMaterial_type pMaterial = *iter;

        // advanced collision detection is infectious.  if either material has it, it's enabled.
        if (m_bAdvancedCollisionPrediction || pMaterial->getAdvancedCollisionPrediction())
        {
            NewtonMaterialSetContinuousCollisionMode(m_pNewtonZone, pMaterial->getMaterialID(), m_id, 1);
        }
        else
        {
            NewtonMaterialSetContinuousCollisionMode(m_pNewtonZone, pMaterial->getMaterialID(), m_id, 0);
        }

        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setCollidable(bool _collide)
{
    m_bCollidable = _collide;

    materials_vector_type materials = m_pZone->getAllMaterials();
    materials_vector_type::const_iterator iter = materials.begin();
    while (iter != materials.end())
    {
        pPhysicsMaterial_type pMaterial = *iter;

        // collision detection is infectious.  if either material has it turned off, it's disabled.
        if (!m_bCollidable || !pMaterial->getCollidable())
        {
            NewtonMaterialSetDefaultCollidable(m_pNewtonZone, pMaterial->getMaterialID(), m_id, 0);
        }
        else
        {
            NewtonMaterialSetDefaultCollidable(m_pNewtonZone, pMaterial->getMaterialID(), m_id, 1);
        }

        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setMatMatrixCollidable(pPhysicsMaterial_type _material, bool _collide)
{
    m_bCollidable = _collide;//likely redundant...?
	NewtonMaterialSetDefaultCollidable(m_pNewtonZone,_material->getMaterialID(), m_id, _collide);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsMaterial::setIgnoreCollision(pPhysicsMaterial_type _material)
{
    NewtonMaterialSetDefaultCollidable(m_pNewtonZone, _material->getMaterialID(), m_id, 0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static int GenericContactBegin(const NewtonMaterial* material, const NewtonBody* body0, const NewtonBody* body1);
static int  GenericContactProcess(const NewtonMaterial* material, const NewtonContact* contact);
static void GenericContactEnd(const NewtonMaterial* material);

static CollisionShape*          g_currentObjectRegister;
static CollisionShape*          g_hitObjectRegister;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsMaterial::setCallbacks()
{
    materials_vector_type materials = m_pZone->getAllMaterials();
    materials_vector_type::const_iterator iter = materials.begin();
    while (iter != materials.end())
    {
        pPhysicsMaterial_type pMaterial = *iter;

        NewtonMaterialSetCollisionCallback(m_pNewtonZone, pMaterial->getMaterialID(), m_id, NULL, GenericContactBegin, GenericContactProcess, GenericContactEnd);
        NewtonMaterialSetCollisionCallback(m_pNewtonZone, m_id, pMaterial->getMaterialID(), NULL, GenericContactBegin, GenericContactProcess, GenericContactEnd);
        iter++;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// this callback is called when the two aabb boxes of the colliding objects overlap
int
GenericContactBegin(const NewtonMaterial* _material, const NewtonBody* _body0, const NewtonBody* _body1)
{
	//we store this pair in registers to ensure they don't get lost, since I've been unable to determine
	//an inherent collision list associated with bodies. Research further later.
	g_currentObjectRegister = static_cast<CollisionShape*>(NewtonBodyGetUserData(_body0));
	g_hitObjectRegister = static_cast<CollisionShape*>(NewtonBodyGetUserData(_body1));

	//std::cout << g_currentObjectRegister->getName() << " hits the AABB of " << g_hitObjectRegister->getName() << "\n";

	CollisionShape::BeginCollisionEventData eventData(_material, *g_currentObjectRegister,*g_hitObjectRegister);
    g_currentObjectRegister->onBoundBoxCollisionEvent(eventData);

	// return one to tell Newton the application wants to proccess this contact
    return eventData.getIgnoreCollision() ? 0 : 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// this callback is called for every contact between the two bodies
int
GenericContactProcess(const NewtonMaterial* _material, const NewtonContact* _contact)
{

/*
	//Was going to use this to differentiate internally wether this was a collision, or a contact, 
	// but we'll leave that up to the ap devs
	Math::Vector3 strikerVel, struckVel;
	striker->getVelocity(strikerVel);
	struck->getVelocity(struckVel);
	Zen::Math::Real strikelen = (strikerVel - struckVel).magnitude();
*/
	//std::cout << g_currentObjectRegister->getName() << " hits " << g_hitObjectRegister->getName() << "\n";
	CollisionShape::DuringCollisionEventData eventData(_material, _contact, *g_currentObjectRegister,*g_hitObjectRegister);
    g_currentObjectRegister->onCollisionEvent(eventData);

	//std::cout << "GenericContactProcess";

	// return one to tell Newton we want to accept this contact
	return 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// this function is called after all contacts for the pairs are proccesed
void
GenericContactEnd(const NewtonMaterial* _material)
{
	//std::cout << g_currentObjectRegister->getName() << " resolves colliding with " << g_hitObjectRegister->getName() << "\n";
	CollisionShape::EndCollisionEventData eventData(_material, *g_currentObjectRegister, *g_hitObjectRegister);
    g_currentObjectRegister->onCollisionResolutionEvent(eventData);

	g_currentObjectRegister = NULL;
	g_hitObjectRegister = NULL;
	// implement any other effects
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
