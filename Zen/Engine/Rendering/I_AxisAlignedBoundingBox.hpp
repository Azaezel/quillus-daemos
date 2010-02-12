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
#ifndef ZEN_ENGINE_RENDERING_I_AXIS_ALIGNED_BOUNDING_BOX_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_AXIS_ALIGNED_BOUNDING_BOX_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Engine/Rendering/I_BoundingVolume.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine{
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_BoundingSphere;
class I_Plane;

class RENDERING_DLL_LINK I_AxisAlignedBoundingBox
:   public I_BoundingVolume
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_AxisAlignedBoundingBox>          pAxisAlignedBoundingBox_type;
    typedef Zen::Memory::managed_weak_ptr<I_AxisAlignedBoundingBox>     wpAxisAlignedBoundingBox_type;
    typedef Zen::Event::Event<wpAxisAlignedBoundingBox_type>            axisAlignedBoundingBoxEvent_type;

    enum corner_type {
        FAR_LEFT_BOTTOM = 0,
        FAR_LEFT_TOP = 1,
        FAR_RIGHT_TOP = 2,
        FAR_RIGHT_BOTTOM = 3,
        NEAR_RIGHT_BOTTOM = 4,
        NEAR_LEFT_BOTTOM = 5,
        NEAR_LEFT_TOP = 6,
        NEAR_RIGHT_TOP = 7
    };
    /// @}

    /// @name Zen::Engine::Rendering::I_AxisAlignedBoundingBox
    /// @{
public:
    virtual I_AxisAlignedBoundingBox& operator=(const I_AxisAlignedBoundingBox& _rhs) = 0;
    virtual const Zen::Math::Vector3& getMinimum() const = 0;
    virtual const Zen::Math::Vector3& getMaximum() const = 0;
    virtual void setMinimum(const Zen::Math::Vector3& _minimum) = 0;
    virtual void setMinimum(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z) = 0;
    virtual void setMinimumX(Zen::Math::Real _x) = 0;
    virtual void setMinimumY(Zen::Math::Real _y) = 0;
    virtual void setMinimumZ(Zen::Math::Real _z) = 0;
    virtual void setMaximum(const Zen::Math::Vector3& _maximum) = 0;
    virtual void setMaximum(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z) = 0;
    virtual void setMaximumX(Zen::Math::Real _x) = 0;
    virtual void setMaximumY(Zen::Math::Real _y) = 0;
    virtual void setMaximumZ(Zen::Math::Real _z) = 0;
    virtual void setExtents(const Zen::Math::Vector3& _minimum, const Zen::Math::Vector3& _maximum) = 0;
    virtual void setExtents(Zen::Math::Real _minimumX, Zen::Math::Real _minimumY, Zen::Math::Real _minimumZ, Zen::Math::Real _maximumX, Zen::Math::Real _maximumY, Zen::Math::Real _maximumZ) = 0;
    virtual const std::vector<Zen::Math::Vector3> getAllCorners() const = 0;
    virtual Zen::Math::Vector3 getCorner(corner_type _corner) const = 0;
    virtual void merge(const I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) = 0;
    virtual void merge(const Zen::Math::Vector3& _point) = 0;
    virtual void transform(const Zen::Math::Matrix4& _matrix) = 0;
    virtual void transformAffine(const Zen::Math::Matrix4& _matrix) = 0;
    virtual void setNull() = 0;
    virtual bool isNull() const = 0;
    virtual bool isFinite() const = 0;
    virtual void setInfinite() = 0;
    virtual bool isInfinite() const = 0;
    virtual I_AxisAlignedBoundingBox& intersection(const I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) = 0;
    virtual void scale(const Zen::Math::Vector3& _scale) = 0;
    virtual Zen::Math::Vector3 getSize() const = 0;
    virtual Zen::Math::Vector3 getHalfSize() const = 0;
    virtual bool operator==(const I_AxisAlignedBoundingBox& _rhs) const = 0;
    virtual bool operator!=(const I_AxisAlignedBoundingBox& _rhs) const = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static I_AxisAlignedBoundingBox& Null();
    static I_AxisAlignedBoundingBox& Infinite();
    /// @}

    /// @name Events
    /// @{
public:
    axisAlignedBoundingBoxEvent_type    onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_AxisAlignedBoundingBox();
    virtual ~I_AxisAlignedBoundingBox();
    /// @}

};  // interface I_AxisAlignedBoundingBox

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory {
    // I_AxisAlignedBoundingBox is managed by a factory method
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_AxisAlignedBoundingBox> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_AXIS_ALIGNED_BOUNDING_BOX_HPP_INCLUDED
