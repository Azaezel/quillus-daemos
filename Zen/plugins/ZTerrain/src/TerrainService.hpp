//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Walt Collins
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
#ifndef ZEN_ZTERRAIN_TERRAIN_SERVICE_HPP_INCLUDED
#define ZEN_ZTERRAIN_TERRAIN_SERVICE_HPP_INCLUDED

#include <Zen/Engine/World/I_TerrainService.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TerrainService
:   public Engine::World::I_TerrainService
,   public Memory::managed_self_ref<Zen::Engine::World::I_TerrainService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule>         pScriptModule_type;
    /// @}

    /// @name I_TerrainService implementation
    /// @{
public:
    virtual void setPhysicsZone(pPhysicsZone_type _pPhysicsZone);
    virtual pPhysicsZone_type getPhysicsZone(void);
    virtual void setPhysicsService(pPhysicsService_type _pService);
    virtual void setPhysicsResourceService(pPhysicsResourceService_type _pService);
    virtual void setRenderingService(pRenderingService_type _pService);
    virtual void setRenderingResourceService(pRenderingResourceService_type _pService);
    virtual pPhysicsService_type getPhysicsService(void);
    virtual pPhysicsResourceService_type getPhysicsResourceService(void);
    virtual pRenderingService_type getRenderingService(void);
    virtual pRenderingResourceService_type getRenderingResourceService(void);
    virtual pTerrain_type createTerrain(void);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name TerrainService implementation
    /// @{
public:
    void onDestroyTerrain(wpTerrain_type _wpTerrain);
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             TerrainService();
    virtual ~TerrainService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pPhysicsZone_type m_pPhysicsZone;
    pPhysicsService_type m_pPhysicsService;
    pPhysicsResourceService_type m_pPhysicsResourceService;
    pRenderingService_type m_pRenderingService;
    pRenderingResourceService_type m_pRenderingResourceService;

    pScriptModule_type          m_pScriptModule;
    ScriptObjectReference_type* m_pScriptObject;
    /// @}

};  // class TerrainService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZTERRAIN_TERRAIN_SERVICE_HPP_INCLUDED
