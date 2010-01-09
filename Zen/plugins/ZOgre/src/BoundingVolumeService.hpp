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
#ifndef ZEN_ZOGRE_BOUNDING_VOLUME_SERVICE_HPP_INCLUDED
#define ZEN_ZOGRE_BOUNDING_VOLUME_SERVICE_HPP_INCLUDED

#include <Zen/Engine/Rendering/I_BoundingVolumeService.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include "Ogre.hpp"

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class BoundingVolumeService
:   public Zen::Engine::Rendering::I_BoundingVolumeService
,   public Memory::managed_self_ref<Zen::Engine::Rendering::I_BoundingVolumeService>
{
    /// @name Types
    /// @{
public:
    typedef std::map< std::string, std::string >        config_type;
    /// @}

    /// @name Zen::Engine::Rendering::I_BoundingVolumeService implementation
    /// @{
public:
#if 0 // Temporarily disabling for now
    virtual pBoundingVolumeTree_type getBoundingVolumeTree(const std::string& _type);
    virtual pBoundingSphere_type createBoundingSphere(const Zen::Math::Vector3& _center, const Zen::Math::Real _radius);
    virtual pAxisAlignedBoundingBox_type createAxisAlignedBoundingBox(const Zen::Math::Vector3& _minimum, const Zen::Math::Vector3& _maximum);
    virtual pPlane_type createPlane(const Zen::Math::Vector3& _normal, const Zen::Math::Real _distance);
#endif // 0
    /// @}

    /// @name Zen::ZOgre::BoundingVolumeService implementation
    /// @{
private:
    /// @}

    /// @name 'Structors
    /// @{
public:
             BoundingVolumeService(const std::string& _type, config_type& _config);
    virtual ~BoundingVolumeService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// @}

};  // class BoundingVolumeService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_BOUNDING_VOLUME_SERVICE_HPP_INCLUDED
