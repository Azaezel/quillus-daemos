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
#ifndef ZEN_ZOGRE_PLANE_HPP_INCLUDED
#define ZEN_ZOGRE_PLANE_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_Plane.hpp>

#include <OgrePlane.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Plane
:   public Zen::Engine::Rendering::I_Plane
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Zen::Engine::Rendering::I_Plane implementation
    /// @{
public:
    virtual side_type getSide(const Zen::Math::Vector3& _vector) const;
    virtual side_type getSide(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const;
    virtual side_type getSide(const Zen::Engine::Rendering::I_BoundingSphere& _boundingSphere) const;
    virtual Zen::Math::Real getDistance(const Zen::Math::Vector3& _vector) const;
    virtual void redefine(const Zen::Math::Vector3& _point1, 
                          const Zen::Math::Vector3& _point2,
                          const Zen::Math::Vector3& _point3);
    virtual void redefine(const Zen::Math::Vector3& _normal,
                          const Zen::Math::Vector3& _point);
    virtual const Zen::Math::Vector3& projectVector(const Zen::Math::Vector3& _vector) const;
    virtual void normalize();
    virtual bool operator==(const Zen::Engine::Rendering::I_Plane& _rhs) const;
    virtual bool operator!=(const Zen::Engine::Rendering::I_Plane& _rhs) const;
    virtual const Zen::Math::Vector3& getNormal() const;
    virtual Zen::Math::Real getDistanceFromOrigin() const;
    /// @}

    /// @name 'Structors
    /// @{
public:
             Plane();
             Plane(const Zen::Engine::Rendering::I_Plane& _rhs);
             Plane(const Zen::Math::Vector3& _normal, 
                   const Zen::Math::Real _distance);
             Plane(const Zen::Math::Vector3& _normal, 
                   const Zen::Math::Vector3 _vector);
             Plane(const Zen::Math::Vector3& _point1,
                   const Zen::Math::Vector3& _point2,
                   const Zen::Math::Vector3& _point3);
    virtual ~Plane();
    /// @}

    /// @name Member variables
    /// @{
private:
    Ogre::Plane m_ogrePlane;
    /// @}

};  // class Plane

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_PLANE_HPP_INCLUDED
