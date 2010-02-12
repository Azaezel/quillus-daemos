//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Knights of Zen
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
#ifndef KNIGHTS_OF_ZEN_GAME_CLIENT_HPP_INCLUDED
#define KNIGHTS_OF_ZEN_GAME_CLIENT_HPP_INCLUDED

#include <Zen/Engine/Client/I_GameClient.hpp>
#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <Zen/Engine/Camera/I_Camera.hpp>
#include <Zen/Engine/Physics/I_PhysicsService.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>

#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Knights {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class GameClient
:   public Zen::Engine::Client::I_GameClient
{
    /// @name Types
    /// @{
public:
    typedef Zen::Engine::Physics::I_PhysicsService*                     pPhysicsService;
    typedef Zen::Engine::Physics::I_PhysicsService::pPhysicsWorld_type  pPhysicsWorld_type;
    typedef Engine::Core::I_BehaviorService::pGameObjectBehaviors_type  pGameObjectBehaviors_type;
    /// @}

    /// @name I_GameClient implementation
    /// @{
public:
    virtual const WindowHandle_type getHandle() const;
    virtual bool init();
    virtual void run();
    /// @}

    /// @name Knights Game Client implementation
    /// @{
public:
    /// Implement this to create some actions for the default
    /// action map.
    /// @param _actionMap Default action map
    void createActions();

    /// Create the default key to action mapping
    void createDefaultMapping();

    /// Implement this to create a default scene
    void createScene();

    Engine::Base::I_BaseGameClient& base();
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
    pPhysicsWorld_type getPhysicsWorld();
    /// @}

    /// @name Move Manager Handlers
    /// @todo Move this to a MoveManager implementation
    /// @{
public:
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

    Zen::Engine::Camera::I_Camera*          m_pCamera;

    pPhysicsWorld_type                      m_pWorld;


    pGameObjectBehaviors_type               m_pDefaultBehavior;
    /// @}

};  // class GameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Knights
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // KNIGHTS_OF_ZEN_GAME_CLIENT_HPP_INCLUDED
