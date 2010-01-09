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
/// @brief I_GameClientFactory interface
///
/// Declaration of the I_GameClientFactory interface.
///
///
/// @author Tony Richards trichards@indiezen.com
/// @author Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_CLIENT_I_GAME_CLIENT_FACTORY_HPP_INCLUDED
#define ZEN_ENGINE_CLIENT_I_GAME_CLIENT_FACTORY_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Plugins/I_ClassFactory.hpp>

#include <boost/shared_ptr.hpp>

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

class CLIENT_DLL_LINK I_GameClientFactory
:   public Zen::Plugins::I_ClassFactory
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_GameClient>       pGameClient_type;
    typedef Memory::managed_weak_ptr<I_GameClient>  wpGameClient_type;
    typedef std::map< std::string, std::string >    config_type;

    // TODO Move this to a common place.  Utility?  Widgets?  Platform?
#ifdef _WIN32
    typedef HWND                                    WindowHandle_type;
#else
    typedef const char*                             WindowHandle_type;
#endif
    /// @}

    /// @name I_GameClientFactory interface
    /// @{
public:
    /// Create a game client
    /// @param _pParent Handle of the parent window.  If this is NULL
    ///         then an OS window is created.  If not NULL, then
    ///         a child window of _pParent is created.  The created
    ///         window, whether it's a parent or a child, is used
    ///         as the rendering viewport.
    virtual pGameClient_type create(const std::string& _type, WindowHandle_type _pParent, config_type& _config) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameClientFactory();
    virtual ~I_GameClientFactory();
    /// @}

};  // interface I_GameClientFactory

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CLIENT_I_GAME_CLIENT_FACTORY_HPP_INCLUDED
