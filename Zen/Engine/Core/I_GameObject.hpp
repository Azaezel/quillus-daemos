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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_CORE_I_GAME_OBJECT_HPP_INCLUDED
#define ZEN_ENGINE_CORE_I_GAME_OBJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ObjectReference;
    }   // namespace Scripting
namespace Engine {
    namespace Rendering
    {
        class I_SceneNode;
        class I_RenderableResource;
    }   // namespace Rendering
    namespace Physics
    {
        class I_PhysicsZone;
        class I_PhysicsActor;
        class I_PhysicsMaterial;
        class I_PhysicsJoint;
    }   // namespace Physics
    namespace Resource
    {
        class I_Resource;
    }   // namespace Resource
    namespace MoveManager
    {
        class I_Controllable;
    }   // namespace MoveManager
namespace Core {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_BaseGameObject;

class GAMECORE_DLL_LINK I_GameObjectData
{
public:
             I_GameObjectData();
    virtual ~I_GameObjectData();
};

class GAMECORE_DLL_LINK I_GameObject
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Rendering::I_SceneNode>            pSceneNode_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsZone>            pPhysicsZone_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsActor>           pPhysicsActor_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsMaterial>        pPhysicsMaterial_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsJoint>           pPhysicsJoint_type;
    typedef Zen::Memory::managed_ptr<Resource::I_Resource>              pResource_type;
    typedef Zen::Memory::managed_ptr<Rendering::I_RenderableResource>   pRenderableResource_type;
    typedef Zen::Memory::managed_ptr<MoveManager::I_Controllable>       pControllable_type;
    /// @}

    /// @name I_BaseGameObject interface
    /// @{
public:
    virtual I_BaseGameObject& base() = 0;
    virtual I_GameObjectData* getData() = 0;
    virtual void setData(I_GameObjectData* _pData) = 0;
    /// @}
};

/// @brief Base Game Object interface.
/// This is the game engine's implementation of a game object.
/// The user should implement I_GameObject and use I_BaseGameObject
/// to tie all of the game object components together.
class GAMECORE_DLL_LINK I_BaseGameObject
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Rendering::I_SceneNode>            pSceneNode_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsZone>            pPhysicsZone_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsActor>           pPhysicsActor_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsMaterial>        pPhysicsMaterial_type;
    typedef Zen::Memory::managed_ptr<Physics::I_PhysicsJoint>           pPhysicsJoint_type;
    typedef Zen::Memory::managed_ptr<Resource::I_Resource>              pResource_type;
    typedef Zen::Memory::managed_ptr<Rendering::I_RenderableResource>   pRenderableResource_type;
    typedef Zen::Memory::managed_ptr<MoveManager::I_Controllable>       pControllable_type;

    typedef I_BaseGameObject*                                               pScriptObject_type;
    typedef Scripting::ObjectReference<I_BaseGameObject>                    ScriptObjectReference_type;
    /// @}

    /// @name I_BaseGameObject interface
    /// @{
public:
    virtual const std::string& getName() const = 0;

    /// Set the game object for this base game object
    virtual void setGameObject(I_GameObject* _pGameObject) = 0;

    /// Get the game object for this base game object.
    virtual I_GameObject* getGameObject() = 0;

    /// Set the position of this object
    /// @param _overridePhysics - true if this position is overriding the current position held
    ///             for this object by the physics engine.  Generally this is only called with
    ///             the value of true passed during the initial object creation, but can be
    ///             true when an object is moved by something other than the physics engine.
    virtual void setPosition(const Math::Point3& _position, bool _overridePhysics = false) = 0;

    /// Set the position of this object
    virtual void setPosition(Math::Real _x, Math::Real _y, Math::Real _z, bool _overridePhysics = false) = 0;

    /// Get the position of this object
    virtual void getPosition(Math::Point3& _position) = 0;

    /// Set the orientation of this object
    virtual void setOrientation(const Math::Quaternion4& _orientation, bool _overridePhysics = false) = 0;

    /// Set the orientation of this object
    virtual void setOrientation(Math::Real _w, Math::Real _x, Math::Real _y, Math::Real _z, bool _overridePhysics = false) = 0;
    virtual void setOrientation(const Math::Degree& _angle, const Math::Vector3& _axis, bool _overridePhysics = false) = 0;

    virtual void setScale(Math::Real _x, Math::Real _y, Math::Real _z) = 0;

    /// Get the scene node to which this object is attached.
    /// The returned scene node might be invalid if the game object is not attached
    /// to a scene node.
    virtual pSceneNode_type getSceneNode() = 0;

    /// Attach this game object to a scene node.  If the game object was
    /// previously attached to a scene node, it will be detached from
    /// that scene node first.
    virtual void attachToSceneNode(pSceneNode_type _pSceneNode) = 0;

    /// Attach a physics actor to this game object
    /// A game object can only be attached to one physics actor.  If
    /// a game object is already attached to a physics actor, this 
    /// method will unattach it from that actor and attach it the one
    /// specified in this call.
    /// @note When this method is called, the implementation must call
    ///         _pPhysicsActor->setGameObject(*this); 
    /// @see Physics::I_PhysicsActor
    virtual void attachPhysicsActor(pPhysicsActor_type _pPhysicsActor) = 0;

    /// Get the physics shape to which this object is attached
    virtual pPhysicsActor_type getPhysicsActor() = 0;

    /// Attach a controllable instance to this game object
    /// A game object can only be attached to one controllable instance. If
    /// a game object is already attached to a controllable instance, this
    /// method will unattach it from that instance and attach it to the one
    /// specified in this call.
    /// @note When this method is called, the implementation must call
    ///         _pControllable->setGameObject(this);
    /// @see MoveManager::I_Controllable
    //virtual void attachControllable(pControllable_type _pControllable) = 0;

    /// Get the controllable instance to which this object is attached
    //virtual pControllable_type getControllable() = 0;

    virtual void setBehaviors(I_BehaviorService::pGameObjectBehaviors_type _pBehaviors) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BaseGameObject();
    virtual ~I_BaseGameObject();
    /// @}

};  // interface I_BaseGameObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Core
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CORE_I_GAME_OBJECT_HPP_INCLUDED
