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
#ifdef USE_HYDRAX

#include "WaterService.hpp"
#include "Water.hpp"
#include "RenderingView.hpp"

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Rendering/I_RenderingService.hpp>
#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_Resource.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <boost/bind.hpp>
#include <iostream>
#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WaterService::WaterService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WaterService::~WaterService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WaterService::setPhysicsService(pPhysicsService_type _pService)
{
    m_pPhysicsService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WaterService::setRenderingService(pRenderingService_type _pService)
{
    m_pRenderingService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WaterService::setPhysicsResourceService(pPhysicsResourceService_type _pService)
{
    m_pPhysicsResourceService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WaterService::setRenderingResourceService(pRenderingResourceService_type _pService)
{
    m_pRenderingResourceService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
WaterService::pWater_type
WaterService::createWater(Zen::Engine::Rendering::I_Canvas& _canvas, config_type& _config)
{
    std::string configPath = _config["configPath"];

    Water* pRawWater = new Water(_canvas, configPath);
    pWater_type pWater = pWater_type(pRawWater, boost::bind(&WaterService::onDestroyWater, this, _1));

    if (!m_pPhysicsService.isValid() || 
        !m_pRenderingService.isValid() ||
        //!m_pPhysicsResourceService.isValid() ||
        !m_pRenderingResourceService.isValid())
    {
        // TODO error - must setup required services prior to creating water objects.
        return pWater;
    }

    wpWater_type wpWater(pWater);
    pRawWater->setSelfReference(wpWater);

    return pWater;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
WaterService::onDestroyWater(wpWater_type _wpWater)
{
    /// Fire the Water's onDestroyEvent
    _wpWater->onDestroyEvent(_wpWater);
    
    /// delete the Water pointer
    Water* pWater = dynamic_cast<Water*>(_wpWater.get());

    if (pWater)
    {
        delete pWater;
    }
    else
    {
        throw Utility::runtime_exception("Zen::ZOgre::WaterService::onDestroyWater() : _wpWater is an invalid Water.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // USE_HYDRAX
