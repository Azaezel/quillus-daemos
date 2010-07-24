//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Walt Collins
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_WORLD_I_CLOUD_PARTICLE_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_CLOUD_PARTICLE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Vector4.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WORLD_DLL_LINK I_CloudParticle
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_CloudParticle>           pCloudParticle_type;
    typedef Zen::Memory::managed_weak_ptr<I_CloudParticle>      wpCloudParticle_type;
    typedef Zen::Event::Event<wpCloudParticle_type>             cloudParticleEvent_type;
    /// @}

    /// @name Zen::Engine::World::I_CloudParticle interface
    /// @{
public:
    virtual float getRadius() const = 0;
    virtual void setRadius(const float _radius) = 0;

    virtual float getTransparency() const = 0;
    virtual void setTransparency(const float _transparency) = 0;

    virtual const Zen::Math::Vector3& getPosition() const = 0;
    virtual void getPosition(const Zen::Math::Vector3& _position) = 0;

    virtual const Zen::Math::Vector4& getBaseColor() const = 0;
    virtual void setBaseColor(const Zen::Math::Vector4& _baseColor) = 0;
    
    virtual int getNumLights() const = 0;
    virtual const Zen::Math::Vector4& getLightColor(const unsigned int _lightIdx) const = 0;
    virtual void addLightColor(const Zen::Math::Vector4& _light) = 0;
    virtual void clearLightColors() = 0;

    virtual float getSquareSortDistance() const = 0;
    virtual void setSquareSortDistance(const float _distance) = 0;

    virtual bool operator<(const I_CloudParticle& _rhs) = 0;
    virtual bool operator>(const I_CloudParticle& _rhs) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    cloudParticleEvent_type     onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CloudParticle();
    virtual ~I_CloudParticle();
    /// @}

};  // I_CloudParticle interface

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory {
    // I_CloudParticle is managed by I_Cloud
    template<>
    struct is_managed_by_factory<Zen::Engine::World::I_CloudParticle> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_CLOUD_PARTICLE_HPP_INCLUDED
