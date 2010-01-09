//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
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

#include "ZenServer.hpp"

#include <Zen/Core/Plugins/I_Application.hpp>
#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_Configuration.hpp>

#include <Zen/Core/Scripting/I_ScriptingManager.hpp>

#include <Zen/Core/Utility/I_EnvironmentHandler.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>

#include <stddef.h>
#include <conio.h>

namespace po = boost::program_options;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZenServer {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZenServer::ZenServer()
:   m_pAppServer(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZenServer::~ZenServer()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenServer::init(int _argc, const char* _argv[])
{
    // Declare the supported options.
    po::options_description description("Allowed options");

    description.add_options()
        ("help,h", "Produce this help message")
        ("config,c", po::value<std::string>(&m_configPath)->default_value("server.xml"), "Configuration XML file")
        //("lang,l", po::value<std::string>(&m_scriptLanguage)->default_value("python"), "Script language extension")
        //("script,s", po::value<std::string>(&m_defaultScript)->default_value("initServer.py"), "Boot script")
        //("interactive,i", po::value<bool>(&m_bRunInteractive)->default_value(false), "Run interactively")
    ;

    po::store(po::parse_command_line(_argc, (char**)_argv, description), m_variables);
    po::notify(m_variables);

    if (m_variables.size() == 0 || m_variables.count("help") == 1)
    {
        std::cout << std::endl << description << std::endl;
        return false;
    }

    std::map<std::string, std::string> newEnv;

    // Skip the first two arguments
    const int skipArgs = 3;
    for(int x = skipArgs; x < _argc; x++)
    {
        std::stringstream stream;
        stream << "arg" << x - skipArgs;
        newEnv[stream.str()] = _argv[x];
    }

    Zen::Utility::I_EnvironmentHandler& environment = Zen::Utility::I_EnvironmentHandler::getDefaultHandler();

    environment.appendEnvironment(newEnv);

    // Initialize application
    boost::filesystem::path configPath = boost::filesystem::system_complete
        (
            boost::filesystem::path(m_configPath, boost::filesystem::native)
        ).normalize();
    Zen::Plugins::I_PluginManager::app_ptr_type pApp = Zen::Plugins::I_PluginManager::getSingleton().installApplication(configPath);

    Zen::Enterprise::AppServer::I_ApplicationServerManager& manager = 
        Zen::Enterprise::AppServer::I_ApplicationServerManager::getSingleton();

    //~pApp->getConfiguration()->getConfigurationElement("protocols");

    //return initializeScriptEngine();

    // Initialize the main application server
    m_pAppServer = &Zen::Enterprise::AppServer::I_ApplicationServer::getInstance("main");

    pConfig_type pAppConfig = pApp->getConfiguration().getConfigurationElement("application");

    // Load the Fractured Universe login server
    //Zen::Enterprise::AppServer::I_ApplicationServerManager::pApplicationService_type
    //    pLoginService = manager.createApplicationService(*m_pAppServer, "FracULoginServer");

    // Install the applications
    pConfig_type pApplications = pAppConfig->getChild("applications");
    if (pApplications)
    {
        m_pAppServer->installApplications(pApplications);
    }

    // Install the protocols
    pConfig_type pProtocols = pAppConfig->getChild("protocols");

    if (pProtocols)
    {
        m_pAppServer->installProtocols(pProtocols);
    }

    //m_pAppServer->installApplication(pLoginService, manager.createLocation("login"));

    m_pAppServer->start();

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
ZenServer::run()
{
    runScriptInit();

    // TODO integrate with the console and wait until told to stop.

    std::cout << "Running.... press a key to terminate." << std::endl;

    getch();

    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenServer::initializeScriptEngine()
{
    m_pScriptEngine = Scripting::I_ScriptingManager::getSingleton().createScriptEngine(m_scriptLanguage);

    if (m_pScriptEngine == NULL)
    {
        std::cout << "Error loading script engine: " << std::endl;
        return false;
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ZenServer::runScriptInit()
{
    if (m_pScriptEngine != NULL)
    {
        if (!m_pScriptEngine->executeScript(m_defaultScript))
        {
            std::cout << "Error loading script " << m_defaultScript << std::endl;
            return false;
        }
    }

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZenServer
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
