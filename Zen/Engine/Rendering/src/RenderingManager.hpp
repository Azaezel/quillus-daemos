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
#ifndef ZEN_ENGINE_RENDERING_RENDERING_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_RENDERING_RENDERING_MANAGER_HPP_INCLUDED

#include "../I_RenderingManager.hpp"

#include "../I_RenderingServiceFactory.hpp"
#include "../I_RenderingService.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Plugins/ServiceCache.hpp>

#include <Zen/Core/Scripting/ObjectReference.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptType;
    }   // namespace Scripting
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RenderingManager
:   public I_RenderingManager
{
    /// @name Types
    /// @{
public:
    typedef Scripting::ObjectReference<I_RenderingService>                      renderingServiceObjectReference_type;
    typedef Zen::Memory::managed_ptr<renderingServiceObjectReference_type>      pRenderingServiceObjectReference_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>                   pScriptType_type;
    typedef Zen::Plugins::ServiceCache<I_RenderingService>                      RenderingServiceCache_type;
    /// @}

    /// @name I_RenderingManager implementation
    /// @{
public:
    virtual pRenderingService_type create(const std::string& _type);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultScriptModule();
    /// @}

    /// @name Additional Implementation
    /// @{
private:
    /// Register the script classes with the scripting engine
    void registerScriptTypes(pScriptEngine_type _pEngine);

    /// Register a service with a script engine
    void registerScriptEngine(pScriptEngine_type _pEngine, pRenderingService_type _pService);
    /// @}

    /// @name 'Structors
    /// @{
public:
             RenderingManager();
    virtual ~RenderingManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    RenderingServiceCache_type      m_renderingServiceCache;

    pScriptEngine_type              m_pDefaultScriptEngine;

    bool                            m_scriptTypesInitialized;

    pScriptModule_type              m_pRenderingModule;
    pScriptType_type                m_pRenderingServiceType;
    pScriptType_type                m_pSceneServiceType;
    pScriptType_type                m_pSceneNodeType;
    pScriptType_type                m_pRenderableResourceType;
    pRenderingServiceObjectReference_type   m_pRenderingServiceObjectReference;
    /// @}

};	// class RenderingManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_RENDERING_MANAGER_HPP_INCLUDED
