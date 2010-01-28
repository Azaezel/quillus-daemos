//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_ENGINE_RENDERING_I_SCENE_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_SCENE_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
    namespace Camera {
        class I_Camera;
    }   // namespace Camera
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Light;
class I_SceneNode;
class I_SceneServiceFactory;
class I_ParticleSystem;

class RENDERING_DLL_LINK I_SceneService
:   public Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef std::string                                 index_type;
    typedef I_SceneServiceFactory                       factory_type;

    typedef Zen::Memory::managed_ptr<I_SceneService>    pScriptObject_type;
    typedef Scripting::ObjectReference<I_SceneService>  ScriptObjectReference_type;

    typedef Memory::managed_weak_ptr<I_SceneService>    wpSceneService_type;

    typedef Memory::managed_ptr<I_SceneNode>            pSceneNode_type;
    typedef Memory::managed_weak_ptr<I_SceneNode>       wpSceneNode_type;

    typedef Memory::managed_ptr<I_ParticleSystem>       pParticleSystem_type;
    typedef Memory::managed_weak_ptr<I_ParticleSystem>  wpParticleSystem_type;

    typedef Event::Event<wpSceneService_type>           SceneServiceEvent_type;

    typedef Zen::Memory::managed_ptr<I_Light>           pLight_type;
    /// @}

    /// @name I_SceneService interface
    /// @{
public:
    /// Create a light of a particular type and name it.
    virtual pLight_type createLight(const std::string& _type, const std::string& _name) = 0;

    /// Create a child node
    virtual pSceneNode_type createChildNode(const std::string& _name) = 0;

    /// Create a particle system
    virtual pParticleSystem_type createParticleSystem(const std::string& _name, const std::string& _resource) = 0;

    /// set the ambient light for the scene
    virtual void setAmbientLight(Math::Real _red = 0.75f, Math::Real _green = 0.75f, Math::Real _blue = 0.75f, Math::Real _alpha = 1.0f) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "SceneService"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
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
    SceneServiceEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SceneService();
    virtual ~I_SceneService();
    /// @}

};	// interface I_SceneService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory 
{
    /// I_SceneService is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_SceneService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~


#endif // ZEN_ENGINE_RENDERING_I_SCENE_SERVICE_HPP_INCLUDED
