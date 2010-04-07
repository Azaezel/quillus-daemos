//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#include "TerrainService.hpp"
#include "Terrain.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>
#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Engine/Rendering/I_RenderingService.hpp>
#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_Resource.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>

#include <boost/bind.hpp>

#include <iostream>
#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::TerrainService()
:   m_pScriptModule(Engine::World::I_WorldManager::getSingleton().getDefaultWorldScriptModule())
,   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::~TerrainService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::pPhysicsZone_type
TerrainService::getPhysicsZone(void)
{
    return m_pPhysicsZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainService::setPhysicsZone(pPhysicsZone_type _pPhysicsZone)
{
    m_pPhysicsZone = _pPhysicsZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainService::setPhysicsService(pPhysicsService_type _pService)
{
    m_pPhysicsService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainService::setRenderingService(pRenderingService_type _pService)
{
    m_pRenderingService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainService::setPhysicsResourceService(pPhysicsResourceService_type _pService)
{
    m_pPhysicsResourceService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainService::setRenderingResourceService(pRenderingResourceService_type _pService)
{
    m_pRenderingResourceService = _pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::pPhysicsService_type
TerrainService::getPhysicsService(void)
{
    return m_pPhysicsService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::pRenderingService_type
TerrainService::getRenderingService(void)
{
    return m_pRenderingService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::pPhysicsResourceService_type
TerrainService::getPhysicsResourceService(void)
{
    return m_pPhysicsResourceService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::pRenderingResourceService_type
TerrainService::getRenderingResourceService(void)
{
    return m_pRenderingResourceService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainService::pTerrain_type
TerrainService::createTerrain(config_type& _physicsConfig, config_type& _renderingConfig)
{
    Terrain* pRawTerrain = new Terrain(this->getSelfReference());
    pTerrain_type pTerrain = pTerrain_type(pRawTerrain, boost::bind(&TerrainService::onDestroyTerrain, this, _1));

    if (!m_pPhysicsService.isValid() || 
        !m_pRenderingService.isValid() ||
        !m_pPhysicsResourceService.isValid() ||
        !m_pRenderingResourceService.isValid())
    {
        std::cout << "Error: must setup required services prior to creating terrain objects in TerrainService::createTerrain()." << std::endl;
        return pTerrain;
    }

    pRawTerrain->loadVisualization(_renderingConfig["fileName"], Zen::Math::Matrix4(Zen::Math::Matrix4::INIT_IDENTITY));

    wpTerrain_type wpTerrain(pTerrain);
    pRawTerrain->setSelfReference(wpTerrain);

    return pTerrain;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainService::onDestroyTerrain(wpTerrain_type _wpTerrain)
{
    /// Fire the Terrain's onDestroyEvent
    _wpTerrain->onDestroyEvent(_wpTerrain);
    
    /// delete the Terrain pointer
    Terrain* pTerrain = dynamic_cast<Terrain*>(_wpTerrain.get());

    if (pTerrain)
    {
        delete pTerrain;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZTerrain::TerrainService::onDestroyTerrain() : _wpTerrain is an invalid Terrain.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
TerrainService::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type
            (m_pScriptModule, m_pScriptModule->getScriptType(getScriptTypeName()), getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
