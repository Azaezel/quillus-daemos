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
#ifndef ZEN_ENGINE_SOUND_SOUND_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_SOUND_SOUND_MANAGER_HPP_INCLUDED

#include "../I_SoundManager.hpp"

#include "../I_SoundServiceFactory.hpp"
#include "../I_SoundService.hpp"

#include <Zen/Core/Plugins/ServiceCache.hpp>

#include <Zen/Core/Scripting/ObjectReference.hpp>
#include <Zen/Core/Scripting/script_module.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptType;
    }   // namespace Scripting
namespace Engine {
namespace Sound {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SoundManager
:   public I_SoundManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Plugins::ServiceCache<I_SoundService, I_SoundServiceFactory>   SoundServiceCache_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>                   pScriptType_type;
    /// @}

    /// @name I_SoundManager implementation
    /// @{
public:
    virtual pService_type create(const std::string& _type, config_type& _config);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultScriptModule();
    /// @}

    /// @name SoundManager implementation
    /// @{
public:
    /// Register a service with a script engine.
    void registerScriptModule();
    /// @}

    /// @name 'Structors
    /// @{
public:
             SoundManager();
    virtual ~SoundManager();
    /// @}

    /// @name Member variables
    /// @{
private:
    SoundServiceCache_type  m_soundServiceCache;

    pScriptEngine_type              m_pDefaultScriptEngine;
    bool                            m_scriptTypesInitialized;

    Zen::Scripting::script_module*	m_pSoundModule;
    /// @}

};  // class SoundManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Sound
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SOUND_SOUND_MANAGER_HPP_INCLUDED
