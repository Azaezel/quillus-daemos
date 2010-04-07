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

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Rendering/I_RenderingServiceFactory.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>
#include <Zen/Engine/Rendering/I_SceneService.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Rendering/I_RenderingService.hpp>
#include <Zen/Engine/Rendering/I_Context.hpp>
#include <Zen/Engine/Rendering/I_View.hpp>
#include <Zen/Engine/Rendering/I_Canvas.hpp>
#include <Zen/Engine/Rendering/I_RenderingCanvas.hpp>
#include <Zen/Engine/Rendering/I_Camera.hpp>

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

    pService = pFactory->create();

    if (m_pDefaultScriptEngine.isValid())
    {
        pService->registerScriptModule(*m_pRenderingModule);
    }

    m_renderingServiceCache.cacheService(_type, pService);

    return pService;

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
RenderingManager::pScriptModule_type
RenderingManager::getDefaultRenderingScriptModule()
{
    return m_pRenderingModule->getScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    // Create a Rendering module
    m_pRenderingModule = new Zen::Scripting::script_module(_pEngine, "Rendering", "Zen Rendering Module");

    m_pRenderingModule->addType<I_RenderingService>("RenderingService", "RenderingService")
        .addMethod("createContext", &I_RenderingService::createContext)
        .addMethod("createView", &I_RenderingService::createView)
    ;

    m_pRenderingModule->addType<I_Context>("Context", "Context")
        .addMethod("setFullScreen", &I_Context::setFullScreen)
        //.addMethod("isFullScreen", &I_Context::isFullScreen)
        //.addMethod("getParentWindow", &I_Context::getParentWindow)
        //.addMethod("getWindow", &I_Context::getWindow)
    ;

    m_pRenderingModule->addType<I_View>("View", "View")
        //.addMethod("setCamera", &I_View::setCamera)
        .addMethod("createSubView", &I_View::createSubView)
        //.addMethod("getCanvas", &I_View::getCanvas)
        //.addMethod("initCanvas", &I_View::initCanvas)
    ;

    m_pRenderingModule->addType<I_Canvas>("Canvas", "Canvas")
        //.addMethod("setSceneGraph", &I_Canvas::setSceneGraph)
    ;

    m_pRenderingModule->addType<I_RenderingCanvas>("RenderingCanvas", "Rendering Canvas")
        //.addMethod("setSceneGraph", &I_RenderingCanvas::setSceneGraph)
        .addMethod("createCamera", &I_RenderingCanvas::createCamera)
        .addMethod("selectCamera", &I_RenderingCanvas::selectCamera)
        //.addMethod("getWidth", &I_RenderingCanvas::getWidth)
        //.addMethod("getHeight", &I_RenderingCanvas::getHeight)
    ;

    m_pRenderingModule->addType<I_Camera>("Camera", "Camera")
        //.addMethod("getName", &I_Camera::getName)
        .addMethod("setAspectRatio", &I_Camera::setAspectRatio)
        .addMethod("setHorizontalFOV", &I_Camera::setHorizontalFOV)
        .addMethod("setNearClipDistance", &I_Camera::setNearClipDistance)
        .addMethod("setFarClipDistance", &I_Camera::setFarClipDistance)
    ;

    m_pRenderingModule->activate();

    m_scriptTypesInitialized = true;
    m_pDefaultScriptEngine = _pEngine;

    registerRenderingScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
RenderingManager::registerRenderingScriptModule()
{
    Threading::CriticalSection guard(m_renderingServiceCache.getLock());

    for(RenderingServiceCache_type::iterator iter = m_renderingServiceCache.begin(); iter != m_renderingServiceCache.end(); iter++)
    {
        iter->second->registerScriptModule(*m_pRenderingModule);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
