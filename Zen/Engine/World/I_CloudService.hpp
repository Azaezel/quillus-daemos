//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_WORLD_I_CLOUD_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_CLOUD_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Resource {
        class I_Resource;
    }   // namespace Resource
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
   
class I_Cloud;
class I_CloudParticle;

class WORLD_DLL_LINK I_CloudService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                     index_type;
    typedef Memory::managed_ptr<I_CloudService>             pCloudService_type;
    typedef Memory::managed_weak_ptr<I_CloudService>        wpCloudService_type;
    typedef Event::Event<wpCloudService_type>               cloudServiceEvent_type;

    typedef Zen::Memory::managed_ptr<I_Cloud>               pCloud_type;
    typedef Zen::Memory::managed_ptr<I_CloudParticle>       pCloudParticle_type;
    typedef Zen::Memory::managed_ptr<Resource::I_Resource>  pResource_type;
    /// @}

    /// @name I_CloudService interface
    /// @{
public:
    virtual pCloud_type getCloud(const pResource_type& _pResource) = 0;
    virtual pCloudParticle_type getCloudParticle() = 0;
    /// @}

    /// @name Event handlers
    /// @{
protected:
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
    cloudServiceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CloudService();
    virtual ~I_CloudService();
    /// @}

};  // interface I_CloudService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory {
    /// I_CloudService is managed by I_CloudServiceFactory
    template<>
    struct is_managed_by_factory<Engine::World::I_CloudService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_CLOUD_SERVICE_HPP_INCLUDED
