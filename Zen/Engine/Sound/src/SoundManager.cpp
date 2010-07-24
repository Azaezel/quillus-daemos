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

#include "SoundManager.hpp"

#include "../I_SoundService.hpp"

#include <Zen/Core/Scripting.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Sound {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundManager::SoundManager()
:   m_pSoundModule(NULL)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundManager::~SoundManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_SoundManager::pService_type
SoundManager::create(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_soundServiceCache.getLock());

    pService_type pService(m_soundServiceCache.getCachedService(_type));

    if( pService.isValid() )
    {
        return pService;
    }

    I_SoundServiceFactory* pFactory = m_soundServiceCache.getFactory(_type);

    if( pFactory == NULL )
    {
        throw Zen::Utility::runtime_exception("SoundManager::create() : Error finding sound service factory.");
    }

    pService = pFactory->create(_type, _config);

    if (!pService.isValid())
    {
        throw Zen::Utility::runtime_exception("SoundManager::create() : Sound factory create() did not return a valid pointer.");
    }

    if (m_pSoundModule != NULL)
    {
        pService->registerScriptModule(*m_pSoundModule);
    }

    return m_soundServiceCache.cacheService(_type, pService);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundManager::pScriptModule_type
SoundManager::getDefaultScriptModule()
{
    if (m_pSoundModule != NULL)
    {
        return m_pSoundModule->getScriptModule();
    }

    return pScriptModule_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundManager::registerScriptModule()
{
    Threading::CriticalSection guard(m_soundServiceCache.getLock());

    for(SoundServiceCache_type::iterator iter = m_soundServiceCache.begin(); iter != m_soundServiceCache.end(); iter++)
    {
		iter->second->registerScriptModule(*m_pSoundModule);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || !_pEngine.isValid())
        return;

    // Create a Sound module
    m_pSoundModule = new Scripting::script_module(_pEngine, "Sound", "Zen Sound Module");

    // Expose I_SoundService to the Script Engine
    m_pSoundModule->addType<I_SoundService>("SoundService", "SoundService")
    ;

    // TODO Don't activate?  Give the service an opportunity to add more methods.
    // TODO Eventually support multiple derived script types, which means we
    // never activate this module.  Instead, the meta data is maintained and
    // derived types copy that meta data, add more methods, and then activates the
    // new derived class.
    m_pSoundModule->activate();

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = true;

    registerScriptModule();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Sound
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
