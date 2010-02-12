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
#ifndef ZEN_ENGINE_INPUT_MANAGER_I_INPUT_SERVICE_FACTORY_HPP_INCLUDED
#define ZEN_ENGINE_INPUT_MANAGER_I_INPUT_SERVICE_FACTORY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Plugins/I_ClassFactory.hpp>

#include <boost/shared_ptr.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_InputService;

class INPUTMANAGER_DLL_LINK I_InputServiceFactory
:   public  Plugins::I_ClassFactory
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<I_InputService>        pService_type;
    typedef Zen::Memory::managed_weak_ptr<I_InputService>   wpService_type;
    typedef std::map<std::string, std::string>              config_type;
    /// @}

    /// @name I_InputServiceFactory interface
    /// @{
public:
    /// Create an input service.
    /// @param _type The type of the input service. Examples of supported types include 
    ///             "keyboard", "mouse", "joystick", and even "Space Pilot".
    ///             Specialized devices like the Space Pilot require additional plugin support.
    /// @param _config Configuration for the service. All ZInput devices require a "WINDOW"
    ///             parameter that has a value of the HWND from I_Context.
    /// @return Pointer to an input service.
    virtual pService_type create(const std::string& _type, config_type& _config) = 0;

    /// @name 'Structors
    /// @{
protected:
             I_InputServiceFactory();
    virtual ~I_InputServiceFactory();
    /// @}

};  // interface I_InputServiceFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_INPUT_MANAGER_I_INPUT_SERVICE_FACTORY_HPP_INCLUDED
