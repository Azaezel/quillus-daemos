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

#include "PhysicsZone.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>

//#include <dVector.h> //no likey. recheck
//refnotes: http://docs.taoframework.com/Tao.Ode/Tao.Ode.Ode.dHashSpaceCreate.html
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::PhysicsZone()
:   m_pScriptModule(Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
{
    m_worldId = dWorldCreate();

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::~PhysicsZone()
{
    dWorldDestroy(m_worldId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::stepSimulation(double _elapsedTime)
{
    dWorldStep(m_worldId, _elapsedTime);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int
PhysicsZone::getDefaultMaterialID()
{
    throw Utility::runtime_exception("PhysicsZone::getDefaultMaterialID(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setZoneSize(const Math::Vector3& _min, const Math::Vector3& _max)
{
    throw Utility::runtime_exception("PhysicsZone::setZoneSize(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setGravity(const Math::Vector3& _grav)
{
    throw Utility::runtime_exception("PhysicsZone::setGravity(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsActor_type
PhysicsZone::createActor()
{
    throw Utility::runtime_exception("PhysicsZone::createActor(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsMaterial_type
PhysicsZone::createMaterial(bool _default)
{
    throw Utility::runtime_exception("PhysicsZone::createMaterial(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::vector<PhysicsZone::pPhysicsMaterial_type>
PhysicsZone::getAllMaterials()
{
    throw Utility::runtime_exception("PhysicsZone::getAllMaterials(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsJoint_type
PhysicsZone::createJoint()
{
    throw Utility::runtime_exception("PhysicsZone::createJoint(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
PhysicsZone::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type
            (m_pScriptModule, m_pScriptModule->getScriptType(getScriptTypeName()), getSelfReference().lock());
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor)
{
    throw Utility::runtime_exception("PhysicsZone::rayCast(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createNullShape()
{
    throw Utility::runtime_exception("PhysicsZone::createNullShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createBoxShape(float _dx, float _dy, float _dz)
{
    throw Utility::runtime_exception("PhysicsZone::createBoxShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createOvoidShape(float _radiusX, float _radiusY, float _radiusZ)
{
    throw Utility::runtime_exception("PhysicsZone::createOvoidShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createCapsuleShape(float _radius, float _height)
{
    throw Utility::runtime_exception("PhysicsZone::createCapsuleShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
    throw Utility::runtime_exception("PhysicsZone::createHeightFieldShapeFromRaw(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform)
{
    throw Utility::runtime_exception("PhysicsZone::createHeightFieldShapeFromSerialization(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createConvexHullShape(std::string _filename)
{
    throw Utility::runtime_exception("PhysicsZone::createConvexHullShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createTreeCollisionShape(std::string _filename)
{
    throw Utility::runtime_exception("PhysicsZone::createTreeCollisionShape(): Error, not implemented.");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
