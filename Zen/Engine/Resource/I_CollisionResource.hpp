//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C)        2010 Matthew Alan Gray
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
#ifndef ZEN_ENGINE_RESOURCE_MANAGER_I_COLLISION_RESOURCE_HPP_INCLUDED
#define ZEN_ENGINE_RESOURCE_MANAGER_I_COLLISION_RESOURCE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Engine/Resource/I_Resource.hpp>
#include <Zen/Engine/Physics/I_CollisionShape.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Resource {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class RESOURCE_DLL_LINK I_CollisionResource
:   public Resource::I_Resource
,   public Physics::I_CollisionShape
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_CollisionResource>      pScriptObject_type;
    typedef Scripting::ObjectReference<I_CollisionResource>    ScriptObjectReference_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "CollisionResource"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_CollisionResource interface
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CollisionResource();
    virtual ~I_CollisionResource();
private:
    I_CollisionResource(const I_CollisionResource& _copy) {}
    /// @}

};  // interface I_CollisionResource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
namespace Memory 
{
    /// I_CollisionResource is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Resource::I_CollisionResource> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RESOURCE_MANAGER_I_COLLISION_RESOURCE_HPP_INCLUDED
