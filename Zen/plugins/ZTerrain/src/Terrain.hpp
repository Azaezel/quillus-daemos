//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
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
#ifndef ZEN_ZTERRAIN_TERRAIN_HPP_INCLUDED
#define ZEN_ZTERRAIN_TERRAIN_HPP_INCLUDED

#include <Zen/Engine/World/I_Terrain.hpp>
#include <Zen/Engine/World/I_TerrainService.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Terrain
:   public Engine::World::I_Terrain
,   public Memory::managed_self_ref<Zen::Engine::World::I_Terrain>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule>         pScriptModule_type;
    typedef Memory::managed_weak_ptr<Engine::World::I_Terrain>          wpTerrain_type;
    typedef Memory::managed_ptr<Engine::Resource::I_Resource>           pResource_type;
    typedef Memory::managed_weak_ptr<Engine::Resource::I_Resource>      wpResource_type;
    typedef Memory::managed_weak_ptr<Engine::World::I_TerrainService>   wpTerrainService_type;
    /// @}

    /// @name I_Terrain implementation
    /// @{
public:
    virtual void setPhysicsActor(pPhysicsActor_type _shape);
    virtual pPhysicsActor_type getPhysicsActor();
    virtual void setResource(pResource_type _resource);
    virtual pResource_type getResource();
    virtual bool loadVisualization(const std::string& _ogreSpecificConfigFileName, const Math::Matrix4& _transform);
    virtual bool loadPhysicsFromRaw(const std::string& _rawFileName, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize);
    virtual bool loadPhysicsFromSerialization(const std::string& _serializationFileName, const Math::Matrix4& _transform);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name Terrain implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             Terrain(wpTerrainService_type wpService);
    virtual ~Terrain();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wpTerrainService_type       m_pTerrainService;
    pResource_type              m_pResource;
    pPhysicsActor_type          m_pActor;

    pScriptModule_type          m_pScriptModule;
    ScriptObjectReference_type* m_pScriptObject;
    /// @}

};  // class Terrain

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZTERRAIN_TERRAIN_HPP_INCLUDED
