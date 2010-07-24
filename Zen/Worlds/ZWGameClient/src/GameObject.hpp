//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Worlds Game Client
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_WORLDS_GAME_OBJECT_HPP_INCLUDED
#define ZEN_WORLDS_GAME_OBJECT_HPP_INCLUDED

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Core/I_GameObject.hpp>
#include <Zen/Engine/Core/I_GameGroup.hpp>

#include <string>
#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Engine {
        namespace Base {
            class I_BaseGameObject;
        }   // namespace Base
    }   // namespace Engine
namespace Worlds {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class GameObject
:   public Zen::Engine::Core::I_GameObject
,   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef Zen::Event::Event<I_GameObject&>                    GameObjectEvent_type;
    typedef Zen::Scripting::I_ScriptEngine::pScriptEngine_type  pScriptEngine_type;

    /// TODO Should these typedefs be in I_GameObject?
    /// Should I_GameObject extend I_ScriptableType?
    typedef I_GameObject*                                       pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<I_GameObject>       ScriptObjectReference_type;
    /// @}

    /// @name I_GameObject implementation
    /// @{
public:
    virtual Zen::Engine::Core::I_BaseGameObject& base();
    virtual Zen::Engine::Core::I_GameObjectData* getData();
    virtual void setData(Zen::Engine::Core::I_GameObjectData* _pData);
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual const std::string& getScriptTypeName();
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name GameObject implementation
    /// @{
public:
    static void createScriptTypes(pScriptEngine_type _pScriptEngine);
    /// @}

    /// @name Events
    /// @{
public:
    GameObjectEvent_type    onObjectDestroyEvent;
    GameObjectEvent_type    onObjectUpdateEvent;
    /// @}

    /// @name 'Structors
    /// @{
public:
             GameObject(Zen::Engine::Core::I_GameGroup& _group, const std::string& _name);
    virtual ~GameObject();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    ScriptObjectReference_type*             m_pScriptObject;
    
    const std::string                       m_name;

    Zen::Engine::Base::I_BaseGameObject*    m_pBaseObject;

    Zen::Engine::Core::I_GameObjectData*    m_pData;
    /// @}

};  // class GameObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Worlds
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_WORLDS_GAME_OBJECT_HPP_INCLUDED
