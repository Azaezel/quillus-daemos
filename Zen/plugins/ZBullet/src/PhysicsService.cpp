//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "PhysicsService.hpp"
#include "PhysicsZone.hpp"

#include <boost/bind.hpp>
#include <exception>
#include <iostream>
#include <btBulletDynamicsCommon.h>

extern ContactAddedCallback		gContactAddedCallback;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBullet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// See CustomMaterialCombinerCallback in Bullet/Demos/ConcaveDemo/ConcavePhysicsDemo.cpp for an example implementation.
///User can override this material combiner by implementing gContactAddedCallback and setting body0->m_collisionFlags |= btCollisionObject::customMaterialCallback;
inline btScalar	calculateCombinedFriction(float friction0,float friction1)
{
	btScalar friction = friction0 * friction1;

	const btScalar MAX_FRICTION  = 10.f;
	if (friction < -MAX_FRICTION)
		friction = -MAX_FRICTION;
	if (friction > MAX_FRICTION)
		friction = MAX_FRICTION;
	return friction;

}

inline btScalar	calculateCombinedRestitution(float restitution0,float restitution1)
{
	return restitution0 * restitution1;
}

static bool CustomMaterialCombinerCallback(btManifoldPoint& cp,	const btCollisionObject* colObj0,int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1)
{
//point of contaact, object, face, child shape, face2, child shape2. see also ConvexDecompositionDemo MyContactCallback that ships with the bullet library

	float friction0 = colObj0->getFriction();
	float friction1 = colObj1->getFriction();
	float restitution0 = colObj0->getRestitution();
	float restitution1 = colObj1->getRestitution();

	if (colObj0->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		friction0 = 1.0;//partId0,index0
		restitution0 = 0.f;
	}
	if (colObj1->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		if (index1&1)
		{
			friction1 = 1.0f;//partId1,index1
		} else
		{
			friction1 = 0.f;
		}
		restitution1 = 0.f;
	}

	cp.m_combinedFriction = calculateCombinedFriction(friction0,friction1);
	cp.m_combinedRestitution = calculateCombinedRestitution(restitution0,restitution1);

	//this return value is currently ignored, but to be on the safe side: return false if you don't calculate friction
	return true;
}

PhysicsService::PhysicsService()
{
    //gContactAddedCallback usage can be found under btManifoldResult::addContactPoint
    //it is an explicitly defined global method handle called during btManifoldResult::addContactPoint
    gContactAddedCallback = CustomMaterialCombinerCallback;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::~PhysicsService()
{
    m_zoneSet.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::pPhysicsZone_type
PhysicsService::createZone(const Math::Vector3& _min, const Math::Vector3& _max)
{
    // TODO evaluate whether or not we should have an unregister function for removing worlds from the list.

    PhysicsZone* pRawZone = new PhysicsZone(_min,_max);
    pPhysicsZone_type pZone = pPhysicsZone_type(pRawZone, boost::bind(&PhysicsService::onDestroyPhysicsZone, this, _1));
    m_zoneSet.insert(pZone);

    wpPhysicsZone_type wpZone(pZone);
    pRawZone->setSelfReference(wpZone);

    return pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::stepSimulation(double _elapsedTime)
{
    // TODO - iterate across the world list and allow them to update themselves.
    for (std::set<pPhysicsZone_type>::iterator iter = m_zoneSet.begin(); iter != m_zoneSet.end(); iter++)
    {
        dynamic_cast<PhysicsZone*>(iter->get())->getZonePtr()->stepSimulation((Zen::Math::Real)_elapsedTime);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::onDestroyPhysicsZone(wpPhysicsZone_type _wpPhysicsZone)
{
    // TODO - remove appropriate entry from m_zoneSet
    //m_zoneSet.erase(iter);

    /// Fire the PhysicsZone's onDestroyEvent
    _wpPhysicsZone->onDestroyEvent(_wpPhysicsZone);
    
    /// delete the PhysicsZone pointer
    PhysicsZone* pPhysicsZone = dynamic_cast<PhysicsZone*>(_wpPhysicsZone.get());

    if (pPhysicsZone)
    {
        delete pPhysicsZone;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsService::onDestroyPhysicsZone() : _wpPhysicsZone is an invalid PhysicsZone.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::onFrame()
{
    // TODO - do we need this function?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
