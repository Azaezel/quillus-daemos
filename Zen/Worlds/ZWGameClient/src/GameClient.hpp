//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Worlds Game Client
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZEN_CYBER_TOWER_DEFENSE_GAME_CLIENT_HPP_INCLUDED
#define ZEN_CYBER_TOWER_DEFENSE_GAME_CLIENT_HPP_INCLUDED

#include <Zen/Engine/Client/I_GameClient.hpp>
#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <Zen/Engine/Camera/I_CameraService.hpp>
#include <Zen/Engine/Camera/I_Camera.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>

#include <Zen/Engine/Input/I_MouseClickEvent.hpp>

#include <Zen/Engine/Navigation/I_NodeComparator.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>
#include <Zen/Starter/Base/BaseCommon/I_BaseGame.hpp>

#include <boost/any.hpp>

#include <Ogre.h>

#include <list>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GUIManager;
class GameObject;
class TowerObject;
class GameGrid;
class GridObject;
class SimpleBrain;
class CreepManager;

class GameClient
:   public Zen::Engine::Client::I_GameClient
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Scripting::I_ScriptType>                    pScriptType_type;
    typedef Zen::Engine::Physics::I_PhysicsWorld::pPhysicsMaterial_type     pPhysicsMaterial_type;
    typedef std::list<GameObject*>                                          GameObjects_type;

    typedef Memory::managed_ptr<Engine::Navigation::I_NodeComparator>       pComparator_type;
    typedef Memory::managed_weak_ptr<Engine::Navigation::I_NodeComparator>  wpComparator_type;
    /// @}

    /// @name I_GameClient implementation
    /// @{
public:
    virtual const WindowHandle_type getHandle() const;
    virtual bool init();
    virtual void run();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name GameClient implementation
    /// @{
public:
    void setupResourcePaths();

    /// Implement this to create or extend script types and link new script methods
    void createScriptTypes();

    /// Implement this to create some actions for the default
    /// action map.
    /// @param _actionMap Default action map
    void createActions();

    /// Create the default key to action mapping
    void createDefaultMapping();

    /// Create the game behaviors.
    void createBehaviors();

    /// Implement this to create a default scene
    void createScene();

    /// Setup physics materials.
    void setupPhysicsMaterials();

    Engine::Base::I_BaseGameClient& base();

    Engine::Base::I_BaseGame& game();

    void queryCursor();

    GameGrid& getGameGrid() const;

    void setDifficultyLevel(int _difficultyLevel);
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Called every frame right before the frame is rendered.
    /// @param _elapsedTime - amount of time that has elapsed since the
    ///             previous frame was rendered (in seconds).
    void beforeRender(double _elapsedTime);

    void moveLeft(boost::any& _parameter);
    void moveRight(boost::any& _parameter);
    void moveUp(boost::any& _parameter);
    void moveDown(boost::any& _parameter);
    void zoom(boost::any& _parameter);
    void print(boost::any& _parameter);

    /// Handle mouse click events.
    /// @note These mouse click events come from the GUIManager and are only events
    ///     that the GUI does not handle.
    void handleMouseClickEvent(Zen::Engine::Input::I_MouseClickEvent& _event);
    /// @}

    /// @name 'Structors
    /// @{
public:
             GameClient(WindowHandle_type _pParent);
    virtual ~GameClient();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Engine::Base::I_BaseGameClient&         m_base;
    Engine::Base::I_BaseGame&               m_game;

    ScriptObjectReference_type*             m_pScriptObject;

    Ogre::Root*                             m_pRoot;
    Ogre::RenderWindow*                     m_pRenderWindow;

    Zen::Memory::managed_ptr<GUIManager>    m_pGUIManager;

    Zen::Math::Vector3                      m_moveDirection;
    Zen::Math::Real                         m_zoomAmount;

    GameObjects_type                        m_gameObjects;

    GameGrid*                               m_pGameGrid;
    SimpleBrain*                            m_pBrain;

    CreepManager*                           m_pCreepManager;

    typedef Engine::Physics::I_PhysicsService::pPhysicsWorld_type   pPhysicsWorld_type;

    /// This is the physics world used for ray casting to
    /// see where the mouse clicks occur.
    pPhysicsWorld_type                      m_pClickWorld;

    typedef Engine::Physics::I_PhysicsWorld::pPhysicsShape_type     pPhysicsShape_type;

    /// This is the plane that ray casts will intersect,
    /// indicating where mouse clicks occur.
    pPhysicsShape_type                      m_pClickPlane;
    /// @}

};  // class GameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_CYBER_TOWER_DEFENSE_GAME_CLIENT_HPP_INCLUDED
