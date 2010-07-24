//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Wheels Sample Game
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
#ifndef INDIEZEN_WHEELS_WHEELS_GAME_CLIENT_HPP_INCLUDED
#define INDIEZEN_WHEELS_WHEELS_GAME_CLIENT_HPP_INCLUDED

#include <Zen/Engine/Client/I_GameClient.hpp>
#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <Zen/Engine/Camera/I_CameraService.hpp>
#include <Zen/Engine/Camera/I_Camera.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>

#include <Zen/Starter/Base/BaseClient/I_BaseGameClient.hpp>

#include <boost/any.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Wheels {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class BallObject;
class BatObject;

class WheelsGameClient
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

    /// @name WheelsGameClient implementation
    /// @{
public:
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
    /// @}

    /// @name Event Handlers
    /// @{
public:
    /// Called every frame right before the frame is rendered.
    /// @param _elapsedTime - amount of time that has elapsed since the 
    ///             previous frame was rendered (in seconds).
    void beforeRender(double _elapsedTime);
    /// @}

    /// @name 'Structors
    /// @{
public:
             WheelsGameClient(WindowHandle_type _pParent);
    virtual ~WheelsGameClient();
    /// @}

    /// @name Member Variables 
    /// @{
private:
    Engine::Base::I_BaseGameClient&         m_base;
    ScriptObjectReference_type*             m_pScriptObject;
    /// @}

};  // class WheelsGameClient

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Wheels
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // INDIEZEN_WHEELS_WHEELS_GAME_CLIENT_HPP_INCLUDED
