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
#ifndef ZEN_ENGINE_RENDERING_I_SCENE_NODE_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_I_SCENE_NODE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Math/Math.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Event/Event.hpp>

#include <boost/any.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Math {
        class Quaternion4;
        class Matrix4;
        class Vector3;
        class Point3;
    }   // namespace Math;
namespace Engine {
    namespace Rendering {
        class I_RenderableResource;
    }   // namespace Resource
    namespace Physics {
        class I_PhysicsZone;
        class I_CollisionShape;
        class I_PhysicsMaterial;
        class I_PhysicsJoint;
    }   // namespace Physics
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_AttachableObject;

class RENDERING_DLL_LINK I_SceneNode
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_SceneNode>       pScriptObject_type;
    typedef Scripting::ObjectReference<I_SceneNode>     ScriptObjectReference_type;

    typedef Zen::Memory::managed_ptr<I_SceneNode>       pSceneNode_type;
    typedef Zen::Memory::managed_weak_ptr<I_SceneNode>  wpSceneNode_type;
    typedef Zen::Event::Event<wpSceneNode_type>         SceneNodeEvent_type;
    /// @}

    /// @name I_SceneNode interface
    /// @{
public:
#if 0 // deprecated
    /// Attach a resource (generally renderable) to this node
    virtual void attachResource(Rendering::I_RenderableResource& _resource) = 0;
#endif // deprecated

    /// Attach a physics actor to this node
    virtual void attachCollisionShape(Physics::I_CollisionShape& _shape) = 0;

    /// Set the position of this node.
    virtual void setPosition(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z) = 0;

    /// Get the real position of this node.
    virtual void getPosition(Zen::Math::Point3& _position) const = 0;

    /// Set the rotation of this node as a transformation matrix.
    virtual void setRotation(Math::Quaternion4 _quaternion) = 0;

    /// Apply a transformation matrix to this node.
    virtual void applyTransformation(Math::Matrix4 _transformationMatrix) = 0;

    /// Set the scale of this node, which effects any attached resources
    virtual void setScale(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z) = 0;

    /// Attach an object to this scene node.
    ///
    /// @param _name Name of the object to attach
    /// @param _object Object to attach to this scene node
    /// @param _translate Offset of the attached object from the parent I_SceneNode
    /// @param _rotate Rotation of the attached object from the parent I_SceneNode
    virtual void attachObject(I_AttachableObject& _object) = 0;

    /// Attach an object to this scene node with an offset.
    ///
    /// @param _name Name of the object to attach
    /// @param _object Object to attach to this scene node
    /// @param _translate Offset of the attached object from the parent I_SceneNode
    /// @param _rotate Rotation of the attached object from the parent I_SceneNode
    virtual void attachObject(const std::string& _name, I_AttachableObject& _object, Math::Vector3& _translate, Math::Quaternion4& _rotate) = 0;

    /// Get the number of objects attached to this node.
    virtual int getNumAttachedObjects() const = 0;

    /// Get an attached object by index.
    virtual I_AttachableObject* getAttachedObject(int _index) const = 0;

    /// Detach an object by index.
    virtual void detachObject(int _index) = 0;

    /// Detach all objects from this scene node.
    virtual void detachAllObjects() = 0;

    /// Set the user data.
    ///
    /// Normally this is the I_BaseGameObject
    virtual void setUserData(boost::any _anyUserData) = 0;

    /// Get the user data.
    virtual const boost::any& getUserData() const = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "SceneNode"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    /// Fired immediately before this object is destroyed.
    /// The payload is about to be destroyed, so do not keep a reference of it around.
    SceneNodeEvent_type onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SceneNode();
    virtual ~I_SceneNode();
    /// @}

};	// interface I_SceneNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
namespace Memory 
{
    /// I_SceneNode is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Rendering::I_SceneNode> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_I_SCENE_NODE_HPP_INCLUDED
