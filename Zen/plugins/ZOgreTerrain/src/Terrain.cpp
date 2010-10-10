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
//	Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "Terrain.hpp"
#include "TerrainService.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Resource/I_Resource.hpp>

#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Engine/Physics/I_PhysicsZone.hpp>
#include <Zen/Engine/Physics/I_CollisionShape.hpp>

#include <Zen/plugins/ZOgre/I_OgreLight.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>

#include <Zen/plugins/ZOgre/I_OgreSceneService.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgreTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Terrain::Terrain(Terrain::wpTerrainService_type _pService)
:   m_pTerrainService(_pService)
,   m_pScriptObject(NULL)
,   m_pTerrainGroup(NULL)
,	m_pSceneManager(NULL)
{
    typedef Zen::Memory::managed_ptr<Zen::Engine::Rendering::I_SceneService>    pSceneService_type;
    pSceneService_type pSceneService = m_pTerrainService->getSceneService();

    typedef Zen::Memory::managed_ptr<Zen::ZOgre::I_OgreSceneService>    pOgreSceneService_type;
    pOgreSceneService_type pOgreSceneService = pSceneService.as<pOgreSceneService_type>();

    if (pOgreSceneService.isValid())
    {
		m_pSceneManager = &pOgreSceneService->getOgreSceneManager();

        /// TODO Don't hardcode this.
        m_pTerrainGroup = new Ogre::TerrainGroup(
            m_pSceneManager,
            Ogre::Terrain::ALIGN_X_Z,
            513,
            12000.0f
        );

        m_pTerrainGroup->setFilenameConvention("testTerrain","dat");
        m_pTerrainGroup->setOrigin(Ogre::Vector3(1000,0,5000));

		for (long x = 0; x <= 0; ++x)
		{
			for (long y = 0; y <= 0; ++y)
			{
				defineTerrain(x,y);
			}
		}

		m_pTerrainGroup->loadAllTerrains(true);
        m_pTerrainGroup->freeTemporaryResources();

		TerrainService* pService = dynamic_cast<TerrainService*>(_pService.get());

		if (pService != NULL)
		{
			m_pTerrainGlobalOptions = &pService->getTerrainGlobalOptions();
		}
    }
	else
	{
		/// TODO Throw exception?
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Terrain::~Terrain()
{
    if (m_pTerrainGroup != NULL)
    {
        delete m_pTerrainGroup;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Terrain::setPhysicsActor(Terrain::pPhysicsActor_type _pActor)
{
    m_pActor = _pActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Terrain::pPhysicsActor_type
Terrain::getPhysicsActor(void)
{
    return m_pActor;
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
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Terrain::loadPhysicsFromRaw(const std::string& _rawFileName, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Terrain::loadPhysicsFromSerialization(const std::string& _serializationFileName, const Math::Matrix4& _transform)
{
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
            (sm_pModule->getScriptModule(),
            sm_pModule->getScriptModule()->getScriptType("OgreTerrain"),
            getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::script_module* Terrain::sm_pModule = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Terrain::registerScriptModule(Zen::Scripting::script_module& _module, Zen::Scripting::script_module& _baseModule)
{
    sm_pModule = &_module;

    _module.addDerivedType<Terrain>(_baseModule.getScriptType("Terrain"),
            "OgreTerrain", "OGRE Terrain")
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Terrain::defineTerrain(long _x, long _y)
{
	Ogre::String filename = m_pTerrainGroup->generateFilename(_x, _y);
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_pTerrainGroup->getResourceGroup(), filename))
	{
		m_pTerrainGroup->defineTerrain(_x, _y);
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Terrain::updateLight(Engine::Rendering::I_Light& _light)
{
	ZOgre::I_OgreLight* pOgreLight =
		dynamic_cast<ZOgre::I_OgreLight*>(&_light);

	if (pOgreLight != NULL)
	{
		Ogre::Light& light(pOgreLight->getOgreLight());

		m_pTerrainGlobalOptions->setLightMapDirection(light.getDerivedDirection());
		m_pTerrainGlobalOptions->setCompositeMapAmbient(m_pSceneManager->getAmbientLight());
		m_pTerrainGlobalOptions->setCompositeMapDiffuse(light.getDiffuseColour());

		m_pTerrainGroup->updateDerivedData(false, Ogre::Terrain::DERIVED_DATA_LIGHTMAP);
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgreTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
