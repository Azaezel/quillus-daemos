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
#ifndef ZEN_ENGINE_PHYSICS_I_PHYSICS_MATERIAL_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_I_PHYSICS_MATERIAL_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Math.hpp>

#include <boost/noncopyable.hpp>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PHYSICS_DLL_LINK I_PhysicsMaterial
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_PhysicsMaterial>            pPhysicsMaterial_type;
    typedef Memory::managed_weak_ptr<I_PhysicsMaterial>       wpPhysicsMaterial_type;
    typedef Event::Event<wpPhysicsMaterial_type>              physicsMaterialEvent_type;
    /// @}

    /// @name I_PhysicsMaterial interface
    /// @{
public:
    virtual int getMaterialID() = 0;
    virtual int getDefaultMaterialID() = 0;

    virtual Math::Real getDynamicFriction() = 0;
    virtual Math::Real getStaticFriction() = 0;
    virtual Math::Real getRestitution() = 0;
    virtual bool getAdvancedCollisionPrediction() = 0;
    virtual bool getCollidable() = 0;
    
    virtual void setDynamicFriction(Math::Real _dynamicFriction) = 0;
    virtual void setStaticFriction(Math::Real _staticFriction) = 0;
    virtual void setRestitution(Math::Real _elasticity) = 0;
    virtual void setAdvancedCollisionPrediction(bool _mode) = 0;
    virtual void setCollidable(bool _collide) = 0;
    virtual void setIgnoreCollision(pPhysicsMaterial_type _material) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    physicsMaterialEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_PhysicsMaterial();
    virtual ~I_PhysicsMaterial();
    /// @}

};  // interface I_PhysicsMaterial

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
namespace Memory 
{
    /// I_PhysicsMaterial is managed by a I_PhysicsZone
    template<>
    struct is_managed_by_factory<Zen::Engine::Physics::I_PhysicsMaterial> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_I_PHYSICS_MATERIAL_HPP_INCLUDED
