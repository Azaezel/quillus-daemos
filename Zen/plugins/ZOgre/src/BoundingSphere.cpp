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

#include "BoundingSphere.hpp"

#include <Zen/Engine/Rendering/I_AxisAlignedBoundingBox.hpp>
#include <Zen/Engine/Rendering/I_Plane.hpp>

#include <OgreAxisAlignedBox.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BoundingSphere::BoundingSphere()
:   m_ogreSphere(Ogre::Vector3(0.0f, 0.0f, 0.0f), 0.0f)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BoundingSphere::BoundingSphere(const Zen::Math::Real _radius)
:   m_ogreSphere(Ogre::Vector3(0.0f, 0.0f, 0.0f), _radius)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BoundingSphere::BoundingSphere(const Zen::Math::Real _radius, 
                               const Zen::Math::Vector3& _center)
:   m_ogreSphere(Ogre::Vector3(_center.m_x, _center.m_y, _center.m_z), _radius)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BoundingSphere::BoundingSphere(const Zen::Math::Real _radius, 
                               const Zen::Math::Real _centerX, 
                               const Zen::Math::Real _centerY, 
                               const Zen::Math::Real _centerZ)
:   m_ogreSphere(Ogre::Vector3(_centerX, _centerY, _centerZ), _radius)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BoundingSphere::~BoundingSphere()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3& 
BoundingSphere::getCenter() const
{
    const Ogre::Vector3& vector3(this->m_ogreSphere.getCenter());
    return *(new Zen::Math::Vector3(vector3.x,vector3.y,vector3.z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BoundingSphere::setCenter(const Zen::Math::Vector3& _center)
{
    this->m_ogreSphere.setCenter(Ogre::Vector3(_center.m_x,
                                               _center.m_y, 
                                               _center.m_z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::intersects(const I_BoundingSphere& _boundingSphere) const
{
    return this->m_ogreSphere.intersects(Ogre::Sphere(this->m_ogreSphere.getCenter(),
                                                      this->m_ogreSphere.getRadius()));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::intersects(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const
{
    const Zen::Math::Vector3& minimum(_axisAlignedBoundingBox.getMinimum());
    const Zen::Math::Vector3& maximum(_axisAlignedBoundingBox.getMaximum());
    return this->m_ogreSphere.intersects(Ogre::AxisAlignedBox(
        Ogre::Vector3(minimum.m_x, minimum.m_y, minimum.m_z),
        Ogre::Vector3(maximum.m_x, maximum.m_y, maximum.m_z)));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::intersects(const Zen::Engine::Rendering::I_Plane& _plane) const
{
    const Zen::Math::Vector3& normal(_plane.getNormal());
    return this->m_ogreSphere.intersects(Ogre::Plane(
        Ogre::Vector3(normal.m_x, normal.m_y, normal.m_z),
        _plane.getDistanceFromOrigin()));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::intersects(const Zen::Math::Vector3& _vector) const
{
    return this->m_ogreSphere.intersects(Ogre::Vector3(_vector.m_x, _vector.m_y, _vector.m_z));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::contains(const Zen::Math::Vector3& _vector) const
{
    const Ogre::Vector3 vector(_vector.m_x, _vector.m_y, _vector.m_z);
    const Zen::Math::Real distance = vector.distance(this->m_ogreSphere.getCenter());
    return distance <= this->m_ogreSphere.getRadius();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::contains(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const
{
    const Zen::Math::Vector3& boxCenter(_axisAlignedBoundingBox.getCenter());
    const Zen::Math::Vector3& boxHalfVector(boxCenter - _axisAlignedBoundingBox.getMaximum());
    const Ogre::Vector3 center(boxCenter.m_x, boxCenter.m_y, boxCenter.m_z);
    const Ogre::Vector3 halfVector(boxHalfVector.m_x, boxHalfVector.m_y, boxHalfVector.m_z);
    const Zen::Math::Real centerDistance = center.distance(this->m_ogreSphere.getCenter());
    return centerDistance + halfVector.length() <= this->m_ogreSphere.getRadius();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::contains(const Zen::Engine::Rendering::I_BoundingSphere& _boundingSphere) const
{
    const Zen::Math::Vector3& sphereCenter(_boundingSphere.getCenter());
    const Ogre::Vector3 center(sphereCenter.m_x, sphereCenter.m_y, sphereCenter.m_z);
    const Zen::Math::Real centerDistance = center.distance(this->m_ogreSphere.getCenter());
    return centerDistance + _boundingSphere.getRadius() <= this->m_ogreSphere.getRadius();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
BoundingSphere::volume() const
{
    return 4*Zen::Math::Math::PI*pow(this->m_ogreSphere.getRadius(),3)/3.0f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real 
BoundingSphere::getRadius() const
{
    return this->m_ogreSphere.getRadius();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
BoundingSphere::setRadius(const Zen::Math::Real _radius)
{
    this->m_ogreSphere.setRadius(_radius);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::operator==(const Zen::Engine::Rendering::I_BoundingSphere& _rhs) const
{
    const Zen::Math::Vector3& rhsCenter(_rhs.getCenter());
    const Ogre::Vector3& ogreCenter(this->m_ogreSphere.getCenter());
    return ( this->m_ogreSphere.getRadius() == _rhs.getRadius() &&
             ogreCenter.x == rhsCenter.m_x &&
             ogreCenter.y == rhsCenter.m_y &&
             ogreCenter.z == rhsCenter.m_z );
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
BoundingSphere::operator!=(const Zen::Engine::Rendering::I_BoundingSphere& _rhs) const
{
    return !(*this == _rhs);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
