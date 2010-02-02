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
#ifndef ZEN_ENGINE_CORE_I_GAME_OBJECT_DATA_HPP_INCLUDED
#define ZEN_ENGINE_CORE_I_GAME_OBJECT_DATA_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Core {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_GameObjectDataFactory;
class I_GameObjectData;
class I_GameObjectType;

class GAMECORE_DLL_LINK I_GameObjectData
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_GameObjectData interface
    /// @{
public:
    /// Get the name of this game object.
    /// Every object in the entire system has a unique name.
    virtual const std::string& getName() const = 0;

    /// Get the parent game object, if any exist.
    /// @return The parent of this game object data, or NULL
    virtual const I_GameObjectData* getParent() const = 0;

    /// Get the game group to which this object belongs
    virtual const I_GameGroupData& getGroup() const = 0;

    /// Get the game object type
    virtual const I_GameObjectType& getGameObjectType() const = 0;

    /// Compose a game object composition.
    virtual I_GameObjectComposition& composeGameObject() = 0;
    /// @}

    /// @name Events
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_GameObjectData();
    virtual ~I_GameObjectData();
    /// @}

    /// @name Inner Classes
    /// @{
public:
    struct GAMECORE_DLL_LINK I_GameObjectDataVisitor
    {
        virtual void begin() = 0;
        virtual void visit(I_GameObjectData& _gameObjectData) = 0;
        virtual void end() = 0;
    };  // interface I_GameObjectDataVisitor
    /// @}

};  // interface I_GameObjectData

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Core
}   // namespace Engine
namespace Memory 
{
    /// I_GameObjectData is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Core::I_GameObjectData> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_CORE_I_GAME_OBJECT_DATA_HPP_INCLUDED
