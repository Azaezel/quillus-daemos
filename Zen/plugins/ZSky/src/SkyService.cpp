//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
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
#include "SkyService.hpp"
#include "Sky.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Rendering/I_RenderingService.hpp>
#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_Resource.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>

#include <boost/bind.hpp>
#include <iostream>
#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZSky {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SkyService::SkyService()
:   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SkyService::~SkyService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SkyService::setPhysicsService(pPhysicsService_type _pService)
{
    m_pPhysicsService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SkyService::setRenderingService(pRenderingService_type _pService)
{
    m_pRenderingService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SkyService::setPhysicsResourceService(pPhysicsResourceService_type _pService)
{
    m_pPhysicsResourceService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SkyService::setRenderingResourceService(pRenderingResourceService_type _pService)
{
    m_pRenderingResourceService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SkyService::pSky_type
SkyService::createSky(config_type& _config)
{
    Sky* pRawSky = new Sky();
    pSky_type pSky = pSky_type(pRawSky, boost::bind(&SkyService::onDestroySky, this, _1));

    if (//!m_pPhysicsService.isValid() || 
        !m_pRenderingService.isValid() ||
        //!m_pPhysicsResourceService.isValid() ||
        !m_pRenderingResourceService.isValid())
    {
        // TODO error - must setup required services prior to creating sky objects.
        return pSky;
    }

    config_type renderingConfig;
    if (_config["type"] == "skybox")
    {
        renderingConfig["type"] = "skybox";
        renderingConfig["scene"] = "default";
        renderingConfig["resourceName"] = _config["resourceName"];
        renderingConfig["distance"] = _config["distance"];
        pSky->setResource(m_pRenderingResourceService->loadResource(renderingConfig));
    }
    else
    {
        std::cout << "Error: unsupported type in SkyService::createSky()" << std::endl;
    }

    wpSky_type wpSky(pSky);
    pRawSky->setSelfReference(wpSky);

    return pSky;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SkyService::onDestroySky(wpSky_type _wpSky)
{
    /// Fire the Sky's onDestroyEvent
    _wpSky->onDestroyEvent(_wpSky);
    
    /// delete the Sky pointer
    Sky* pSky = dynamic_cast<Sky*>(_wpSky.get());

    if (pSky)
    {
        delete pSky;
    }
    else
    {
        throw Utility::runtime_exception("Zen::ZSky::SkyService::onDestroySky() : _wpSky is an invalid Sky.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::script_module* sm_pScriptModule = NULL;
static std::string sm_scriptTypeName("SkyService");
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SkyService::registerScriptModule(Zen::Scripting::script_module& _module)
{
    sm_pScriptModule = &_module;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
SkyService::getScriptTypeName()
{
    return sm_scriptTypeName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
SkyService::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptWrapper_type(sm_pScriptModule->getScriptModule(),
            sm_pScriptModule->getScriptModule()->getScriptType(getScriptTypeName()),
            this->getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZSky
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
