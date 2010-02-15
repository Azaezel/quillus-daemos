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
#ifndef ZEN_PONG_CLIENT_BASE_OBJECT_HPP_INCLUDED
#define ZEN_PONG_CLIENT_BASE_OBJECT_HPP_INCLUDED

#include <Zen/Engine/Core/I_GameGroup.hpp>

#include <Zen/Starter/Base/BaseCommon/I_BaseGameObject.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Pong {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class PongGameClient;

class BaseObject
:   public Zen::Engine::Core::I_GameObject
{
    /// @name I_GameObject implementation
    /// @{
public:
    virtual Engine::Core::I_BaseGameObject& base();
    virtual Engine::Core::I_GameObjectData* getData();
    virtual void setData(Engine::Core::I_GameObjectData* _pData);
    /// @}

    /// @name BaseObject implementation
    /// @{
public:
    void loadRenderable(const std::string& _meshName, const std::string& _materialName = "");
    Engine::Base::I_BaseGameObject& baseBase();
    /// @}

    /// @name Behaviors
    /// @{
public:
    static void setScale(Engine::Core::I_BaseGameObject& _object, float _x, float _y, float _z);
    /// @}

    /// @name 'Structors
    /// @{
public:
             BaseObject(Zen::Engine::Core::I_GameGroup& _group, const std::string& _name);
    virtual ~BaseObject();
    /// @}

    /// @name Member Variables
    /// @{
protected:
    const std::string                       m_name;
    Engine::Base::I_BaseGameObject*         m_pBase;
    Engine::Core::I_GameObjectData*         m_pGameData;
    pRenderableResource_type                m_pRenderable;
    /// @}

};  // class BaseObject

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Engine::Core::I_BaseGameObject&
BaseObject::base()
{
    return *m_pBase;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
Engine::Base::I_BaseGameObject&
BaseObject::baseBase()
{
    return *m_pBase;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Pong
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_PONG_CLIENT_BASE_OBJECT_HPP_INCLUDED
