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
        return pService;
    }

    I_SkyServiceFactory* pFactory = m_skyServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO: Error
        std::cout << "ERROR: createSkyService() could not create factory" << std::endl;
        return pService;
    }

    return m_skyServiceCache.cacheService(_type, pFactory->create(_type, _config));
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
    registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if(m_pDefaultScriptEngine == NULL)
    {
        Threading::CriticalSection guard(m_terrainServiceCache.getLock());

        for(terrainServiceCache_type::iterator iter = m_terrainServiceCache.begin(); iter != m_terrainServiceCache.end(); iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WorldManager::pScriptModule_type
WorldManager::getDefaultWorldScriptModule()
{
    return m_pZoneModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorldManager::registerScriptEngine(pScriptEngine_type _pEngine, pTerrainService_type _pService)
{
    new Scripting::ObjectReference<I_TerrainService>(m_pZoneModule, m_pTerrainServiceType, _pService, "terrainService");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ObjectReference*
script_createTerrain(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> parms)
{
    Zen::Scripting::ObjectReference<I_TerrainService>* pObject = dynamic_cast<Zen::Scripting::ObjectReference<I_TerrainService>*>(_pObject);

    I_TerrainService::pTerrain_type pTerrain = pObject->getRawObject()->createTerrain();

    Zen::Scripting::ObjectReference<I_Terrain>* pScriptObject = 
        new Zen::Scripting::ObjectReference<I_Terrain>(pObject->getModule(), pObject->getModule()->getScriptType("Terrain"), pTerrain);

    return pScriptObject;
}

#if 0 // deferred
void
script_loadVisualization(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> parms)
{
    Zen::Scripting::ObjectReference<I_TerrainService>* pObject = dynamic_cast<Zen::Scripting::ObjectReference<I_TerrainService>*>(_pObject);

    I_TerrainService::pTerrain_type pTerrain = pObject->getRawObject()->createTerrain();

    Zen::Scripting::ObjectReference<I_Terrain>* pScriptObject = 
        new Zen::Scripting::ObjectReference<I_Terrain>(pObject->getModule(), pObject->getModule()->getScriptType("Terrain"), pTerrain);

    return pScriptObject;
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WorldManager::registerScriptTypes(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || _pEngine == NULL)
        return;

    m_pZoneModule = _pEngine->createScriptModule("World", "Zen Rendering Module");

    m_pTerrainServiceType = m_pZoneModule->createScriptType("TerrainService", "Terrain Service", 0);
    m_pTerrainServiceType->addMethod("createTerrain", "Create a Terrain", script_createTerrain);

#if 0 // deferred
    m_pTerrainType = m_pZoneModule->createScriptType("Terrain", "Terrain", 0);
    m_pTerrainType->addMethod("loadVisualization", "Load the Visual part of a terrain", script_loadVisualization);
    m_pTerrainType->addMethod("loadPhysicsFromRaw", "Load the physics part of a terrain from a RAW file.", script_loadPhysicsFromRaw);
    m_pTerrainType->addMethod("loadPhysicsFromSerialization", "Load Physics from a serialized file", script_loadPhysicsFromSerialization);
#endif 

    m_pZoneModule->activate();

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
