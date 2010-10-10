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
#ifndef ZEN_ENGINE_WORLD_I_TERRAIN_GROUP_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_TERRAIN_GROUP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Engine/World/I_TerrainChunk.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WORLD_DLL_LINK I_TerrainGroup
:   public virtual Zen::Scripting::I_ScriptableType
,   public Zen::Memory::managed_self_ref<I_TerrainGroup>
,   public boost::noncopyable
{
    /// @name Forward Declarations
    /// @{
public:
    struct I_TerrainChunkVisitor;
    /// @}

    /// @name Types
    /// @{
public:
    enum Orientation_type
    {
        ORIENTATION_X_Z = 0,
        ORIENTATION_X_Y = 1,
        ORIENTATION_Y_Z = 2,
        ORIENTATION_Z_X = 3,    // Not currently supported
        ORIENTATION_Y_X = 4,    // Not currently supported
        ORIENTATION_Z_Y = 5     // Not currently supported
    };  // enum Orientation_type

    typedef Zen::Memory::managed_ptr<I_TerrainGroup>            pScriptObject_type;
    typedef Scripting::ObjectReference<I_TerrainGroup>          ScriptObjectReference_type;
    typedef ScriptObjectReference_type                          ScriptWrapper_type;
    typedef ScriptWrapper_type*                                 pScriptWrapper_type;

    typedef Memory::managed_ptr<I_TerrainGroup>                 pTerrainGroup_type;
    typedef Memory::managed_weak_ptr<I_TerrainGroup>            wpTerrainGroup_type;

    typedef Memory::managed_ptr<I_TerrainChunk>                 pTerrainChunk_type;
    typedef Memory::managed_weak_ptr<I_TerrainChunk>            wpTerrainChunk_type;

    typedef Event::Event<wpTerrainGroup_type>                   terrainGroupEvent_type;
    /// @}

    /// @name I_TerrainGroup interface
    /// @{
public:
    /// Define the terrain group name.
    virtual void setName(const std::string& _name) = 0;

    /// Get the terrain group name.
    virtual const std::string& getName() const = 0;

    virtual void visitTerrainChunks(I_TerrainChunkVisitor& _visitor) const = 0;
    virtual void visitTerrainChunks(I_TerrainChunkVisitor& _visitor) = 0;

    /// Get an existing terrain chunk by id
    virtual pTerrainChunk_type getTerrainChunk(const I_TerrainChunk::TerrainChunkId& _id) = 0;

    /// Create a new terrain chunk by id
    virtual pTerrainChunk_type createTerrainChunk(const I_TerrainChunk::TerrainChunkId& _id) = 0;

    /// Remove an existing terrain chunk by id
    virtual void removeTerrainChunk(const I_TerrainChunk::TerrainChunkId& _id) = 0;

    /// Define the center position of the terrain group.
    virtual void setOrigin(const Zen::Math::Vector3& _origin) = 0;

    /// Get the center position of the terrain group.
    virtual const Zen::Math::Vector3 getOrigin() const = 0;

    /// Define the orientation of the terrain group.
    virtual void setOrientation(Orientation_type _orientation) = 0;

    /// Get the orientation of the terrain group.
    virtual Orientation_type getOrientation() const = 0;

    /// Define the terrain size in vertices (along one edge)
    virtual void setTerrainSize(boost::uint16_t _terrainSize) = 0;

    /// Get the terrain chunk size in vertices (along one edge)
    virtual boost::uint16_t getTerrainSize() const = 0;

    /// Define the world size of a terrain chunk
    virtual void setWorldSize(Zen::Math::Real _worldSize) = 0;

    /// Get the world size of a terrain chunk
    virtual Zen::Math::Real getWorldSize() const = 0;

    /// Load the terrain group, thereby loading all terrain chunks.
    virtual void load(bool _synchronous = false) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    terrainGroupEvent_type  onDestroyEvent;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "TerrainGroup"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Inner Structures
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct I_TerrainChunkVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_TerrainChunk& _terrainChunk) = 0;
        virtual void visit(const I_TerrainChunk& _terrainChunk) = 0;
        virtual void end() = 0;
    };  // interface I_TerrainChunkVisitor
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainGroup();
    virtual ~I_TerrainGroup();
    /// @}

};  // interface I_TerrainGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory
{
    /// I_TerrainGroup is managed by a I_TerrainService
    template<>
    struct is_managed_by_factory<Zen::Engine::World::I_TerrainGroup> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_TERRAIN_GROUP_HPP_INCLUDED
