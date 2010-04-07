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

#include <Zen/Core/Scripting.hpp>

#include "../I_RenderingManager.hpp"

#include "../I_RenderingServiceFactory.hpp"
#include "../I_RenderingService.hpp"

#include <Zen/Core/Plugins/ServiceCache.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Rendering {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class RenderingManager
:   public I_RenderingManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Plugins::ServiceCache<I_RenderingService>                      RenderingServiceCache_type;
    /// @}

    /// @name I_RenderingManager implementation
    /// @{
public:
    virtual pRenderingService_type create(const std::string& _type);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultRenderingScriptModule();
    /// @}

    /// @name Additional Implementation
    /// @{
private:
    /// Register the script module with all of the previously created rendering services.
    void registerRenderingScriptModule();
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

    Zen::Scripting::script_module*  m_pRenderingModule;
    /// @}

};	// class RenderingManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Rendering
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RENDERING_RENDERING_MANAGER_HPP_INCLUDED
