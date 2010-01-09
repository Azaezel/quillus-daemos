//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_WORLD_I_WORLD_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_WORLD_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptEngine;
        class I_ScriptModule;
    }   // namespace Scripting
namespace Engine {
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_WorldService;
class I_CloudService;
class I_TerrainService;
class I_SkyService;
class I_WaterService;

class WORLD_DLL_LINK I_WorldManager
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_WorldService>         pZoneService_type;
    typedef Memory::managed_weak_ptr<I_WorldService>    wpZoneService_type;
    typedef Memory::managed_ptr<I_CloudService>         pCloudService_type;
    typedef Memory::managed_weak_ptr<I_CloudService>    wpCloudService_type;
    typedef Memory::managed_ptr<I_TerrainService>       pTerrainService_type;
    typedef Memory::managed_weak_ptr<I_TerrainService>  wpTerrainService_type;
    typedef Memory::managed_ptr<I_SkyService>           pSkyService_type;
    typedef Memory::managed_weak_ptr<I_SkyService>      wpSkyService_type;
    typedef Memory::managed_ptr<I_WaterService>         pWaterService_type;
    typedef Memory::managed_weak_ptr<I_WaterService>    wpWaterService_type;

    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine>                 pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule>                 pScriptModule_type;

    typedef std::map<std::string, std::string>          config_type;
    /// @}

    /// @name I_WorldManger interface
    /// @{
public:
    virtual pZoneService_type createZoneService(const std::string& _type, config_type& _config) = 0;
    virtual pCloudService_type createCloudService(const std::string& _type, config_type& _config) = 0;
    virtual pTerrainService_type createTerrainService(const std::string& _type, config_type& _config) = 0;
    virtual pSkyService_type createSkyService(const std::string& _type, config_type& _config) = 0;
    virtual pWaterService_type createWaterService(const std::string& _type, config_type& _config) = 0;


    /// Register the default script engine for all Zen::Engine::World services
    /// Every Zen::Engine::World service that has been created or is created in the future
    /// will use this script engine.
    /// @param _pEngine NULL to set the default engine to none, but doing so will
    ///             not unregister the script engine to services that have already
    ///             been created.  It will only prevent subsequent services from
    ///             using this script engine.
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine) = 0;

    /// Get the default World script module.
    virtual pScriptModule_type getDefaultWorldScriptModule() = 0;

    /// @}

    /// @name Static Methods
    /// @{
public:
    /// Get the singleton
    static I_WorldManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WorldManager();
    virtual ~I_WorldManager();
    /// @}

};  // interface I_WorldManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_WORLD_MANAGER_HPP_INCLUDED
