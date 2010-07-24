//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Walt Collins
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
#ifndef ZEN_ZSKY_SKY_HPP_INCLUDED
#define ZEN_ZSKY_SKY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/World/I_Sky.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZSky {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Sky
:   public Engine::World::I_Sky
,   public Memory::managed_self_ref<Zen::Engine::World::I_Sky>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptModule>    pScriptModule_type;

    typedef Memory::managed_weak_ptr<Engine::World::I_Sky>           wpSky_type;
    typedef Memory::managed_ptr<Engine::Resource::I_Resource>            pResource_type;
    typedef Memory::managed_weak_ptr<Engine::Resource::I_Resource>       wpResource_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_Sky implementation
    /// @{
public:
    virtual void setResource(pResource_type _resource);
    virtual pResource_type getResource(void);
    /// @}

    /// @name Sky implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
public:
             Sky();
    virtual ~Sky();
    /// @}

    /// @name Member Variables
    /// @{
private:
    pScriptModule_type              m_pScriptModule;
    ScriptObjectReference_type*     m_pScriptObject;

    pResource_type m_pResource;
    /// @}

};  // class Sky

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZSky
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZSKY_SKY_HPP_INCLUDED
