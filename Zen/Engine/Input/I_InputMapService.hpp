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
#ifndef ZEN_ENGINE_I_INPUT_MAP_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_I_INPUT_MAP_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>
#include <Zen/Core/Plugins/I_Service.hpp>
#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Input {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_KeyMap;

/// Input Map Service.
/// This service maps key and other input into actions.
class INPUTMANAGER_DLL_LINK I_InputMapService
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_InputMapService>          pScriptObject_type;
    typedef Scripting::ObjectReference<I_InputMapService>   ScriptObjectReference_type;

    typedef Zen::Memory::managed_ptr<I_KeyMap>              pKeyMap_type;
    /// @}

    /// @name I_InputMapService interface
    /// @{
public:
    /// Create an input map.
    virtual pKeyMap_type createKeyMap(const std::string& _name) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "InputMapService"
    /// Override this method if you create a derived type
    const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_InputMapService();
    virtual ~I_InputMapService();
    /// @}

};  // interface I_InputMapService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Input
}   // namespace Engine
namespace Memory 
{
    /// I_InputMapService is managed by a factory
    template<>
	struct is_managed_by_factory<Zen::Engine::Input::I_InputMapService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_I_INPUT_MAP_SERVICE_HPP_INCLUDED
