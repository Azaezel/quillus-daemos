//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_ENGINE_WORLD_I_TERRAIN_CHUNK_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_TERRAIN_CHUNK_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_TerrainHeightfield;

class WORLD_DLL_LINK I_TerrainChunk
:   public virtual Zen::Scripting::I_ScriptableType
,   public Zen::Memory::managed_self_ref<I_TerrainChunk>
,   public boost::noncopyable
{
    /// @name Forward Declarations
    /// @{
public:
    struct TerrainChunkId;
    /// @}

    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_TerrainChunk>            pScriptObject_type;
    typedef Scripting::ObjectReference<I_TerrainChunk>          ScriptObjectReference_type;
    typedef ScriptObjectReference_type                          ScriptWrapper_type;
    typedef ScriptWrapper_type*                                 pScriptWrapper_type;

    typedef Memory::managed_ptr<I_TerrainChunk>                 pTerrainChunk_type;
    typedef Memory::managed_weak_ptr<I_TerrainChunk>            wpTerrainChunk_type;

    typedef Event::Event<wpTerrainChunk_type>                   terrainChunkEvent_type;
    /// @}

    /// @name I_TerrainChunk interface
    /// @{
public:
    virtual const TerrainChunkId& getId() const = 0;

    /// Get the height at a particular grid location on the heightmap.
    virtual Zen::Math::Real getHeight(int _x, int _z) const = 0;

    /// Get the width in number of grid points along one edge of the heightmap.
    virtual int getWidth() const = 0;

    /// Get the world width along one edge of the heightmap.
    virtual Zen::Math::Real getWorldWidth() const = 0;

    /// Get the height offset of the heightmap.
    virtual Zen::Math::Real getOffset() const = 0;

    /// Get the skirt size of the heightmap.
    virtual Zen::Math::Real getSkirtSize() const = 0;

    /// Get the heightmap max height.
    virtual Zen::Math::Real getMaxHeight() const = 0;

    /// Get the heightmap min height.
    virtual Zen::Math::Real getMinHeight() const = 0;

    /// Get the terrain chunk position.
    virtual Zen::Math::Vector3& getPosition() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    terrainChunkEvent_type  onDestroyEvent;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "TerrainChunk"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Inner structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// Interface for a terrain chunk id.
    class WORLD_DLL_LINK I_NativeTerrainChunkId
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name I_NativeTerrainChunkId interface
        /// @{
    public:
        virtual bool operator==(const I_NativeTerrainChunkId& _otherId) const = 0;
        virtual bool operator!=(const I_NativeTerrainChunkId& _otherId) const = 0;
        virtual bool operator< (const I_NativeTerrainChunkId& _otherId) const = 0;
        virtual I_NativeTerrainChunkId* clone() const = 0;
        virtual std::string toString() const = 0;
        /// @}

        /// @name 'Structors
        /// @{
    public:
                 I_NativeTerrainChunkId() {}
        virtual ~I_NativeTerrainChunkId() {}
        /// @}

    };  // interface I_NativeTerrainChunkId
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct TerrainChunkId
    {
        /// @name Types
        /// @{
    public:
        /// @}

        /// @name TerrainChunkId implementation
        /// @{
    public:
        TerrainChunkId& operator= (const TerrainChunkId& _otherId)       {delete m_pNativeTerrainChunkId; m_pNativeTerrainChunkId = _otherId.m_pNativeTerrainChunkId->clone(); return *this;}
        bool            operator==(const TerrainChunkId& _otherId) const {return (m_pNativeTerrainChunkId == _otherId.m_pNativeTerrainChunkId) || ( m_pNativeTerrainChunkId &&  _otherId.m_pNativeTerrainChunkId && (*m_pNativeTerrainChunkId == *_otherId.m_pNativeTerrainChunkId));}
        bool            operator!=(const TerrainChunkId& _otherId) const {return (m_pNativeTerrainChunkId != _otherId.m_pNativeTerrainChunkId) && (!m_pNativeTerrainChunkId || !_otherId.m_pNativeTerrainChunkId || (*m_pNativeTerrainChunkId != *_otherId.m_pNativeTerrainChunkId));}
        bool            operator< (const TerrainChunkId& _otherId) const {assert(m_pNativeTerrainChunkId != NULL && _otherId.m_pNativeTerrainChunkId != NULL); return (*m_pNativeTerrainChunkId <  *_otherId.m_pNativeTerrainChunkId);}
                        operator std::string ()                    const {return m_pNativeTerrainChunkId ? m_pNativeTerrainChunkId->toString() : "";}
        std::string toString()                                     const {return m_pNativeTerrainChunkId ? m_pNativeTerrainChunkId->toString() : "";}
        /// @}

        /// @name 'Structors
        /// @{
    public:
         TerrainChunkId(I_NativeTerrainChunkId* const _pId = NULL) : m_pNativeTerrainChunkId(_pId)                                      {}
         TerrainChunkId(const TerrainChunkId& _otherId)            : m_pNativeTerrainChunkId(_otherId.m_pNativeTerrainChunkId->clone()) {}
        ~TerrainChunkId()                                                                                                               {delete m_pNativeTerrainChunkId;}
        /// @}

        /// @name Member Variables
        /// @{
    public:
        I_NativeTerrainChunkId* m_pNativeTerrainChunkId;
        /// @}

    };  // struct TerrainChunkId
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainChunk();
    virtual ~I_TerrainChunk();
    /// @}

};  // interface I_TerrainChunk

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory
{
    /// I_TerrainChunk is managed by a I_TerrainService
    template<>
    struct is_managed_by_factory<Zen::Engine::World::I_TerrainChunk> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_TERRAIN_CHUNK_HPP_INCLUDED
