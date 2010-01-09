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
#ifndef ZEN_ENGINE_RENDERING_I_BOUNDING_VOLUME_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_BOUNDING_VOLUME_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Vector3.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Plane;
class I_BoundingSphere;
class I_AxisAlignedBoundingBox;
class I_BoundingVolumeTree;

class RENDERING_DLL_LINK I_BoundingVolumeService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                         index_type;

    typedef Zen::Memory::managed_ptr<I_BoundingVolumeService>       pBoundingVolumeService_type;
    typedef Zen::Memory::managed_weak_ptr<I_BoundingVolumeService>  wpBoundingVolumeService_type;
    typedef Zen::Event::Event<wpBoundingVolumeService_type>         boundingVolumeServiceEvent_type;

    typedef Zen::Memory::managed_ptr<I_Plane>                       pPlane_type;
    typedef Zen::Memory::managed_ptr<I_BoundingSphere>              pBoundingSphere_type;
    typedef Zen::Memory::managed_ptr<I_AxisAlignedBoundingBox>      pAxisAlignedBoundingBox_type;
    typedef Zen::Memory::managed_ptr<I_BoundingVolumeTree>          pBoundingVolumeTree_type;
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolumeService interface
    /// @{
public:
#if 0   // Temporarily disabling for now
    virtual pBoundingVolumeTree_type getBoundingVolumeTree(const std::string& _type) = 0;
    virtual pBoundingSphere_type createBoundingSphere(const Zen::Math::Vector3& _center, const Zen::Math::Real _radius) = 0;
    virtual pAxisAlignedBoundingBox_type createAxisAlignedBoundingBox(const Zen::Math::Vector3& _minimum, const Zen::Math::Vector3& _maximum) = 0;
    virtual pPlane_type createPlane(const Zen::Math::Vector3& _normal, const Zen::Math::Real _distance) = 0;
#endif // 0
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    /// Fired immediately before this object is destroyed.
    /// The payload is about to be destroyed, so do not keep a reference of it around.
    boundingVolumeServiceEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BoundingVolumeService();
    virtual ~I_BoundingVolumeService();
    /// @}

};	// interface I_SceneService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory {
    /// I_BoundingVolumeService is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_BoundingVolumeService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_BOUNDING_VOLUME_SERVICE_HPP_INCLUDED
