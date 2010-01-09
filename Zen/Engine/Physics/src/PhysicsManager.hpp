//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_PHYSICS_PHYSICS_MANAGER_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_PHYSICS_MANAGER_HPP_INCLUDED

#include "../I_PhysicsManager.hpp"

#include "../I_PhysicsServiceFactory.hpp"
#include "../I_PhysicsService.hpp"

#include <Zen/Core/Plugins/ServiceCache.hpp>

#include <Zen/Core/Scripting/ObjectReference.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Scripting {
        class I_ScriptType;
    }   // namespace Scripting
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsManager
:   public I_PhysicsManager
{
    /// @name Types
    /// @{
public:
    typedef Zen::Plugins::ServiceCache<I_PhysicsService, I_PhysicsServiceFactory>    PhysicsServiceCache_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptType>                   pScriptType_type;
    /// @}

    /// @name I_PhysicsManager implementation
    /// @{
public:
    virtual pPhysicsService_type create(const std::string& _type, config_type& _config);
    virtual void registerDefaultScriptEngine(pScriptEngine_type _pEngine);
    virtual pScriptModule_type getDefaultScriptModule();
    /// @}

    /// @name PhysicsManager implementation
    /// @{
public:
    void onFrame(FrameDelta_type frameDelta);

    /// Register the script classes with the scripting engine
    void registerScriptTypes(pScriptEngine_type _pEngine);

    /// Register a service with a script engine
    void registerScriptEngine(pScriptEngine_type _pEngine, pPhysicsService_type _pService);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsManager();
    virtual ~PhysicsManager();
    /// @}

    /// @name Member Variables
    /// @{
private:
    PhysicsServiceCache_type        m_physicsServiceCache;

    pScriptEngine_type              m_pDefaultScriptEngine;
    bool                            m_scriptTypesInitialized;

    pScriptModule_type              m_pPhysicsModule;
    pScriptType_type                m_pPhysicsServiceType;
    pScriptType_type                m_pPhysicsZoneType;
    pScriptType_type                m_pCollisionShapeType;

    /// @}

};  // class PhysicsManager

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_PHYSICS_MANAGER_HPP_INCLUDED

