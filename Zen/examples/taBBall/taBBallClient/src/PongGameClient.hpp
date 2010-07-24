//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen taBBall
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
#ifndef INDIEZEN_PONG_PONG_GAME_CLIENT_HPP_INCLUDED
#define INDIEZEN_PONG_PONG_GAME_CLIENT_HPP_INCLUDED

#include "ClampRotationBehavior.hpp"
#include "ClampMovementBehavior.hpp"
#include "BallVelocityBehavior.hpp"
#include "GravityBehavior.hpp"
#include "ZeroTorqueBehavior.hpp"

#include <Zen/Engine/Client/I_GameClient.hpp>

#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <Zen/Engine/Camera/I_CameraService.hpp>
#include <Zen/Engine/Camera/I_Camera.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>
#include <Zen/Starter/Base/BaseCommon/I_BaseGame.hpp>

#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Pong {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class BallObject;
class BatObject;

class PongGameClient
:   public Zen::Engine::Client::I_GameClient
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<Scripting::I_ScriptType>                pScriptType_type;
    typedef Zen::Engine::Physics::I_PhysicsWorld::pPhysicsMaterial_type pPhysicsMaterial_type;
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

    /// @name Pong Game Client implementation
    /// @{
public:
    /// Return the extension point name of the scripting engine to use.
    //virtual std::string& getScriptEngineName();

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

    Engine::Base::I_BaseGameClient& baseClient() const;
    Engine::Base::I_BaseGame& baseGame() const;
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Called every frame right before the frame is rendered.
    /// @param _elapsedTime - amount of time that has elapsed since the 
    ///             previous frame was rendered (in seconds).
    void beforeRender(double _elapsedTime);
    /// @}

    /// @name PongGameClient implementation
    /// @{
public:
    /// @}

    /// @name Move Manager Handlers
    /// @todo Move this to a MoveManager implementation
    /// @{
public:
    void moveLeft(boost::any& _parameter);
    void moveRight(boost::any& _parameter);
    void spawnBall(boost::any& _parameter);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PongGameClient(WindowHandle_type _pParent);
    virtual ~PongGameClient();
    /// @}

    /// @name Member Variables 
    /// @{
private:
    Engine::Base::I_BaseGameClient&         m_baseClient;
    Engine::Base::I_BaseGame&               m_baseGame;
    ScriptObjectReference_type*             m_pScriptObject;

    //Zen::Engine::Camera::I_Camera*          m_pCamera;

    //BallObject*                             m_pBall;
    BatObject*                              m_pPlayer;

    typedef Engine::Core::I_BehaviorService::pGameObjectBehaviors_type  pGameObjectBehaviors_type;

    pGameObjectBehaviors_type               m_pDefaultBehavior;
    pGameObjectBehaviors_type               m_pBatBehaviors;
    pGameObjectBehaviors_type               m_pBallBehaviors;
    pGameObjectBehaviors_type               m_pGravityBallBehaviors;

    ClampRotationBehavior                   m_clampRotationBehavior;
    ClampMovementBehavior                   m_batClampMovement;
    ClampMovementBehavior                   m_ballClampMovement;
    BallVelocityBehavior                    m_ballVelocity;
    GravityBehavior                         m_gravity;
    ZeroTorqueBehavior                      m_zeroTorque;

    //Engine::Camera::I_CameraService::pCamera_type m_pCameraController;

    pPhysicsMaterial_type                   m_materialDefault;
    pPhysicsMaterial_type                   m_materialBall;
    pPhysicsMaterial_type                   m_materialBat;
    pPhysicsMaterial_type                   m_materialWall;
    pPhysicsMaterial_type                   m_materialBottomWall;
    /// @}

};  // class PongGameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Pong
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_PONG_PONG_GAME_CLIENT_HPP_INCLUDED
