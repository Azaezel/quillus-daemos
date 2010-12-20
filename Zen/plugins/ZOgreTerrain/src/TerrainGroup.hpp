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
#ifndef ZEN_ZOGRE_TERRAIN_TERRAIN_GROUP_HPP_INCLUDED
#define ZEN_ZOGRE_TERRAIN_TERRAIN_GROUP_HPP_INCLUDED

#include <Zen/Engine/World/I_TerrainGroup.hpp>
#include <Zen/Engine/World/I_TerrainService.hpp>

#include <OgreTerrainGroup.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgreTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class TerrainChunk;

class TerrainGroup
:   public Engine::World::I_TerrainGroup
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Engine::World::I_TerrainService>   wpTerrainService_type;

    typedef std::map<std::string, std::string>                          config_type;
    /// @}

    /// @name I_TerrainGroup implementation
    /// @{
public:
    virtual void setName(const std::string& _name);
    virtual const std::string& getName() const;
    virtual void visitTerrainChunks(I_TerrainChunkVisitor& _visitor) const;
    virtual void visitTerrainChunks(I_TerrainChunkVisitor& _visitor);
    virtual pTerrainChunk_type getTerrainChunk(const Engine::World::I_TerrainChunk::TerrainChunkId& _id);
    virtual pTerrainChunk_type createTerrainChunk(const Engine::World::I_TerrainChunk::TerrainChunkId& _id);
    virtual void removeTerrainChunk(const Engine::World::I_TerrainChunk::TerrainChunkId& _id);
    virtual void setOrigin(const Zen::Math::Vector3& _origin);
    virtual const Zen::Math::Vector3 getOrigin() const;
    virtual void setOrientation(Orientation_type _orientation);
    virtual Orientation_type getOrientation() const;
    virtual void setTerrainSize(boost::uint16_t _terrainSize);
    virtual boost::uint16_t getTerrainSize() const;
    virtual void setWorldSize(Zen::Math::Real _worldSize);
    virtual Zen::Math::Real getWorldSize() const;
    virtual void load(bool _synchronous = false);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name TerrainGroup implementation
    /// @{
public:
    void destroyTerrainChunk(wpTerrainChunk_type _wpTerrainChunk);
    Ogre::TerrainGroup& getOgreTerrainGroup();
    void scriptSetOrientation(int _orientation);
    int scriptGetOrientation();
    void scriptSetTerrainSize(int _size);
    pTerrainChunk_type scriptGetTerrainChunk(int _x, int _y);
    pTerrainChunk_type scriptCreateTerrainChunk(int _x, int _y);
    void scriptRemoveTerrainChunk(int _x, int _y);
    static void registerScriptModule(Zen::Scripting::script_module& _module, Zen::Scripting::script_module& _baseModule);
    /// @}

    /// @name 'Structors
    /// @{
public:
    TerrainGroup(wpTerrainService_type _wpService, const std::string& _name);
    virtual ~TerrainGroup();
    /// @}

    /// @name Member Variables
    /// @{
private:
    static Zen::Scripting::script_module*   sm_pModule;
    ScriptObjectReference_type*             m_pScriptObject;

    wpTerrainService_type                   m_pTerrainService;

    Ogre::TerrainGroup*                     m_pTerrainGroup;

    typedef std::map<Zen::Engine::World::I_TerrainChunk::TerrainChunkId, TerrainChunk*> TerrainChunkMap_type;
    TerrainChunkMap_type                    m_terrainChunkMap;

    bool                                    m_loaded;
    std::string                             m_name;
    /// @}

};  // class TerrainGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgreTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_TERRAIN_TERRAIN_GROUP_HPP_INCLUDED
