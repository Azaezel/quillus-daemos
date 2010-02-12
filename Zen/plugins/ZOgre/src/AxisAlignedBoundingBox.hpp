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
#ifndef ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_HPP_INCLUDED
#define ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_HPP_INCLUDED

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Engine/Rendering/I_AxisAlignedBoundingBox.hpp>

#include <OgreAxisAlignedBox.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class AxisAlignedBoundingBox
:   public Zen::Engine::Rendering::I_AxisAlignedBoundingBox
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolume implementation
    /// @{
public:
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

    /// @name Zen::Engine::Rendering::I_AxisAlignedBoundingBox implementation
    /// @{
public:
    virtual Zen::Engine::Rendering::I_AxisAlignedBoundingBox& operator=(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _rhs);
    virtual const Zen::Math::Vector3& getMinimum() const;
    virtual const Zen::Math::Vector3& getMaximum() const;
    virtual void setMinimum(const Zen::Math::Vector3& _minimum);
    virtual void setMinimum(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z);
    virtual void setMinimumX(Zen::Math::Real _x);
    virtual void setMinimumY(Zen::Math::Real _y);
    virtual void setMinimumZ(Zen::Math::Real _z);
    virtual void setMaximum(const Zen::Math::Vector3& _maximum);
    virtual void setMaximum(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z);
    virtual void setMaximumX(Zen::Math::Real _x);
    virtual void setMaximumY(Zen::Math::Real _y);
    virtual void setMaximumZ(Zen::Math::Real _z);
    virtual void setExtents(const Zen::Math::Vector3& _minimum, const Zen::Math::Vector3& _maximum);
    virtual void setExtents(Zen::Math::Real _minimumX, Zen::Math::Real _minimumY, Zen::Math::Real _minimumZ, Zen::Math::Real _maximumX, Zen::Math::Real _maximumY, Zen::Math::Real _maximumZ);
    virtual const std::vector<Zen::Math::Vector3> getAllCorners() const;
    virtual Zen::Math::Vector3 getCorner(corner_type _corner) const;
    virtual void merge(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox);
    virtual void merge(const Zen::Math::Vector3& _point);
    virtual void transform(const Zen::Math::Matrix4& _matrix);
    virtual void transformAffine(const Zen::Math::Matrix4& _matrix);
    virtual void setNull();
    virtual bool isNull() const;
    virtual bool isFinite() const;
    virtual void setInfinite();
    virtual bool isInfinite() const;
    virtual Zen::Engine::Rendering::I_AxisAlignedBoundingBox& intersection(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox);
    virtual void scale(const Zen::Math::Vector3& _scale);
    virtual Zen::Math::Vector3 getSize() const;
    virtual Zen::Math::Vector3 getHalfSize() const;
    virtual bool operator==(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _rhs) const;
    virtual bool operator!=(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _rhs) const;
    /// @}

    /// @name 'Structors
    /// @{
             AxisAlignedBoundingBox();
             AxisAlignedBoundingBox(const Zen::Engine::Rendering::I_AxisAlignedBoundingBox& _axisAlignedBoundingBox);
             AxisAlignedBoundingBox(const Zen::Math::Vector3& _minimum, 
                                    const Zen::Math::Vector3& _maximum);
             AxisAlignedBoundingBox(const Zen::Math::Real _minX,
                                    const Zen::Math::Real _minY,
                                    const Zen::Math::Real _minZ,
                                    const Zen::Math::Real _maxX,
                                    const Zen::Math::Real _maxY,
                                    const Zen::Math::Real _maxZ);
    virtual ~AxisAlignedBoundingBox();
    /// @}

    /// @name Member variables
    /// @{
private:
    Ogre::AxisAlignedBox    m_ogreAxisAlignedBox;
    /// @}

};  // class I_AxisAlignedBoundingBox

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_AXIS_ALIGNED_BOUNDING_BOX_HPP_INCLUDED
