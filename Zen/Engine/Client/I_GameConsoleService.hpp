//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_GAMECLIENT_I_GAME_CONSOLE_SERVICE_HPP_INCLUDED
#define ZEN_GAMECLIENT_I_GAME_CONSOLE_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

#include <string>
#include <map>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif // WIN32

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace GameClient {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_GameClient;

/// Basic game client interface
class GAMECLIENT_DLL_LINK I_GameConsoleService
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
#ifdef _WIN32
    typedef HWND                                    window_handle_type;
#else
    // TODO Support other platforms
#error Unsupported platform!
#endif

    typedef std::map<std::string, std::string>      environment_type;
    /// @}

    /// @name I_GameConsoleService interface
    /// @{
public:
    /// Append environment variables to the game client.
    virtual void appendEnvironment(environment_type& _environment) = 0;

    /// Set the command line variables
    virtual void setCommandLine(int argc, const char* argv[]) = 0;

    /// Load a game client by name
    virtual I_GameClient* loadGameClient(const std::string& _gameName) = 0;

    virtual void unloadGameClient(I_GameClient* _pGameClient) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameConsoleService();
    virtual ~I_GameConsoleService();
    /// @}

};  // interface I_GameConsoleService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameClient
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_GAMECLIENT_I_GAME_CONSOLE_SERVICE_HPP_INCLUDED
