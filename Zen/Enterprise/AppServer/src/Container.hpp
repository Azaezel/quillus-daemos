//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Server
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
#ifndef ZEN_ENTERPRISE_APPSERVER_CONTAINER_HPP_INCLUDED
#define ZEN_ENTERPRISE_APPSERVER_CONTAINER_HPP_INCLUDED

#include "../I_Container.hpp"

#include <Zen/Core/Plugins/I_ConfigurationElement.hpp>

#include <Zen/Core/Scripting/I_ScriptingManager.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>

#include <Zen/Enterprise/AppServer/I_ApplicationServerManager.hpp>
#include <Zen/Enterprise/AppServer/I_ProtocolService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationService.hpp>
#include <Zen/Enterprise/AppServer/I_ApplicationServer.hpp>
#include <Zen/Enterprise/AppServer/I_ResourceLocation.hpp>

#include <boost/program_options.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Enterprise {
namespace AppServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

/// Container for Application Server(s)
class Container
:   public I_Container
{
    /// @name Types
    /// @{
public:
    typedef Scripting::I_ScriptingManager::pScriptEngine_type   pScriptEngine_type;

    typedef boost::program_options::variables_map   variables_map_type;

    typedef Plugins::I_ConfigurationElement::const_ptr_type pConfig_type;
    /// @}

    /// @name I_Container implementation
    /// @{
public:
    /// Initialize the server
    virtual bool init(int _argc, const char* _argv[]);

    /// Run the server
    virtual int run();

    virtual I_ApplicationServer& getApplicationServer();
    /// @}

    /// @name Container implementation
    /// @{
private:
    bool initializeScriptEngine();
    bool runScriptInit();
    /// @}

    /// @name 'Structors
    /// @{
public:
             Container();
    virtual ~Container();
    /// @}

    /// @name Member Variables
    /// @{
private:
    /// Variables from the command line
    variables_map_type          m_variables;
    
    /// Application configuration path
    std::string                 m_configPath;

    /// Script language plugin to load
    std::string                 m_scriptLanguage;

    /// Default script file to load
    std::string                 m_defaultScript;

    /// Run interactively after loading the script?
    bool                        m_bRunInteractive;

    pScriptEngine_type          m_pScriptEngine;

    /// Primary application server
    I_ApplicationServer*    m_pAppServer;

    /// @}

};  // class Container

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace AppServer
}   // namespace Enterprise
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENTERPRISE_APPSERVER_CONTAINER_HPP_INCLUDED
