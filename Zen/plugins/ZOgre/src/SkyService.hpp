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
#ifndef ZEN_ZOGRE_SKY_SERVICE_HPP_INCLUDED
#define ZEN_ZOGRE_SKY_SERVICE_HPP_INCLUDED

#include <Zen/Engine/World/I_SkyService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SkyService
:   public Zen::Engine::World::I_SkyService
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_SkyService implementation
    /// @{
public:
    virtual void setPhysicsService(pPhysicsService_type _pService);
    virtual void setRenderingService(pRenderingService_type _pService);
    virtual void setPhysicsResourceService(pPhysicsResourceService_type _pService);
    virtual void setRenderingResourceService(pRenderingResourceService_type _pService);
    virtual pSky_type createSky(config_type& _config);
    /// @}

    /// @name SkyService implementation
    /// @{
private:
    void destroy(wpSky_type _wpSky);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class SkyServiceFactory;
             SkyService();
    virtual ~SkyService();
    /// @}

    /// @name Member variables
    /// @{
private:
    pPhysicsService_type            m_pPhysicsService;
    pRenderingService_type          m_pRenderingService;
    pPhysicsResourceService_type    m_pPhysicsResourceService;
    pRenderingResourceService_type  m_pRenderingResourceService;
    /// @}

};  // class SkyService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_SKY_SERVICE_HPP_INCLUDED
