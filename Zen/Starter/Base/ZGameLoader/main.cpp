//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
//	Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include <Zen/Core/Utility/I_EnvironmentHandler.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Scripting/I_ScriptingManager.hpp>
#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>

#include <Zen/Engine/Client/I_GameClient.hpp>
#include <Zen/Engine/Client/I_GameClientFactory.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>

#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include <stdexcept>
#include <iostream>

#include <stddef.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Scripting::I_ScriptingManager::pScriptModule_type sm_pModule;
static Zen::Scripting::I_ScriptModule::pScriptType_type sm_pEnvironmentType;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ZenRoot
{
public:
    typedef Zen::Memory::managed_ptr<ZenRoot>   pScriptObject_type;

    ZenRoot()
    {
    }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ZenEnvironment
{
public:
    typedef Zen::Memory::managed_ptr<ZenEnvironment>   pScriptObject_type;

    ZenEnvironment()
    {
    }

    static std::string get(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        std::string value = boost::any_cast<std::string>(_parms[0]);

        return Zen::Utility::I_EnvironmentHandler::getDefaultHandler().getEnvironment(value);
    }

    static void set(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
    {
        std::string name = boost::any_cast<std::string>(_parms[0]);
        std::string value = boost::any_cast<std::string>(_parms[1]);

        Zen::Utility::I_EnvironmentHandler::getDefaultHandler().setEnvironment(name, value);
    }

};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static Zen::Memory::managed_ptr<ZenEnvironment> gZenEnvironment(new ZenEnvironment);
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Scripting::I_ObjectReference*
getEnvironment(Zen::Scripting::I_ObjectReference* _pObject)
{
    Zen::Scripting::ObjectReference<ZenRoot>* pObject = dynamic_cast<Zen::Scripting::ObjectReference<ZenRoot>*>(_pObject);

    ZenRoot* pZenObject = pObject->getRawObject();

    Zen::Scripting::ObjectReference<ZenEnvironment>* pScriptObject =
        new Zen::Scripting::ObjectReference<ZenEnvironment>(sm_pModule, sm_pEnvironmentType, gZenEnvironment);

    return pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Engine::Client::I_GameClientFactory::pGameClient_type
getGameClient(const std::string& _name)
{
    // First, load the plugin.
    Zen::Plugins::I_PluginManager::getSingleton().installPlugin(_name);

    Zen::Plugins::I_ExtensionRegistry& extensionRegistry = Zen::Plugins::I_ExtensionRegistry::getSingleton();
    Zen::Plugins::I_ExtensionQuery* const pQuery = extensionRegistry.createQuery();

    pQuery->setNamespace("Zen::Engine::Client");
    pQuery->setExtensionPoint("GameClient");
    pQuery->setType(_name);

    // Get the extensions
    // Note that the result set takes ownership of pQuery
    Zen::Plugins::I_ExtensionRegistry::extension_result_set_ptr_type pExtensions = extensionRegistry.findExtensions(pQuery);

    // Grab the first extension; if there are more installed then eventually we can
    // deal with that but for now just grab one.
    Zen::Plugins::I_ExtensionRegistry::extension_result_set_type::iterator pExtensionIter = pExtensions->begin();

    if (pExtensionIter != pExtensions->end())
    {
        Zen::Plugins::I_ExtensionRegistry::class_factory_ref_type
            classFactory(extensionRegistry.getClassFactory(*pExtensionIter));

        Zen::Engine::Client::I_GameClientFactory*
            pFactory(dynamic_cast<Zen::Engine::Client::I_GameClientFactory*>(&classFactory));

        Zen::Engine::Client::I_GameClientFactory::config_type config;
        Zen::Engine::Client::I_GameClientFactory::pGameClient_type pGameClient = pFactory->create(_name, NULL, config);

        pGameClient->setSelfReference(pGameClient.getWeak());

        return pGameClient;
    }
    else
    {
        // TODO Throw an exception / Log an error
        std::cout << "getGameClient(): Error finding game client " << _name << std::endl;
    }

    // TODO Throw an exception
    throw Zen::Utility::runtime_exception("Error");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// TODO This type should be Starter.Base I_GameClient::ptr_type
static Zen::Engine::Client::I_GameClientFactory::pGameClient_type sm_pGameClient;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
Zen::Scripting::I_ObjectReference*
createGameClient(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
{
    Zen::Scripting::ObjectReference<ZenRoot>* pObject = dynamic_cast<Zen::Scripting::ObjectReference<ZenRoot>*>(_pObject);

    std::string gameClientName = boost::any_cast<std::string>(_parms[0]);

    // Create the game client
    sm_pGameClient = getGameClient(gameClientName);

    // Activate the game client script module so that it's accessible via script.
    sm_pGameClient->activateGameClientScriptModule();

    return sm_pGameClient->getScriptObject();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
void
gameClientInit(Zen::Scripting::I_ObjectReference* _pObject)
{
    Zen::Engine::Client::I_GameClient::ScriptObjectReference_type* pObject =
        dynamic_cast<Zen::Engine::Client::I_GameClient::ScriptObjectReference_type*>(_pObject);

    pObject->getObject()->init();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static
void
gameClientRun(Zen::Scripting::I_ObjectReference* _pObject)
{
    Zen::Engine::Client::I_GameClient::ScriptObjectReference_type* pObject =
        dynamic_cast<Zen::Engine::Client::I_GameClient::ScriptObjectReference_type*>(_pObject);

    // Zen::Engine::Client::I_GameClient* pGameClient = pObject->getObject().get();

    pObject->getObject()->run();
    //pGameClient->run();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int main(int argc, const char* argv[])
{
    if (argc < 3 || argc == 4)
    {
        std::cout << "Usage: ZGameLoader Config.xml GameName [script engine] [init script] [other game args]" << std::endl;
        return 1;
    }

    try
    {
        std::map<std::string, std::string> newEnv;
        boost::filesystem::path currentPath = boost::filesystem::current_path();
        newEnv["gameRoot"] = currentPath.string();

        for(int x = 0; x < argc; x++)
        {
            std::stringstream stream;
            stream << "arg" << x;
            newEnv[stream.str()] = argv[x];
        }

        Zen::Utility::I_EnvironmentHandler& environment = Zen::Utility::I_EnvironmentHandler::getDefaultHandler();

        // Install the application
        boost::filesystem::path configPath = boost::filesystem::system_complete
            (
                boost::filesystem::path(argv[1], boost::filesystem::native)
            ).normalize();

        Zen::Plugins::I_PluginManager::getSingleton().setRootPath('~', currentPath);
        Zen::Plugins::I_PluginManager::app_ptr_type pApp = Zen::Plugins::I_PluginManager::getSingleton().installApplication(configPath);


        environment.appendEnvironment(newEnv);
        environment.setCommandLine(argc, argv);

        if (argc >= 5)
        {
            Zen::Engine::Base::I_BaseGameClient& base = Zen::Engine::Base::I_BaseGameClient::getSingleton();

            base.initScriptEngine(argv[3]);

            Zen::Scripting::I_ScriptEngine& scriptEngine = base.getScriptEngine();

            sm_pModule = scriptEngine.createScriptModule("ZenModule", "Zen Module");

            Zen::Scripting::I_ScriptModule::pScriptType_type
                pZenType = sm_pModule->createScriptType("ZenType", "IndieZen Class", 0);
            pZenType->addMethod("getEnvironment", "Get the Operating System Environment", getEnvironment);
            pZenType->addMethod("createGameClient", "Create a Game Client", createGameClient);

            sm_pEnvironmentType = sm_pModule->createScriptType("Environment", "Zen Core Environment", 0);
            sm_pEnvironmentType->addMethod("get", "Get a value from the environment", ZenEnvironment::get);
            sm_pEnvironmentType->addMethod("set", "Set a value in the environment", ZenEnvironment::set);

            // Activate the ZenModule
            sm_pModule->activate();

            // Go ahead and add run() to the GameClient, but it won't be activated until
            // the I_GameClient is created by ZenType::createGameClient()
            base.getGameClientScriptType()->addMethod("run", "Execute the game client's main loop", gameClientRun);

            Zen::Memory::managed_ptr<ZenRoot> pZenRootObject(new ZenRoot);
            Zen::Scripting::ObjectReference<ZenRoot>* pScriptObject =
                new Zen::Scripting::ObjectReference<ZenRoot>(sm_pModule, pZenType, pZenRootObject, "Zen");

            scriptEngine.executeScript(argv[4]);
        }
        else
        {

            // Install / get the game client extension
            Zen::Engine::Client::I_GameClientFactory::pGameClient_type pClient = getGameClient(argv[2]);

            pClient->init();

            // Run the game client
            pClient->run();
        }
    }
    catch(Zen::Utility::runtime_exception& ex)
    {
        std::cout << ex.what() << std::endl;
#ifdef WIN32
        ::MessageBoxA(NULL, ex.what(), "Error", MB_ICONEXCLAMATION | MB_OK);
#endif
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
#ifdef WIN32
        ::MessageBoxA(NULL, ex.what(), "Error", MB_ICONEXCLAMATION | MB_OK);
#endif
    }
    catch(...)
    {
        std::cout << "ERROR: Caught unknown exception." << std::endl;
    }

    return 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
