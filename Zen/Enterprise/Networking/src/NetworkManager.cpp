//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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

#include "NetworkManager.hpp"

#include "../I_NetworkServiceFactory.hpp"

#include <Zen/Core/Plugins/I_PluginManager.hpp>
#include <Zen/Core/Plugins/I_ExtensionRegistry.hpp>
#include <Zen/Core/Plugins/I_Application.hpp>
#include <Zen/Core/Plugins/I_ExtensionQuery.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Networking {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NetworkManager::NetworkManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NetworkManager::~NetworkManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_NetworkService*
NetworkManager::createNetworkService(const std::string& _name)
{
    I_NetworkServiceFactory* pFactory = NULL;

    Plugins::I_ExtensionRegistry& extensionRegistry = Plugins::I_ExtensionRegistry::getSingleton();
    Zen::Plugins::I_ExtensionQuery* const pQuery = extensionRegistry.createQuery();

    pQuery->setNamespace("IndieZen::Networking");
    pQuery->setExtensionPoint("NetworkService");
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

        pFactory = (dynamic_cast<I_NetworkServiceFactory*>(&classFactory));

        I_NetworkService* const pService = pFactory->create(_name);

        m_factories[pService] = pFactory;

        return pService;
    }
    else
    {
        // TODO Throw an exception / Log an error
        std::cout << "NetworkManager::createNetworkService(): Error finding NetworkService for " << _name << std::endl;
    }

    return NULL;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
NetworkManager::destroyNetworkService(I_NetworkService* _pNetworkService)
{
    m_factories[_pNetworkService]->destroy(_pNetworkService);

    // I_Extension needs to implement a return() method and we should
    // keep track of the extension used to create each factory and 
    // destroy the factory here.
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Networking
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
