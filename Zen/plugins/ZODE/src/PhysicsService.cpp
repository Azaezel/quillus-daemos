//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#include "PhysicsZone.hpp"

#include <boost/bind.hpp>
#include <exception>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::PhysicsService()
{
    std::cout << "PhysicsService::PhysicsService()" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::~PhysicsService()
{
    std::cout << "PhysicsService::~PhysicsService()" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsService::pPhysicsZone_type
PhysicsService::createZone()
{
    std::cout << "PhysicsService::createZone()" << std::endl;
    pPhysicsZone_type pZone = pPhysicsZone_type(new PhysicsZone(), boost::bind(&PhysicsService::onDestroyPhysicsZone, this, _1));

    // Make sure getWeak() works correctly.  I've had problems with it in the past.
    std::cout << "PhysicsService::createZone(): Checking weak pointer" << std::endl;
    assert(pZone.getWeak().get());
    std::cout << "PhysicsService::createZone(): Weak pointer is good" << std::endl;

    m_zones.insert(pZone.getWeak());
    std::cout << "PhysicsService::createZone(): Done" << std::endl;

    return pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::stepSimulation(double _elapsedTime)
{
    // TODO Guard
    for(ZoneCollection_type::iterator iter = m_zones.begin(); iter != m_zones.end(); iter++)
    {
        iter->lock()->stepSimulation(_elapsedTime);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsService::onDestroyPhysicsZone(wpPhysicsZone_type _wpPhysicsZone)
{
    // TODO Guard
    m_zones.erase(_wpPhysicsZone);

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
        throw Zen::Utility::runtime_exception("Zen::ZODE::PhysicsService::onDestroyPhysicsZone() : _wpPhysicsZone is an invalid PhysicsZone.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
