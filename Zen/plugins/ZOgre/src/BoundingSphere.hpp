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
#ifndef ZEN_ZOGRE_BOUNDING_SPHERE_HPP_INCLUDED
#define ZEN_ZOGRE_BOUNDING_SPHERE_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_BoundingSphere.hpp>

#include <OgreSphere.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Engine {
    namespace Rendering {
        class I_AxisAlignedBoundingBox;
        class I_Plane;
    }   // namespace Rendering
    }   // namespace Engine
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BoundingSphere
:   public Zen::Engine::Rendering::I_BoundingSphere
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolume implementation
    /// @{
    virtual const Zen::Math::Vector3& getCenter() const;
    virtual void setCenter(const Zen::Math::Vector3& _center);
    virtual bool intersects(const Zen::Engine::Rendering::I_BoundingSphere& _boundingSphere) const;
    virtual bool intersects(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const;
    virtual bool intersects(const Zen::Engine::Rendering::I_Plane& _plane) const;
    virtual bool intersects(const Zen::Math::Vector3& _vector) const;
    virtual bool contains(const Zen::Math::Vector3& _vector) const;
    virtual bool contains(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const;
    virtual bool contains(const Zen::Engine::Rendering::I_BoundingSphere& _boundingSphere) const;
    virtual Zen::Math::Real volume() const;
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingSphere implementation
    /// @{
public:
    virtual Zen::Math::Real getRadius() const;
    virtual void setRadius(const Zen::Math::Real _radius);
    virtual bool operator==(const Zen::Engine::Rendering::I_BoundingSphere& _rhs) const;
    virtual bool operator!=(const Zen::Engine::Rendering::I_BoundingSphere& _rhs) const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             BoundingSphere();
             BoundingSphere(const Zen::Math::Real _radius);
             BoundingSphere(const Zen::Math::Real _radius, const Zen::Math::Vector3& _center);
             BoundingSphere(const Zen::Math::Real _radius,
                            const Zen::Math::Real _centerX,
                            const Zen::Math::Real _centerY,
                            const Zen::Math::Real _centerZ);
    virtual ~BoundingSphere();
    /// @}

    /// @name Member variables
    /// @{
private:
    Ogre::Sphere        m_ogreSphere;
    /// @}
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_BOUNDING_SPHERE_HPP_INCLUDED
