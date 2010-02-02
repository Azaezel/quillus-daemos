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

#include "RenderingManager.hpp"

#include <Zen/Engine/Rendering/I_RenderingServiceFactory.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/I_ScriptObject.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Resource/I_Resource.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <sstream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingManager::RenderingManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingManager::~RenderingManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_RenderingManager::pRenderingService_type
RenderingManager::create(const std::string& _type)
{
    Threading::CriticalSection guard(m_renderingServiceCache.getLock());

    pRenderingService_type pService(m_renderingServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_RenderingServiceFactory* pFactory = m_renderingServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        std::stringstream errorMessage;
        errorMessage << "Zen::Rendering::RenderingManager::create() : Error: could not create service factory for type " << _type;

        throw Utility::runtime_exception(errorMessage.str());
    }

    if (m_pDefaultScriptEngine.isValid())
    {
        registerScriptEngine(m_pDefaultScriptEngine, pService);
    }

    return m_renderingServiceCache.cacheService(_type, pFactory->create());

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if(!m_pDefaultScriptEngine.isValid())
    {
        Threading::CriticalSection guard(m_renderingServiceCache.getLock());

        for(RenderingServiceCache_type::iterator iter = m_renderingServiceCache.begin(); iter != m_renderingServiceCache.end(); iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingManager::pScriptModule_type
RenderingManager::getDefaultScriptModule()
{
    return m_pRenderingModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingManager::registerScriptEngine(pScriptEngine_type _pEngine, pRenderingService_type _pService)
{
    new I_RenderingService::ScriptObjectReference_type(m_pRenderingModule, m_pRenderingServiceType, _pService, "renderingService");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// C version of I_SceneService::createChildNode
static Scripting::I_ObjectReference* createChildNode(Scripting::I_ObjectReference* _pService, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_SceneService>*  pService = dynamic_cast<Scripting::ObjectReference<I_SceneService>*>(_pService);

    if (pService)
    {
        std::string name = boost::any_cast<std::string>(_parms[0]);

        Memory::managed_ptr<I_SceneNode> pNode = pService->getObject()->createChildNode(name);

        return new Scripting::ObjectReference<I_SceneNode>(pService->getModule(), pService->getModule()->getScriptType(pNode->getScriptTypeName()), pNode);
    }

    // TODO Error
    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// C version of I_SceneNode::attachObject
static void attachObject(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_SceneNode>* pObject = dynamic_cast<Scripting::ObjectReference<I_SceneNode>*>(_pObject);

    if (pObject)
    {
        // The first parameter is an I_ObjectReference that wraps an I_RenderableResource
        Scripting::I_ObjectReference* pResource = boost::any_cast<Scripting::I_ObjectReference*>(_parms[0]);

        // Convert to the correct ObjectReference type
        Scripting::ObjectReference<Resource::I_Resource>* pNativeResource =
            dynamic_cast<Scripting::ObjectReference<Resource::I_Resource>*>
            (pResource);

        Rendering::I_RenderableResource* pRenderable =
            dynamic_cast<Rendering::I_RenderableResource*>(pNativeResource->getObject().get());

        pObject->getObject()->attachObject(*pRenderable);
        return;
    }

    throw Utility::runtime_exception("Zen::Rendering::RenderingService::attachObject() : _pObject is an invalid object reference.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// C version of I_SceneNode::setPosition
static void setPosition(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_SceneNode>*  pObject = dynamic_cast<Scripting::ObjectReference<I_SceneNode>*>(_pObject);

    if (pObject)
    {
        Math::Real x = boost::any_cast<Math::Real>(_parms[0]);
        Math::Real y = boost::any_cast<Math::Real>(_parms[1]);
        Math::Real z = boost::any_cast<Math::Real>(_parms[2]);

        pObject->getObject()->setPosition(x, y, z);
        return;
    }

    throw Utility::runtime_exception("Zen::Rendering::RenderingService::setPosition() : _pObject is an invalid object reference.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// C version of I_SceneNode::setScale
static void setScale(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_SceneNode>*  pObject = dynamic_cast<Scripting::ObjectReference<I_SceneNode>*>(_pObject);

    if (pObject)
    {
        Math::Real x = boost::any_cast<Math::Real>(_parms[0]);
        Math::Real y = boost::any_cast<Math::Real>(_parms[1]);
        Math::Real z = boost::any_cast<Math::Real>(_parms[2]);

        pObject->getObject()->setScale(x, y, z);
        return;
    }

    throw Utility::runtime_exception("Zen::Rendering::RenderingService::setScale() : _pObject is an invalid object reference.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// C version of I_RenderableResource::setMaterialName
static void setMaterialName(Scripting::I_ObjectReference* _pObj, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<Resource::I_Resource>* pObj = dynamic_cast<Scripting::ObjectReference<Resource::I_Resource>*>(_pObj);

    if (pObj)
    {
        std::string name = boost::any_cast<std::string>(_parms[0]);

        I_RenderableResource* pResource =
            dynamic_cast<I_RenderableResource*>(pObj->getRawObject());

        pResource->setMaterialName(name);
        return;
    }

    throw Utility::runtime_exception("Zen::Engine::Resource::RenderableResource::setMaterialName() : _pObj is an invalid object reference.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingManager::registerScriptTypes(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    // Create a Rendering module
    m_pRenderingModule = _pEngine->createScriptModule("Rendering", "Zen Rendering Module");

    // Expose I_RenderingService to the Script Engine
    m_pRenderingServiceType = m_pRenderingModule->createScriptType("RenderingService", "Rendering Service", 0);

    // Expose I_SceneService to the Script Engine
    m_pSceneServiceType = m_pRenderingModule->createScriptType("SceneService", "Scene Service", 0);
    m_pSceneServiceType->addMethod("createChildNode", "Create a child node in the scene graph", createChildNode);

    // Expose I_SceneNode to the Script Engine
    // Some of these methods aren't exposed yet because the other dependent classes aren't
    //      registered with the Script Engine.
    // TODO Finish this
    m_pSceneNodeType = m_pRenderingModule->createScriptType("SceneNode", "Scene Node", 0);
    m_pSceneNodeType->addMethod("attachObject", "Attach a resource to this node", attachObject);
    //m_pSceneNodeType->addMethod("attachPhysicsActor", "Attach a resource to this node", attachPhysicsActor);
    m_pSceneNodeType->addMethod("setPosition", "Attach a resource to this node", setPosition);
    //m_pSceneNodeType->addMethod("setRotation", "Attach a resource to this node", setRotation);
    //m_pSceneNodeType->addMethod("applyTransformation", "Attach a resource to this node", applyTransformation);
    m_pSceneNodeType->addMethod("setScale", "Attach a resource to this node", setScale);

    // Expose I_RenderableResource to the Script Engine
    m_pRenderableResourceType = m_pRenderingModule->createScriptType("RenderableResource", "Renderable Resource", 0);
    m_pRenderableResourceType->addMethod("setMaterialName", "Set the material name", setMaterialName);

    m_pRenderingModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
