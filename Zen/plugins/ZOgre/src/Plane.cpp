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

#include "Plane.hpp"

#include <Zen/Engine/Rendering/I_AxisAlignedBoundingBox.hpp>
#include <Zen/Engine/Rendering/I_BoundingSphere.hpp>

#include <OgreAxisAlignedBox.h>
#include <OgreSphere.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::Plane()
:   m_ogrePlane()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::Plane(const Zen::Engine::Rendering::I_Plane& _rhs)
{
    const Zen::Math::Vector3& normal(_rhs.getNormal());

#ifndef ZEN_MATH_USE_DOUBLE

    m_ogrePlane = Ogre::Plane(Ogre::Vector3(normal.m_array),
                              _rhs.getDistanceFromOrigin());

#else   // ZEN_MATH_USE_DOUBLE

    m_ogrePlane = Ogre::Plane(Ogre::Vector3(normal.m_x, normal.m_y, normal.m_z),
                              _rhs.getDistanceFromOrigin());

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::Plane(const Zen::Math::Vector3& _normal, 
                   const Zen::Math::Real _distance)

#ifndef ZEN_MATH_USE_DOUBLE

:   m_ogrePlane(Ogre::Vector3(_normal.m_array),_distance)

#else   // ZEN_MATH_USE_DOUBLE

:   m_ogrePlane(Ogre::Vector3(_normal.m_x, _normal.m_y, _normal.m_z),_distance)

#endif  // ZEN_MATH_USE_DOUBLE

{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::Plane(const Zen::Math::Vector3& _normal, 
                   const Zen::Math::Vector3 _vector)

#ifndef ZEN_MATH_USE_DOUBLE

:   m_ogrePlane(Ogre::Vector3(_normal.m_array),Ogre::Vector3(_vector.m_array))

#else   // ZEN_MATH_USE_DOUBLE

:   m_ogrePlane(Ogre::Vector3(_normal.m_x, _normal.m_y, _normal.m_z),Ogre::Vector3(_vector.m_x, _vector.m_y, _vector.m_z))

#endif  // ZEN_MATH_USE_DOUBLE

{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::Plane(const Zen::Math::Vector3& _point1,
                   const Zen::Math::Vector3& _point2,
                   const Zen::Math::Vector3& _point3)

#ifndef ZEN_MATH_USE_DOUBLE

:   m_ogrePlane(Ogre::Vector3(_point1.m_array),
                Ogre::Vector3(_point2.m_array),
                Ogre::Vector3(_point3.m_array))

#else   // ZEN_MATH_USE_DOUBLE

:   m_ogrePlane(Ogre::Vector3(_point1.m_x, _point1.m_y, _point1.m_z),
                Ogre::Vector3(_point2.m_x, _point2.m_y, _point2.m_z),
                Ogre::Vector3(_point3.m_x, _point3.m_y, _point3.m_z))

#endif  // ZEN_MATH_USE_DOUBLE

{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::~Plane()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::side_type 
Plane::getSide(const Zen::Math::Vector3& _vector) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    return (Plane::side_type)(this->m_ogrePlane.getSide(Ogre::Vector3(_vector.m_array)));

#else   // ZEN_MATH_USE_DOUBLE

    return (Plane::side_type)(this->m_ogrePlane.getSide(Ogre::Vector3(_vector.m_x, _vector.m_y, _vector.m_z)));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::side_type 
Plane::getSide(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    return (Plane::side_type)(this->m_ogrePlane.getSide(
        Ogre::AxisAlignedBox(Ogre::Vector3(_axisAlignedBoundingBox.getMinimum().m_array),
                             Ogre::Vector3(_axisAlignedBoundingBox.getMaximum().m_array))));

#else   // ZEN_MATH_USE_DOUBLE

    const Zen::Math::Vector3& minVector(_axisAlignedBoundingBox.getMinimum());
    const Zen::Math::Vector3& maxVector(_axisAlignedBoundingBox.getMaximum());
    return (Plane::side_type)(this->m_ogrePlane.getSide(
        Ogre::AxisAlignedBox(Ogre::Vector3(minVector.m_x, minVector.m_y, minVector.m_z),
                             Ogre::Vector3(maxVector.m_x, maxVector.m_y, maxVector.m_z))));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Plane::side_type 
Plane::getSide(const Zen::Engine::Rendering::I_BoundingSphere& _boundingSphere) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    const Ogre::Vector3 center(_boundingSphere.getCenter().m_array);

#else   // ZEN_MATH_USE_DOUBLE

    const Zen::Math::Vector3& zCenter(_boundingSphere.getCenter());
    const Ogre::Vector3 center(zCenter.m_x, zCenter.m_y, zCenter.m_z);

#endif  // ZEN_MATH_USE_DOUBLE

    const Ogre::Real radius(_boundingSphere.getRadius());
    const Ogre::Vector3 offset(radius,radius,radius);
    const Ogre::AxisAlignedBox ogreBox(center - offset, center + offset);
    return (Plane::side_type)(this->m_ogrePlane.getSide(ogreBox));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Plane::getDistance(const Zen::Math::Vector3& _vector) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    return this->m_ogrePlane.getDistance(Ogre::Vector3(_vector.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    return this->m_ogrePlane.getDistance(Ogre::Vector3(_vector.m_x, _vector.m_y, _vector.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Plane::redefine(const Zen::Math::Vector3& _point1, 
                const Zen::Math::Vector3& _point2,
                const Zen::Math::Vector3& _point3)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogrePlane.redefine(Ogre::Vector3(_point1.m_array),
                               Ogre::Vector3(_point2.m_array),
                               Ogre::Vector3(_point3.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    this->m_ogrePlane.redefine(Ogre::Vector3(_point1.m_x, _point1.m_y, _point1.m_z),
                               Ogre::Vector3(_point2.m_x, _point2.m_y, _point2.m_z),
                               Ogre::Vector3(_point3.m_x, _point3.m_y, _point3.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Plane::redefine(const Zen::Math::Vector3& _normal,
                const Zen::Math::Vector3& _point)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogrePlane.redefine(Ogre::Vector3(_normal.m_array),
                               Ogre::Vector3(_point.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    this->m_ogrePlane.redefine(Ogre::Vector3(_normal.m_x, _normal.m_y, _normal.m_z),
                               Ogre::Vector3(_point.m_x, _point.m_y, _point.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3& 
Plane::projectVector(const Zen::Math::Vector3& _vector) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    const Ogre::Vector3& projection(this->m_ogrePlane.projectVector(Ogre::Vector3(_vector.m_array)));

#else   // ZEN_MATH_USE_DOUBLE

    const Ogre::Vector3& projection(this->m_ogrePlane.projectVector(Ogre::Vector3(_vector.m_x, _vector.m_y, _vector.m_z)));

#endif  // ZEN_MATH_USE_DOUBLE

    return *(new Zen::Math::Vector3(projection.x,projection.y,projection.z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Plane::normalize()
{
    this->m_ogrePlane.normalise();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
Plane::operator==(const Zen::Engine::Rendering::I_Plane& _rhs) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    const Ogre::Plane& rhs(Ogre::Plane(Ogre::Vector3(_rhs.getNormal().m_array),_rhs.getDistanceFromOrigin()));

#else   // ZEN_MATH_USE_DOUBLE

    const Zen::Math::Vector3& normal(_rhs.getNormal());
    const Ogre::Plane& rhs(Ogre::Plane(Ogre::Vector3(normal.m_x, normal.m_y, normal.m_z),_rhs.getDistanceFromOrigin()));

#endif  // ZEN_MATH_USE_DOUBLE

    return (this->m_ogrePlane == rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
Plane::operator!=(const Zen::Engine::Rendering::I_Plane& _rhs) const
{
    return !(*this == _rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3& 
Plane::getNormal() const
{
    const Ogre::Vector3& normal(this->m_ogrePlane.normal);
    return *(new Zen::Math::Vector3(normal.x,normal.y,normal.z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
Plane::getDistanceFromOrigin() const
{
    return this->m_ogrePlane.d;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
