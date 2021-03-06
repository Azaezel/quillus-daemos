//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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

#include "PhysicsZone.hpp"
#include "PhysicsJoint.hpp"
#include "PhysicsMaterial.hpp"
#include "PhysicsActor.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>

#include <boost/bind.hpp>
#include <Zen/Core/Math/Vector4.hpp>

//#include <dVector.h> //no likey. recheck

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZNewton {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::PhysicsZone()
:   m_pScriptModule(Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
{
    m_pZone = NewtonCreate(NULL, NULL);

    m_vGravity = Math::Vector3(0.0f, -9.8f, 0.0f);
    m_bDefaultMaterialCreated = false;

    NewtonSetFrictionModel(m_pZone, 1);
    NewtonSetSolverModel(m_pZone, 2);
    
    Math::Vector3 min = Math::Vector3(-1000,-1000,-1000);
    Math::Vector3 max = Math::Vector3(1000,1000,1000);

	// set the size of the world
    NewtonSetWorldSize(m_pZone, min.m_array, max.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::~PhysicsZone()
{
    NewtonDestroy(m_pZone);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
NewtonWorld*
PhysicsZone::getZonePtr(void)
{
    return m_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setZonePtr(NewtonWorld* _pZone)
{
    m_pZone = (NewtonWorld*)_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
PhysicsZone::getDefaultMaterialID()
{
    return NewtonMaterialGetDefaultGroupID(m_pZone);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setGravity(const Math::Vector3& _grav)
{
    // note that Newton does not support a global gravity value, instead it must
    // be applied as an additional downward force in any force callback functions.
    // this value is held here simply as a storage place for now.
    // TODO - use this value in a gravity force calculation, or pass it to another object which does.
    m_vGravity = _grav;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setZoneSize(const Math::Vector3& _min, const Math::Vector3& _max)
{
	// set the size of the world
    NewtonSetWorldSize(m_pZone, _min.m_array, _max.m_array);
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsActor_type 
PhysicsZone::createActor()
{
    PhysicsActor* pRawActor = new PhysicsActor(getSelfReference());
    pPhysicsActor_type pActor = pPhysicsActor_type(pRawActor, boost::bind(&PhysicsZone::onDestroyActor, this, _1));

    wpPhysicsActor_type wpActor(pActor);
    pRawActor->setSelfReference(wpActor);

    return pActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
std::vector<PhysicsZone::pPhysicsMaterial_type>
PhysicsZone::getAllMaterials()
{
    return m_materials;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsMaterial_type 
PhysicsZone::createMaterial(bool _default)
{
    PhysicsMaterial* pRawMaterial;

    if (m_bDefaultMaterialCreated)
    {
        // only one "default" material allowed
        if (_default)
        {
            // TODO - warning here, default material already created
        }
        pRawMaterial = new PhysicsMaterial(getSelfReference(), false);
    }
    else
    {
        m_bDefaultMaterialCreated = _default;
        pRawMaterial = new PhysicsMaterial(getSelfReference(), _default);
    }

    pPhysicsMaterial_type pMaterial = pPhysicsMaterial_type(pRawMaterial, boost::bind(&PhysicsZone::onDestroyPhysicsMaterial, this, _1));

    wpPhysicsMaterial_type wpMaterial(pMaterial);
    pRawMaterial->setSelfReference(wpMaterial);

    m_materials.push_back(pMaterial);

    return pMaterial;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pPhysicsJoint_type
PhysicsZone::createJoint()
{
    PhysicsJoint* pRawJoint = new PhysicsJoint(getSelfReference());
    pPhysicsJoint_type pJoint = pPhysicsJoint_type(pRawJoint, boost::bind(&PhysicsZone::onDestroyPhysicsJoint, this, _1));

    wpPhysicsJoint_type wpJoint(pJoint);
    pRawJoint->setSelfReference(wpJoint);

    return pJoint;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::onDestroyActor(wpPhysicsActor_type _wpPhysicsActor)
{
    /// Fire the CollisionShape's onDestroyEvent
    _wpPhysicsActor->onDestroyEvent(_wpPhysicsActor);
    
    /// delete the CollisionShape pointer
    PhysicsActor* pPhysicsActor = dynamic_cast<PhysicsActor*>(_wpPhysicsActor.get());

    if (pPhysicsActor)
    {
        delete pPhysicsActor;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::onDestroyActor() : _wpCollisionShape is an invalid CollisionShape.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::onDestroyPhysicsMaterial(wpPhysicsMaterial_type _wpPhysicsMaterial)
{
    /// Fire the PhysicsMaterial's onDestroyEvent
    _wpPhysicsMaterial->onDestroyEvent(_wpPhysicsMaterial);
    
    /// delete the PhysicsMaterial pointer
    PhysicsMaterial* pPhysicsMaterial = dynamic_cast<PhysicsMaterial*>(_wpPhysicsMaterial.get());

    if (pPhysicsMaterial)
    {
        delete pPhysicsMaterial;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::onDestroyPhysicsMaterial() : _wpPhysicsMaterial is an invalid PhysicsMaterial.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::onDestroyPhysicsJoint(wpPhysicsJoint_type _wpPhysicsJoint)
{
    /// Fire the PhysicsJoint's onDestroyEvent
    _wpPhysicsJoint->onDestroyEvent(_wpPhysicsJoint);
    
    /// delete the PhysicsJoint pointer
    PhysicsJoint* pPhysicsJoint = dynamic_cast<PhysicsJoint*>(_wpPhysicsJoint.get());

    if (pPhysicsJoint)
    {
        delete pPhysicsJoint;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::onDestroyPhysicsJoint() : _wpPhysicsJoint is an invalid PhysicsJoint.");
    }
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
//see http://www.newtondynamics.com/wiki/index.php5?title=NewtonWorldRayCast
//for reference
void 
PhysicsZone::rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor)
{
    Math::Vector3 offset = _ray.getDirection();
    offset.normalize();
    offset *= _maxDistance;
    Math::Point3 endpoint = _ray.getOrigin() + offset;

    RayCastResult rayCastQuery(_maxDistance, _visitor);
    NewtonWorldRayCast(m_pZone, _ray.getOrigin().m_array, endpoint.m_array, rayCastFilter, &rayCastQuery, rayCastPrefilter);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//implement a ray cast filter
Zen::Math::Real 
PhysicsZone::rayCastFilter(const NewtonBody* _pBody, const Zen::Math::Real* _pNormal, int _collisionID, void* _pUserData, Zen::Math::Real _intersectDistance)
{
    PhysicsZone::RayCastResult* pCastquery = static_cast<PhysicsZone::RayCastResult*>(_pUserData);
    PhysicsActor* pRawPhysicsActor = static_cast<PhysicsActor*>(NewtonBodyGetUserData(_pBody));
    assert(pCastquery != NULL);
    assert(pRawPhysicsActor != NULL);

    Math::Vector3 normal(
        static_cast<const Math::Real*>(_pNormal)
    );

    return (pCastquery->m_visitor.visit(
                pRawPhysicsActor->getSelfReference().lock(), 
                normal, 
                (Math::Real)_intersectDistance * pCastquery->m_distance
            )) ? 1.0f : 0.0f;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//newton 1.53 Tutorial_06_UtilityFuntionality says:
// implement a ray cast pre-filter
unsigned int
PhysicsZone::rayCastPrefilter(const NewtonBody* _pBody, const NewtonCollision* _pCollision, void* _pUserData)
{
    PhysicsZone::RayCastResult* pCastquery = static_cast<PhysicsZone::RayCastResult*>(_pUserData);
    PhysicsActor* pRawPhysicsActor = static_cast<PhysicsActor*>(NewtonBodyGetUserData(_pBody));
    assert(pCastquery != NULL);
    assert(pRawPhysicsActor != NULL);

    Math::Point3 position;
    position = pRawPhysicsActor->getPosition();

    return (pCastquery->m_visitor.filter(pRawPhysicsActor->getSelfReference().lock())) ? 1 : 0;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createNullShape()
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));
    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateNull(m_pZone));
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createBoxShape(float _dx, float _dy, float _dz)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));
    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateBox(m_pZone, _dx, _dy, _dz, NULL));
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createOvoidShape(float _radiusX, float _radiusY, float _radiusZ)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));
    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateSphere(m_pZone, _radiusX, _radiusY, _radiusZ, NULL));
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return pShape;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createCapsuleShape(float _radius, float _height)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));
    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateCapsule(m_pZone, _radius, _height, NULL));
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return pShape;
}

//TODO - Break These Out into a proper resource - bjr
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createConvexHullShape(std::string _filename)
{
    // TODO impl
    std::cout << "Error: ConvexHull physics shape not yet implemented." << std::endl;

    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));
    //collision = pShape->setShapePtr(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), ..., NULL));
    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createTreeCollisionShape(std::string _filename)
{
    // TODO impl
    std::cout << "Error: TreeCollision physics shape not yet implemented." << std::endl;

    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));
    //collision = pShape->setShapePtr(NewtonCreateTreeCollision(dynamic_cast<PhysicsZone*>(m_pZone.get())->getZonePtr(), ...));
    return pShape;
}



//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// GLOBAL
void
ZNewtonSerializeFile(void* serializeHandle, const void* buffer, size_t size)
{
	fwrite(buffer, size, 1, (FILE*)serializeHandle);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// GLOBAL
void
ZNewtonDeSerializeFile(void* serializeHandle, void* buffer, size_t size)
{
	fread(buffer, size, 1, (FILE*)serializeHandle);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateTreeCollision(m_pZone, NULL));
    std::cout << "\nTreeCollision construction begun from RAW file..." << std::endl;
    NewtonTreeCollisionBeginBuild((dynamic_cast<CollisionShape*>(pShape.get())->getShapePtr()));

    // TODO - verify the file is a RAW file
    FILE* file;
    file = fopen(_filename.c_str(), "rb");
    if( file== NULL )
	{
        std::cout << "Error: could not open file " << _filename.c_str() << "in CollisionShape::initHeightFieldShape()." << std::endl;
        
            throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::createHeightFieldShapeFromRaw - Could not Open File!");
    }

    // read the RAW file into a heightfield array
    // TODO - don't assume this is a 16-bit RAW file
    Zen::Math::Real heightScaleFactor = 65536.0f / _maxHeight;
    // TODO - change to managed pointer:
    Zen::Math::Real* pHeightFieldArray = new Zen::Math::Real[_size * _size];
    size_t row, column;
    for (row = 0; row < _size; row++)
    {
        U16 height = 0;
        for (column = 0; column < _size; column++)
        {
            fread(&height, sizeof(U16), 1, file);
            pHeightFieldArray[column + (row * _size)] = height / heightScaleFactor;
        }
    }
    fclose(file);

    // now parse the array to create triangle vertices
    std::cout << "TreeCollision finished reading into Array.  Now assigning vertices for row ";
    Zen::Math::Real vertexArray[9];
    const Zen::Math::Real* pVertexArray = &vertexArray[0];
    size_t sizeMinusOne = _size - 1;
    size_t arrIndex;
    int strideSize = sizeof(Zen::Math::Real) * 3;
    for (row = 0; row < sizeMinusOne; row++)
    {
        std::cout << " " << row;
        for (column = 0; column < sizeMinusOne; column++)
        {
            arrIndex = column + (row * _size);

            vertexArray[0] = (Zen::Math::Real)column * _scaleXY;
            vertexArray[1] = pHeightFieldArray[arrIndex + _size];
            vertexArray[2] = (Zen::Math::Real)(row + 1.0f) * _scaleXY;

            vertexArray[3] = (Zen::Math::Real)(column + 1.0f) * _scaleXY;
            vertexArray[4] = pHeightFieldArray[arrIndex + 1];
            vertexArray[5] = (Zen::Math::Real)row * _scaleXY;

            vertexArray[6] = (Zen::Math::Real)column * _scaleXY;
            vertexArray[7] = pHeightFieldArray[arrIndex];
            vertexArray[8] = (Zen::Math::Real)row * _scaleXY;

            NewtonTreeCollisionAddFace(dynamic_cast<CollisionShape*>(pShape.get())->getShapePtr(), 3, pVertexArray, strideSize, 0);

            vertexArray[0] = (Zen::Math::Real)(column + 1.0f) * _scaleXY;
            vertexArray[1] = pHeightFieldArray[arrIndex + _size + 1];
            vertexArray[2] = (Zen::Math::Real)(row + 1.0f) * _scaleXY;

            vertexArray[3] = (Zen::Math::Real)(column + 1.0f) * _scaleXY;
            vertexArray[4] = pHeightFieldArray[arrIndex + 1];
            vertexArray[5] = (Zen::Math::Real)row * _scaleXY;

            vertexArray[6] = (Zen::Math::Real)column * _scaleXY;
            vertexArray[7] = pHeightFieldArray[arrIndex + _size];
            vertexArray[8] = (Zen::Math::Real)(row + 1.0f) * _scaleXY;

            NewtonTreeCollisionAddFace(dynamic_cast<CollisionShape*>(pShape.get())->getShapePtr(), 3, pVertexArray, strideSize, 0);
        }
    }

    std::cout << "\nTreeCollision finished assigning vertices.\nNow beginning mesh optimization...\n\nPlease be patient, as this may take a few MINUTES depending on your computer." << std::endl;
    // TODO - reset this to final parm of 1 to force optimization.  set to 0 for testing speed only.
    NewtonTreeCollisionEndBuild(dynamic_cast<CollisionShape*>(pShape.get())->getShapePtr(), 1);
    //NewtonTreeCollisionEndBuild(collision, 0);
    std::cout << "TreeCollision finished optimizing.  Completed." << std::endl;
    delete pHeightFieldArray;

    if (_bSerialize)
    {
	    //FILE* file;
        //errno_t errcode;
        std::string serializeFile = _filename;
	    serializeFile.append(".collision");
		file = fopen(serializeFile.c_str(), "wb");
		if( file== NULL )
		{
            std::cout << "Error: could not open file " << _filename.c_str() << " for writing while serializing terrain." << std::endl;
            throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::createHeightFieldShapeFromRaw - Could not Serialize File!");
        }
        std::cout << "\nTreeCollision serializing started to file " << serializeFile.c_str() << std::endl;
	    NewtonTreeCollisionSerialize(dynamic_cast<CollisionShape*>(pShape.get())->getShapePtr(), ZNewtonSerializeFile, file);
        std::cout << "TreeCollision serializing completed." << std::endl;
	    fclose(file);
    }

    /* this is not description logic
    // Note: collision is deleted in attachBody() so do not use it after this function call.
    if (!attachBody((NewtonCollision *)pShape->getShapePtr()))
        return false;

    NewtonBodySetMatrix(pShape->getActorPtr(), _transform.m_array);

    return true;
    */

    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform)
{
    std::cout << "\nLoading TreeCollision from file " << _filename.c_str() << std::endl;
	FILE* file;

    file = fopen(_filename.c_str(), "rb");
    if( file==0 )
    {
        std::cout << "Error: could not open file " << _filename.c_str() << " for loading terrain." << std::endl;
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::createHeightFieldShapeFromSerialization - Could not Open File!");
    }
    
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateTreeCollision(m_pZone, NULL));

    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateTreeCollisionFromSerialization(m_pZone, NULL, ZNewtonDeSerializeFile, file));
	fclose (file);
    std::cout << "Finished loading TreeCollision from file." << std::endl;
    
    /* this is not description logic
    // Note: collision is deleted in attachBody() so do not use it after this function call.
    if (!attachBody((NewtonCollision *)pShape->getShapePtr()))
        return false;

    NewtonBodySetMatrix(pShape->getActorPtr(), _transform.m_array);

    return true;
    */
    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::destroyCollisionShape(wpCollisionShape_type _wpCollisionShape)
{
    // TODO - remove appropriate entry from m_zoneSet
    //m_zoneSet.erase(iter);

    /// Fire the PhysicsZone's onDestroyEvent
    _wpCollisionShape->onDestroyEvent(_wpCollisionShape);
    
    /// delete the PhysicsZone pointer
    CollisionShape* pCollisionShape = dynamic_cast<CollisionShape*>(_wpCollisionShape.get());

    if (pCollisionShape)
    {
        delete pCollisionShape;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZNewton::PhysicsZone::destroyCollisionShape() : _wpCollisionShape is an invalid CollisionShape.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::destroyPhysicsActor(wpPhysicsActor_type _wpPhysicsActor)
{
    // TODO - remove appropriate entry from m_zoneSet
    //m_zoneSet.erase(iter);

    /// Fire the PhysicsZone's onDestroyEvent
    _wpPhysicsActor->onDestroyEvent(_wpPhysicsActor);
    
    /// delete the PhysicsZone pointer
    PhysicsActor* pPhysicsActor = dynamic_cast<PhysicsActor*>(_wpPhysicsActor.get());

    if (pPhysicsActor)
    {
        delete pPhysicsActor;
    }
    else
    {
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsZone::destroyPhysicsActor() : _wpPhysicsActor is an invalid PhysicsActor.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::stepSimulation(double _elapsedTime)
{
    NewtonUpdate(getZonePtr(), (Zen::Math::Real)_elapsedTime);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
