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
#ifndef ZEN_ENGINE_WORLD_I_SKY_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_SKY_HPP_INCLUDED

#include "Configuration.hpp"

//#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Resource {
        class I_Resource;
    }
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WORLD_DLL_LINK I_Sky
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Sky>            pSky_type;
    typedef Memory::managed_weak_ptr<I_Sky>       wpSky_type;
    typedef Event::Event<wpSky_type>              SkyEvent_type;
    typedef Memory::managed_ptr<Resource::I_Resource>           pResource_type;
    typedef Memory::managed_weak_ptr<Resource::I_Resource>      wpResource_type;
    /// @}

    /// @name I_Sky interface
    /// @{
public:
    virtual void setResource(pResource_type _resource) = 0;
    virtual pResource_type getResource(void) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    SkyEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Sky();
    virtual ~I_Sky();
    /// @}

};  // interface I_Sky

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory 
{
    /// I_Sky is managed by a I_SkyService
    template<>
    struct is_managed_by_factory<Zen::Engine::World::I_Sky> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_SKY_HPP_INCLUDED
