//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZOGRE_WATER_HPP_INCLUDED
#define ZEN_ZOGRE_WATER_HPP_INCLUDED

#ifdef USE_HYDRAX

#include "RenderingView.hpp"

#include "Ogre.hpp"

#include <Hydrax/Hydrax.h>
#include <Hydrax/Modules/Module.h>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/World/I_Water.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Water
:   public Engine::World::I_Water
,   public Memory::managed_self_ref<Zen::Engine::World::I_Water>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Engine::World::I_Water>        wpWater_type;
    typedef Memory::managed_ptr<Engine::Resource::I_Resource>       pResource_type;
    typedef Memory::managed_weak_ptr<Engine::Resource::I_Resource>  wpResource_type;
    /// @}

    /// @name I_Water implementation
    /// @{
public:
    virtual void update(Zen::Math::Real _timeDelta);
    virtual void setPolygonMode(polygonMode_enum _mode);
    virtual polygonMode_enum getPolygonMode() const;
    virtual void setShaderMode(shaderMode_enum _mode);
    virtual shaderMode_enum getShaderMode() const;
    virtual void setPosition(const Math::Vector3& _position);
    virtual const Zen::Math::Vector3 getPosition() const;
    virtual Zen::Math::Real getHeight(const Zen::Math::Vector3& _position) const;
    virtual void rotate(const Math::Quaternion4& _q);
    virtual void setFullReflectionDistance(const Zen::Math::Real _distance);
    virtual Zen::Math::Real getFullReflectionDistance() const;
    virtual void setGlobalTransparency(const Zen::Math::Real _transparency);
    virtual Zen::Math::Real getGlobalTransparency() const;
    virtual void setWaterColor(const Zen::Math::Vector3& _color);
    virtual const Zen::Math::Vector3 getWaterColor() const;
    virtual void setNormalDistortion(const Zen::Math::Real _distortion);
    virtual Zen::Math::Real getNormalDistortion() const;
    virtual void setSunPosition(const Zen::Math::Vector3& _position);
    virtual const Zen::Math::Vector3 getSunPosition() const;
    virtual void setSunStrength(const Zen::Math::Real _strength);
    virtual Zen::Math::Real getSunStrength() const;
    virtual void setSunArea(const Zen::Math::Real _area);
    virtual Zen::Math::Real getSunArea() const;
    virtual void setSunColor(const Zen::Math::Vector3& _color);
    virtual const Zen::Math::Vector3 getSunColor() const;
    virtual void setFoamMaxDistance(const Zen::Math::Real _distance);
    virtual Zen::Math::Real getFoamMaxDistance() const;
    virtual void setFoamScale(const Zen::Math::Real _scale);
    virtual Zen::Math::Real getFoamScale() const;
    virtual void setFoamStart(const Zen::Math::Real _start);
    virtual Zen::Math::Real getFoamStart() const;
    virtual void setFoamTransparency(const Zen::Math::Real _transparency);
    virtual Zen::Math::Real getFoamTransparency() const;
    virtual void setDepthLimit(const Zen::Math::Real _depth);
    virtual Zen::Math::Real getDepthLimit() const;
    virtual void setSmoothPower(const Zen::Math::Real _power);
    virtual Zen::Math::Real getSmoothPower() const;
    virtual void setCausticsScale(const Zen::Math::Real _scale);
    virtual Zen::Math::Real getCausticsScale() const;
    virtual void setCausticsPower(const Zen::Math::Real _power);
    virtual Zen::Math::Real getCausticsPower() const;
    virtual void setCausticsEnd(const Zen::Math::Real _end);
    virtual Zen::Math::Real getCausticsEnd() const;
    virtual void setGodRaysExposure(const Zen::Math::Vector3& _exposure);
    virtual const Zen::Math::Vector3 getGodRaysExposure() const;
    virtual void setGodRaysIntensity(const Zen::Math::Real _intensity);
    virtual Zen::Math::Real getGodRaysIntensity() const;
    /// @}

    /// @name Water implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             Water(Zen::Engine::Rendering::I_Canvas& _canvas, const std::string& _configPath);
    virtual ~Water();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Hydrax::Hydrax*         m_pHydrax;

    Ogre::SceneManager*     m_pSceneManager;
    Ogre::Camera*           m_pCamera;
    Ogre::Viewport*         m_pViewport;
    /// @}

};  // class Water

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // USE_HYDRAX

#endif // ZEN_ZOGRE_WATER_HPP_INCLUDED
