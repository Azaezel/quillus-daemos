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
/// @brief I_GameClientManager interface
///
/// Declaration of the I_GameClientManager interface.
///
///
/// @author Tony Richards trichards@indiezen.com
/// @author Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_CLIENT_I_GAME_CLIENT_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_CLIENT_I_GAME_CLIENT_MANAGER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <string>
#include <map>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifdef WIN32
#include <windows.h>
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_GameClient;

class CLIENT_DLL_LINK I_GameClientManager
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_GameClient>           pService_type;
    typedef Memory::managed_weak_ptr<I_GameClient>      wpService_type;
    typedef std::map< std::string, std::string >        config_type;

    typedef double                                      frameDelta_type;
    typedef Event::Event<frameDelta_type>               frameEvent_type;

    // TODO Move this to a common place.  Utility?  Widgets?  Platform?
#ifdef _WIN32
    typedef HWND                                    windowHandle_type;
#else
    typedef void*                                   windowHandle_type;
#endif
    /// @}

    /// @name I_GameClientManager interface
    /// @{
public:
    virtual pService_type create(const std::string& _type, config_type& _config) = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    /// Get the singleton
    static I_GameClientManager& getSingleton();
    /// @}

    /// @name Events
    /// @{
public:
    frameEvent_type onFrameEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameClientManager();
    virtual ~I_GameClientManager();
    /// @}

};  // interface I_GameClientManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CLIENT_I_GAME_CLIENT_MANAGER_HPP_INCLUDED
