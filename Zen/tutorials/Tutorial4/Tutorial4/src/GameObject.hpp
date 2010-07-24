//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
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
// This project is part of the Zen Engine Tutorials
// 
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMECLIENT_GAME_OBJECT_HPP_INCLUDED
#define GAMECLIENT_GAME_OBJECT_HPP_INCLUDED

#include <Zen/Core/Scripting/I_ScriptableType.hpp>

#include <Zen/Engine/Core/I_GameObject.hpp>
#include <Zen/Engine/Core/I_GameGroup.hpp>

#include <Zen/Starter/Base/BaseCommon/I_BaseGameObject.hpp>

#include <string>
#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class GameClient;

class GameObject
:   public Zen::Engine::Core::I_GameObject
,   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef std::vector<pRenderableResource_type>               RenderableResources_type;
    typedef Zen::Event::Event<I_GameObject&>                    GameObjectEvent_type;

    typedef GameObject*                                         pScriptObject_type;
    typedef Zen::Scripting::I_ScriptModule::pScriptType_type    pScriptType_type;
    typedef Zen::Scripting::ObjectReference<GameObject>         ScriptObjectReference_type;
    /// @}

    /// @name I_GameObject implementation
    /// @{
public:
    virtual const Zen::Engine::Core::I_BaseGameObject& base() const;
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
    /// Remove the renderable objects
    void clearRenderables();

    /// Load a renderable mesh and (optionally) set the material name.
    pRenderableResource_type loadRenderable(const std::string& _meshName, const std::string& _materialName = "");

    /// Set the material for the given mesh.
    /// @param _index 0 based index of the mesh, where 0 is the first mesh loaded, 1 
    ///         is the second mesh loaded, etc.
    void setMaterial(const std::string& _materialName, int _index);

    Zen::Engine::Base::I_BaseGameObject& baseBase();

    void setVisible(bool _visible);
    bool isVisible() const;

    // TODO TR - Is this necessary?
    //Zen::Engine::Core::I_GameObjectData* getData();
    /// @}

    /// @name Script support
    /// @{
public:
    /// Initialize the script type
    static void initScriptType(GameClient& _gameClient);
    /// @}

    /// @name Events
    /// @{
public:
    GameObjectEvent_type    onObjectDestroyEvent;
    GameObjectEvent_type    onObjectUpdateEvent;
    GameObjectEvent_type    onObjectMoveEvent;
    /// @}

    /// @name Behaviors
    /// @{
public:
    static void setScale(Zen::Engine::Core::I_BaseGameObject& _object, float _x, float _y, float _z);
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
    bool                                    m_isVisible;
    Zen::Engine::Base::I_BaseGameObject*    m_pBaseObject;

    RenderableResources_type                m_renderables;

    Zen::Engine::Core::I_GameObjectData*    m_pData;
    /// @}

};  // class GameObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
const Zen::Engine::Core::I_BaseGameObject&
GameObject::base() const
{
    return *m_pBaseObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Zen::Engine::Core::I_BaseGameObject&
GameObject::base()
{
    return *m_pBaseObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Zen::Engine::Base::I_BaseGameObject&
GameObject::baseBase()
{
    return *m_pBaseObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMECLIENT_GAME_OBJECT_HPP_INCLUDED
