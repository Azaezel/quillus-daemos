//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
#ifndef ZEN_ENGINE_WORLD_I_SKY_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_SKY_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Scripting.hpp>

#include <map>
#include <string>

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

class I_Sky;

class WORLD_DLL_LINK I_SkyService
:   public Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef std::string                                     index_type;

    typedef Zen::Memory::managed_ptr<I_SkyService>          pScriptObject_type;
    typedef Scripting::ObjectReference<I_SkyService>        ScriptObjectReference_type;
    typedef ScriptObjectReference_type                      ScriptWrapper_type;
    typedef ScriptWrapper_type*                             pScriptWrapper_type;

    typedef Memory::managed_ptr<I_SkyService>               pSkyService_type;
    typedef Memory::managed_weak_ptr<I_SkyService>          wpSkyService_type;
    typedef Event::Event<wpSkyService_type>                 skyServiceEvent_type;
    typedef std::map<std::string, std::string>              config_type;
    typedef Memory::managed_ptr<I_Sky>                      pSky_type;
    typedef Memory::managed_weak_ptr<I_Sky>                 wpSky_type;

    typedef Memory::managed_ptr<Engine::Physics::I_PhysicsService>      pPhysicsService_type;
    typedef Memory::managed_ptr<Engine::Resource::I_ResourceService>    pPhysicsResourceService_type;
    typedef Memory::managed_ptr<Engine::Rendering::I_RenderingService>  pRenderingService_type;
    typedef Memory::managed_ptr<Engine::Resource::I_ResourceService>    pRenderingResourceService_type;

    /// @}

    /// @name I_SkyService interface
    /// @{
public:
    virtual void setPhysicsService(pPhysicsService_type _pService) = 0;
    virtual void setRenderingService(pRenderingService_type _pService) = 0;
    virtual void setPhysicsResourceService(pPhysicsResourceService_type _pService) = 0;
    virtual void setRenderingResourceService(pRenderingResourceService_type _pService) = 0;
    virtual pSky_type createSky(config_type& _config) = 0;

    /// @todo Should this be moved to I_ScriptableType?
    virtual void registerScriptModule(Zen::Scripting::script_module& _module) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "SkyService"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
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
    skyServiceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SkyService();
    virtual ~I_SkyService();
    /// @}

};  // interface I_SkyService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory {
    /// I_SkyService is managed by I_SkyServiceFactory
    template<>
    struct is_managed_by_factory<Engine::World::I_SkyService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_SKY_SERVICE_HPP_INCLUDED
