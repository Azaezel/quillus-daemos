//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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
#ifndef ZEN_ENGINE_CORE_I_GAME_GROUP_HPP_INCLUDED
#define ZEN_ENGINE_CORE_I_GAME_GROUP_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Scripting/I_ScriptableType.hpp>
#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Engine/Core/I_BehaviorService.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Core {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_BaseGameObject;

/// @brief Game Object interface
class GAMECORE_DLL_LINK I_GameGroup
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_GameGroup*                                pScriptObject_type;
    typedef Scripting::ObjectReference<I_GameGroup>     ScriptObjectReference_type;
    struct I_GameObjectVisitor;
    /// @}

    /// @name I_GameGroup interface
    /// @{
public:
    /// Create a game object.  Don't keep a reference to the returned object.
    /// If you need it again, get it using getObject()
    virtual I_BaseGameObject& createObject(const std::string& _name) = 0;

    /// Add a game object to this group.
    /// The group takes ownership of the object.
    virtual void addObject(I_BaseGameObject& _gameObject) = 0;

    /// Get a game object that was created by createObject()
    ///  Don't keep a reference to the returned object.
    /// If you need it again, get it using getObject()
    virtual I_BaseGameObject& getObject(const std::string& _name) = 0;

    /// Destroy an object
    virtual void destroyObject(const std::string& _name) = 0;

    /// Get all of the objects in this group
    virtual void getObjects(struct I_GameObjectVisitor& _visitor) = 0;

    /// Create a sub group
    virtual I_GameGroup& createGroup(const std::string& _name) = 0;

    /// Get a game sub group
    virtual I_GameGroup& getGroup(const std::string& _name) = 0;

    /// Destroy a group
    /// This also destroys all of the objects within the group
    virtual void destroyGroup(const std::string& _name) = 0;

    /// Get the default behaviors for this group and override the one's
    /// you would like to override.  The order of overriding is important.
    /// Overriding a group will also override the behavior of all sub groups.
    /// It's best to override a group first, then apply overrides to sub groups.
    virtual void setDefaultBehaviors(I_BehaviorService::pGameObjectBehaviors_type _pBehaviors) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameGroup();
    virtual ~I_GameGroup();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct I_GameObjectVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_BaseGameObject& _object) = 0;
        virtual void end() = 0;
    };  // struct I_GameObjectVisitor

};  // interface I_GameGroup

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Core
}   // namespace Engine
namespace Memory 
{
    /// I_GameGroup is managed by user defined factories (like Base starter kit)
    template<>
    struct is_managed_by_factory<Zen::Engine::Core::I_GameGroup> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CORE_I_GAME_GROUP_HPP_INCLUDED
