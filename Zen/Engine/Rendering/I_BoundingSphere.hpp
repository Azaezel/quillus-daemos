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
#ifndef ZEN_ENGINE_RENDERING_I_BOUNDING_SPHERE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_BOUNDING_SPHERE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Engine/Rendering/I_BoundingVolume.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine{
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_AxisAlignedBoundingBox;
class I_Plane;

class RENDERING_DLL_LINK I_BoundingSphere
:   public I_BoundingVolume
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_BoundingSphere>          pBoundingSphere_type;
    typedef Zen::Memory::managed_weak_ptr<I_BoundingSphere>     wpBoundingSphere_type;
    typedef Zen::Event::Event<wpBoundingSphere_type>            boundingSphereEvent_type;
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingSphere
    /// @{
public:
    virtual Zen::Math::Real getRadius() const = 0;
    virtual void setRadius(const Zen::Math::Real _radius) = 0;
    virtual bool operator==(const I_BoundingSphere& _rhs) const = 0;
    virtual bool operator!=(const I_BoundingSphere& _rhs) const = 0;
    /// @}

    /// @name Events
    /// @{
public:
    boundingSphereEvent_type    onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BoundingSphere();
    virtual ~I_BoundingSphere();
    /// @}

};  // interface I_BoundingSphere

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory {
    // I_BoundingSphere is managed by a factory method
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_BoundingSphere> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_BOUNDING_SPHERE_HPP_INCLUDED
