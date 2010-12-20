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

#include "TerrainGroup.hpp"
#include "TerrainChunk.hpp"
#include "TerrainService.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/plugins/ZOgre/I_OgreLight.hpp>

#include <Zen/Engine/World/I_WorldManager.hpp>

#include <Zen/plugins/ZOgre/I_OgreSceneService.hpp>

#include <boost/assign/list_of.hpp>
#include <boost/bind.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgreTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroup::TerrainGroup(wpTerrainService_type _wpService, const std::string& _name)
:   m_pTerrainService(_wpService)
,   m_pScriptObject(NULL)
,   m_pTerrainGroup(NULL)
,   m_terrainChunkMap()
,   m_loaded(false)
,   m_name(_name)
{
    typedef Zen::Memory::managed_ptr<Zen::Engine::Rendering::I_SceneService>    pSceneService_type;
    pSceneService_type pSceneService = m_pTerrainService->getSceneService();

    typedef Zen::Memory::managed_ptr<Zen::ZOgre::I_OgreSceneService>    pOgreSceneService_type;
    pOgreSceneService_type pOgreSceneService = pSceneService.as<pOgreSceneService_type>();

    if (pOgreSceneService.isValid())
    {
        Ogre::SceneManager& sceneManager = pOgreSceneService->getOgreSceneManager();

        m_pTerrainGroup = new Ogre::TerrainGroup(&sceneManager);
    }

    m_pTerrainGroup->setFilenameConvention(m_name,"terrainchunk");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroup::~TerrainGroup()
{
    if (m_pTerrainGroup != NULL)
    {
        delete m_pTerrainGroup;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::setName(const std::string& _name)
{
    m_name = _name;
    m_pTerrainGroup->setFilenameConvention(m_name, "terrainchunk");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
TerrainGroup::getName() const
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::visitTerrainChunks(I_TerrainChunkVisitor& _visitor) const
{
    _visitor.begin();

    TerrainChunkMap_type::const_iterator iter = m_terrainChunkMap.begin();
    while (iter != m_terrainChunkMap.end())
    {
        _visitor.visit(*(iter->second));
        ++iter;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::visitTerrainChunks(I_TerrainChunkVisitor& _visitor)
{
    _visitor.begin();

    TerrainChunkMap_type::iterator iter = m_terrainChunkMap.end();
    while (iter != m_terrainChunkMap.end())
    {
        _visitor.visit(*(iter->second));
        ++iter;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroup::pTerrainChunk_type
TerrainGroup::getTerrainChunk(const Engine::World::I_TerrainChunk::TerrainChunkId& _id)
{
    TerrainChunkMap_type::iterator iter = m_terrainChunkMap.find(_id);
    if (iter != m_terrainChunkMap.end())
    {
        return iter->second->getSelfReference().lock();
    }

    return pTerrainChunk_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroup::pTerrainChunk_type
TerrainGroup::createTerrainChunk(const Engine::World::I_TerrainChunk::TerrainChunkId& _id)
{
    TerrainChunkMap_type::iterator iter = m_terrainChunkMap.find(_id);
    if (iter != m_terrainChunkMap.end())
    {
        return iter->second->getSelfReference().lock();
    }

    TerrainChunk::NativeTerrainChunkId* pId = 
        dynamic_cast<TerrainChunk::NativeTerrainChunkId*>(_id.m_pNativeTerrainChunkId);

    if (pId != NULL)
    {
        long x, y;
        m_pTerrainGroup->unpackIndex(pId->getNativeId(), &x, &y);

        m_pTerrainGroup->defineTerrain(x, y);

        TerrainChunk* pRawTerrainChunk = new TerrainChunk(
            *this,
            *m_pTerrainGroup->getTerrain(x, y),
            x,
            y
        );

        pTerrainChunk_type pTerrainChunk(
            pRawTerrainChunk,
            boost::bind(&TerrainGroup::destroyTerrainChunk, this, _1)
        );

        m_terrainChunkMap[_id] = pRawTerrainChunk;

        return pTerrainChunk;
    }

    throw Zen::Utility::runtime_exception("TerrainGroup::createTerrainChunk() : Could not create terrain chunk.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::destroyTerrainChunk(wpTerrainChunk_type _wpTerrainChunk)
{
    TerrainChunk* pTerrainChunk =
        dynamic_cast<TerrainChunk*>(_wpTerrainChunk.get());

    if (pTerrainChunk != NULL)
    {
        // Only delete the TerrainChunk if it isn't stored in m_terrainChunkMap.
        TerrainChunkMap_type::iterator iter = m_terrainChunkMap.find(pTerrainChunk->getId());
        if (iter == m_terrainChunkMap.end())
        {
            delete pTerrainChunk;
        }
    }
    else
    {
        throw Zen::Utility::runtime_exception("TerrainGroup::destroyTerrainChunk() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::removeTerrainChunk(const Engine::World::I_TerrainChunk::TerrainChunkId& _id)
{
    TerrainChunkMap_type::iterator iter = m_terrainChunkMap.find(_id);
    if (iter != m_terrainChunkMap.end())
    {
        m_terrainChunkMap.erase(iter);
        return;
    }

    // TODO Error?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
TerrainGroup::setOrigin(const Zen::Math::Vector3& _origin)
{
    Ogre::Vector3 origin(_origin.m_array);
    m_pTerrainGroup->setOrigin(origin);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3 
TerrainGroup::getOrigin() const
{
    const Ogre::Vector3& origin(m_pTerrainGroup->getOrigin());
    return Zen::Math::Vector3(origin.ptr());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef std::map<TerrainGroup::Orientation_type, Ogre::Terrain::Alignment>  OrientationToAlignmentMap_type;
const OrientationToAlignmentMap_type orientationToAlignmentMap = boost::assign::map_list_of<TerrainGroup::Orientation_type, Ogre::Terrain::Alignment>
    (TerrainGroup::ORIENTATION_X_Z, Ogre::Terrain::ALIGN_X_Z)
    (TerrainGroup::ORIENTATION_X_Y, Ogre::Terrain::ALIGN_X_Y)
    (TerrainGroup::ORIENTATION_Y_X, Ogre::Terrain::ALIGN_Y_Z);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
TerrainGroup::setOrientation(Orientation_type _orientation)
{
    OrientationToAlignmentMap_type::const_iterator iter = orientationToAlignmentMap.find(_orientation);
    if (iter != orientationToAlignmentMap.end())
    {
        if (!m_loaded)
        {
            m_pTerrainGroup->getDefaultImportSettings().terrainAlign = iter->second;
            return;
        }

        /// TODO Set entire terrain group as dirty and reload
        throw Zen::Utility::runtime_exception("TerrainGroup::setOrientation() : Cannot change orientation while terrain is loaded.");
    }

    throw Zen::Utility::runtime_exception("TerrainGroup::setOrientation() : Orientation not supported.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
typedef std::map<Ogre::Terrain::Alignment, TerrainGroup::Orientation_type>  AlignmentToOrientationMap_type;
const AlignmentToOrientationMap_type alignmentToOrientationMap = boost::assign::map_list_of<Ogre::Terrain::Alignment, TerrainGroup::Orientation_type>
    (Ogre::Terrain::ALIGN_X_Z, TerrainGroup::ORIENTATION_X_Z)
    (Ogre::Terrain::ALIGN_X_Y, TerrainGroup::ORIENTATION_X_Y)
    (Ogre::Terrain::ALIGN_Y_Z, TerrainGroup::ORIENTATION_Y_Z);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroup::Orientation_type 
TerrainGroup::getOrientation() const
{
    assert(m_pTerrainGroup != NULL);
    AlignmentToOrientationMap_type::const_iterator iter = alignmentToOrientationMap.find(m_pTerrainGroup->getAlignment());
    if (iter != alignmentToOrientationMap.end())
    {
        return iter->second;
    }

    // Should never get here.
    throw Zen::Utility::runtime_exception("TerrainGroup::getOrientation() : Alignment not supported.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
TerrainGroup::setTerrainSize(boost::uint16_t _terrainSize)
{
    if (!m_loaded)
    {
        m_pTerrainGroup->getDefaultImportSettings().terrainSize = _terrainSize;
        return;
    }

    /// TODO Set entire terrain group as dirty and reload
    throw Zen::Utility::runtime_exception("TerrainGroup::setTerrainSize() : Cannot change orientation while terrain is loaded.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
boost::uint16_t 
TerrainGroup::getTerrainSize() const
{
    return m_pTerrainGroup->getDefaultImportSettings().terrainSize;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
TerrainGroup::setWorldSize(Zen::Math::Real _worldSize)
{
    if (!m_loaded)
    {
        m_pTerrainGroup->getDefaultImportSettings().worldSize = _worldSize;
        return;
    }

    /// TODO Set entire terrain group as dirty and reload
    throw Zen::Utility::runtime_exception("TerrainGroup::setWorldSize() : Cannot change orientation while terrain is loaded.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
TerrainGroup::getWorldSize() const
{
    return m_pTerrainGroup->getDefaultImportSettings().worldSize;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::load(bool _synchronous)
{
    m_pTerrainGroup->loadAllTerrains(_synchronous);
    m_loaded = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
TerrainGroup::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            sm_pModule->getScriptModule(),
            sm_pModule->getScriptModule()->getScriptType("OgreTerrainGroup"),
            getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Ogre::TerrainGroup&
TerrainGroup::getOgreTerrainGroup()
{
    assert(m_pTerrainGroup != NULL);
    return *m_pTerrainGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::scriptSetOrientation(int _orientation)
{
    setOrientation(static_cast<Orientation_type>(_orientation));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
TerrainGroup::scriptGetOrientation()
{
    return static_cast<int>(getOrientation());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroup::pTerrainChunk_type
TerrainGroup::scriptGetTerrainChunk(int _x, int _y)
{
    Ogre::PageID pageId = m_pTerrainGroup->packIndex(_x, _y);

    TerrainChunk::TerrainChunkId id(new TerrainChunk::NativeTerrainChunkId(pageId));

    return getTerrainChunk(id);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroup::pTerrainChunk_type
TerrainGroup::scriptCreateTerrainChunk(int _x, int _y)
{
    Ogre::PageID pageId = m_pTerrainGroup->packIndex(_x, _y);

    TerrainChunk::TerrainChunkId id(new TerrainChunk::NativeTerrainChunkId(pageId));

    return createTerrainChunk(id);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::scriptRemoveTerrainChunk(int _x, int _y)
{
    Ogre::PageID pageId = m_pTerrainGroup->packIndex(_x, _y);

    TerrainChunk::TerrainChunkId id(new TerrainChunk::NativeTerrainChunkId(pageId));

    removeTerrainChunk(id);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::scriptSetTerrainSize(int _size)
{
    setTerrainSize(_size);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::script_module* TerrainGroup::sm_pModule = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroup::registerScriptModule(Zen::Scripting::script_module& _module, Zen::Scripting::script_module& _baseModule)
{
    sm_pModule = &_module;

    _module.addDerivedType<TerrainGroup>(_baseModule.getScriptType("TerrainGroup"),
            "OgreTerrainGroup", "OGRE Terrain")
        .addMethod("setOrientation", &TerrainGroup::scriptSetOrientation)
        .addMethod("getOrientation", &TerrainGroup::scriptGetOrientation)
        .addMethod("getTerrainChunk", &TerrainGroup::scriptGetTerrainChunk)
        .addMethod("createTerrainChunk", &TerrainGroup::scriptCreateTerrainChunk)
        .addMethod("removeTerrainChunk", &TerrainGroup::scriptRemoveTerrainChunk)
        .addMethod("setTerrainSize", &TerrainGroup::scriptSetTerrainSize)
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgreTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
