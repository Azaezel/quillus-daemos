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
#ifndef ZEN_ENGINE_WIDGETS_I_WIDGET_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_WIDGETS_I_WIDGET_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptEngine;
        class I_ScriptModule;
    }   // namespace Scripting
namespace Engine {
namespace Widgets {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_WidgetService;

class WIDGETS_DLL_LINK I_WidgetManager
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine> pScriptEngine_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;

    typedef Zen::Memory::managed_ptr<I_WidgetService>        pService_type;
    typedef Zen::Memory::managed_weak_ptr<I_WidgetService>   wpService_type;

    typedef std::map< std::string, std::string >        config_type;
    /// @}

    /// @name I_WidgetManager interface
    /// @{
public:
    /// Create a widget service by loading a plugin.
    /// @param _type - Type of widget service to create.
    /// @param _config - Additional configuration data required by the widget service.
    ///         The format of this configuration data is defined by the plugin, so read
    ///         your widget service plugin documentation.
    virtual pService_type create(const std::string& _type, config_type& _config, pScriptEngine_type _pScriptEngine) = 0;

    /// Register the default script engine for all widget services.
    /// Every widget service that has been created or is created in the future
    /// will use this script engine.
    /// @param _pEngine NULL to set the default engine to none, but doing so will
    ///             not unregister the script engine to services that have already
    ///             been created.  It will only prevent subsequent services from
    ///             using this script engine.
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine) = 0;

    /// Get the default Widgets script module.
    virtual pScriptModule_type getDefaultScriptModule() = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    /// Get the singleton
    static I_WidgetManager& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WidgetManager();
    virtual ~I_WidgetManager();
    /// @}

};	// interface I_WidgetManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Widgets
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WIDGETS_I_WIDGET_MANAGER_HPP_INCLUDED
