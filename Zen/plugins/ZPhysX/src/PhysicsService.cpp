//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#include "PhysicsService.hpp"
#include "PhysicsWorld.hpp"

#include <boost/bind.hpp>
#include <exception>
#include <iostream>
//refnote: http://motorola.usc.edu/trac/gge/browser/ogreaddons/PhysX/Samples/SampleMultipleScenes/src/NxMultipleScenes.cpp?rev=2601
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPhysX {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
template<> PhysicsService* Singleton<PhysicsService>::ms_Singleton = 0;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService* PhysicsService::getSingletonPtr(void)
{
    return ms_Singleton;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService& PhysicsService::getSingleton(void)
{  
    assert (ms_Singleton);
	return (*ms_Singleton);  
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NxPhysicsSDK*
PhysicsService::getPhysicsSDKPtr(void)
{
    return m_pPhysicsSDK;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::PhysicsService()
{
    NxPhysicsSDKDesc desc;
    NxSDKCreateError errorCode = NXCE_NO_ERROR;
    m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL, desc, &errorCode);
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
        NxScene* pZone = (NxScene*)iter->get()->getZonePtr();

        //gotta figure out equivalents for 
        /*
        if (m_fFixedTimeStep > 0.0) {
            pZone->setTiming(m_fFixedTimeStep, set_substeps, NX_TIMESTEP_FIXED);
            } else {
            pZone->setTiming(_elapsedTime, set_substeps, NX_TIMESTEP_FIXED);
            }
        */
        //that aren't reliant on extending basic needs for functionaing *at all* into the aplication...
        pZone->simulate(_elapsedTime);
        pZone->flushStream();
        pZone->fetchResults(NX_RIGID_BODY_FINISHED, true);
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
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsService::onDestroyPhysicsZone() : _wpPhysicsZone is an invalid PhysicsZone.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::onFrame()
{
    // TODO - do we need this function?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPhysX
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
