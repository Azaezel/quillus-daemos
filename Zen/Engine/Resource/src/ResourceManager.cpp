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

#include "CompoundResource.hpp"
#include "ResourceManager.hpp"

#include <Zen/Engine/Resource/I_CompoundResource.hpp>
#include <Zen/Engine/Resource/I_ResourceServiceFactory.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Resource {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

ResourceManager::ResourceManager()
:   m_scriptTypesInitialized(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceManager::~ResourceManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ResourceManager::pCompoundResourceService_type
ResourceManager::createCompoundResource()
{
    // TODO Not sure where this is used anymore... possibly Zen Studio
    throw Utility::runtime_exception("ResourceManager::createCompoundResource(): Error, not implemented");
#if 0 // Not implemented
    I_ResourceManager::pCompoundResourceService_type pReturn;

	return pReturn;
#endif // 0
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ResourceManager::pResourceService_type
ResourceManager::create(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_resourceServiceCache.getLock());

    pResourceService_type pService(m_resourceServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_ResourceServiceFactory* pFactory = m_resourceServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO Error
        throw Utility::runtime_exception("Zen::Rendering::RenderingManager::create() : Error: could not create resource service factory for type " + _type);
        return pService;
    }

    pService = m_resourceServiceCache.cacheService(_type, pFactory->create(_type));

    if (m_pDefaultScriptEngine.isValid())
    {
        registerScriptEngine(m_pDefaultScriptEngine, pService);
    }

    return pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if( !m_pDefaultScriptEngine.isValid() )
    {
        Threading::CriticalSection guard(m_resourceServiceCache.getLock());

        for(resource_service_cache_type::iterator iter = m_resourceServiceCache.begin(); iter != m_resourceServiceCache.end(); iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceManager::pScriptModule_type
ResourceManager::getDefaultScriptModule()
{
    return m_pResourceModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceManager::registerScriptEngine(pScriptEngine_type _pEngine, pResourceService_type _pService)
{
    new Scripting::ObjectReference<I_ResourceService>(m_pResourceModule, m_pResourceServiceType, _pService, _pService->getScriptSingletonName());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// C version of the I_ResourceService::addResourceLocation
static void addResourceLocation(Scripting::I_ObjectReference* _pService, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_ResourceService>*  pService = dynamic_cast<Scripting::ObjectReference<I_ResourceService>*>(_pService);

    if (pService)
    {
        std::string path = boost::any_cast<std::string>(_parms[0]);
        std::string type = boost::any_cast<std::string>(_parms[1]);
        std::string group = boost::any_cast<std::string>(_parms[2]);
        bool recursive = boost::any_cast<bool>(_parms[3]);

        pService->getRawObject()->addResourceLocation(path, type, group, recursive);
    }

    // TODO Error
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// C version of the I_ResourceService::loadResource
static Scripting::I_ObjectReference* loadResource(Scripting::I_ObjectReference* _pService, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_ResourceService>*  pService = dynamic_cast<Scripting::ObjectReference<I_ResourceService>*>(_pService);
    if (pService)
    {
        // TODO throw Utility::runtime_exception("Tony, please fix this");
        // The C API should be static Scripting::I_ObjectReference* loadResource(Scripting::I_ObjectReference* _pService, std::map<std::string, boost::any> _parms), or something like that...

        std::map<std::string, std::string> config;
        config["fileName"] = boost::any_cast<std::string>(_parms[0]);
        config["label"] = boost::any_cast<std::string>(_parms[1]);

        if (_parms.size() > 2)
        {
            config["scene"] = boost::any_cast<std::string>(_parms[2]);
        }

        Memory::managed_ptr<I_ResourceService> pResourceService = pService->getObject();
        Memory::managed_ptr<I_Resource> pResource = pResourceService->loadResource(config);

        return new Scripting::ObjectReference<I_Resource>(pService->getModule(), pService->getModule()->getScriptType(pResource->getScriptTypeName()), pResource);
    }

    throw Utility::runtime_exception("Zen::Engine::Resource::ResourceService::loadResource() : _pService is an invalid object reference.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0 // moved to RenderingManager
/// C version of I_RenderableResource::setMaterialName
static void setMaterialName(Scripting::I_ObjectReference* _pObj, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_Resource>* pObj = dynamic_cast<Scripting::ObjectReference<I_Resource>*>(_pObj);

    if (pObj)
    {
        std::string name = boost::any_cast<std::string>(_parms[0]);

        Rendering::I_RenderableResource* pResource =
            dynamic_cast<Rendering::I_RenderableResource*>(pObj->getRawObject());

        pResource->setMaterialName(name);
    }

    throw Utility::runtime_exception("Zen::Engine::Resource::RenderableResource::setMaterialName() : _pObj is an invalid object reference.");
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0   // deprecated
/// C version of I_RenderableResource::setNormaliseNormals
static void setNormaliseNormals(Scripting::I_ObjectReference* _pObj, std::vector<boost::any> _parms)
{
    Scripting::ObjectReference<I_RenderableResource>* pObj = dynamic_cast<Scripting::ObjectReference<I_RenderableResource>*>(_pObj);

    if (pObj)
    {
        bool normalise = boost::any_cast<bool>(_parms[0]);

        pObj->getObject()->setNormaliseNormals(normalise);
    }

    throw Utility::runtime_exception("Zen::Engine::Resource::RenderableResource::setNormaliseNormals() : _pObj is an invalid object reference.");
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceManager::registerScriptTypes(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || _pEngine == NULL)
        return;

    // Create a ResourceManager module
    m_pResourceModule = _pEngine->createScriptModule("ResourceManager", "Zen Resource Manager Module");

    // Expose I_ResourceService to the Script Engine
    m_pResourceServiceType = m_pResourceModule->createScriptType("ResourceService", "Resource Service", 0);
    m_pResourceServiceType->addMethod("addResourceLocation", "Add a resource location to the path", addResourceLocation);
    m_pResourceServiceType->addMethod("loadResource", "Load a resource", loadResource);

    // Expose I_Resource to the Script Engine
    m_pResourceType = m_pResourceModule->createScriptType("Resource", "Resource", 0);

#if 0 // Moved to RenderingManager
    // Expose I_RenderableResource to the Script Engine
    m_pRenderableResourceType = m_pResourceModule->createScriptType("RenderableResource", "Renderable Resource", 0);
    m_pRenderableResourceType->addMethod("setMaterialName", "Set the material name", setMaterialName);
#endif

#if 0   // deprecated
    m_pRenderableResourceType->addMethod("setNormaliseNormals", "Indicate if the normals should be normalized.  Useful after scaling.", setNormaliseNormals);
#endif  // deprecated

    m_pResourceModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
