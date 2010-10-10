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
#ifndef ZEN_ZOGRE_TERRAIN_TERRAIN_CHUNK_HPP_INCLUDED
#define ZEN_ZOGRE_TERRAIN_TERRAIN_CHUNK_HPP_INCLUDED

#include <Zen/Engine/World/I_TerrainChunk.hpp>
#include <Zen/Engine/World/I_TerrainGroup.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPaging.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgreTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TerrainGroup;

class TerrainChunk
:   public Engine::World::I_TerrainChunk
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Engine::World::I_TerrainGroup>   wpTerrainGroup_type;
    /// @}

    /// @name I_TerrainChunk implementation
    /// @{
public:
    virtual const TerrainChunkId& getId() const;
    virtual Zen::Math::Real getHeight(int _x, int _z) const;
    virtual int getWidth() const;
    virtual Zen::Math::Real getWorldWidth() const;
    virtual Zen::Math::Real getOffset() const;
    virtual Zen::Math::Real getSkirtSize() const;
    virtual Zen::Math::Real getMaxHeight() const;
    virtual Zen::Math::Real getMinHeight() const;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name TerrainChunk implementation
    /// @{
public:
    static void registerScriptModule(Zen::Scripting::script_module& _module, Zen::Scripting::script_module& _baseModule);
    /// @}

    /// @name Inner Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    class NativeTerrainChunkId
    :   public I_TerrainChunk::I_NativeTerrainChunkId
    {
        /// @name Friend Declarations
        /// @{
    private:
        friend class TerrainChunk;
        /// @}

        /// @name NativeTerrainChunkId implementation
        /// @{
    public:
        virtual bool operator==(const I_NativeTerrainChunkId& _otherId) const;
        virtual bool operator!=(const I_NativeTerrainChunkId& _otherId) const;
        virtual bool operator< (const I_NativeTerrainChunkId& _otherId) const;
        virtual I_NativeTerrainChunkId* clone() const;
        virtual std::string toString() const;
        virtual const Ogre::PageID& getNativeId() const;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 NativeTerrainChunkId() : m_nativeTerrainChunkId() {}
                 NativeTerrainChunkId(Ogre::PageID _id) : m_nativeTerrainChunkId(_id) {}
        virtual ~NativeTerrainChunkId() {}
        /// @}

        /// @name Member Variables
        /// @{
    private:
        Ogre::PageID    m_nativeTerrainChunkId;
        /// @}

    };  // class NativeTerrainChunkId
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
public:
             TerrainChunk(TerrainGroup& _parent, Ogre::Terrain& _terrain, long _x, long _y);
    virtual ~TerrainChunk();
    /// @}

    /// @name Member Variables
    /// @{
private:
    static Zen::Scripting::script_module*   sm_pModule;
    ScriptObjectReference_type*             m_pScriptObject;

    TerrainGroup&                           m_parent;
    long                                    m_x, m_y;
    TerrainChunkId                          m_id;

    Ogre::Terrain*                          m_pTerrainChunk;
    /// @}

};  // class TerrainChunk

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgreTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_TERRAIN_TERRAIN_CHUNK_HPP_INCLUDED
