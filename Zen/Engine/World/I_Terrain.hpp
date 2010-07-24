//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Walt Collins
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
#ifndef ZEN_ENGINE_WORLD_I_TERRAIN_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_TERRAIN_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

//#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Math/Matrix4.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Math {
        class Matrix4;
    }   // namespace Math
namespace Engine {
    namespace Resource {
        class I_Resource;
    }
    namespace Physics {
        class I_PhysicsActor;
    }
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class WORLD_DLL_LINK I_Terrain
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Terrain>                 pScriptObject_type;
    typedef Scripting::ObjectReference<I_Terrain>               ScriptObjectReference_type;

    typedef Memory::managed_ptr<Physics::I_PhysicsActor>        pPhysicsActor_type;
    typedef Memory::managed_ptr<I_Terrain>                      pTerrain_type;
    typedef Memory::managed_weak_ptr<I_Terrain>                 wpTerrain_type;
    typedef Event::Event<wpTerrain_type>                        terrainEvent_type;
    typedef Memory::managed_ptr<Resource::I_Resource>           pResource_type;
    typedef Memory::managed_weak_ptr<Resource::I_Resource>      wpResource_type;
    /// @}

    /// @name I_Terrain interface
    /// @{
public:
    virtual void setPhysicsActor(pPhysicsActor_type _pActor) = 0;
    virtual pPhysicsActor_type getPhysicsActor() = 0;
    virtual void setResource(pResource_type _resource) = 0;
    virtual pResource_type getResource() = 0;
    virtual bool loadVisualization(const std::string& _ogreSpecificConfigFileName, const Math::Matrix4& _transform) = 0;
    virtual bool loadPhysicsFromRaw(const std::string& _rawFileName, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize) = 0;
    virtual bool loadPhysicsFromSerialization(const std::string& _serializationFileName, const Math::Matrix4& _transform) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    terrainEvent_type onDestroyEvent;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "Terrain"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Terrain();
    virtual ~I_Terrain();
    /// @}

};  // interface I_Terrain

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory
{
    /// I_Terrain is managed by a I_TerrainService
    template<>
    struct is_managed_by_factory<Zen::Engine::World::I_Terrain> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_TERRAIN_HPP_INCLUDED
