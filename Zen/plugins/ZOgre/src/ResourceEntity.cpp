//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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

#include "ResourceEntity.hpp"
#include "AnimationState.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Resource/I_ResourceManager.hpp>
#include <Zen/Engine/Rendering/I_AnimationState.hpp>

#include "Mesh.hpp"

#include "Ogre.hpp"

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceEntity::ResourceEntity(Ogre::Entity* _pEntity, Ogre::SceneManager* _pSceneManager)
    :   m_pEntity(_pEntity)
    ,   m_pSceneManager(_pSceneManager)
    ,   m_pScriptObject(NULL)
{
    m_pEntity->setUserAny(Ogre::Any(this));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceEntity::~ResourceEntity()
{
    m_pSceneManager->destroyEntity(m_pEntity);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
    ResourceEntity::setMaterialName(const std::string& _name)
{
    m_pEntity->setMaterialName(_name);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void
    ResourceEntity::setNormaliseNormals(bool _normalise)
{
    // See http://www.indiezen.org/wiki/ticket/103
#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 6
    // No longer necessary.
#else
    m_pEntity->setNormaliseNormals(_normalise);
#endif
}
#endif  // deprecated

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void
    ResourceEntity::updateAllAnimations(double _elapsedTime)
{
    Ogre::AnimationStateSet* set = m_pEntity->getAllAnimationStates();
    if ((set != 0) && set->hasEnabledAnimationState())
    {
        Ogre::ConstEnabledAnimationStateIterator stateIt = set->getEnabledAnimationStateIterator();
        while (stateIt.hasMoreElements())
        {
            Ogre::AnimationState* animState = stateIt.getNext();
            animState->addTime(_elapsedTime);
        }
    }
}
#endif  // deprecated

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
void
    ResourceEntity::enableAnimation(const std::string& _animationName, bool _enable)
{
    m_pEntity->getAnimationState(_animationName)->setEnabled(_enable);
}
#endif  // deprecated

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
    ResourceEntity::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            Engine::Resource::I_ResourceManager::getSingleton().getDefaultResourceScriptModule(), 
            Engine::Resource::I_ResourceManager::getSingleton().getDefaultResourceScriptModule()->getScriptType(getScriptTypeName()), 
            getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceEntity::setVisible(bool _visible)
{
    m_pEntity->setVisible(_visible);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ResourceEntity::getVisible() const
{
    return m_pEntity->getVisible();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceEntity::pAnimationState_type
ResourceEntity::getAnimationState(const std::string& _name) const
{
    return pAnimationState_type(new AnimationState(*m_pEntity->getAnimationState(_name)));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceEntity::getAnimationStates(I_AnimationStateVisitor& _visitor) const
{
    _visitor.begin();

    Ogre::AnimationStateSet* set = m_pEntity->getAllAnimationStates();
    if ((set != 0) && set->hasEnabledAnimationState())
    {
        Ogre::ConstEnabledAnimationStateIterator stateIt = set->getEnabledAnimationStateIterator();

        AnimationState animationState;
        while (stateIt.hasMoreElements())
        {
            animationState.setState(*stateIt.getNext());
            _visitor.visit(animationState);
        }
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceEntity::pMesh_type
ResourceEntity::getMesh()
{
    Mesh* pRawMesh = new Mesh(getEntity()->getMesh());

    pMesh_type pMesh(pRawMesh, boost::bind(&ResourceEntity::destroyMesh, this, _1));

    return pMesh;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceEntity::destroyMesh(pMesh_type::weak_ptr_type _pMesh)
{
    Mesh* pMesh = dynamic_cast<Mesh*>(_pMesh.lock().get());

    if (pMesh)
    {
        delete pMesh;
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
