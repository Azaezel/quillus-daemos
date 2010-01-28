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
#ifndef ZEN_ENGINE_WORLD_I_TERRAIN_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_TERRAIN_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Resource {
        class I_ResourceService;
    }
    namespace Physics {
        class I_PhysicsService;
        class I_PhysicsZone;
    }
    namespace Rendering {
        class I_RenderingService;
    }
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Terrain;

class WORLD_DLL_LINK I_TerrainService
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef std::string                                                 index_type;

    typedef Zen::Memory::managed_ptr<I_TerrainService>                  pScriptObject_type;
    typedef Scripting::ObjectReference<I_TerrainService>                ScriptObjectReference_type;

    typedef Memory::managed_ptr<I_TerrainService>                       pTerrainService_type;
    typedef Memory::managed_weak_ptr<I_TerrainService>                  wpTerrainService_type;
    typedef Event::Event<wpTerrainService_type>                         terrainServiceEvent_type;
    typedef std::map<std::string, std::string>                          config_type;
    typedef Memory::managed_ptr<Engine::Physics::I_PhysicsZone>        pPhysicsZone_type;
    typedef Memory::managed_ptr<I_Terrain>                              pTerrain_type;
    typedef Memory::managed_weak_ptr<I_Terrain>                         wpTerrain_type;

    typedef Memory::managed_ptr<Engine::Physics::I_PhysicsService>      pPhysicsService_type;
    typedef Memory::managed_ptr<Engine::Resource::I_ResourceService>    pPhysicsResourceService_type;
    typedef Memory::managed_ptr<Engine::Rendering::I_RenderingService>  pRenderingService_type;
    typedef Memory::managed_ptr<Engine::Resource::I_ResourceService>    pRenderingResourceService_type;
    /// @}

    /// @name I_TerrainService interface
    /// @{
public:
    virtual void setPhysicsZone(pPhysicsZone_type _pPhysicsZone) = 0;
    virtual pPhysicsZone_type getPhysicsZone(void) = 0;
    virtual void setPhysicsService(pPhysicsService_type _pService) = 0;
    virtual void setRenderingService(pRenderingService_type _pService) = 0;
    virtual void setPhysicsResourceService(pPhysicsResourceService_type _pService) = 0;
    virtual void setRenderingResourceService(pRenderingResourceService_type _pService) = 0;
    virtual pPhysicsService_type getPhysicsService(void) = 0;
    virtual pPhysicsResourceService_type getPhysicsResourceService(void) = 0;
    virtual pRenderingService_type getRenderingService(void) = 0;
    virtual pRenderingResourceService_type getRenderingResourceService(void) = 0;
    virtual pTerrain_type createTerrain(void) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "TerrainService"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    terrainServiceEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainService();
    virtual ~I_TerrainService();
    /// @}

};  // interface I_TerrainService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory {
    /// I_TerrainService is managed by I_TerrainServiceFactory
    template<>
    struct is_managed_by_factory<Engine::World::I_TerrainService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_TERRAIN_SERVICE_HPP_INCLUDED
