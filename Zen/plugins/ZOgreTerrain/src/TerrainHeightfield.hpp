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
#ifndef ZEN_ZOGRE_TERRAIN_TERRAIN_HEIGHTFIELD_HPP_INCLUDED
#define ZEN_ZOGRE_TERRAIN_TERRAIN_HEIGHTFIELD_HPP_INCLUDED

#include <Zen/Engine/World/I_TerrainHeightfield.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgreTerrain {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TerrainChunk;

class TerrainHeightfield
:   public Engine::World::I_TerrainHeightfield
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_TerrainHeightfield implementation
    /// @{
public:
    virtual Engine::World::I_TerrainChunk& getParent() const;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name TerrainHeightfield implementation
    /// @{
public:
    static void registerScriptModule(Zen::Scripting::script_module& _module, Zen::Scripting::script_module& _baseModule);
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             TerrainHeightfield(TerrainChunk& _parent);
    virtual ~TerrainHeightfield();
    /// @}

    /// @name Member Variables
    /// @{
private:
    static Zen::Scripting::script_module*   sm_pModule;
    ScriptObjectReference_type*             m_pScriptObject;

    TerrainChunk&                           m_parent;
    /// @}

};  // class TerrainHeightfield

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgreTerrain
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_TERRAIN_TERRAIN_HEIGHTFIELD_HPP_INCLUDED
