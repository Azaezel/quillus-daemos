//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2010 Tony Richards
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
#ifndef ZEN_ZOGRE_RESOURCE_ENTITY_HPP_INCLUDED
#define ZEN_ZOGRE_RESOURCE_ENTITY_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Rendering/I_RenderableResource.hpp>

#include <OgreRoot.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOgre {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ResourceEntity
:   virtual public Zen::Engine::Rendering::I_RenderableResource
,   public Zen::Memory::managed_self_ref<Zen::Engine::Resource::I_Resource>
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_Renderable implementation
    /// @{
public:
    virtual void setMaterialName(const std::string& _name);
#if 0   // deprecated
    virtual void setNormaliseNormals(bool _normalise);
    virtual void updateAllAnimations(double _elapsedTime);
    virtual void enableAnimation(const std::string& _animationName, bool _enable);
#endif  // deprecated

    virtual void setVisible(bool _visible);
    virtual bool getVisible() const;
    /// @}

    /// @name I_RenderableResource implementation
    /// @{
    virtual pAnimationState_type getAnimationState(const std::string& _name) const;
    virtual void getAnimationStates(I_AnimationStateVisitor& _visitor) const;
    virtual pMesh_type getMesh();
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name ResourceEntity implementation
    /// @{
private:
    void destroyMesh(pMesh_type::weak_ptr_type _pMesh);
    /// @}

    /// @name Getter / Setter
    /// @{
public:
    Ogre::Entity* getEntity();
    /// @}

    /// @name 'Structors
    /// @{
public:
             ResourceEntity(Ogre::Entity* _pEntity, Ogre::SceneManager* _pSceneManager);
    virtual ~ResourceEntity();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Ogre::Entity*                   m_pEntity;
    Ogre::SceneManager*             m_pSceneManager;
    ScriptObjectReference_type*     m_pScriptObject;
    /// @}

};  // class ResourceEntity

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

inline
Ogre::Entity*
ResourceEntity::getEntity()
{
    return m_pEntity;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOgre
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOGRE_RESOURCE_ENTITY_HPP_INCLUDED
