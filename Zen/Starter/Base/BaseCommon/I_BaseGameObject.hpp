//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Base Starter Kit
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
#ifndef ZEN_ENGINE_BASE_I_BASE_GAME_OBJECT_HPP_INCLUDED
#define ZEN_ENGINE_BASE_I_BASE_GAME_OBJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>

#include <Zen/Engine/Core/I_GameObject.hpp>

#include <Zen/Engine/Resource/I_Resource.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>
#include <Zen/Engine/Rendering/I_SceneNode.hpp>
#include <Zen/Engine/Physics/I_PhysicsActor.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Base {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_BaseGame;

/// @brief Base Game Object
///
/// Starter Kits and Games should create compositions with this object to create specialized
/// implementations.
///
/// A Game Object is any object inside of the game, whether it's renderable
/// or not, collidable or not, controllable or not, etc.
class BASECOMMON_DLL_LINK I_BaseGameObject
:   public Zen::Engine::Core::I_BaseGameObject
{

protected:
    /// Use getPhysicsActor() to create a physics shape
    virtual void attachPhysicsActor(pPhysicsActor_type _pPhysicsActor) = 0;

    /// @name I_BaseGameObject interface
    /// @{
public:
    virtual I_BaseGame& getGame() = 0;

    virtual const Math::Point3& getPosition() const = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    static I_BaseGameObject& getObject(pSceneNode_type _pSceneNode);
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_BaseGameObject();
    virtual ~I_BaseGameObject();
    /// @}

};  // interface I_BaseGameObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Base
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_BASE_CLIENT_I_BASE_GAME_OBJECT_HPP_INCLUDED
