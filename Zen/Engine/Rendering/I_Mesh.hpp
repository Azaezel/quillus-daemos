//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_RENDERING_I_MESH_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_MESH_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <boost/noncopyable.hpp>
#include <boost/array.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Mesh interface.
class RENDERING_DLL_LINK I_Mesh
:   public Zen::Scripting::I_ScriptableType
,   public Memory::managed_self_ref<Zen::Engine::Rendering::I_Mesh>
,   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_Mesh>            pScriptObject_type;
    typedef Scripting::ObjectReference<I_Mesh>          ScriptObjectReference_type;
    typedef ScriptObjectReference_type                  ScriptWrapper_type;
    typedef ScriptWrapper_type*                         pScriptWrapper_type;

    typedef Zen::Memory::managed_ptr<I_Mesh>            pMesh_type;
    typedef Zen::Memory::managed_weak_ptr<I_Mesh>       wpMesh_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_Mesh interface
    /// @{
public:
    virtual void getMesh(std::vector<Zen::Math::Point3>& _vertices, std::vector<boost::array<int, 3> >& _indices) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_Mesh();
    virtual ~I_Mesh();
    /// @}

};  // interface I_Mesh

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory {
    // I_Mesh is managed by factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_Mesh> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_MESH_HPP_INCLUDED
