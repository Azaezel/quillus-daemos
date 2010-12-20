//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ENGINE_RENDERING_I_RENDERABLE_RESOURCE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_RENDERABLE_RESOURCE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Engine/Resource/I_Resource.hpp>
#include <Zen/Engine/Rendering/I_Renderable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_AnimationState;
class I_Mesh;

class RENDERING_DLL_LINK I_RenderableResource
:   public Resource::I_Resource
,   public I_Renderable
{
    /// @name Forward declarations
    /// @{
public:
    struct I_AnimationStateVisitor;
    /// @}
    
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_RenderableResource>      pScriptObject_type;
    typedef Scripting::ObjectReference<I_RenderableResource>    ScriptObjectReference_type;
    typedef Zen::Memory::managed_ptr<Zen::Engine::Rendering::I_AnimationState>      pAnimationState_type;
    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Rendering::I_AnimationState> wpAnimationState_type;
    typedef Zen::Memory::managed_ptr<I_Mesh>                    pMesh_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "RenderableResource"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name I_RenderableResource interface
    /// @{
public:
    /// Gets the current animation state
    virtual pAnimationState_type getAnimationState(const std::string& _name) const = 0;

    /// Provides access to all the animation states for this resource
    /// via a visitor.
    virtual void getAnimationStates(I_AnimationStateVisitor& _visitor) const = 0;

    /// Get the mesh object.
    virtual pMesh_type getMesh() = 0;
    /// @}

    /// @name Inner classes
    /// @{
public:
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    struct I_AnimationStateVisitor
    {
        /// @name I_AnimationStateVisitor interface
        /// @{
        virtual void begin() = 0;

        virtual void visit(I_AnimationState& _animationState) = 0;

        virtual void end() = 0;
        /// @}
    };  /// interface I_AnimationStateVisitor
    //-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_RenderableResource();
    virtual ~I_RenderableResource();
private:
    I_RenderableResource(const I_RenderableResource& _copy) {}
    /// @}

};  // interface I_RenderableResource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory 
{
    /// I_RenderableResource is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_RenderableResource> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_RENDERABLE_RESOURCE_HPP_INCLUDED
