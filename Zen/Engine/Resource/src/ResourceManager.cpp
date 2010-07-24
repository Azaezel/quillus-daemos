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

    pService = pFactory->create(_type);

    if (m_pDefaultScriptEngine.isValid())
    {
        pService->registerScriptModule(*m_pResourceModule);
    }

    m_resourceServiceCache.cacheService(_type, pService);

    return pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceManager::pScriptModule_type
ResourceManager::getDefaultResourceScriptModule()
{
    return m_pResourceModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    m_pResourceModule = new Zen::Scripting::script_module(_pEngine, "Resource", "Zen Resource Module");

    m_pResourceModule->addType<I_ResourceService>("ResourceService", "Resource Service")
        .addMethod("addResourceLocation", &I_ResourceService::addResourceLocation)
        .addMethod("loadResource", &I_ResourceService::loadResource)
    ;

    m_pResourceModule->addType<I_Resource>("Resource", "Resource")
    ;

    m_pResourceModule->addType<Engine::Rendering::I_RenderableResource>("RenderableResource", "Renderable Resource")
    ;

#if 0 // Moved to RenderingManager
    // Expose I_RenderableResource to the Script Engine
    m_pRenderableResourceType = m_pResourceModule->createScriptType("RenderableResource", "Renderable Resource", 0);
    m_pRenderableResourceType->addMethod("setMaterialName", "Set the material name", setMaterialName);
#endif

#if 0   // deprecated
    m_pRenderableResourceType->addMethod("setNormaliseNormals", "Indicate if the normals should be normalized.  Useful after scaling.", setNormaliseNormals);
#endif  // deprecated

    m_pResourceModule->activate();

    m_scriptTypesInitialized = true;
    m_pDefaultScriptEngine = _pEngine;

    registerResourceScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceManager::registerResourceScriptModule()
{
    Threading::CriticalSection guard(m_resourceServiceCache.getLock());

    for(ResourceServiceCache_type::iterator iter = m_resourceServiceCache.begin(); iter != m_resourceServiceCache.end(); iter++)
    {
        iter->second->registerScriptModule(*m_pResourceModule);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
