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
#ifndef ZEN_ENGINE_WORLD_WORLD_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_WORLD_MANAGER_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

#include "../I_WorldManager.hpp"

#include "../I_WorldServiceFactory.hpp"
#include "../I_WorldService.hpp"
#include "../I_CloudServiceFactory.hpp"
#include "../I_CloudService.hpp"
#include "../I_TerrainServiceFactory.hpp"
#include "../I_TerrainService.hpp"
#include "../I_SkyServiceFactory.hpp"
#include "../I_SkyService.hpp"
#include "../I_WaterServiceFactory.hpp"
#include "../I_WaterService.hpp"

#include <Zen/Core/Plugins/ServiceCache.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WorldManager
:   public I_WorldManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Plugins::ServiceCache<I_WorldService, I_WorldServiceFactory>     worldServiceCache_type;
    typedef Zen::Plugins::ServiceCache<I_CloudService, I_CloudServiceFactory>     cloudServiceCache_type;
    typedef Zen::Plugins::ServiceCache<I_TerrainService, I_TerrainServiceFactory> terrainServiceCache_type;
    typedef Zen::Plugins::ServiceCache<I_SkyService, I_SkyServiceFactory>         skyServiceCache_type;
    typedef Zen::Plugins::ServiceCache<I_WaterService, I_WaterServiceFactory>     waterServiceCache_type;

    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>                   pScriptType_type;
    /// @}

    /// @name I_WorldManager implementation
    /// @{
public:
    virtual pZoneService_type createZoneService(const std::string& _type, config_type& _config);
    virtual pCloudService_type createCloudService(const std::string& _type, config_type& _config);
    virtual pTerrainService_type createTerrainService(const std::string& _type, config_type& _config);
    virtual pSkyService_type createSkyService(const std::string& _type, config_type& _config);
    virtual pWaterService_type createWaterService(const std::string& _type, config_type& _config);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultWorldScriptModule();
    /// @}

    /// @name Additional Implementation
    /// @{
private:
    /// Register the script module with all of the previously
    /// created sky services.
    void registerSkyScriptModule();
    /// @}

    /// @name 'Structors
    /// @{
public:
             WorldManager();
    virtual ~WorldManager();
    /// @}

    /// @name Member variables
    /// @{
private:
    worldServiceCache_type    m_worldServiceCache;
    cloudServiceCache_type    m_cloudServiceCache;
    terrainServiceCache_type  m_terrainServiceCache;
    skyServiceCache_type      m_skyServiceCache;
    waterServiceCache_type    m_waterServiceCache;

    pScriptEngine_type              m_pDefaultScriptEngine;

    bool                            m_scriptTypesInitialized;

    Zen::Scripting::script_module*  m_pWorldModule;
    /// @}

};  // class WorldManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_WORLD_MANAGER_HPP_INCLUDED
