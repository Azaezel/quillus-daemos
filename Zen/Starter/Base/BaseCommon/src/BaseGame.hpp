//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ENGINE_BASE_GAME_HPP_INCLUDED
#define ZEN_ENGINE_BASE_GAME_HPP_INCLUDED

#include "../I_BaseGame.hpp"

#include "BehaviorService.hpp"

#include <Zen/Core/Event/I_EventManager.hpp>

#include <map>
#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameGroup;

/// Base Game
class BaseGame
:   public I_BaseGame
{
    /// @name Types
    /// @{
public:
    typedef Zen::Event::I_EventManager::pEventService_type  pEventService_type;
    /// @}

    /// @name I_BaseGame implemenetation
    /// @{
public:
    virtual bool initPhysicsService(const std::string& _type);
    virtual bool initPhysicsResourceService(const std::string& _type);
    virtual pPhysicsService_type getPhysicsService();
    virtual pPhysicsResourceService_type getPhysicsResourceService();
    virtual pPhysicsZone_type getCurrentPhysicsZone();
    virtual void setCurrentPhysicsZone(pPhysicsZone_type _pPhysicsZone);
    virtual bool initTerrainService(const std::string& _type);
    virtual pTerrainService_type getTerrainService();
    virtual bool initSkyService(const std::string& _type);
    virtual pSkyService_type getSkyService();

    virtual Core::I_GameGroup& getRootGroup();

    virtual Core::I_BehaviorService& getBehaviorService();

    virtual bool initNavigationService(const std::string& _type);
    virtual Navigation::I_NavigationService& getNavigationService();

    virtual void initScriptModule(pScriptEngine_type _pScriptEngine);
    virtual pScriptModule_type getScriptModule();
    virtual void initScriptTypes();
    virtual void createScriptObjects();

    virtual pScriptType_type getGameObjectScriptType();
    virtual pScriptType_type getGameGroupScriptType();
    virtual Event::I_ActionMap& getActionMap();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name 'Structors
    /// @{
public:
             BaseGame();
    virtual ~BaseGame();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    ScriptObjectReference_type*                             m_pScriptObject;

    Core::I_GameGroup*                                      m_pMainGroup;

    pPhysicsZone_type                                       m_pPhysicsZone;
    pPhysicsService_type                                    m_pPhysicsService;
    pPhysicsResourceService_type                            m_pPhysicsResourceService;

    pTerrainService_type                                    m_pTerrainService;

    pSkyService_type                                        m_pSkyService;

    Zen::Memory::managed_ptr<Navigation::I_NavigationService> m_pNavigationService;

    BehaviorService                                         m_behaviorService;

    pScriptEngine_type                                      m_pScriptEngine;
    pScriptModule_type                                      m_pModule;
    pEventService_type                                      m_pEventService;

    pScriptType_type                                        m_pGameScriptType;
    pScriptType_type                                        m_pGameGroupScriptType;
    pScriptType_type                                        m_pGameObjectScriptType;
    pScriptType_type                                        m_pPhysicsZoneScriptType;
    pScriptType_type                                        m_pCollisionShapeScriptType;
    /// @}

};  // class BaseGame

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_BASE_GAME_HPP_INCLUDED
