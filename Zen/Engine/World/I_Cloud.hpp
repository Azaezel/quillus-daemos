//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_WORLD_I_CLOUD_HPP_INCLUDED
#define ZEN_ENGINE_WORLD_I_CLOUD_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Vector3.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace World {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_CloudParticle;

class WORLD_DLL_LINK I_Cloud
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Cloud>           pCloud_type;
    typedef Zen::Memory::managed_weak_ptr<I_Cloud>      wpCloud_type;
    typedef Zen::Event::Event<wpCloud_type>             cloudEvent_type;

    typedef Zen::Memory::managed_ptr<I_CloudParticle>   pCloudParticle_type;
    /// @}

    /// @name Zen::Engine::I_Cloud interface
    /// @{
public:
    virtual void addParticle(pCloudParticle_type& _cloudParticle) = 0;
    virtual void dropParticle(pCloudParticle_type& _cloudParticle) = 0;

    virtual void translate(const Zen::Math::Vector3& _translation) = 0;
    virtual void rotate(const Zen::Math::Vector3& _rotation) = 0;
    virtual void scale(const Zen::Math::Vector3& _scale) = 0;

    virtual void display() = 0;

    virtual void illuminate() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    cloudEvent_type     onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Cloud();
    virtual ~I_Cloud();
    /// @}

};  // I_Cloud interface

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace World
}   // namespace Engine
namespace Memory {
    // I_Cloud is managed by I_CloudService
    template<>
    struct is_managed_by_factory<Zen::Engine::World::I_Cloud> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WORLD_I_CLOUD_HPP_INCLUDED