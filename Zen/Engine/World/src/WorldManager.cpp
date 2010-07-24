//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Walt Collins
// Copyright (C)        2008 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "WorldManager.hpp"

#include <Zen/Engine/World/I_Terrain.hpp>
#include <Zen/Engine/World/I_Sky.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/I_ScriptObject.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorldManager::WorldManager()
:   m_scriptTypesInitialized(false)
,   m_pWorldModule(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorldManager::~WorldManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WorldManager::pZoneService_type
WorldManager::createZoneService(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_worldServiceCache.getLock());

    pZoneService_type pService(m_worldServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_WorldServiceFactory* pFactory = m_worldServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO: Error
        return pService;
    }

    return m_worldServiceCache.cacheService(_type, pFactory->create(_type, _config));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WorldManager::pCloudService_type
WorldManager::createCloudService(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_cloudServiceCache.getLock());

    pCloudService_type pService(m_cloudServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_CloudServiceFactory* pFactory = m_cloudServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO: Error
        return pService;
    }

    return m_cloudServiceCache.cacheService(_type, pFactory->create(_type, _config));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WorldManager::pTerrainService_type
WorldManager::createTerrainService(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_terrainServiceCache.getLock());

    pTerrainService_type pService(m_terrainServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_TerrainServiceFactory* pFactory = m_terrainServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO: Error
        std::cout << "ERROR: createTerrainService() could not create factory" << std::endl;
        return pService;
    }

    return m_terrainServiceCache.cacheService(_type, pFactory->create(_type, _config));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WorldManager::pSkyService_type
WorldManager::createSkyService(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_skyServiceCache.getLock());

    pSkyService_type pService(m_skyServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        pService->registerScriptModule(*m_pWorldModule);

        return pService;
    }

    I_SkyServiceFactory* pFactory = m_skyServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO: Error
        std::cout << "ERROR: createSkyService() could not create factory" << std::endl;
        return pService;
    }

    pService = pFactory->create(_type, _config);
    if (m_pWorldModule)
    {
        pService->registerScriptModule(*m_pWorldModule);
    }

    m_skyServiceCache.cacheService(_type, pService);

    return pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_WorldManager::pWaterService_type
WorldManager::createWaterService(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_waterServiceCache.getLock());

    pWaterService_type pService(m_waterServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_WaterServiceFactory* pFactory = m_waterServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO: Error
        std::cout << "ERROR: createWaterService() could not create factory" << std::endl;
        return pService;
    }

    return m_waterServiceCache.cacheService(_type, pFactory->create(_type, _config));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// @author Tony Richards
/// @brief Register the default script engine
/// 
/// The purpose of this method is to register a script engine that will be used
/// by all of the services created by this manager.  It's also the starting point
/// for registering the script types that the Zen::Engine::World services expose.
void
WorldManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    // Only do this once.
    if (m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    m_pWorldModule = new Zen::Scripting::script_module(_pEngine, "World", "Zen World Module");

    m_pWorldModule->addType<I_TerrainService>("TerrainService", "TerrainService")
        .addMethod("createTerrain", &I_TerrainService::createTerrain)
        .addMethod("setPhysicsZone", &I_TerrainService::setPhysicsZone)
    ;

    m_pWorldModule->addType<I_Terrain>("Terrain", "Terrain")
    ;

    m_pWorldModule->addType<I_SkyService>("SkyService", "SkyService")
        .addMethod("createSky", &I_SkyService::createSky)
    ;

    m_pWorldModule->addType<I_Sky>("Sky", "Sky")
    ;

    m_pWorldModule->activate();

    m_scriptTypesInitialized = true;
    m_pDefaultScriptEngine = _pEngine;

    registerSkyScriptModule();

    // TODO Register the script module with the other services
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorldManager::pScriptModule_type
WorldManager::getDefaultWorldScriptModule()
{
    return m_pWorldModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorldManager::registerSkyScriptModule()
{
    Threading::CriticalSection guard(m_skyServiceCache.getLock());

    for(skyServiceCache_type::iterator iter = m_skyServiceCache.begin(); iter != m_skyServiceCache.end(); iter++)
    {
        iter->second->registerScriptModule(*m_pWorldModule);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
