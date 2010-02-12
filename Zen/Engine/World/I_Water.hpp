//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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
#ifndef ZEN_ENGINE_WORLD_I_WATER_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_WATER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Resource {
        class I_Resource;
    }
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WORLD_DLL_LINK I_Water
{
    /// @name Types
    /// @{
public:
    enum polygonMode_enum
    {
        PMODE_SOLID,
        PMODE_WIREFRAME,
        PMODE_POINT,
        PMODE_COUNT
    };

    enum shaderMode_enum
    {
        SMODE_HLSL,
        SMODE_CG,
        SMODE_GLSL,
        SMODE_COUNT
    };

    typedef Memory::managed_ptr<I_Water>            pWater_type;
    typedef Memory::managed_weak_ptr<I_Water>       wpWater_type;
    typedef Event::Event<wpWater_type>              WaterEvent_type;
    typedef Memory::managed_ptr<Resource::I_Resource>           pResource_type;
    typedef Memory::managed_weak_ptr<Resource::I_Resource>      wpResource_type;
    /// @}

    /// @name I_Water interface
    /// @{
public:
    /// Update time elapsed since last frame.
    virtual void update(Zen::Math::Real _timeDelta) = 0;

    /// Set the polygon mode (Solid, Wireframe, Point)
    virtual void setPolygonMode(polygonMode_enum _mode) = 0;

    /// Get the polygon mode
    virtual polygonMode_enum getPolygonMode() const = 0;

    /// Set the shader mode
    virtual void setShaderMode(shaderMode_enum _mode) = 0;

    /// Get the shader mode
    virtual shaderMode_enum getShaderMode() const = 0;

    /// Set water position
    virtual void setPosition(const Math::Vector3& _position) = 0;

    /// Get water position
    virtual const Zen::Math::Vector3 getPosition() const = 0;

    /// Get water height at a specified world space coordinate (Y coord is ignored)
    /// @remarks If the world position is outside of the mesh bounds, return -1.
    virtual Zen::Math::Real getHeight(const Zen::Math::Vector3& _position) const = 0;

    /// Rotate water and planes
    virtual void rotate(const Math::Quaternion4& _q) = 0;

    /// Set full reflection distance
    virtual void setFullReflectionDistance(const Zen::Math::Real _distance) = 0;

    /// Get full reflection distance
    virtual Zen::Math::Real getFullReflectionDistance() const = 0;

    /// Set global transparency
    virtual void setGlobalTransparency(const Zen::Math::Real _transparency) = 0;

    /// Get global transparency
    virtual Zen::Math::Real getGlobalTransparency() const = 0;

    /// Set water color
    virtual void setWaterColor(const Zen::Math::Vector3& _color) = 0;

    /// Get water color
    virtual const Zen::Math::Vector3 getWaterColor() const = 0;

    /// Set normal distortion
    virtual void setNormalDistortion(const Zen::Math::Real _distortion) = 0;

    /// Get normal distortion
    virtual Zen::Math::Real getNormalDistortion() const = 0;

    /// Set sun position
    virtual void setSunPosition(const Zen::Math::Vector3& _position) = 0;

    /// Get sun position
    virtual const Zen::Math::Vector3 getSunPosition() const = 0;

    /// Set sun strength
    virtual void setSunStrength(const Zen::Math::Real _strength) = 0;

    /// Get sun strength
    virtual Zen::Math::Real getSunStrength() const = 0;

    /// Set sun area
    virtual void setSunArea(const Zen::Math::Real _area) = 0;

    /// Get sun area
    virtual Zen::Math::Real getSunArea() const = 0;

    /// Set sun color
    virtual void setSunColor(const Zen::Math::Vector3& _color) = 0;

    /// Get sun color
    virtual const Zen::Math::Vector3 getSunColor() const = 0;

    /// Set foam max distance
    virtual void setFoamMaxDistance(const Zen::Math::Real _distance) = 0;

    /// Get foam max distance
    virtual Zen::Math::Real getFoamMaxDistance() const = 0;

    /// Set foam scale
    virtual void setFoamScale(const Zen::Math::Real _scale) = 0;

    /// Get foam scale
    virtual Zen::Math::Real getFoamScale() const = 0;

    /// Set foam start
    virtual void setFoamStart(const Zen::Math::Real _start) = 0;

    /// Get foam start
    virtual Zen::Math::Real getFoamStart() const = 0;

    /// Set foam transparency
    virtual void setFoamTransparency(const Zen::Math::Real _transparency) = 0;

    /// Get foam transparency
    virtual Zen::Math::Real getFoamTransparency() const = 0;

    /// Set depth limit
    virtual void setDepthLimit(const Zen::Math::Real _depth) = 0;

    /// Get depth limit
    virtual Zen::Math::Real getDepthLimit() const = 0;

    /// Set smoothing power
    virtual void setSmoothPower(const Zen::Math::Real _power) = 0;

    /// Get smoothing power
    virtual Zen::Math::Real getSmoothPower() const = 0;

    /// Set caustics scale
    virtual void setCausticsScale(const Zen::Math::Real _scale) = 0;

    /// Get caustics scale
    virtual Zen::Math::Real getCausticsScale() const = 0;

    /// Set caustics power
    virtual void setCausticsPower(const Zen::Math::Real _power) = 0;

    /// Get caustics power
    virtual Zen::Math::Real getCausticsPower() const = 0;

    /// Set caustics end
    virtual void setCausticsEnd(const Zen::Math::Real _end) = 0;

    /// Get caustics end
    virtual Zen::Math::Real getCausticsEnd() const = 0;

    /// Set god rays exposure
    virtual void setGodRaysExposure(const Zen::Math::Vector3& _exposure) = 0;

    /// Get god rays exposure
    virtual const Zen::Math::Vector3 getGodRaysExposure() const = 0;

    /// Set god rays intensity
    virtual void setGodRaysIntensity(const Zen::Math::Real _intensity) = 0;

    /// Get god rays intensity
    virtual Zen::Math::Real getGodRaysIntensity() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    WaterEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Water();
    virtual ~I_Water();
    /// @}

};  // interface I_Water

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory 
{
    /// I_Water is managed by a I_WaterService
    template<>
    struct is_managed_by_factory<Zen::Engine::World::I_Water> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_WATER_HPP_INCLUDED
