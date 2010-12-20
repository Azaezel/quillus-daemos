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
#include "../I_PhysicsZone.hpp"
#include "../I_PhysicsService.hpp"
#include "../I_CollisionShape.hpp"

#include <Zen/Engine/World/I_TerrainChunk.hpp>

#include <Zen/Core/Math/Point3.hpp>

#include <Zen/Core/Scripting.hpp>
//#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
//#include <Zen/Core/Scripting/I_ScriptModule.hpp>
//#include <Zen/Core/Scripting/I_ScriptType.hpp>


#include <boost/any.hpp>

#include <vector>
#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

PhysicsManager::PhysicsManager()
:   m_pPhysicsModule(NULL)
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

    pService = pFactory->create(_type, _config);

    if (!pService.isValid())
    {
        throw Utility::runtime_exception("PhysicsManager::create(): Phyics factory create() did not return a valid pointer.");
    }

    if (m_pPhysicsModule != NULL)
    {
        pService->registerScriptModule(*m_pPhysicsModule);
    }

    return m_physicsServiceCache.cacheService(_type, pService);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::onFrame(FrameDelta_type frameDelta)
{
    // Fire the PhysicsManager::onFrameEvent event
    this->onFrameEvent(frameDelta);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsManager::pScriptModule_type
PhysicsManager::getDefaultScriptModule()
{
    return m_pPhysicsModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerScriptModule()
{
    Threading::CriticalSection guard(m_physicsServiceCache.getLock());

    for(PhysicsServiceCache_type::iterator iter = m_physicsServiceCache.begin(); iter != m_physicsServiceCache.end(); iter++)
    {
		iter->second->registerScriptModule(*m_pPhysicsModule);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    // Create a Physics module
    m_pPhysicsModule = new Scripting::script_module(_pEngine, "Physics", "Zen Physics Module");
    //m_pPhysicsModule = _pEngine->createScriptModule("Physics", "Zen Physics Module");

    // Expose I_PhysicsService to the Script Engine
    m_pPhysicsModule->addType<I_PhysicsService>("PhysicsService", "PhysicsService")
        .addMethod("createZone", &I_PhysicsService::createZone)
    ;
    // Expose I_PhysicsZone to the Script Engine
    m_pPhysicsModule->addType<I_PhysicsZone>("PhysicsZone", "Physics Zone")
        .addMethod("setBoundary", &I_PhysicsZone::setBoundary)
        .addMethod("createHeightFieldShape", &I_PhysicsZone::createHeightFieldShape)
	;

    m_pPhysicsModule->addType<I_CollisionShape>("CollisionShape", "Collision Shape")
    ;

    // Expose I_PhysicsActor to the Script Engine
    m_pPhysicsModule->addType<I_PhysicsActor>("PhysicsActor", "Physics Actor")
    		.addMethod("setMass", &I_PhysicsActor::setMass)
    		// TODO add the methods needed by tutorial 4
	;

    // TODO Don't activate?  Give the service an opportunity to add more methods.
    // TODO Eventually support multiple derived script types, which means we
    // never activate this module.  Instead, the meta data is maintained and
    // derived types copy that meta data, add more methods, and then activates the
    // new derived class.
    m_pPhysicsModule->activate();

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = true;

    registerScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

