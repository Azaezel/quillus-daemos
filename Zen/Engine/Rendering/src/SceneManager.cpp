//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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

#include "SceneManager.hpp"

#include <Zen/Engine/Rendering/I_SceneServiceFactory.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>

#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneManager::SceneManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneManager::~SceneManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SceneManager::pSceneService_type
SceneManager::create(const std::string& _type)
{
    Threading::CriticalSection guard(m_sceneServiceCache.getLock());

    pSceneService_type pService(m_sceneServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_SceneServiceFactory* pFactory = m_sceneServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        std::stringstream errorMessage;
        errorMessage << "Zen::Rendering::SceneManager::create() : Error: could not create service factory for type " << _type;

        throw Utility::runtime_exception(errorMessage.str());
    }

    pService = m_sceneServiceCache.cacheService(_type, pFactory->create());

    if (m_pDefaultScriptEngine.isValid())
    {
        registerScriptEngine(m_pDefaultScriptEngine, pService);
    }

    return pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    ///registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if(!m_pDefaultScriptEngine.isValid())
    {
        Threading::CriticalSection guard(m_sceneServiceCache.getLock());

        for(scene_service_cache_type::iterator iter = m_sceneServiceCache.begin(); iter != m_sceneServiceCache.end(); iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneManager::pScriptModule_type
SceneManager::getDefaultScriptModule()
{
    return m_pModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneManager::registerScriptEngine(pScriptEngine_type _pEngine, pSceneService_type _pService)
{
    // Allow the scene service to append it's own meta data.
    _pService->registerScriptEngine(_pEngine);

    // 
    //new I_RenderingService::ScriptObjectReference_type(m_pRenderingModule, m_pRenderingServiceType, _pService, "renderingService");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
