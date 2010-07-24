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
#ifndef ZEN_ENGINE_WIDGETS_WIDGETS_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_WIDGETS_WIDGETS_MANAGER_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

#include "../I_WidgetManager.hpp"

#include "../I_WidgetServiceFactory.hpp"
#include "../I_WidgetService.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Plugins/ServiceCache.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Widgets {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class WidgetManager
:   public I_WidgetManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Scripting::ObjectReference<I_WidgetService>    ObjectReference_type;
    typedef Zen::Memory::managed_ptr<ObjectReference_type>      pObjectReference_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>   pScriptType_type;
    typedef Zen::Plugins::ServiceCache<I_WidgetService>         ServiceCache_type;
    /// @}

    /// @name I_WidgetManager implementation
    /// @{
public:
    virtual pService_type create(const std::string& _type, config_type& _config, pScriptEngine_type _pScriptEngine);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultScriptModule();
    /// @}

    /// @name WidgetManager implementation
    /// @{
private:
    void registerWidgetsScriptModule();
    /// @}

    /// @name 'Structors
    /// @{
public:
             WidgetManager();
    virtual ~WidgetManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ServiceCache_type               m_serviceCache;

    pScriptEngine_type              m_pDefaultScriptEngine;

    bool                            m_scriptTypesInitialized;

    Zen::Scripting::script_module*  m_pWidgetsModule;
    /// @}

};  // class WidgetManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Widgets
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WIDGETS_WIDGETS_MANAGER_HPP_INCLUDED
