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

#include "AxisAlignedBoundingBox.hpp"

#include <Zen/Engine/Rendering/I_BoundingSphere.hpp>
#include <Zen/Engine/Rendering/I_Plane.hpp>

#include <OgreSphere.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
:   m_ogreAxisAlignedBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox)
:   m_ogreAxisAlignedBox()
{
    const Zen::Math::Vector3& minimum(_axisAlignedBoundingBox.getMinimum());
    const Zen::Math::Vector3& maximum(_axisAlignedBoundingBox.getMaximum());
    this->m_ogreAxisAlignedBox.setMinimum(minimum.m_x, minimum.m_y, minimum.m_z);
    this->m_ogreAxisAlignedBox.setMaximum(maximum.m_x, maximum.m_y, maximum.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Zen::Math::Vector3& _minimum, 
                                    const Zen::Math::Vector3& _maximum)
:   m_ogreAxisAlignedBox(_minimum.m_x,
                         _minimum.m_y,
                         _minimum.m_z,
                         _maximum.m_x,
                         _maximum.m_y,
                         _maximum.m_z)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const Zen::Math::Real _minX,
                                    const Zen::Math::Real _minY,
                                    const Zen::Math::Real _minZ,
                                    const Zen::Math::Real _maxX,
                                    const Zen::Math::Real _maxY,
                                    const Zen::Math::Real _maxZ)
:   m_ogreAxisAlignedBox(_minX,
                         _minY,
                         _minZ,
                         _maxX,
                         _maxY,
                         _maxZ)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AxisAlignedBoundingBox::~AxisAlignedBoundingBox()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3& 
AxisAlignedBoundingBox::getCenter() const
{
    const Ogre::Vector3& center(this->m_ogreAxisAlignedBox.getCenter());
    return *(new Zen::Math::Vector3(center.x,center.y,center.z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setCenter(const Zen::Math::Vector3& _center)
{
    Zen::Math::Vector3 offset(_center - this->getCenter());
    const Ogre::Vector3& minimum(this->m_ogreAxisAlignedBox.getMinimum());
    const Ogre::Vector3& maximum(this->m_ogreAxisAlignedBox.getMaximum());
    this->setExtents(minimum.x + offset.m_x,
                     minimum.y + offset.m_y,
                     minimum.z + offset.m_z,
                     maximum.x + offset.m_x,
                     maximum.y + offset.m_y,
                     maximum.z + offset.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::intersects(const Zen::Engine::Rendering::I_BoundingSphere& _boundingSphere) const
{
    const Zen::Math::Vector3& center(_boundingSphere.getCenter());

#ifndef ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.intersects(
        Ogre::Sphere(Ogre::Vector3(center.m_array),
                     _boundingSphere.getRadius()));

#else   // ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.intersects(
        Ogre::Sphere(Ogre::Vector3(center.m_x,center.m_y,center.m_z),
                     _boundingSphere.getRadius()));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::intersects(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const
{
    const Zen::Math::Vector3& minimum(_axisAlignedBoundingBox.getMinimum());
    const Zen::Math::Vector3& maximum(_axisAlignedBoundingBox.getMaximum());
    return this->m_ogreAxisAlignedBox.intersects(
        Ogre::AxisAlignedBox(minimum.m_x,
                             minimum.m_y,
                             minimum.m_z,
                             maximum.m_x,
                             maximum.m_y,
                             maximum.m_z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::intersects(const Zen::Engine::Rendering::I_Plane& _plane) const
{
    const Zen::Math::Vector3& normal(_plane.getNormal());

#ifndef ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.intersects(
        Ogre::Plane(Ogre::Vector3(normal.m_array),
                    _plane.getDistanceFromOrigin()));

#else   // ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.intersects(
        Ogre::Plane(Ogre::Vector3(normal.m_x, normal.m_y, normal.m_z),
                    _plane.getDistanceFromOrigin()));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::intersects(const Zen::Math::Vector3& _vector) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.intersects(
        Ogre::Vector3(_vector.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.intersects(
        Ogre::Vector3(_vector.m_x, _vector.m_y, _vector.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::contains(const Zen::Math::Vector3& _vector) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.contains(
        Ogre::Vector3(_vector.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.contains(
        Ogre::Vector3(_vector.m_x, _vector.m_y, _vector.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::contains(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const
{

#ifndef ZEN_MATH_USE_DOUBLE

    return this->m_ogreAxisAlignedBox.contains(
        Ogre::AxisAlignedBox(Ogre::Vector3(_axisAlignedBoundingBox.getMinimum().m_array),
                             Ogre::Vector3(_axisAlignedBoundingBox.getMaximum().m_array)));

#else   // ZEN_MATH_USE_DOUBLE

    const Zen::Math::Vector3& minVector(_axisAlignedBoundingBox.getMinimum());
    const Zen::Math::Vector3& maxVector(_axisAlignedBoundingBox.getMaximum());
    return this->m_ogreAxisAlignedBox.contains(
        Ogre::AxisAlignedBox(Ogre::Vector3(minVector.m_x, minVector.m_y, minVector.m_z),
                             Ogre::Vector3(maxVector.m_x, maxVector.m_y, maxVector.m_z)));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::contains(const Zen::Engine::Rendering::I_BoundingSphere& _boundingSphere) const
{
    const Zen::Math::Vector3& center(_boundingSphere.getCenter());
    const Zen::Math::Real radius(_boundingSphere.getRadius());
    return this->m_ogreAxisAlignedBox.contains(
        Ogre::AxisAlignedBox(Ogre::Vector3(center.m_x - radius, 
                                           center.m_y - radius, 
                                           center.m_z - radius),
                             Ogre::Vector3(center.m_x + radius, 
                                           center.m_y + radius, 
                                           center.m_z + radius)));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
AxisAlignedBoundingBox::volume() const
{
    return this->m_ogreAxisAlignedBox.volume();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Rendering::I_AxisAlignedBoundingBox& 
AxisAlignedBoundingBox::operator=(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _rhs)
{
    const Ogre::Vector3& minimum(this->m_ogreAxisAlignedBox.getMinimum());
    const Ogre::Vector3& maximum(this->m_ogreAxisAlignedBox.getMaximum());
    return *(new AxisAlignedBoundingBox(minimum.x,
                                        minimum.y,
                                        minimum.z,
                                        maximum.x,
                                        maximum.y,
                                        maximum.z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3& 
AxisAlignedBoundingBox::getMinimum() const
{
    const Ogre::Vector3& minimum(this->m_ogreAxisAlignedBox.getMinimum());
    return *(new Zen::Math::Vector3(minimum.x,minimum.y,minimum.z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3& 
AxisAlignedBoundingBox::getMaximum() const
{
    const Ogre::Vector3& maximum(this->m_ogreAxisAlignedBox.getMaximum());
    return *(new Zen::Math::Vector3(maximum.x,maximum.y,maximum.z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMinimum(const Zen::Math::Vector3& _minimum)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.setMinimum(Ogre::Vector3(_minimum.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.setMinimum(Ogre::Vector3(_minimum.m_x, _minimum.m_y, _minimum.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMinimum(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z)
{
    this->m_ogreAxisAlignedBox.setMinimum(_x,_y,_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMinimumX(Zen::Math::Real _x)
{
    this->m_ogreAxisAlignedBox.setMinimumX(_x);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMinimumY(Zen::Math::Real _y)
{
    this->m_ogreAxisAlignedBox.setMinimumY(_y);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMinimumZ(Zen::Math::Real _z)
{
    this->m_ogreAxisAlignedBox.setMinimumZ(_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMaximum(const Zen::Math::Vector3& _maximum)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.setMaximum(Ogre::Vector3(_maximum.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.setMaximum(Ogre::Vector3(_maximum.m_x, _maximum.m_y, _maximum.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMaximum(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z)
{
    this->m_ogreAxisAlignedBox.setMaximum(_x,_y,_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMaximumX(Zen::Math::Real _x)
{
    this->m_ogreAxisAlignedBox.setMaximumX(_x);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMaximumY(Zen::Math::Real _y)
{
    this->m_ogreAxisAlignedBox.setMaximumY(_y);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setMaximumZ(Zen::Math::Real _z)
{
    this->m_ogreAxisAlignedBox.setMaximumZ(_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setExtents(const Zen::Math::Vector3& _minimum, const Zen::Math::Vector3& _maximum)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.setExtents(Ogre::Vector3(_minimum.m_array),
                                          Ogre::Vector3(_maximum.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.setExtents(Ogre::Vector3(_minimum.m_x, _minimum.m_y, _minimum.m_z),
                                          Ogre::Vector3(_maximum.m_x, _minimum.m_y, _minimum.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setExtents(Zen::Math::Real _minimumX, Zen::Math::Real _minimumY, Zen::Math::Real _minimumZ, Zen::Math::Real _maximumX, Zen::Math::Real _maximumY, Zen::Math::Real _maximumZ)
{
    this->m_ogreAxisAlignedBox.setExtents(_minimumX,
                                          _minimumY,
                                          _minimumZ,
                                          _maximumX,
                                          _maximumY,
                                          _maximumZ);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::vector<Zen::Math::Vector3> 
AxisAlignedBoundingBox::getAllCorners() const
{
    unsigned int i = 0;
    std::vector<Zen::Math::Vector3> corners(8);
    const Ogre::Vector3* ogreCorners(this->m_ogreAxisAlignedBox.getAllCorners());

    for( i = 0 ; i < corners.size() ; i++ )
    {
        corners[i] = Zen::Math::Vector3(ogreCorners[i].x, ogreCorners[i].y, ogreCorners[i].z);
    }

    return corners;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Vector3 
AxisAlignedBoundingBox::getCorner(corner_type _corner) const
{
    const Ogre::Vector3& corner(this->m_ogreAxisAlignedBox.getCorner(
        (Ogre::AxisAlignedBox::CornerEnum)_corner));
    return Zen::Math::Vector3(corner.x,corner.y,corner.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::merge(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.merge(
        Ogre::AxisAlignedBox(Ogre::Vector3(_axisAlignedBoundingBox.getMinimum().m_array),
                             Ogre::Vector3(_axisAlignedBoundingBox.getMaximum().m_array)));

#else   // ZEN_MATH_USE_DOUBLE

    const Zen::Math::Vector3& minVector(_axisAlignedBoundingBox.getMinimum());
    const Zen::Math::Vector3& maxVector(_axisAlignedBoundingBox.getMaximum());
    this->m_ogreAxisAlignedBox.merge(
        Ogre::AxisAlignedBox(Ogre::Vector3(minVector.m_x, minVector.m_y, minVector.m_z),
                             Ogre::Vector3(maxVector.m_x, maxVector.m_y, maxVector.m_z)));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::merge(const Zen::Math::Vector3& _point)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.merge(Ogre::Vector3(_point.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.merge(Ogre::Vector3(_point.m_x, _point.m_y, _point.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::transform(const Zen::Math::Matrix4& _matrix)
{
    this->m_ogreAxisAlignedBox.transform(Ogre::Matrix4(_matrix.getElement(0,0),
                                                       _matrix.getElement(0,1),
                                                       _matrix.getElement(0,2),
                                                       _matrix.getElement(0,3),
                                                       _matrix.getElement(1,0),
                                                       _matrix.getElement(1,1),
                                                       _matrix.getElement(1,2),
                                                       _matrix.getElement(1,3),
                                                       _matrix.getElement(2,0),
                                                       _matrix.getElement(2,1),
                                                       _matrix.getElement(2,2),
                                                       _matrix.getElement(2,3),
                                                       _matrix.getElement(3,0),
                                                       _matrix.getElement(3,1),
                                                       _matrix.getElement(3,2),
                                                       _matrix.getElement(3,3)));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::transformAffine(const Zen::Math::Matrix4& _matrix)
{
    this->m_ogreAxisAlignedBox.transformAffine(Ogre::Matrix4(_matrix.getElement(0,0),
                                                             _matrix.getElement(0,1),
                                                             _matrix.getElement(0,2),
                                                             _matrix.getElement(0,3),
                                                             _matrix.getElement(1,0),
                                                             _matrix.getElement(1,1),
                                                             _matrix.getElement(1,2),
                                                             _matrix.getElement(1,3),
                                                             _matrix.getElement(2,0),
                                                             _matrix.getElement(2,1),
                                                             _matrix.getElement(2,2),
                                                             _matrix.getElement(2,3),
                                                             _matrix.getElement(3,0),
                                                             _matrix.getElement(3,1),
                                                             _matrix.getElement(3,2),
                                                             _matrix.getElement(3,3)));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setNull()
{
    this->m_ogreAxisAlignedBox.setNull();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::isNull() const
{
    return this->m_ogreAxisAlignedBox.isNull();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::isFinite() const
{
    return this->m_ogreAxisAlignedBox.isFinite();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::setInfinite()
{
    this->m_ogreAxisAlignedBox.setInfinite();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::isInfinite() const
{
    return this->m_ogreAxisAlignedBox.isInfinite();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Engine::Rendering::I_AxisAlignedBoundingBox& 
AxisAlignedBoundingBox::intersection(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox)
{

#ifndef ZEN_MATH_USE_DOUBLE

    const Ogre::AxisAlignedBox& ogreBox(this->m_ogreAxisAlignedBox.intersection(
        Ogre::AxisAlignedBox(Ogre::Vector3(_axisAlignedBoundingBox.getMinimum().m_array),
                             Ogre::Vector3(_axisAlignedBoundingBox.getMaximum().m_array))));

#else   // ZEN_MATH_USE_DOUBLE

    const Zen::Math::Vector3& minVector(_axisAlignedBoundingBox.getMinimum());
    const Zen::Math::Vector3& maxVector(_axisAlignedBoundingBox.getMaximum());
    const Ogre::AxisAlignedBox& ogreBox(this->m_ogreAxisAlignedBox.intersection(
        Ogre::AxisAlignedBox(Ogre::Vector3(minVector.m_x, minVector.m_y, minVector.m_z),
                             Ogre::Vector3(maxVector.m_x, maxVector.m_y, maxVector.m_z))));

#endif  // ZEN_MATH_USE_DOUBLE

    const Ogre::Vector3& minimum(ogreBox.getMinimum());
    const Ogre::Vector3& maximum(ogreBox.getMaximum());

    return *(new AxisAlignedBoundingBox(Zen::Math::Vector3(minimum.x,minimum.y,minimum.x),
                                        Zen::Math::Vector3(maximum.x,maximum.y,maximum.z)));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
AxisAlignedBoundingBox::scale(const Zen::Math::Vector3& _scale)
{

#ifndef ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.scale(Ogre::Vector3(_scale.m_array));

#else   // ZEN_MATH_USE_DOUBLE

    this->m_ogreAxisAlignedBox.scale(Ogre::Vector3(_scale.m_x, _scale.m_y, _scale.m_z));

#endif  // ZEN_MATH_USE_DOUBLE

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Vector3 
AxisAlignedBoundingBox::getSize() const
{
    const Ogre::Vector3& size(this->m_ogreAxisAlignedBox.getSize());
    return Zen::Math::Vector3(size.x,size.y,size.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Vector3 
AxisAlignedBoundingBox::getHalfSize() const
{
    const Ogre::Vector3& halfSize(this->m_ogreAxisAlignedBox.getHalfSize());
    return Zen::Math::Vector3(halfSize.x,halfSize.y,halfSize.z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::operator==(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _rhs) const
{

    const Zen::Math::Vector3& minimum(_rhs.getMinimum());
    const Zen::Math::Vector3& maximum(_rhs.getMaximum());

#ifndef ZEN_MATH_USE_DOUBLE

    return ( this->m_ogreAxisAlignedBox == Ogre::AxisAlignedBox(Ogre::Vector3(minimum.m_array),
                                                                Ogre::Vector3(maximum.m_array)));

#else   // ZEN_MATH_USE_DOUBLE

    return ( this->m_ogreAxisAlignedBox == Ogre::AxisAlignedBox(Ogre::Vector3(minimum.m_x, minimum.m_y, minimum.m_z),
                                                                Ogre::Vector3(maximum.m_x, maximum.m_y, maximum.m_z)));

#endif  // ZEN_MATH_USE_DOUBLE
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
AxisAlignedBoundingBox::operator!=(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _rhs) const
{
    return !(*this == _rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
