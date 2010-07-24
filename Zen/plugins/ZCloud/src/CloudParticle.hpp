//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZCLOUD_CLOUD_PARTICLE_HPP_INCLUDED
#define ZEN_ZCLOUD_CLOUD_PARTICLE_HPP_INCLUDED

#include <Zen/Engine/World/I_CloudParticle.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCloud {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class CloudParticle
:   public Zen::Engine::World::I_CloudParticle
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Zen::Engine::World::I_CloudParticle implementation
    /// @{
public:
    virtual float getRadius() const;
    virtual void setRadius(const float _radius);

    virtual float getTransparency() const;
    virtual void setTransparency(const float _transparency);

    virtual const Zen::Math::Vector3& getPosition() const;
    virtual void getPosition(const Zen::Math::Vector3& _position);

    virtual const Zen::Math::Vector4& getBaseColor() const;
    virtual void setBaseColor(const Zen::Math::Vector4& _baseColor);
    
    virtual int getNumLights() const;
    virtual const Zen::Math::Vector4& getLightColor(const unsigned int _lightIdx) const;
    virtual void addLightColor(const Zen::Math::Vector4& _light);
    virtual void clearLightColors();

    virtual float getSquareSortDistance() const;
    virtual void setSquareSortDistance(const float _distance);

    virtual bool operator<(const I_CloudParticle& _rhs);
    virtual bool operator>(const I_CloudParticle& _rhs);
    /// @}

    /// @name 'Structors
    /// @{
public:
             CloudParticle();
             CloudParticle(const Zen::Math::Vector3& _position,
                           const float _radius,
                           const Zen::Math::Vector4& _baseColor,
                           const float _transparency = 0);
    virtual ~CloudParticle();
    /// @}

    /// @name Member variables
    /// @{
private:
    float                           m_radius;
    float                           m_transparency;
    Zen::Math::Vector3              m_position;
    Zen::Math::Vector4              m_baseColor;
    std::vector<Zen::Math::Vector4> m_lightColors;
    float                           m_squareSortDistance;
    /// @}

};  // class CloudParticle

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCloud
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZCLOUD_CLOUD_PARTICLE_HPP_INCLUDED
