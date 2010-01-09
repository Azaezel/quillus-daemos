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
#include "PhysicsService.hpp"
#include "PhysicsWorld.hpp"

#include <boost/bind.hpp>
#include <exception>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::PhysicsService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::~PhysicsService()
{
    m_zoneSet.clear();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::pPhysicsZone_type
PhysicsService::createZone(void)
{
    // TODO evaluate whether or not we should have an unregister function for removing worlds from the list.

    PhysicsZone* pRawZone = new PhysicsZone();
    pPhysicsZone_type pZone = pPhysicsZone_type(pRawZone, boost::bind(&PhysicsService::onDestroyPhysicsWorld, this, _1));
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
        //PAL utilizes a g_contacts.clear() before, and a dJointGroupEmpty (g_contactgroup); after the next two lines
        //in other words, we'll need to do some explicit instructions here
        dWorldStep ((dWorldID)iter->get()->getZonePtr(), _elapsedTime);
        dSpaceCollide((dSpaceID)iter->get()->getSpacePtr(),0,&GenericContactProcess);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::onDestroyPhysicsWorld(wpPhysicsZone_type _wpPhysicsZone)
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
        throw Zen::Utility::runtime_exception("Zen::ZODE::PhysicsService::onDestroyPhysicsWorld() : _wpPhysicsZone is an invalid PhysicsZone.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::onFrame()
{
    // TODO - do we need this function?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// this callback is called when processing the object list to see if bjects are colliding
static void
GenericContactProcess(void *data, dGeomID o1, dGeomID o2)
{
//insert variant of fairly long PAL implementation chain here
//see static void nearCallback (void *data, dGeomID o1, dGeomID o2) for sorting, using spaces,
//and creating contact joints
}
