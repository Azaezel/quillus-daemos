//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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

#include "Terrain.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_Resource.hpp>

#include <Zen/Engine/Physics/I_PhysicsShape.hpp>
#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Terrain::Terrain(Terrain::wpTerrainService_type _pService)
:   m_pTerrainService(_pService)
,   m_pScriptModule(Engine::World::I_WorldManager::getSingleton().getDefaultWorldScriptModule())
,   m_pScriptObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Terrain::~Terrain()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Terrain::setCollisionShape(Terrain::pCollisionShape_type _shape)
{
    m_pCollisionShape = _shape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Terrain::pCollisionShape_type
Terrain::getCollisionShape(void)
{
    return m_pCollisionShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Terrain::setResource(Terrain::pResource_type _resource)
{
    m_pResource = _resource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Terrain::pResource_type
Terrain::getResource(void)
{
    return m_pResource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Terrain::loadVisualization(const std::string& _ogreSpecificConfigFileName, const Math::Matrix4& _transform)
{
    // TODO - other scene managers may not accept heightFields for terrains
    Zen::Engine::Resource::I_ResourceService::config_type renderingConfig;
    renderingConfig["type"] = "terrain";
    renderingConfig["fileName"] = _ogreSpecificConfigFileName;
    setResource(m_pTerrainService->getRenderingResourceService()->loadResource(renderingConfig));

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Terrain::loadPhysicsFromRaw(const std::string& _rawFileName, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
    Zen::Engine::World::I_Terrain::pCollisionShape_type pShape = m_pTerrainService->getPhysicsZone()->createShape();
    if (!pShape->initHeightFieldShapeFromRaw(_rawFileName, _size, _maxHeight, _scaleXY, _transform, _bSerialize))
    {
        std::cout << "Error: could not setup heightfield shape in Terrain::loadPhysicsFromRaw()." << std::endl;
    }

    setCollisionShape(pShape);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Terrain::loadPhysicsFromSerialization(const std::string& _serializationFileName, const Math::Matrix4& _transform)
{
    Zen::Engine::World::I_Terrain::pCollisionShape_type pShape = m_pTerrainService->getPhysicsZone()->createShape();
    if (!pShape->initHeightFieldShapeFromSerialization(_serializationFileName, _transform))
    {
        std::cout << "Error: could not setup heightfield shape in Terrain::loadPhysicsFromSerialization()." << std::endl;
        return false;
    }

    setCollisionShape(pShape);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
Terrain::getScriptObject()
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
