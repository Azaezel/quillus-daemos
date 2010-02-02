//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_WORLD_I_WATER_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_WATER_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Engine/Rendering/I_View.hpp>

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
    }
    namespace Rendering {
        class I_RenderingService;
    }
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Water;

class WORLD_DLL_LINK I_WaterService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                      index_type;
    typedef Memory::managed_ptr<I_WaterService>                pWaterService_type;
    typedef Memory::managed_weak_ptr<I_WaterService>           wpWaterService_type;
    typedef Event::Event<wpWaterService_type>                  waterServiceEvent_type;
    typedef std::map<std::string, std::string>               config_type;
    typedef Memory::managed_ptr<I_Water>                       pWater_type;
    typedef Memory::managed_weak_ptr<I_Water>                  wpWater_type;

    typedef Memory::managed_ptr<Engine::Physics::I_PhysicsService>      pPhysicsService_type;
    typedef Memory::managed_ptr<Engine::Resource::I_ResourceService>    pPhysicsResourceService_type;
    typedef Memory::managed_ptr<Engine::Rendering::I_RenderingService>  pRenderingService_type;
    typedef Memory::managed_ptr<Engine::Resource::I_ResourceService>    pRenderingResourceService_type;
    /// @}

    /// @name I_WATERService interface
    /// @{
public:
    virtual void setPhysicsService(pPhysicsService_type _pService) = 0;
    virtual void setRenderingService(pRenderingService_type _pService) = 0;
    virtual void setPhysicsResourceService(pPhysicsResourceService_type _pService) = 0;
    virtual void setRenderingResourceService(pRenderingResourceService_type _pService) = 0;
    virtual pWater_type createWater(Zen::Engine::Rendering::I_Canvas& _view, config_type& _config) = 0;
    /// @}

    /// @name Event handlers
    /// @{
protected:
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
    waterServiceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WaterService();
    virtual ~I_WaterService();
    /// @}

};  // interface I_WaterService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory {
    /// I_WaterService is managed by I_WaterServiceFactory
    template<>
    struct is_managed_by_factory<Engine::World::I_WaterService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_WATER_SERVICE_HPP_INCLUDED
