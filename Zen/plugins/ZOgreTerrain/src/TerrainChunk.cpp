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

#include "TerrainChunk.hpp"
#include "TerrainGroup.hpp"
#include "TerrainService.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgreTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunk::TerrainChunk(TerrainGroup& _parent, Ogre::Terrain& _terrain, long _x, long _y)
:   m_parent(_parent)
,   m_x(_x)
,   m_y(_y)
,   m_pScriptObject(NULL)
,   m_pTerrainChunk(&_terrain)
{
    m_id = TerrainChunkId(
        new NativeTerrainChunkId(
            m_parent.getOgreTerrainGroup().packIndex(
                m_x, m_y
            )
        )
    );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunk::~TerrainChunk()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const TerrainChunk::TerrainChunkId&
TerrainChunk::getId() const
{
    return m_id;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
TerrainChunk::getHeight(int _x, int _z) const
{
    return m_pTerrainChunk->getHeightAtPoint(_x, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
TerrainChunk::getWidth() const
{
    return m_parent.getTerrainSize();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
TerrainChunk::getWorldWidth() const
{
    return m_parent.getWorldSize();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
TerrainChunk::getOffset() const
{
    return 0.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
TerrainChunk::getSkirtSize() const
{
    return m_pTerrainChunk->getSkirtSize();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
TerrainChunk::getMaxHeight() const
{
    return m_pTerrainChunk->getMaxHeight();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
TerrainChunk::getMinHeight() const
{
    return m_pTerrainChunk->getMinHeight();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
TerrainChunk::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type
            (sm_pModule->getScriptModule(),
            sm_pModule->getScriptModule()->getScriptType("OgreTerrainChunk"),
            getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::script_module* TerrainChunk::sm_pModule = NULL;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunk::registerScriptModule(Zen::Scripting::script_module& _module, Zen::Scripting::script_module& _baseModule)
{
    sm_pModule = &_module;

    _module.addDerivedType<TerrainChunk>(_baseModule.getScriptType("TerrainChunk"),
            "OgreTerrainChunk", "OGRE Terrain")
    ;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
TerrainChunk::NativeTerrainChunkId::operator==(const I_NativeTerrainChunkId& _otherId) const
{
    const NativeTerrainChunkId* const pNativeTerrainChunkId = dynamic_cast<const NativeTerrainChunkId*>(&_otherId);
    return (pNativeTerrainChunkId != NULL) && (m_nativeTerrainChunkId == pNativeTerrainChunkId->m_nativeTerrainChunkId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
TerrainChunk::NativeTerrainChunkId::operator!=(const I_NativeTerrainChunkId& _otherId) const
{
    const NativeTerrainChunkId* const pNativeTerrainChunkId = dynamic_cast<const NativeTerrainChunkId*>(&_otherId);
    return (pNativeTerrainChunkId == NULL) && (m_nativeTerrainChunkId != pNativeTerrainChunkId->m_nativeTerrainChunkId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
TerrainChunk::NativeTerrainChunkId::operator<(const I_NativeTerrainChunkId& _otherId) const
{
    const NativeTerrainChunkId* const pNativeTerrainChunkId = dynamic_cast<const NativeTerrainChunkId*>(&_otherId);
    assert(pNativeTerrainChunkId != NULL);
    return (m_nativeTerrainChunkId < pNativeTerrainChunkId->m_nativeTerrainChunkId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::World::I_TerrainChunk::I_NativeTerrainChunkId*
TerrainChunk::NativeTerrainChunkId::clone() const
{
    return new NativeTerrainChunkId(m_nativeTerrainChunkId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::string
TerrainChunk::NativeTerrainChunkId::toString() const
{
    std::ostringstream oStream;

    /// TODO This should be done via a call to a Ogre::TerrainGroup::unpackIndex()
    boost::uint16_t x16, y16;
    x16 = static_cast<boost::uint16_t>(m_nativeTerrainChunkId & 0xFFFF);
    y16 = static_cast<boost::uint16_t>((m_nativeTerrainChunkId >> 16) & 0xFFFF);

    long x, y;
    x = static_cast<long>(x16);
    y = static_cast<long>(y16);

    oStream << "<" << x << "," << y << ">";
    return oStream.str();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Ogre::PageID&
TerrainChunk::NativeTerrainChunkId::getNativeId() const
{
    return m_nativeTerrainChunkId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgreTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
