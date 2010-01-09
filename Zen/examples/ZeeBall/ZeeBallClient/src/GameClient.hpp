//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zee Ball Game Client
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
#ifndef ZEEBALL_CLIENT_GAME_CLIENT_HPP_INCLUDED
#define ZEEBALL_CLIENT_GAME_CLIENT_HPP_INCLUDED

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

#include <boost/any.hpp>

#include "../../ZeeBallCommon/I_networking.hpp"
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ZeeBall {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class BallObject;
class BatObject;

class GameClient
:   public Zen::Engine::Client::I_GameClient
{
    /// @name Types
    /// @{
public:
	typedef Zen::Memory::managed_ptr<Zen::Scripting::I_ScriptType>                pScriptType_type;
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
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name Game Client implementation
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

    Zen::Engine::Base::I_BaseGameClient& base();
    /// @}
	
	/// @name Net Hooks
    /// @{

public:
	void script_setNetworkingRole(Zen::Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _args);
	unsigned short getZonePort();
public:
	enum networkRoles
	{
		SERVER,
		CLIENT,
		CLIENTERVER,
	};
	networkRoles m_networkRole;

	boost::asio::ip::address_v4 m_connectionIp;
	unsigned short m_connectionPort;
	I_NetServer* m_baseServer;

    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Called every frame right before the frame is rendered.
    /// @param _elapsedTime - amount of time that has elapsed since the 
    ///             previous frame was rendered (in seconds).
    void beforeRender(double _elapsedTime);
    /// @}

    /// @name GameClient implementation
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
    
	/* additional leftovers
    void moveUp(boost::any& _parameter);
    void moveDown(boost::any& _parameter);
    void moveForward(boost::any& _parameter);
    void moveBackward(boost::any& _parameter);
    void rotateLeft(boost::any& _parameter);
    void rotateRight(boost::any& _parameter);
    void rotateUp(boost::any& _parameter);
    void rotateDown(boost::any& _parameter);
    */
    
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
    Zen::Engine::Base::I_BaseGameClient&         m_base;
    ScriptObjectReference_type*             m_pScriptObject;

    //Zen::Engine::Camera::I_Camera*          m_pCamera;

    //BallObject*                             m_pBall;
    BatObject*                              m_pPlayer;

    typedef Zen::Engine::Core::I_BehaviorService::pGameObjectBehaviors_type  pGameObjectBehaviors_type;

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

    Zen::Engine::Camera::I_CameraService::pCamera_type m_pCameraController;

    pPhysicsMaterial_type                   m_materialDefault;
    pPhysicsMaterial_type                   m_materialBall;
    pPhysicsMaterial_type                   m_materialBat;
    pPhysicsMaterial_type                   m_materialWall;
    pPhysicsMaterial_type                   m_materialBottomWall;
    /// @}
	bool									hosting;
	signed short							currentZone;

};  // class GameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace ZeeBall
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEEBALL_CLIENT_GAME_CLIENT_HPP_INCLUDED
