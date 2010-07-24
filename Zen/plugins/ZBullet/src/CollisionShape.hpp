//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2009 Brian Roberts
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
#ifndef ZEN_ZBULLET_COLLISION_SHAPE_HPP_INCLUDED
#define ZEN_ZBULLET_COLLISION_SHAPE_HPP_INCLUDED
// TR - Newton.h should include stdlib.h but it doesn't.
#include <stdlib.h>
#include <btBulletDynamicsCommon.h>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Matrix4.hpp>

#include <Zen/Engine/Physics/I_CollisionShape.hpp>
#include <Zen/Engine/Core/I_GameObject.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBullet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

// Reference Note: http://www.bulletphysics.com/Bullet/BulletFull/classbtCollisionShape.html
class PhysicsZone;

class CollisionShape
:   public Engine::Physics::I_CollisionShape
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_CollisionShape>
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsZone> wpPhysicsZone_type;
    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsZone>      pPhysicsZone_type;
	typedef Memory::managed_ptr<Zen::Engine::Core::I_GameObject>        pOwningObject_type;
    /// @}

    /// @name I_CollisionShape implementation
    /// @{
public:

    virtual const std::string& getName();
    virtual void setName(const std::string& _name);
	virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

public:
    /// @ For Internal Bullet usage
    /// @{
    btCollisionShape* getShapePtr();
    void setShapePtr(btCollisionShape* _shape);
    void setIsNUllShape(bool _isnull){m_IsNUllFlaged = _isnull;};
    bool isNULLShape(){return m_IsNUllFlaged;};
    /// @}

    /// @name CollisionShape implementation
    /// @{
private:
    static void ApplyForceAndTorqueCallback(const btRigidBody* _pBody);
    static void ActivationStateCallback(const btRigidBody* body, unsigned state);

    bool attachBody(btCollisionShape* _collision);
    void applyForce(const Math::Vector3& _force);
    void applyTorque(const Math::Vector3& _torque);

    void setActivationState(unsigned _state);
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name 'Structors
    /// @{
public:
             CollisionShape();
    virtual ~CollisionShape();
    /// @}

    /// @name Member Variables
    /// @{
private:
    btCollisionShape*                   m_pShape;
    wpPhysicsZone_type                  m_pZone;
    Zen::Math::Real                     m_scaleX, m_scaleY, m_scaleZ;
    Scripting::I_ObjectReference*       m_pScriptObject;
    std::string                         m_name;
    Zen::Math::Real                     m_minStrikeLen;
    unsigned                            m_activationState;
    int                                 m_collisionGroup;
    bool                                m_IsNUllFlaged;    
    pOwningObject_type                  m_pOwner;
    /// @}

};  // class CollisionShape

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // ZEN_ZBULLET_PHYSICS_SHAPE_HPP_INCLUDED