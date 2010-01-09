//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
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
#ifndef ZEN_ENGINE_BASE_GAME_OBJECT_HPP_INCLUDED
#define ZEN_ENGINE_BASE_GAME_OBJECT_HPP_INCLUDED

#include "../I_BaseGameObject.hpp"

#include "GameObjectBehaviors.hpp"

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Engine/Core/I_GameObject.hpp>

#include <Zen/Engine/Resource/I_Resource.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Physics/I_PhysicsShape.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class BaseGame;

/// @brief Base Game Object
///
/// Starter Kits and Games should derive from this object to create specialized
/// implementations.
///
/// A Game Object is any object inside of the game, whether it's renderable
/// or not, collidable or not, controllable or not, etc.
class GameObject
:   public I_BaseGameObject
{
    /// @name I_BaseGameObject implementation
    /// @{
public:
    virtual const std::string& getName() const;

    virtual void setGameObject(Core::I_GameObject* _pGameObject);
    virtual Core::I_GameObject* getGameObject();

    //virtual pRenderableResource_type getResource();

    virtual pSceneNode_type getSceneNode();
    virtual void attachToSceneNode(pSceneNode_type _pSceneNode);

    virtual void attachCollisionShape(pCollisionShape_type _pCollisionShape);
    virtual pCollisionShape_type getCollisionShape();

    virtual void setPosition(const Math::Point3& _position, bool _overridePhysics = false);
    virtual void setPosition(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z, bool _overridePhysics = false);
    virtual void getPosition(Math::Point3& _position);

    virtual void setOrientation(const Math::Quaternion4& _orientation, bool _overridePhysics = false);
    virtual void setOrientation(Zen::Math::Real _w, Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z, bool _overridePhysics = false);
    virtual void setOrientation(const Math::Degree& _angle, const Math::Vector3& _axis, bool _overridePhysics = false);

    virtual void setScale(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z);

    virtual void setBehaviors(Core::I_BehaviorService::pGameObjectBehaviors_type _pBehaviors);

    virtual Scripting::I_ObjectReference* getScriptObject();

    /// @}

    /// @name Base::I_BaseGameObject implementation
    /// @{
public:
    I_BaseGame& getGame();

    //void loadResource(const std::string& _resource);
    //void setMaterialName(const std::string& _material);

    const Math::Point3& getPosition() const;
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Called every frame right before the frame is rendered.
    /// @param _elapsedTime - amount of time that has elapsed since the 
    ///             previous frame was rendered (in seconds).
    //void beforeRender(double _elapsedTime);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class BehavioredGameObject;

             GameObject(BaseGame& _client, const std::string& _name);
    virtual ~GameObject();
    /// @}

    /// @name Member Variables
    /// @{
private:
    BaseGame&     m_game;
    std::string     m_name;

    pRenderableResource_type                m_pRenderable;
    pSceneNode_type                         m_pNode;
    pCollisionShape_type                      m_pCollisionShape;

    Math::Point3                            m_position;
    Math::Quaternion4                       m_orientation;

    /// "User" game object
    Core::I_GameObject*                     m_pGameObject;
    /// @}

};  // class GameObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_BASE_GAME_OBJECT_HPP_INCLUDED
