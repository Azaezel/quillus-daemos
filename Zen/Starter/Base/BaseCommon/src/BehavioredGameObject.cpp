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
#include "BehavioredGameObject.hpp"

#include "BaseGame.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>

#include <Zen/Engine/Physics/I_PhysicsZone.hpp>

#include <boost/bind.hpp>
#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BehavioredGameObject::BehavioredGameObject(BaseGame& _game, const std::string& _name, GameObjectBehaviors* _pBehaviors)
:   m_pBehaviors(_pBehaviors)
,   m_game(_game)
{
    m_pGameObject = new GameObject(_game, _name);

    m_pScriptObject = new Core::I_BaseGameObject::ScriptObjectReference_type
        (_game.getScriptModule(), _game.getScriptModule()->getScriptType(getScriptTypeName()), this);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BehavioredGameObject::~BehavioredGameObject()
{
    delete m_pGameObject;
    delete m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
BehavioredGameObject::getName() const
{
    return m_pGameObject->getName();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setGameObject(Core::I_GameObject* _pGameObject)
{
    m_pGameObject->setGameObject(_pGameObject);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Core::I_GameObject*
BehavioredGameObject::getGameObject()
{
    return m_pGameObject->getGameObject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
BehavioredGameObject::pRenderableResource_type
BehavioredGameObject::getResource()
{
    return m_pGameObject->getResource();
}
#endif
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BehavioredGameObject::pSceneNode_type
BehavioredGameObject::getSceneNode()
{
    return m_pGameObject->getSceneNode();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::attachToSceneNode(pSceneNode_type _pSceneNode)
{
    pSceneNode_type pPreviousSceneNode = m_pGameObject->getSceneNode();

    if (pPreviousSceneNode.isValid())
    {
        // TODO is boost::any(0) a good value to use to reset
        // the user data?
        pPreviousSceneNode->setUserData(boost::any(0));
    }
    m_pGameObject->attachToSceneNode(_pSceneNode);
    _pSceneNode->setUserData(boost::any(this));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::attachPhysicsActor(pPhysicsActor_type _pPhysicsActor)
{
    m_pGameObject->attachPhysicsActor(_pPhysicsActor);

    _pPhysicsActor->onTransformEvent.connect(boost::bind(&BehavioredGameObject::objectTransformCallback, this, _1));
    _pPhysicsActor->onApplyForcesEvent.connect(boost::bind(&BehavioredGameObject::objectForcesCallback, this, _1));
	_pPhysicsActor->onBoundBoxCollisionEvent.connect(boost::bind(&BehavioredGameObject::objectBoundBoxCollisionCallback, this, _1));
	_pPhysicsActor->onCollisionEvent.connect(boost::bind(&BehavioredGameObject::objectCollisionCallback, this, _1));
	_pPhysicsActor->onCollisionResolutionEvent.connect(boost::bind(&BehavioredGameObject::objectCollisionResolutionCallback, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::objectTransformCallback(Physics::I_PhysicsActor::I_TransformEventData& _data)
{
    // Use the behaviors
    if (m_pBehaviors)
    {
        m_pBehaviors->handlePhysicsTransform(*m_pGameObject, _data);
    }

    // Then apply the transform to the game object.
    // TODO Should this be yet another behavior so it can be overriden if necessary?

    Zen::Math::Point3 pos;
    _data.getTransform().getPosition(pos);

    setPosition(pos);
    Zen::Math::Quaternion4 orient(_data.getTransform());
    setOrientation(orient);

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::objectForcesCallback(Physics::I_PhysicsActor::I_ApplyForcesEventData& _data)
{
    if (m_pBehaviors)
    {
        m_pBehaviors->handlePhysicsForce(*m_pGameObject, _data);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::objectBoundBoxCollisionCallback(Physics::I_PhysicsActor::I_BeginCollisionEventData& _data)
{
    if (m_pBehaviors)
    {
        m_pBehaviors->handleBoundBoxCollision(*m_pGameObject, _data);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::objectCollisionCallback(Physics::I_PhysicsActor::I_DuringCollisionEventData& _data)
{
    if (m_pBehaviors)
    {
        m_pBehaviors->handleCollision(*m_pGameObject, _data);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::objectCollisionResolutionCallback(Physics::I_PhysicsActor::I_EndCollisionEventData& _data)
{
	//std::cout << "BehavioredGameObject::objectCollisionCallback";
    if (m_pBehaviors)
    {
        m_pBehaviors->handleCollisionResolution(*m_pGameObject, _data);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BehavioredGameObject::pPhysicsActor_type
BehavioredGameObject::getPhysicsActor()
{
    if(!m_pGameObject->getPhysicsActor().isValid())
    {
        attachPhysicsActor(m_game.getCurrentPhysicsZone()->createActor());
    }

    return m_pGameObject->getPhysicsActor();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_BaseGame&
BehavioredGameObject::getGame()
{
    return m_pGameObject->getGame();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated
void
BehavioredGameObject::loadResource(const std::string& _resource)
{
    m_pGameObject->loadResource(_resource);

    m_pGameObject->getSceneNode()->setUserData(boost::any(this));
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated
void
BehavioredGameObject::setMaterialName(const std::string& _material)
{
    m_pGameObject->setMaterialName(_material);
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setPosition(const Math::Point3& _position, bool _overridePhysics)
{
    m_pGameObject->setPosition(_position, _overridePhysics);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setPosition(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z, bool _overridePhysics)
{
    m_pGameObject->setPosition(_x, _y, _z, _overridePhysics);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setOrientation(const Math::Quaternion4& _orientation, bool _overridePhysics)
{
    m_pGameObject->setOrientation(_orientation, _overridePhysics);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setOrientation(Zen::Math::Real _w, Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z, bool _overridePhysics)
{
    m_pGameObject->setOrientation(_w, _x, _y, _z, _overridePhysics);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setOrientation(const Math::Degree& _angle, const Math::Vector3& _axis, bool _overridePhysics)
{
    m_pGameObject->setOrientation(_angle, _axis, _overridePhysics);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::getPosition(Math::Point3& _position)
{
    m_pGameObject->getPosition(_position);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Point3&
BehavioredGameObject::getPosition() const
{
    return m_pGameObject->getPosition();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setScale(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z)
{
    if (m_pBehaviors)
    {
        m_pBehaviors->setScale(*m_pGameObject, _x, _y, _z);
    }
    else
    {
        m_pGameObject->setScale(_x, _y, _z);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
BehavioredGameObject::setBehaviors(Core::I_BehaviorService::pGameObjectBehaviors_type _pBehaviors)
{
    m_pBehaviors = dynamic_cast<GameObjectBehaviors*>(_pBehaviors);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
BehavioredGameObject::getScriptObject()
{
    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
