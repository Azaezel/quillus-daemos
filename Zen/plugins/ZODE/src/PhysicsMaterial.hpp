//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZODE_PHYSICS_MATERIAL_HPP_INCLUDED
#define ZEN_ZODE_PHYSICS_MATERIAL_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class PhysicsMaterial
:   public Engine::Physics::I_PhysicsMaterial
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsMaterial>
{
    /// @name Types
    /// @{
public:
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
             PhysicsMaterial();
    virtual ~PhysicsMaterial();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class PhysicsMaterial

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_PHYSICS_MATERIAL_HPP_INCLUDED
