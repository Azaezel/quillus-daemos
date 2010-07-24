//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Walt Collins
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_PHYSICS_I_PHYSICS_COLLISION_SHAPE_HPP_INCLUDED
#define ZEN_ENGINE_PHYSICS_I_PHYSICS_COLLISION_SHAPE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Core/I_GameObject.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Math {
        class Matrix4;
        class Vector3;
        class Point3;
    }   // namespace Math
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_PhysicsZone;
class I_PhysicsMaterial;

class PHYSICS_DLL_LINK I_CollisionShape
:   public virtual Zen::Scripting::I_ScriptableType
{
    /// @name Types
    /// @{
public:
    typedef I_CollisionShape*                                   pScriptObject_type;
    typedef Scripting::ObjectReference<I_CollisionShape>        ScriptObjectReference_type;
    typedef ScriptObjectReference_type                          ScriptWrapper_type;
    typedef ScriptWrapper_type*                                 pScriptWrapper_type;

    typedef Memory::managed_weak_ptr<I_CollisionShape>          wpCollisionShape_type;
    typedef Zen::Event::Event<wpCollisionShape_type>            collisionShapeEvent_type;

    enum CollisionShapeType
    {
        NULL_SHAPE,
        SPHERE_SHAPE,
        BOX_SHAPE,
        CYLINDER_SHAPE,
        OVOID_SHAPE,
        CAPSULE_SHAPE,
        HEIGHTFIELD_SHAPE
    };
    /// @}

    /// @name I_CollisionShape interface
    /// @{
    virtual CollisionShapeType getType() = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "CollisionShape"
    /// Override this method if you create a derived type
    virtual const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    collisionShapeEvent_type    onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_CollisionShape();
    virtual ~I_CollisionShape();
    /// @}

    /// WTF is this doing on an interface?
public:
    //bool m_IsStatic;
	//Math::Real m_mass;

};  // interface I_CollisionShape

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
namespace Memory
{
    /// I_CollisionShape is managed by a I_PhysicsZone
    template<>
    struct is_managed_by_factory<Zen::Engine::Physics::I_CollisionShape> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_PHYSICS_I_PHYSICS_COLLISION_SHAPE_HPP_INCLUDED
