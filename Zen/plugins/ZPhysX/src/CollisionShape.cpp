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
#include "PhysicsShape.hpp"
#include "PhysicsWorld.hpp"

#include <Zen/Core/Scripting/ObjectReference.hpp>

#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsManager.hpp>
#include <Zen/Engine/Physics/I_PhysicsService.hpp>
#include <Zen/Engine/Physics/I_PhysicsWorld.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

//#include <Zen/Core/Memory/managed_ptr.hpp>

#include <iostream>
#include <cstddef>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPhysX {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CollisionShape::CollisionShape(wpPhysicsZone_type _zone)
:   m_body(NULL)
,   m_pZone(_zone)
,   m_pScriptObject(NULL)
,   m_activationState(1)
{
    // TODO impl scale
    m_scaleX = 1.0f;
    m_scaleY = 1.0f;
    m_scaleZ = 1.0f;

    m_name = "";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CollisionShape::~CollisionShape()
{
    //NewtonDestroyBody((NewtonWorld*)m_pZone->getZonePtr(), m_body);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CollisionShape::wpPhysicsZone_type
CollisionShape::getPhysicsZone()
{
    return m_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setName(const std::string& _name)
{
    m_name = _name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string& 
CollisionShape::getName()
{
    return m_name;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void*
CollisionShape::getBodyPtr()
{
    return m_body;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::attachBody(NewtonCollision* _collision)
{
    // valid Newton collision primitives include the following:
    //   Null, Box, Sphere (ovoid), Cone, Capsule, Cylinder, ChamferCylinder,
    //   ConvexHull (not impl yet), CompoundCollision (not impl),
    //   TreeCollision (not impl yet), UserMesh (heightfield)

    m_body = NewtonCreateBody((NewtonWorld*)m_pZone->getZonePtr(), _collision);
    NewtonReleaseCollision((NewtonWorld*)m_pZone->getZonePtr(), _collision);

    // set the transform call back function
    NewtonBodySetTransformCallback(m_body, TransformCallback);
    NewtonBodySetForceAndTorqueCallback(m_body, ApplyForceAndTorqueCallback);
    NewtonBodySetAutoactiveCallback(m_body, ActivationStateCallback);

    //NewtonBodySetFreezeTreshold(m_body, 

    NewtonBodySetUserData(m_body, this);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::initNullShape()
{
    return attachBody(NewtonCreateNull((NewtonWorld*)m_pZone->getZonePtr()));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::initBoxShape(float _dx, float _dy, float _dz)
{
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return attachBody(NewtonCreateBox((NewtonWorld*)m_pZone->getZonePtr(), _dx, _dy, _dz, NULL));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::initOvoidShape(float _radiusX, float _radiusY, float _radiusZ)
{
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return attachBody(NewtonCreateSphere((NewtonWorld*)m_pZone->getZonePtr(), _radiusX, _radiusY, _radiusZ, NULL));
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::initCapsuleShape(float _radius, float _height)
{
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return attachBody(NewtonCreateCapsule((NewtonWorld*)m_pZone->getZonePtr(), _radius, _height, NULL));
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
bool
CollisionShape::initHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
    NewtonCollision* collision = NewtonCreateTreeCollision((NewtonWorld*)m_pZone->getZonePtr(), NULL);
    std::cout << "\nTreeCollision construction begun from RAW file..." << std::endl;
    NewtonTreeCollisionBeginBuild(collision);

    // TODO - verify the file is a RAW file
    FILE* file;
    file = fopen(_filename.c_str(), "rb");
    if( file== NULL )
	{
        std::cout << "Error: could not open file " << _filename.c_str() << "in CollisionShape::initHeightFieldShape()." << std::endl;
        return false;
    }

    // read the RAW file into a heightfield array
    // TODO - don't assume this is a 16-bit RAW file
    dFloat heightScaleFactor = 65536.0f / _maxHeight;
    // TODO - change to managed pointer:
    dFloat* pHeightFieldArray = new dFloat[_size * _size];
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
    dFloat vertexArray[9];
    const dFloat* pVertexArray = &vertexArray[0];
    size_t sizeMinusOne = _size - 1;
    size_t arrIndex;
    int strideSize = sizeof(dFloat) * 3;
    for (row = 0; row < sizeMinusOne; row++)
    {
        std::cout << " " << row;
        for (column = 0; column < sizeMinusOne; column++)
        {
            arrIndex = column + (row * _size);

            vertexArray[0] = (dFloat)column * _scaleXY;
            vertexArray[1] = pHeightFieldArray[arrIndex + _size];
            vertexArray[2] = (dFloat)(row + 1.0f) * _scaleXY;

            vertexArray[3] = (dFloat)(column + 1.0f) * _scaleXY;
            vertexArray[4] = pHeightFieldArray[arrIndex + 1];
            vertexArray[5] = (dFloat)row * _scaleXY;

            vertexArray[6] = (dFloat)column * _scaleXY;
            vertexArray[7] = pHeightFieldArray[arrIndex];
            vertexArray[8] = (dFloat)row * _scaleXY;

            NewtonTreeCollisionAddFace(collision, 3, pVertexArray, strideSize, 0);

            vertexArray[0] = (dFloat)(column + 1.0f) * _scaleXY;
            vertexArray[1] = pHeightFieldArray[arrIndex + _size + 1];
            vertexArray[2] = (dFloat)(row + 1.0f) * _scaleXY;

            vertexArray[3] = (dFloat)(column + 1.0f) * _scaleXY;
            vertexArray[4] = pHeightFieldArray[arrIndex + 1];
            vertexArray[5] = (dFloat)row * _scaleXY;

            vertexArray[6] = (dFloat)column * _scaleXY;
            vertexArray[7] = pHeightFieldArray[arrIndex + _size];
            vertexArray[8] = (dFloat)(row + 1.0f) * _scaleXY;

            NewtonTreeCollisionAddFace(collision, 3, pVertexArray, strideSize, 0);
        }
    }

    std::cout << "\nTreeCollision finished assigning vertices.\nNow beginning mesh optimization...\n\nPlease be patient, as this may take a few MINUTES depending on your computer." << std::endl;
    // TODO - reset this to final parm of 1 to force optimization.  set to 0 for testing speed only.
    NewtonTreeCollisionEndBuild(collision, 1);
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
            return false;
        }
        std::cout << "\nTreeCollision serializing started to file " << serializeFile.c_str() << std::endl;
	    NewtonTreeCollisionSerialize(collision, ZNewtonSerializeFile, file);
        std::cout << "TreeCollision serializing completed." << std::endl;
	    fclose(file);
    }

    // Note: collision is deleted in attachBody() so do not use it after this function call.
    if (!attachBody(collision))
        return false;

    NewtonBodySetMatrix(m_body, _transform.m_array);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::initHeightFieldShapeFromSerialization(std::string _filename, const Math::Matrix4& _transform)
{
    std::cout << "\nLoading TreeCollision from file " << _filename.c_str() << std::endl;
	FILE* file;

    file = fopen(_filename.c_str(), "rb");
    if( file==0 )
    {
        std::cout << "Error: could not open file " << _filename.c_str() << " for loading terrain." << std::endl;
        return false;
    }
    NewtonCollision* collision = NewtonCreateTreeCollisionFromSerialization((NewtonWorld*)m_pZone->getZonePtr(), NULL, ZNewtonDeSerializeFile, file);
	fclose (file);
    std::cout << "Finished loading TreeCollision from file." << std::endl;

    // Note: collision is deleted in attachBody() so do not use it after this function call.
    if (!attachBody(collision))
        return false;

    NewtonBodySetMatrix(m_body, _transform.m_array);

    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setAdvancedCollisionPrediction(bool _mode)
{
    if (_mode)
    {
        NewtonBodySetContinuousCollisionMode(m_body, 1);
    }
    else
    {
        NewtonBodySetContinuousCollisionMode(m_body, 0);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::getAdvancedCollisionPrediction()
{
    return (NewtonBodyGetContinuousCollisionMode(m_body) != 0);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setMaterial(CollisionShape::pPhysicsMaterial_type _material)
{
    m_material = _material;
    NewtonBodySetMaterialGroupID(m_body, m_material->getMaterialID());
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CollisionShape::pPhysicsMaterial_type
CollisionShape::getMaterial()
{
    return m_material;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::initConvexHullShape(std::string _filename)
{
    // TODO impl
    std::cout << "Error: ConvexHull physics shape not yet implemented." << std::endl;
    //collision = NewtonCreateConvexHull((NewtonWorld*)m_pZone->getZonePtr(), ..., NULL);
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
CollisionShape::initTreeCollisionShape(std::string _filename)
{
    // TODO impl
    std::cout << "Error: TreeCollision physics shape not yet implemented." << std::endl;
    //collision = NewtonCreateTreeCollision((NewtonWorld*)m_pZone->getZonePtr(), ...);
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real
CollisionShape::getLinearDamping()
{
    return Math::Real(NewtonBodyGetLinearDamping(m_body));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setLinearDamping(float _damping)
{
    NewtonBodySetLinearDamping(m_body, _damping);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setAutoFreeze(bool _bFreeze)
{
    NewtonBodySetAutoFreeze(m_body, _bFreeze ? 1 : 0);
    NewtonWorldUnfreezeBody((NewtonWorld*)m_pZone->getZonePtr(), m_body);
    m_activationState = 1;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
float
CollisionShape::getMass()
{
    dFloat mass;
    dFloat Ixx;
    dFloat Iyy;
    dFloat Izz;
    NewtonBodyGetMassMatrix(m_body, &mass, &Ixx, &Iyy, &Izz);

    return mass;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
CollisionShape::SetStatic(bool _isStatic = false)
{
	m_IsStatic = _isStatic;
	if (_isStatic)
	{
		setMass(0);
	}
	else
		setMass(m_mass);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
CollisionShape::setCollisionGroup(int _collisionGroup)
{
	m_collisionGroup = _collisionGroup;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
CollisionShape::getCollisionGroup()
{
	return m_collisionGroup;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
CollisionShape::setOwner(pOwningObject_type _pOwner)
{
	m_pOwner = _pOwner;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
CollisionShape::pOwningObject_type
CollisionShape::getOwner()
{
	return m_pOwner;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setMass(float _mass)
{
    m_Actor->setMass(_mass);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setScale(float _x, float _y, float _z)
{
    m_scaleX = _x;
    m_scaleY = _y;
    m_scaleZ = _z;

    std::cout << "Warning: PhysX CollisionShape::setScale() not implemented for Generic Shapes!" << std::endl;    
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setPosition(const Math::Point3& _pos)
{
    m_Actor.setGlobalPosition(NxVec3(_pos.m_array));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::getPosition(Math::Point3& _position)
{
    //NxVec3::get returns an Array of 3 floats.
    Math::Real *pos = m_Actor.getGlobalPosition().get();
    _position.m_array = *pos;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::getOrientation(Math::Quaternion4& _orient)
{
    Math::Matrix4 matrix;
    getOrientation(matrix);
    _orient = Math::Quaternion4(matrix);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::getOrientation(Math::Matrix4& _orient)
{
    NxMat33 orient = m_Actor.getGlobalOrientation();
    Vector3 *left, *right, *up;
    orient.getRow(0).get(left);
    orient.getRow(1).get(right);
    orient.getRow(2).get(up);

    Math::Matrix4 matrix = Math::Matrix4(left->m_x,left->m_y,left->m_z,0.0,
                                         right->m_x,right->m_y,right->m_z,0.0,
                                         up->m_x,up->m_y,up->m_z,0.0);    
    // transfer values to the return matrix: redundant? -bjr
    for (int i = 0; i < 16; i++)
    {
        _orient.m_array[i] = matrix.m_array[i];
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setOrientation(const Math::Quaternion4& _orient)
{
    Math::Matrix4 matrix = Math::Matrix4(_orient);
    setOrientation(matrix);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setOrientation(const Math::Matrix4& _orient)
{
    m_activationState = 1;

    NxVec3 nxLeft = NxVec3(_orient.getLeftVector().m_array);
    NxVec3 nxUp = NxVec3(_orient.getLeftVector().m_array);
    NxVec3 nxForward = NxVec3(_orient.getLeftVector().m_array);

    m_Actor.setGlobalOrientation(NxMat33(nxLeft,nxUp,nxForward));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setVelocity(const Math::Vector3& _velocity)
{
    m_activationState = 1;
    NxVec3 v;

	v.x = _velocity.x;
	v.y = _velocity.y;
	v.z = _velocity.z;
	m_Actor->setLinearVelocity(v);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::getVelocity(Math::Vector3& _velocity)
{
	NxVec3 v;
	v = m_Actor->getLinearVelocity();
	_velocity.x=v.x;
	_velocity.y=v.y;
	_velocity.z=v.z;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::getTorque(Math::Vector3& _velocity)
{
    std::cout << "Warning: PhysX CollisionShape::getTorque() not implemented for generic Shapes!" << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Vector3
CollisionShape::getAngularMomentum()
{
	NxVec3 v;
	v = m_Actor->getAngularVelocity();

    Math::Vector3 momentum;
	momentum.x=v.x;
	momentum.y=v.y;
	momentum.z=v.z;
    return momentum;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setAngularMomentum(const Math::Vector3& _omega)
{
    NxVec3 v;
	v.x = _omega.x;
	v.y = _omega.y;
	v.z = _omega.z;
	m_Actor->setAngularVelocity(v);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Vector3
CollisionShape::getAngularDamping()
{
    Math::Vector3 damping;
    NewtonBodyGetAngularDamping(m_body, damping.m_array);
    return damping;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setAngularDamping(const Math::Vector3& _damping)
{
    NewtonBodySetAngularDamping(m_body, _damping.m_array);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// set the transformation of a rigid body
void 
CollisionShape::TransformCallback(const NewtonBody* _body, const dFloat* _matrix)
{
    void* pBody = NewtonBodyGetUserData(_body);
    if (pBody != NULL)
    {
        CollisionShape* pShape = static_cast<CollisionShape*>(pBody);

        // Only use the transform callback if the state is active
        if (pShape->m_activationState != 0)
        {
            Math::Matrix4 transform;
            for(int x = 0; x < 16; x++)
            {
                transform.m_array[x] = _matrix[x];
            }
            TransformEventData evenData(*pShape, transform);
            pShape->onTransformEvent(evenData);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::ApplyForceAndTorqueCallback(const NewtonBody* _body)
{
    class ApplyForcesEventData
    :   public I_ApplyForcesEventData
    {
        /// @name I_ApplyForcesEventData implementation
        /// @{
    public:
        /// Get the physics shape associated with this event
        virtual I_CollisionShape& getShape()
        {
            return *m_pShape;
        }

        /// Apply a force to this shape
        virtual void applyForce(const Math::Vector3& _force)
        {
            m_pShape->applyForce(_force);
        }

        /// Apply torque to this shape
        virtual void applyTorque(const Math::Vector3& _torque)
        {
            m_pShape->applyTorque(_torque);
        }

        /// @}

        /// @name 'Structors
        /// @{
    public:
        ApplyForcesEventData(CollisionShape* _pShape)
        :   m_pShape(_pShape)
        {
        }

        virtual ~ApplyForcesEventData()
        {
        }
        /// @}

        /// @name Member Variables
        /// @{
    private:
        CollisionShape*   m_pShape;
        /// @}

    };  // class ApplyForcesEventData;

    void* pBody = NewtonBodyGetUserData(_body);
    if (pBody != NULL)
    {
        CollisionShape* pCollisionShape = static_cast<CollisionShape*>(pBody);

        // Only apply forces if the state is active
        if (pCollisionShape->m_activationState != 0)
        {
            ApplyForcesEventData eventData(pCollisionShape);
            eventData.getShape().onApplyForcesEvent(eventData);
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::ActivationStateCallback(const NewtonBody* _body, unsigned _state)
{
    void* pBody = NewtonBodyGetUserData(_body);
    if (pBody != NULL)
    {
        static_cast<CollisionShape*>(pBody)->setActivationState(_state);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Physics::I_CollisionShape&
CollisionShape::TransformEventData::getShape()
{
    return m_shape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Matrix4&
CollisionShape::TransformEventData::getTransform() const
{
    return m_transform;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Physics::I_CollisionShape&
CollisionShape::CollisionEventData::getShape()
{
    return m_shape;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Engine::Physics::I_CollisionShape&
CollisionShape::CollisionEventData::getOtherShape()
{
    return m_otherShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
CollisionShape::getScriptObject()
{
    if (m_pScriptObject == NULL)
    {
        Zen::Engine::Physics::I_PhysicsManager::pScriptModule_type pModule =
            Zen::Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule();

        if(pModule.isValid())
        {
            m_pScriptObject = new Zen::Engine::Physics::I_CollisionShape::ScriptObjectReference_type
                (pModule, pModule->getScriptType("CollisionShape"), this);
        }
        else
        {
            m_pScriptObject = NULL;
        }
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::applyForce(const Math::Vector3& _force)
{

/*
enum NxForceMode  
 
   Parameter to addForce*() calls, determines the exact operation that is carried out. 


See also:
NxActor.addForce() NxActor.addTorque() 
Enumerator: 
NX_FORCE  parameter has unit of mass * distance/ time^2, i.e. a force  
NX_IMPULSE  parameter has unit of mass * distance /time  
NX_VELOCITY_CHANGE  parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.  
NX_SMOOTH_IMPULSE  same as NX_IMPULSE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.  
NX_SMOOTH_VELOCITY_CHANGE  same as NX_VELOCITY_CHANGE but the effect is applied over all substeps. Use this for motion controllers that repeatedly apply an impulse.  
NX_ACCELERATION  parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
*/
    //going to go with a convention that applyForce is a stright up vector, and applyImpulse is a vector that strikes a point, taking other things into account... at least for now -bjr
    m_Actor->addForce(_force.m_array,NX_SMOOTH_VELOCITY_CHANGE,true);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::applyTorque(const Math::Vector3& _torque)
{
    NxVec3 v;
	v.x=_torque.m_x;
    v.y=_torque.m_y;
    v.z=_torque.m_z;
	m_Actor->addTorque(v);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
CollisionShape::setActivationState(unsigned _state)
{
    m_activationState = _state;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZNewton
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
