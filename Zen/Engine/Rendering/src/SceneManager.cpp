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

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Rendering/I_SceneServiceFactory.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_Light.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>

#include <Zen/Core/Plugins/I_Configuration.hpp>
#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneManager::SceneManager()
:   m_scriptTypesInitialized(false)
,   m_pSceneModule(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneManager::~SceneManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SceneManager::pSceneService_type
SceneManager::create(const std::string& _type, const std::string& _sceneName, const int _sceneType)
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

    pService = pFactory->create(_sceneName, _sceneType);

    if (m_pSceneModule)
    {
        std::cout << "Registering script engine with scene service" << std::endl;
        pService->registerScriptModule(*m_pSceneModule);
    }

    std::cout << "Cached scene service " << _type << "::" << _sceneName << std::endl;
    m_sceneServiceCache.cacheService(_type, pService);

    std::cout << "Returning scene service." << std::endl;
    return pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SceneManager::pScriptModule_type
SceneManager::getDefaultSceneScriptModule()
{
    return m_pSceneModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    if(m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    // Create a Scene module
    m_pSceneModule = new Zen::Scripting::script_module(_pEngine, "Scene", "Zen Scene Module");

    m_pSceneModule->addType<I_SceneService>("SceneService", "SceneService")
        .addMethod("setAmbientLight", &I_SceneService::setAmbientLight)
        .addMethod("createLight", &I_SceneService::createLight)
        .addMethod("createChildNode", &I_SceneService::createChildNode)
    ;

    m_pSceneModule->addType<I_Light>("Light", "Light")
        .addMethod("setPosition", &I_Light::setPosition)
        .addMethod("setDirection", &I_Light::setDirection)
        .addMethod("setDiffuse", &I_Light::setDiffuse)
        .addMethod("setSpecular", &I_Light::setSpecular)
        .addMethod("setAttenuation", &I_Light::setAttenuation)
        .addMethod("setRange", &I_Light::setRange)
        .addMethod("setType", &I_Light::setType)
        .addMethod("enableShadowCasting", &I_Light::enableShadowCasting)
        .addMethod("setPower", &I_Light::setPower)
    ;

    m_pSceneModule->activate();

    m_scriptTypesInitialized = true;
    m_pDefaultScriptEngine = _pEngine;

    registerSceneScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SceneManager::registerSceneScriptModule()
{
    Threading::CriticalSection guard(m_sceneServiceCache.getLock());

    for(SceneServiceCache_type::iterator iter = m_sceneServiceCache.begin(); iter != m_sceneServiceCache.end(); iter++)
    {
        iter->second->registerScriptModule(*m_pSceneModule);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
