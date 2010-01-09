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
/// @brief GameClientManager interface
///
/// Definition of the GameClientManager interface.
///
///
/// @author Tony Richards trichards@indiezen.com
/// @author Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "GameClientManager.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClientManager::GameClientManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameClientManager::~GameClientManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_GameClientManager::pService_type
GameClientManager::create(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(this->m_serviceCache.getLock());

    // Check to see if the service already exists in the service cache
    {
        pService_type pService(this->m_serviceCache.getCachedService(_type));

        if( pService.isValid() )
        {
            return pService;
        }
    }

    // Else, get the service factory
    I_GameClientFactory* pFactory = this->m_serviceCache.getFactory(_type);

    if( pFactory == NULL )
    {
        Utility::runtime_exception("GameClientManager::create(): Error, could not get factory.");
    }

    windowHandle_type handle = NULL;

    // Check to see if the desired parent service exists in the service cache
    config_type::iterator iter = _config.find("parent");
    if( iter != _config.end() )
    {
        pService_type pService(this->m_serviceCache.getCachedService(iter->second));

        if( pService.isValid() )
        {
            handle = pService->getHandle();
        }
        else
        {
            Utility::runtime_exception("GameClientManager::create(): Error, parent window does not exist in service cache.");
        }
    }

    // Create and cache the service
    pService_type pService(this->m_serviceCache.cacheService( _type, pFactory->create(_type, handle, _config) ));

    // Connect GameClientManager::onFrame() to the GameClient onFrameEvent
    pService->onFrameEvent.connect(boost::bind(&GameClientManager::onFrame,this,_1));

    // Create and cache the service
    return pService;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameClientManager::onFrame(frameDelta_type frameDelta)
{
    // Fire the GameClientManager::onFrameEvent event
    this->onFrameEvent(frameDelta);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
