//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_ZODE_PHYSICS_MATERIAL_HPP_INCLUDED
#define ZEN_ZODE_PHYSICS_MATERIAL_HPP_INCLUDED

/*
// TR - Newton.h should include stdlib.h but it doesn't.
#include <stdlib.h>
#include <Newton.h>
ODE libs
*/

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>
#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>
#include <Zen/Core/Math/Math.hpp>

#include <vector>

#include "PhysicsShape.hpp"

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsZone;

class PhysicsMaterial
:   public Engine::Physics::I_PhysicsMaterial
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsMaterial>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsWorld> wpPhysicsZone_type;
    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsMaterial>         pPhysicsMaterial_type;
    typedef std::vector<pPhysicsMaterial_type>         materials_vector_type;
    /// @}

    /// @name I_PhysicsMaterial implementation
    /// @{
public:
    virtual int getMaterialID();
    virtual int getDefaultMaterialID();

    virtual Math::Real getDynamicFriction();
    virtual Math::Real getStaticFriction();
    virtual Math::Real getRestitution();
    virtual bool getAdvancedCollisionPrediction();
    virtual bool getCollidable();

    virtual void setDynamicFriction(Math::Real _dynamicFriction);
    virtual void setStaticFriction(Math::Real _staticFriction);
    virtual void setRestitution(Math::Real _elasticity);
    virtual void setAdvancedCollisionPrediction(bool _mode);
    virtual void setCollidable(bool _collide);
    virtual void setIgnoreCollision(pPhysicsMaterial_type _material);

    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsMaterial(wpPhysicsZone_type _zone, bool _default);
    virtual ~PhysicsMaterial();
    /// @}

    /// @name Member Variables
    /// @{
private:
    wpPhysicsZone_type  m_pZone;
    int                  m_defaultGroupID;
    int                  m_id;
    bool                 m_bCollidable;
    bool                 m_bAdvancedCollisionPrediction;
    Math::Real           m_restitution;
    Math::Real           m_dynamicFriction;
    Math::Real           m_staticFriction;
    /// @}
};  // class PhysicsMaterial

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_PHYSICS_MATERIAL_HPP_INCLUDED
