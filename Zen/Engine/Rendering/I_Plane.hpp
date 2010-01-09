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
#ifndef ZEN_ENGINE_RENDERING_I_PLANE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_PLANE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Math.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine{
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_AxisAlignedBoundingBox;
class I_BoundingSphere;

class RENDERING_DLL_LINK I_Plane
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Plane>          pPlane_type;
    typedef Zen::Memory::managed_weak_ptr<I_Plane>     wpPlane_type;
    typedef Zen::Event::Event<wpPlane_type>            planeEvent_type;

    enum side_type
    {
        NO_SIDE,
        POSITIVE_SIDE,
        NEGATIVE_SIDE,
        BOTH_SIDE
    };
    /// @}

    /// @name Zen::Engine::Rendering::I_Plane interface
    /// @{
public:
    virtual side_type getSide(const Zen::Math::Vector3& _vector) const = 0;
    virtual side_type getSide(const I_AxisAlignedBoundingBox& _axisAlignedBoundingBox) const = 0;
    virtual side_type getSide(const I_BoundingSphere& _boundingSphere) const = 0;
    virtual Zen::Math::Real getDistance(const Zen::Math::Vector3& _vector) const = 0;
    virtual void redefine(const Zen::Math::Vector3& _point1, 
                          const Zen::Math::Vector3& _point2,
                          const Zen::Math::Vector3& _point3) = 0;
    virtual void redefine(const Zen::Math::Vector3& _normal,
                          const Zen::Math::Vector3& _point) = 0;
    virtual const Zen::Math::Vector3& projectVector(const Zen::Math::Vector3& _vector) const = 0;
    virtual void normalize() = 0;
    virtual bool operator==(const I_Plane& _rhs) const = 0;
    virtual bool operator!=(const I_Plane& _rhs) const = 0;
    virtual const Zen::Math::Vector3& getNormal() const = 0;
    virtual Zen::Math::Real getDistanceFromOrigin() const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    planeEvent_type    onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Plane();
    virtual ~I_Plane();
    /// @}

};  // interface I_Plane

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory {
    // I_Plane is managed by a factory method
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_Plane> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_PLANE_HPP_INCLUDED
