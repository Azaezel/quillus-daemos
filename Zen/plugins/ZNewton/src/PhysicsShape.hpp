//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C)        2008 Walt Collins
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZNEWTON_PHYSICS_SHAPE_HPP_INCLUDED
#define ZEN_ZNEWTON_PHYSICS_SHAPE_HPP_INCLUDED

// TR - Newton.h should include stdlib.h but it doesn't.
#include <stdlib.h>
#include <Newton.h>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include <Zen/Core/Math/Quaternion4.hpp>
#include <Zen/Core/Math/Matrix4.hpp>

#include <Zen/Engine/Physics/I_PhysicsShape.hpp>
#include <Zen/Engine/Core/I_GameObject.hpp>

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZNewton {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsZone;

class CollisionShape
:   public Engine::Physics::I_CollisionShape
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_CollisionShape>
{
    /// @name Types
    /// @{
public:
    typedef unsigned short U16;
    //typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_CollisionShape> wpCollisionShape_type;
    typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsZone> wpPhysicsZone_type;
    typedef Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsZone>      pPhysicsZone_type;
	typedef Memory::managed_ptr<Zen::Engine::Core::I_GameObject>        pOwningObject_type;
    /// @}

    /// @name I_CollisionShape implementation
    /// @{
public:
    virtual wpPhysicsZone_type getPhysicsZone();
    virtual void setMaterial(pPhysicsMaterial_type _material);
    virtual pPhysicsMaterial_type getMaterial();
    virtual void* getBodyPtr();
    virtual const std::string& getName();
    virtual void setName(const std::string& _name);

    virtual float getMass();
    virtual void setMass(float _mass);
    virtual void setScale(float _x, float _y, float _z);
    virtual void setPosition(const Math::Point3& _pos);
    virtual void getPosition(Math::Point3& _pos);
    virtual void getOrientation(Math::Quaternion4& _orient);
    virtual void setOrientation(const Math::Quaternion4& _orient);
    virtual void getOrientation(Math::Matrix4& _orient);
    virtual void setOrientation(const Math::Matrix4& _orient);
    virtual void setVelocity(const Math::Vector3& _velocity);
    virtual void getVelocity(Math::Vector3& _velocity);
    virtual void getTorque(Math::Vector3& _torque);
    virtual Math::Vector3 getAngularMomentum();
    virtual void setAngularMomentum(const Math::Vector3& _omega);
    virtual Math::Vector3 getAngularDamping();
    virtual void setAngularDamping(const Math::Vector3& _damping);
    virtual Math::Real getLinearDamping();
    virtual void setLinearDamping(float _damping);
    virtual void setAutoFreeze(bool _bFreeze);
    
    //TODO: remove this entire list, and implement via physicsResource primitives
    virtual bool initNullShape();
    virtual bool initBoxShape(float _dx, float _dy, float _dz);
    virtual bool initOvoidShape(float _radiusX, float _radiusY, float _radiusZ);
    virtual bool initCapsuleShape(float _radius, float _height);
    virtual bool initHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize);
    virtual bool initHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform);
    virtual bool initConvexHullShape(std::string _filename);
    virtual bool initTreeCollisionShape(std::string _filename);
    // end list

    virtual void setAdvancedCollisionPrediction(bool _mode);
    virtual bool getAdvancedCollisionPrediction();
	virtual void SetStatic(bool _isStatic);
	virtual Scripting::I_ObjectReference* getScriptObject();

	virtual void setCollisionGroup(int _collisionGroup);
	virtual int getCollisionGroup();

	virtual void setOwner(pOwningObject_type _pOwner);
	virtual pOwningObject_type getOwner();

    /// @}

    /// @name CollisionShape implementation
    /// @{
private:
    Math::Matrix4 transposeMatrixRtoC(Math::Matrix4 _mat);
    static void TransformCallback(const NewtonBody* _body, const dFloat* _matrix);
    static void ApplyForceAndTorqueCallback(const NewtonBody* _pBody);
    static void ActivationStateCallback(const NewtonBody* body, unsigned state);

    bool attachBody(NewtonCollision* _collision);
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
             CollisionShape(wpPhysicsZone_type _zone);
    virtual ~CollisionShape();
    /// @}

    /// @name Member Variables
    /// @{
private:
    NewtonBody*                             m_body;
    wpPhysicsZone_type                     m_pZone;
    pPhysicsMaterial_type                   m_material;
    dFloat                                  m_scaleX, m_scaleY, m_scaleZ;
    Scripting::I_ObjectReference*           m_pScriptObject;
    std::string                             m_name;
	Zen::Math::Real							m_minStrikeLen;
    unsigned                                m_activationState;
	int										m_collisionGroup;

	pOwningObject_type                      m_pOwner;
    /// @}

    /// @name Inner Classes
    /// @{
public:
    class TransformEventData
    :   public I_TransformEventData
    {
    public:
        virtual I_CollisionShape& getShape();
        virtual const Math::Matrix4& getTransform() const;

    public:
        TransformEventData(Engine::Physics::I_CollisionShape& _shape, Math::Matrix4& _transform);

    private:
        Engine::Physics::I_CollisionShape&        m_shape;
        Math::Matrix4&                          m_transform;

    };  // interface I_TransformEventData

    class CollisionEventData
    :   public I_CollisionEventData
    {
    public:
        virtual I_CollisionShape& getShape();
        virtual I_CollisionShape& getOtherShape();

    public:

        CollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape);

    protected:
        const NewtonMaterial*                   m_pMaterial;
        Engine::Physics::I_CollisionShape&        m_shape;
        Engine::Physics::I_CollisionShape&		m_otherShape;

    };  // class CollisionEventData

    class BeginCollisionEventData
    :   public CollisionEventData
    ,   public I_BeginCollisionEventData
    {
    public:
        virtual I_CollisionShape& getShape() { return CollisionEventData::getShape(); }
        virtual I_CollisionShape& getOtherShape() { return CollisionEventData::getOtherShape(); }
        virtual void ignoreCollision() { m_ignoreCollision = true; }

    public:
        bool getIgnoreCollision() const { return m_ignoreCollision; }

    public:
        BeginCollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape);

    private:
        bool        m_ignoreCollision;

    };  // class BeginCollisionEventData

    class DuringCollisionEventData
    :   public CollisionEventData
    ,   public I_DuringCollisionEventData
    {
    public:
        virtual I_CollisionShape& getShape() { return CollisionEventData::getShape(); }
        virtual I_CollisionShape& getOtherShape() { return CollisionEventData::getOtherShape(); }

        virtual void disableContact() const
        {
            NewtonMaterialDisableContact(m_pMaterial);
        }

        virtual Math::Real getContactSpeed() const
        {
            return NewtonMaterialGetContactNormalSpeed(m_pMaterial, m_pContact);
        }

        virtual void getContactForce(Math::Vector3& _forceVector) const
        {
            NewtonMaterialGetContactForce(m_pMaterial, _forceVector.m_array);
        }

        virtual void getContactPosition(Math::Point3& _contactPosition) const
        {
            Math::Vector3 contactNormal;
            NewtonMaterialGetContactPositionAndNormal(m_pMaterial, _contactPosition.m_array, contactNormal.m_array);
        }

        virtual void getContactNormal(Math::Vector3& _contactNormal) const
        {
            Math::Point3 contactPosition;
            NewtonMaterialGetContactPositionAndNormal(m_pMaterial, contactPosition.m_array, _contactNormal.m_array);
        }

        virtual void getContactPositionAndNormal(Math::Point3& _contactPosition, Math::Vector3& _contactNormal) const
        {
            NewtonMaterialGetContactPositionAndNormal(m_pMaterial, _contactPosition.m_array, _contactNormal.m_array);
        }

        virtual void getContactTangentDirections(Math::Vector3& _direction0, Math::Vector3& _direction1) const
        {
            NewtonMaterialGetContactTangentDirections(m_pMaterial, _direction0.m_array, _direction1.m_array);
        }

        virtual Math::Real getContactTangentSpeed(bool _primary = true) const
        {
            return NewtonMaterialGetContactTangentSpeed(m_pMaterial, m_pContact, _primary ? 0 : 1);
        }

        virtual void setContactRestitution(Math::Real _restitution)
        {
            NewtonMaterialSetContactElasticity(m_pMaterial, _restitution);
        }

        virtual void setContactFrictionState(int _state, bool _primary = true)
        {
            NewtonMaterialSetContactFrictionState(m_pMaterial, _state, _primary ? 0 : 1);
        }

        virtual void setStaticFrictionCoefficient(Math::Real _coefficent, bool _primary = true)
        {
            NewtonMaterialSetContactStaticFrictionCoef(m_pMaterial, _coefficent, _primary ? 0 : 1);
        }

        virtual void setKineticFrictionCoefficient(Math::Real _coefficent, bool _primary = true)
        {
            NewtonMaterialSetContactKineticFrictionCoef(m_pMaterial, _coefficent, _primary ? 0 : 1);
        }

        virtual void setContactTangentAcceleration(Math::Real _acceleration, bool _primary = true)
        {
            NewtonMaterialSetContactTangentAcceleration(m_pMaterial, _acceleration, _primary ? 0 : 1);
        }

        virtual void setContactTangentVector(Math::Vector3 _tangent)
        {
            NewtonMaterialContactRotateTangentDirections(m_pMaterial, _tangent.m_array);
        }

    public:

        DuringCollisionEventData(const NewtonMaterial* _pMaterial, const NewtonContact* _contact, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape);

    private:
        const NewtonContact*        m_pContact;
    };  // class DuringCollisionEventData


    class EndCollisionEventData
    :   public CollisionEventData
    ,   public I_EndCollisionEventData
    {
    public:
        virtual I_CollisionShape& getShape() { return CollisionEventData::getShape(); }
        virtual I_CollisionShape& getOtherShape() { return CollisionEventData::getOtherShape(); }

    public:

        EndCollisionEventData(const NewtonMaterial* _material, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape);

    private:

    };  // class EndCollisionEventData

};  // class CollisionShape

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CollisionShape::TransformEventData::TransformEventData(I_CollisionShape& _shape, Math::Matrix4& _transform)
:   m_shape(_shape)
,   m_transform(_transform)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CollisionShape::CollisionEventData::CollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape)
:   m_pMaterial(_pMaterial)
,   m_shape(_shape)
,   m_otherShape(_otherShape)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CollisionShape::BeginCollisionEventData::BeginCollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape)
:   CollisionEventData(_pMaterial, _shape, _otherShape)
,   m_ignoreCollision(false)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CollisionShape::DuringCollisionEventData::DuringCollisionEventData(const NewtonMaterial* _pMaterial, const NewtonContact* _pContact, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape)
:   CollisionEventData(_pMaterial, _shape, _otherShape)
,   m_pContact(_pContact)
{
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
inline
CollisionShape::EndCollisionEventData::EndCollisionEventData(const NewtonMaterial* _pMaterial, Engine::Physics::I_CollisionShape& _shape, Engine::Physics::I_CollisionShape& _otherShape)
:   CollisionEventData(_pMaterial, _shape, _otherShape)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZNEWTON_PHYSICS_SHAPE_HPP_INCLUDED
