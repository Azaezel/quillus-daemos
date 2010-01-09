//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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

#include "SceneService.hpp"
#include "SceneNode.hpp"
#include "ParticleSystem.hpp"

#include "NullCamera.hpp"
#include "NullLight.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Rendering/I_RenderingManager.hpp>

#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneService::SceneService()
:   m_pScriptObject(NULL)
{
    //m_pSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "default");
    //Ogre::Root::getSingleton().initialise(false, "IndieZen Rendering Window");
    //m_pSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "default");
    m_pSceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "default");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneService::~SceneService()
{
    std::cout << "SceneService::~SceneService()" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneService::pLight_type
SceneService::createLight(const std::string& _type, const std::string& _name)
{
    NullLight* pRawLight = new NullLight(m_pSceneManager->createLight(_name));

    pLight_type pLight(pRawLight, onDestroyLight);

    return pLight;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneService::pParticleSystem_type
SceneService::createParticleSystem(const std::string& _name, const std::string& _resource)
{
    Ogre::ParticleSystem* const pOgreParticleSystem = m_pSceneManager->createParticleSystem(_name,_resource);
    ParticleSystem* pRawParticleSystem = new ParticleSystem(pOgreParticleSystem);

    pParticleSystem_type pParticleSystem(pRawParticleSystem, &destroyParticleSystem);

    return pParticleSystem;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Rendering::I_SceneService::pSceneNode_type
SceneService::createChildNode(const std::string &_name)
{
    Ogre::SceneNode* const pOgreSceneNode = m_pSceneManager->getRootSceneNode()->createChildSceneNode(_name);
    SceneNode* pRawSceneNode = new SceneNode(pOgreSceneNode);
    pSceneNode_type pNode(pRawSceneNode, &destroySceneNode);

    pRawSceneNode->setSelfReference(pNode.getWeak());

    pOgreSceneNode->setUserAny(Ogre::Any(pRawSceneNode));

    // TODO Keep a copy of pNode?  Probably not necessary since m_pSceneManager does that for us

    return pNode;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneService::setAmbientLight(Math::Real _red, Math::Real _green, Math::Real _blue, Math::Real _alpha)
{
    m_pSceneManager->setAmbientLight(Ogre::ColourValue(_red, _green, _blue, _alpha));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneService::destroySceneNode(wpSceneNode_type _pNode)
{
    // Fire the node's onDestroyEvent
    _pNode->onDestroyEvent(_pNode);

    SceneNode* pNode = dynamic_cast<SceneNode*>(_pNode.get());

    if (pNode != NULL)
    {
        delete pNode;
    }
    else
    {
        throw Zen::Utility::runtime_exception("SceneService::destroySceneNode() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneService::destroyParticleSystem(wpParticleSystem_type _pParticleSystem)
{
    ParticleSystem* pParticleSystem =
        dynamic_cast<ParticleSystem*>(_pParticleSystem.get());

    if( pParticleSystem != NULL )
    {
        delete pParticleSystem;
    }
    else
    {
        throw Zen::Utility::runtime_exception("SceneService::destroyParticleSystem() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneService::onDestroyLight(wpLight_type& _wpLight)
{
    /// Fire the Light's onDestroyEvent
    _wpLight->onDestroyEvent(_wpLight);

    /// Delete the Light
    NullLight* pLight = dynamic_cast<NullLight*>(_wpLight.get());

    if( pLight )
    {
        delete pLight;
    }
    else
    {
        throw Utility::runtime_exception("Zen::ZOgre::SceneService::onDestroyLight() : _wpLight is an invalid NullLight.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
SceneService::getScriptObject()
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
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
