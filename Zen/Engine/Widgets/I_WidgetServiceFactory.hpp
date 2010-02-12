//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)		2009 Denis Hilliard
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
#ifndef ZEN_ENGINE_WIDGETS_I_WIDGET_SERVICE_FACTORY_HPP_INCLUDED
#define ZEN_ENGINE_WIDGETS_I_WIDGET_SERVICE_FACTORY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Plugins/I_ClassFactory.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Widgets {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_WidgetService;

class WIDGETS_DLL_LINK I_WidgetServiceFactory
:   public Plugins::I_ClassFactory
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_WidgetService>                    pWidgetService_type;
    typedef Memory::managed_weak_ptr<I_WidgetService>               wpWidgetService_type;
    typedef std::map< std::string, std::string >                    config_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptEngine>     pScriptEngine_type;
    /// @}

    /// @name I_WidgetServiceFactory interface
    /// @{
public:
    /// @param _pScriptEngine - The script engine to which the widget service should bind.
    virtual pWidgetService_type create(const std::string& _type, config_type& _config, pScriptEngine_type _pScriptEngine) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_WidgetServiceFactory();
    virtual ~I_WidgetServiceFactory();
    /// @}

};	// interface I_WidgetServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Widgets
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_WIDGETS_I_WIDGET_SERVICE_FACTORY_HPP_INCLUDED
