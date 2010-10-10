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
#ifndef ZEN_ZODE_HEIGHTFIELD_COLLISION_SHAPE_HPP_INCLUDED
#define ZEN_ZODE_HEIGHTFIELD_COLLISION_SHAPE_HPP_INCLUDED

#include "CollisionShape.hpp"

#include <ode/ode.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Engine {
        namespace World {
            class I_TerrainChunk;
        }   // namespace World
    }   // namespace Engine
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class HeightfieldCollisionShape
:   public CollisionShape
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Engine::World::I_TerrainChunk>  pTerrainChunk_type;
    /// @}

    /// @name HeightfieldCollisionShape implementation
    /// @{
public:
    Zen::Math::Real getHeight(int _x, int _z);
    /// @}

    /// @name 'Structors
    /// @{
public:
             HeightfieldCollisionShape(dSpaceID _spaceId, pTerrainChunk_type _pTerrainChunk);
    virtual ~HeightfieldCollisionShape();
    /// @}

    /// @name Member Variables
    /// @{
private:
    dHeightfieldDataID              m_heightfieldDataId;
    pTerrainChunk_type              m_pTerrainChunk;
    /// @}

};  // class HeightfieldCollisionShape

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_HEIGHTFIELD_COLLISION_SHAPE_HPP_INCLUDED
