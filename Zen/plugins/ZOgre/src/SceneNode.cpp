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

#include "SceneNode.hpp"
#include "Camera.hpp"
#include "ParticleSystem.hpp"

#include <Zen/Core/Math/Matrix4.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Point3.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Rendering/I_RenderingManager.hpp>

#include "Ogre.hpp"

#include <stdexcept>
#include <sstream>

#include <stddef.h>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneNode::SceneNode(Ogre::SceneNode* _pNode)
:   m_pNode(_pNode)
,   m_pResource(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneNode::~SceneNode()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // deprecated.  See attachObject
void
SceneNode::attachResource(Engine::Rendering::I_RenderableResource& _resource)
{
    m_pResource = dynamic_cast<ResourceEntity*>(&_resource);

    if (m_pResource == NULL)
    {
        throw Utility::runtime_exception("SceneNode::attachResource(): Error, _resource is invalid type.");
    }

    m_pNode->attachObject(m_pResource->getEntity());
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SceneNode::attachPhysicsActor(Engine::Physics::I_CollisionShape& _CollisionShape)
{
    m_pCollisionShape = &_CollisionShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::setPosition(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z)
{
    m_pNode->setPosition(_x, _y, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::getPosition(Zen::Math::Point3& _position) const
{
    const Ogre::Vector3& position = m_pNode->_getDerivedPosition();
    _position.m_x = position.x;
    _position.m_y = position.y;
    _position.m_z = position.z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SceneNode::setRotation(Zen::Math::Real _phi, Zen::Math::Real _theta, Zen::Math::Real _psi)
{
    // TODO: need to implement
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SceneNode::setRotation(Math::Quaternion4 _quaternion)
{
    // TODO: need to implement
    m_pNode->setOrientation(_quaternion.m_w, _quaternion.m_x, _quaternion.m_y, _quaternion.m_z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
SceneNode::applyTransformation(Math::Matrix4 _transformationMatrix)
{
    // TODO: need to implement
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::setScale(Zen::Math::Real _x, Zen::Math::Real _y, Zen::Math::Real _z)
{
    m_pNode->setScale(_x, _y, _z);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
SceneNode::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            Engine::Rendering::I_RenderingManager::getSingleton().getDefaultScriptModule(), 
            Engine::Rendering::I_RenderingManager::getSingleton().getDefaultScriptModule()->getScriptType(getScriptTypeName()), getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::attachObject(Engine::Rendering::I_AttachableObject& _object)
{
    ResourceEntity* pResource = dynamic_cast<ResourceEntity*>(&_object);

    if (pResource != NULL)
    {
        m_pNode->attachObject(pResource->getEntity());
        return;
    }

    ParticleSystem* pParticleSystem = dynamic_cast<ParticleSystem*>(&_object);

    if( pParticleSystem != NULL )
    {
        const Ogre::ParticleSystem* pOgreParticleSystem = &pParticleSystem->getOgreParticleSystem();
        Ogre::SceneNode* pNode = m_pNode->createChildSceneNode(pOgreParticleSystem->getName());
        return;
    }

    // TODO Error since the dynamic cast failed?
    // TODO Handle cameras
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::attachObject(const std::string& _name, Engine::Rendering::I_AttachableObject& _object, Math::Vector3& _translate, Math::Quaternion4& _rotate)
{
    // HACK Shouldn't assume this is a camera
    Camera* pCamera = dynamic_cast<Camera*>(&_object);

    if (pCamera)
    {
        Ogre::Vector3 translate(_translate.m_array);
        Ogre::Quaternion rotate(_rotate.m_array);

        Ogre::SceneNode* pParentNode = m_pNode->createChildSceneNode(_name, translate, rotate);
        pParentNode->setInheritScale(false);
        pParentNode->setInitialState();

        std::stringstream childName;
        childName << _name << "_child";

        Ogre::SceneNode* pChildNode = pParentNode->createChildSceneNode(childName.str());

        pCamera->setSceneNode(pChildNode);

        pChildNode->setInheritScale(false);
        pChildNode->setInitialState();

        pChildNode->attachObject(&pCamera->getOgreCamera());
        return;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
SceneNode::getNumAttachedObjects() const
{
    return m_pNode->numAttachedObjects();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_AttachableObject*
SceneNode::getAttachedObject(int _index) const
{
    const Ogre::Any& any = m_pNode->getAttachedObject(_index)->getUserAny();
    if (!any.isEmpty())
    {
        ResourceEntity* pRawEntity = Ogre::any_cast<ResourceEntity*>(any);
        return pRawEntity;
    }

    // TODO Throw an exception?
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::detachObject(int _index)
{
    m_pNode->detachObject(_index);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::detachAllObjects()
{
    m_pNode->detachAllObjects();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneNode::setUserData(boost::any _anyUserData)
{
    m_userData = _anyUserData;
}

const boost::any&
SceneNode::getUserData() const
{
    return m_userData;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
