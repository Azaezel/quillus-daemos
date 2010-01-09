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
#include "GameObject.hpp"

#include "BaseGame.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>

#include <Zen/Engine/Physics/I_PhysicsZone.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObject::GameObject(BaseGame& _client, const std::string& _name)
:   m_game(_client)
,   m_name(_name)
,   m_pPhysicsActor()
,   m_pGameObject(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObject::~GameObject()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
GameObject::getName() const
{
    return m_name;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setGameObject(Core::I_GameObject* _pGameObject)
{
    m_pGameObject = _pGameObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Core::I_GameObject*
GameObject::getGameObject()
{
    return m_pGameObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
GameObject::pRenderableResource_type
GameObject::getResource()
{
    return m_pRenderable.as<pRenderableResource_type>();
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObject::pSceneNode_type
GameObject::getSceneNode()
{
    return m_pNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::attachToSceneNode(pSceneNode_type _pSceneNode)
{
    m_pNode = _pSceneNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::attachPhysicsActor(pPhysicsActor_type _pPhysicsActor)
{
    m_pPhysicsActor = _pPhysicsActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObject::pPhysicsActor_type
GameObject::getPhysicsActor()
{
    return m_pPhysicsActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_BaseGame&
GameObject::getGame()
{
    return m_game;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated
void
GameObject::loadResource(const std::string& _resource)
{
    Zen::Engine::Resource::I_ResourceManager::config_type config;
    config["fileName"] = _resource;
    config["label"] = m_name;
    config["type"] = "entity";
    m_pRenderable = m_game.getRenderingResourceService().loadResource(config).as<pRenderableResource_type>();

    m_game.onBeforeFrameRenderedEvent.connect(boost::bind(&GameObject::beforeRender, this, _1));

    m_pNode = m_game.getSceneService().createChildNode(m_name);
    m_pNode->attachResource(*m_pRenderable.get());

    m_pNode->setUserData(this);
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated
void
GameObject::beforeRender(double _elapsedTime)
{
    m_pRenderable->updateAllAnimations(_elapsedTime);
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated
void
GameObject::setMaterialName(const std::string& _material)
{
    m_pRenderable->setMaterialName(_material);
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setPosition(const Math::Point3& _position, bool _overridePhysics)
{
    if (getSceneNode().isValid())
    {
        getSceneNode()->setPosition(_position.m_x, _position.m_y, _position.m_z);
    }

    m_position = _position;

    if (_overridePhysics && getPhysicsActor().isValid() )
    {
        getPhysicsActor()->setPosition(_position);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setPosition(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z, bool _overridePhysics)
{
    // translate parms and redirect to alternate function signature
    return setPosition(Math::Point3(_x, _y, _z), _overridePhysics);

    // DO NOT ADD CHANGES TO THIS FUNCTION.  Correct the alternate setPosition() above.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::getPosition(Math::Point3& _position)
{
    _position = m_position;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setOrientation(const Math::Quaternion4& _orientation, bool _overridePhysics)
{
    // TODO - implement I_SceneNode::setOrientation() and then uncomment the next 2 lines
    //getSceneNode()->setOrientation(_orientation);
    getSceneNode()->setRotation(_orientation);
    m_orientation = _orientation;

    if (_overridePhysics && getPhysicsActor().isValid() )
    {
        getPhysicsActor()->setOrientation(_orientation);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setOrientation(Zen::Math::Real _w, Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z, bool _overridePhysics)
{
    // translate parms and redirect to alternate function signature
    return setOrientation(Math::Quaternion4(_w, _x, _y, _z), _overridePhysics);

    // DO NOT ADD CHANGES TO THIS FUNCTION.  Correct the alternate setOrientation() above.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setOrientation(const Math::Degree& _angle, const Math::Vector3& _axis, bool _overridePhysics)
{
    // translate parms and redirect to alternate function signature
    return setOrientation(Math::Quaternion4(Math::Radian(_angle), _axis), _overridePhysics);

    // DO NOT ADD CHANGES TO THIS FUNCTION.  Correct the alternate setOrientation() above.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Point3&
GameObject::getPosition() const
{
    return m_position;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setScale(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z)
{
    getSceneNode()->setScale(_x, _y, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObject::setBehaviors(Core::I_BehaviorService::pGameObjectBehaviors_type _pBehaviors)
{
    throw Utility::runtime_exception("GameObject::setBehaviors(): Error, this method should never be reached!");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
GameObject::getScriptObject()
{
    throw Utility::runtime_exception("GameObject::getScriptObject(): Error, this method should never be reached!");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
