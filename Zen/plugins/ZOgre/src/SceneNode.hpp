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
#ifndef ZEN_ZOGRE_SCENE_NODE_HPP_INCLUDED
#define ZEN_ZOGRE_SCENE_NODE_HPP_INCLUDED

#include "ResourceEntity.hpp"

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Engine/Rendering/I_SceneNode.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class SceneNode
:   public Zen::Engine::Rendering::I_SceneNode
,   public Zen::Memory::managed_self_ref<Zen::Engine::Rendering::I_SceneNode>
{
    /// @name I_SceneNode implementation
    /// @{
public:
    //virtual void attachResource(Engine::Rendering::I_RenderableResource& _resource);
    virtual void attachPhysicsActor(Engine::Physics::I_CollisionShape& _shape);
    virtual void setPosition(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z);
    virtual void getPosition(Zen::Math::Point3& _position) const;
    virtual void setRotation(Zen::Math::Real _phi, Zen::Math::Real _theta, Zen::Math::Real _psi);
    virtual void setRotation(Math::Quaternion4 _quaternion);
    virtual void applyTransformation(Math::Matrix4 _transformationMatrix);
    virtual void setScale(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z);
    virtual void attachObject(Engine::Rendering::I_AttachableObject& _object);
    virtual void attachObject(const std::string& _name, Engine::Rendering::I_AttachableObject& _object, Math::Vector3& _translate, Math::Quaternion4& _rotate);
    virtual int getNumAttachedObjects() const;
    virtual Engine::Rendering::I_AttachableObject* getAttachedObject(int _index) const;
    virtual void detachObject(int _index);
    virtual void detachAllObjects();

    virtual void setUserData(boost::any _anyUserData);
    virtual const boost::any& getUserData() const;
    /// @}

    /// @name I_Renderable implementation
    /// @{
public:
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name 'Structors
    /// @{
public:
             SceneNode(Ogre::SceneNode* _pNode);
    virtual ~SceneNode();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogre::SceneNode*                    m_pNode;
    ResourceEntity*                     m_pResource;
    Engine::Physics::I_CollisionShape*    m_pCollisionShape;
    ScriptObjectReference_type*         m_pScriptObject;

    boost::any                          m_userData;
    /// @}

};  // class SceneNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_SCENE_NODE_HPP_INCLUDED
