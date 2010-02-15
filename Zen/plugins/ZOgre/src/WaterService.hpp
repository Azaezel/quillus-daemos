//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
#ifndef ZEN_ZOGRE_WATER_SERVICE_HPP_INCLUDED
#define ZEN_ZOGRE_WATER_SERVICE_HPP_INCLUDED

#ifdef USE_HYDRAX

#include <Zen/Engine/World/I_WaterService.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WaterService
:   public Engine::World::I_WaterService
,   public Memory::managed_self_ref<Zen::Engine::World::I_WaterService>
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_WaterService implementation
    /// @{
public:
    virtual void setPhysicsService(pPhysicsService_type _pService);
    virtual void setPhysicsResourceService(pPhysicsResourceService_type _pService);
    virtual void setRenderingService(pRenderingService_type _pService);
    virtual void setRenderingResourceService(pRenderingResourceService_type _pService);
    virtual pWater_type createWater(Zen::Engine::Rendering::I_Canvas& _canvas, config_type& _config);
    /// @}

    /// @name WaterService implementation
    /// @{
public:
    void onDestroyWater(wpWater_type _wpWater);
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             WaterService();
    virtual ~WaterService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pPhysicsService_type m_pPhysicsService;
    pPhysicsResourceService_type m_pPhysicsResourceService;
    pRenderingService_type m_pRenderingService;
    pRenderingResourceService_type m_pRenderingResourceService;
    /// @}

};  // class WaterService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // USE_HYRDAX

#endif // ZEN_ZOGRE_WATER_SERVICE_HPP_INCLUDED
