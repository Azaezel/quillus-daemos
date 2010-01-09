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
#ifndef ZEN_ENGINE_RESOURCE_MANAGER_I_RESOURCE_HPP_INCLUDED
#define ZEN_ENGINE_RESOURCE_MANAGER_I_RESOURCE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Resource {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RESOURCE_DLL_LINK I_Resource
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    enum ResourceTypes
    {
        THREESPACE_RESOURCE,
        SOUND_BUFFER_RESOURCE,
        COMPOUND_RESOURCE
    };

    typedef Zen::Memory::managed_ptr<I_Resource>    pScriptObject_type;
    typedef Scripting::ObjectReference<I_Resource>  ScriptObjectReference_type;

    typedef Memory::managed_ptr<I_Resource>         pResource_type;
    typedef Memory::managed_weak_ptr<I_Resource>    wpResource_type;
    typedef Event::Event<wpResource_type>           resourceEvent_type;
    /// @}

    /// @name I_Resource interface
    /// @{
public:
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "Resource"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    resourceEvent_type      onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Resource();
    virtual ~I_Resource();
    /// @}

};  // interface I_Resource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
namespace Memory {
    // I_Resource is managed by I_ResourceFactory
    template<>
    struct is_managed_by_factory<Engine::Resource::I_Resource> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RESOURCE_MANAGER_I_RESOURCE_HPP_INCLUDED
