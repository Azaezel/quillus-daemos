//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#include "ResourceService.hpp"
#include "ResourceEntity.hpp"

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Resource/I_ResourceManager.hpp>

#include <OgreResourceGroupManager.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::ResourceService()
:   m_root(Ogre::Root::getSingleton())
,   m_groupManager(Ogre::ResourceGroupManager::getSingleton())
,   m_bInitialized(false)
,   m_pScriptObject(NULL)
,   m_pModule(NULL)
{
    m_pGroupInitLock = Threading::MutexFactory::create();

    //m_pSceneManager = m_root.getSceneManager("default");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::~ResourceService()
{
    Threading::MutexFactory::destroy(m_pGroupInitLock);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::addResourceLocation(const std::string& _path, const std::string& _type,
                                     const std::string& _group, bool _recursive)
{
    m_groupManager.addResourceLocation(_path, _type, _group, _recursive);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::removeResourceLocation(const std::string& _path, const std::string& _group)
{
    m_groupManager.removeResourceLocation(_path, _group);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::initialiseAllResourceGroups()
{
    if (!m_bInitialized)
    {
        Threading::CriticalSection guard(m_pGroupInitLock);
        if (!m_bInitialized)
        {
            m_groupManager.initialiseAllResourceGroups();

            m_bInitialized = true;
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::loadResourceGroup(const std::string& _group)
{
    if (m_bInitialized)
    {
        m_groupManager.initialiseResourceGroup(_group);
        m_groupManager.loadResourceGroup(_group);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::unloadResourceGroup(const std::string& _group)
{
    if (m_bInitialized)
    {
        m_groupManager.unloadResourceGroup(_group, false);
        m_groupManager.clearResourceGroup(_group);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Resource::I_ResourceService::pResource_type
ResourceService::loadResource(config_type& _config)
{
    initialiseAllResourceGroups();

    // TODO Maintain a pointer to this object

    if (_config["type"] == "entity")
    {
        Ogre::SceneManager* pScene = m_root.getSceneManager(_config["scene"]);

        Ogre::Entity* pEntity = pScene->createEntity(_config["label"], _config["fileName"]);
        ResourceEntity* pRawPointer = new ResourceEntity(pEntity,pScene);
        pResource_type pResourceEntity(pRawPointer, boost::bind(&ResourceService::destroyResource, this, _1));
        wpResource_type pWeakPtr(pResourceEntity);
        pRawPointer->setSelfReference(pWeakPtr);

        return pResourceEntity;
    }
    else if (_config["type"] == "terrain")
    {
        Ogre::SceneManager* pScene = m_root.getSceneManager(_config["scene"]);

        std::cout << "ZOgre::loadResource() loading terrain..." << std::endl;
        pScene->setWorldGeometry(_config["fileName"]);

        // return an empty resource, since there's no ogre Entity created by setWorldGeometry()
        pResource_type emptyResource;
        return emptyResource;
    }
    else if (_config["type"] == "skybox")
    {
        Ogre::SceneManager* pScene = m_root.getSceneManager(_config["scene"]);

        std::cout << "ZOgre::loadResource() loading skybox..." << std::endl;
        pScene->setSkyBox(true, _config["resourceName"], boost::lexical_cast<Math::Real,std::string>(_config["distance"]), true);

        // return an empty resource, since there's no ogre Entity created by setSkyBox()
        pResource_type emptyResource;
        return emptyResource;
    }
    else
    {
        std::cout << "ZOgre::loadResource() error - no 'type' tag in config" << std::endl;
        pResource_type emptyResource;
        return emptyResource;
    }

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::getResourceNames(I_ResourceNameVisitor& _visitor, 
                                  const std::string& _group,
                                  const std::string& _pattern) const
{
    _visitor.begin();

    Ogre::StringVectorPtr pStringVector = 
        m_groupManager.findResourceNames(_group, _pattern);

    Ogre::StringVector::iterator iter = pStringVector->begin();
    while (iter != pStringVector->end())
    {
        _visitor.visit(*iter);
        iter++;
    }

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::destroyResource(wpResource_type _pResource)
{
    ResourceEntity* pResource = dynamic_cast<ResourceEntity*>(_pResource.get());

    if(pResource)
    {
        _pResource->onDestroyEvent(_pResource);
        delete pResource;
    }
    else
    {
        // What other kind of resources could this be?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_scriptSingletonName("renderingResourceService");

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ResourceService::getScriptSingletonName() const
{
    return sm_scriptSingletonName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
ResourceService::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_pModule->getScriptModule(),
            m_pModule->getScriptModule()->getScriptType(getScriptTypeName()), 
            getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::registerScriptModule(Zen::Scripting::script_module& _module)
{
    m_pModule = &_module;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
