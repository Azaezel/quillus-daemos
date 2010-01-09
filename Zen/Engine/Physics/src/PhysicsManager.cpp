//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "PhysicsManager.hpp"
#include "../I_PhysicsActor.hpp"

#include <Zen/Core/Math/Point3.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <boost/any.hpp>

#include <vector>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

PhysicsManager::PhysicsManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsManager::~PhysicsManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_PhysicsManager::pPhysicsService_type
PhysicsManager::create(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_physicsServiceCache.getLock());

    pPhysicsService_type pService(m_physicsServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        std::cout << "Returning physics service from cache." << std::endl;
        return pService;
    }

    I_PhysicsServiceFactory* pFactory = m_physicsServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        throw Utility::runtime_exception("PhysicsManager::create(): Error finding physics service factory.");
    }

#if 0 // do this to add scripting support
    if (m_pDefaultScriptEngine != NULL)
    {
        registerScriptEngine(m_pDefaultScriptEngine, pService);
    }
#endif

    pPhysicsService_type pPhysicsService = pFactory->create(_type, _config);

    if (!pPhysicsService.isValid())
    {
        throw Utility::runtime_exception("PhysicsManager::create(): Phyics factory create() did not return a valid pointer.");
    }

    return m_physicsServiceCache.cacheService(_type, pPhysicsService);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::onFrame(FrameDelta_type frameDelta)
{
    // Fire the PhysicsManager::onFrameEvent event
    this->onFrameEvent(frameDelta);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    std::cout << "PhysicsManager::registerDefaultScriptEngine()" << std::endl;
    registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if(m_pDefaultScriptEngine == NULL)
    {
        Threading::CriticalSection guard(m_physicsServiceCache.getLock());

        for(PhysicsServiceCache_type::iterator iter = m_physicsServiceCache.begin(); iter != m_physicsServiceCache.end(); iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsManager::pScriptModule_type
PhysicsManager::getDefaultScriptModule()
{
    return m_pPhysicsModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerScriptEngine(pScriptEngine_type _pEngine, pPhysicsService_type _pService)
{
    // TODO - implement me
    //new Scripting::ObjectReference<I_PhysicsService>(m_pPhysicsModule, m_pPhysicsServiceType, _pService, "physicsService");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void
setMass(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(float _mass)
{
    I_PhysicsActor::ScriptObjectReference_type* pObject = dynamic_cast<I_PhysicsActor::ScriptObjectReference_type*>(_pObject);

    Math::Real mass = boost::any_cast<Math::Real>(_parms[0]);

    return pObject->getObject()->setMass(mass);
}

static void
setPosition(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Vector3& _position)
{
    I_PhysicsActor::ScriptObjectReference_type* pObject = dynamic_cast<I_PhysicsActor::ScriptObjectReference_type*>(_pObject);

    // TODO This should be a Point3 and not a Vector3
    Math::Vector3* pPosition = boost::any_cast<Math::Vector3*>(_parms[0]);

    Math::Point3 realPosition(pPosition->m_x, pPosition->m_y, pPosition->m_z);
    pObject->getObject()->setPosition(realPosition);
}

static void
getPosition(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(Math::Vector3& _position)
{
    I_PhysicsActor::ScriptObjectReference_type* pObject = dynamic_cast<I_PhysicsActor::ScriptObjectReference_type*>(_pObject);

    // TR - Why is this a vector?
    Math::Vector3* pPosition = boost::any_cast<Math::Vector3*>(_parms[0]);

    Math::Point3 position = pObject->getObject()->getPosition();

    pPosition->m_x = position.m_x;
    pPosition->m_y = position.m_y;
    pPosition->m_z = position.m_z;
}

static void
setOrientation(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Quaternion4& _orient)
{
    I_PhysicsActor::ScriptObjectReference_type* pObject = dynamic_cast<I_PhysicsActor::ScriptObjectReference_type*>(_pObject);

    Math::Quaternion4* pOrient = boost::any_cast<Math::Quaternion4*>(_parms[0]);

    pObject->getObject()->setOrientation(*pOrient);
}

static void
setLinearVelocity(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Vector3& _velocity)
{
    I_PhysicsActor::ScriptObjectReference_type* pObject = dynamic_cast<I_PhysicsActor::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pVelocity = boost::any_cast<Math::Vector3*>(_parms[0]);

    pObject->getObject()->setLinearVelocity(*pVelocity);
}

static void
getLinearVelocity(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(Math::Vector3& _velocity)
{
    I_PhysicsActor::ScriptObjectReference_type* pObject = dynamic_cast<I_PhysicsActor::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pVelocity = boost::any_cast<Math::Vector3*>(_parms[0]);

    pObject->getObject()->getLinearVelocity(*pVelocity);
}

static void
setAngularVelocity(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Vector3& _omega)
{
    I_PhysicsActor::ScriptObjectReference_type* pObject = dynamic_cast<I_PhysicsActor::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pOmega = boost::any_cast<Math::Vector3*>(_parms[0]);

    pObject->getObject()->setAngularVelocity(*pOmega);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerScriptTypes(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || _pEngine == NULL)
        return;

    // Create a Physics module
    m_pPhysicsModule = _pEngine->createScriptModule("Physics", "Zen Physics Module");

    // Expose I_PhysicsService to the Script Engine
    //m_pPhysicsServiceType = m_pPhysicsModule->createScriptType("PhysicsService", "Physics Service", 0);
    //m_pSceneServiceType->addMethod("createZone", "Create a Physics World", createChildNode);

    // Expose I_PhysicsWord to the Script Engine
    m_pPhysicsZoneType = m_pPhysicsModule->createScriptType("PhysicsZone", "Physics Zone", 0);
    //m_pPhysicsZoneType->addMethod("setZoneSize", "Set the size of the world.", setZoneSize);
    //m_pPhysicsZoneType->addMethod("createShape", "", createShape);

    // Expose I_PhysicsActor to the Script Engine
    m_pPhysicsActorType = m_pPhysicsModule->createScriptType("PhysicsActor", "Physics Actor", 0);
    m_pPhysicsActorType->addMethod("setMass", "", setMass);
    m_pPhysicsActorType->addMethod("setPosition", "", setPosition);
    m_pPhysicsActorType->addMethod("getPosition", "", getPosition);
    m_pPhysicsActorType->addMethod("setOrientation", "", setOrientation);
    m_pPhysicsActorType->addMethod("setLinearVelocity", "", setLinearVelocity);
    m_pPhysicsActorType->addMethod("getLinearVelocity", "", getLinearVelocity);
    m_pPhysicsActorType->addMethod("setAngularVelocity", "", setAngularVelocity);

    m_pPhysicsModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

