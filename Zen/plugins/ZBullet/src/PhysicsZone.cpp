//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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

#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBullet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//refnote: http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World
PhysicsZone::PhysicsZone(const Math::Vector3& _min, const Math::Vector3& _max)
:   m_pScriptModule(Engine::Physics::I_PhysicsManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
{

    m_bDefaultMaterialCreated = false;

    // Build the broadphase
    int maxProxies = 1024;
    btVector3 worldAabbMin(-10000,-10000,-10000);
    btVector3 worldAabbMax(10000,10000,10000);
    m_pBroadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
 
    // Set up the collision configuration and dispatcher
    m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
    m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
 
    // The actual physics solver
    m_pSolver = new btSequentialImpulseConstraintSolver;
 
    // The world.
    m_pZone = new btDiscreteDynamicsWorld(m_pDispatcher,m_pBroadphase,m_pSolver,m_pCollisionConfiguration);
    m_pZone->setGravity(btVector3(0.0f, -9.8f, 0.0f));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::~PhysicsZone()
{
    if (m_pZone!= NULL) delete m_pZone;
    if (m_pSolver!= NULL) delete m_pSolver;
    if (m_pDispatcher!= NULL) delete m_pDispatcher;
    if (m_pCollisionConfiguration!= NULL) delete m_pCollisionConfiguration;
    if (m_pBroadphase != NULL) delete m_pBroadphase;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
btDiscreteDynamicsWorld*
PhysicsZone::getZonePtr(void)
{
    return m_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsZone::setZonePtr(btDiscreteDynamicsWorld* _pZone)
{
    m_pZone = (btDiscreteDynamicsWorld*)_pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
PhysicsZone::getDefaultMaterialID()
{
    return NULL;
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
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsZone::onDestroyActor() : _wpCollisionShape is an invalid CollisionShape.");
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
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsZone::onDestroyPhysicsMaterial() : _wpPhysicsMaterial is an invalid PhysicsMaterial.");
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
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsZone::onDestroyPhysicsJoint() : _wpPhysicsJoint is an invalid PhysicsJoint.");
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
//see http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=4152&hilit=raytest
//for reference
void 
PhysicsZone::rayCast(Math::Ray _ray, Math::Real _maxDistance, I_CollisionVisitor& _visitor)
{
    Math::Vector3 offset = _ray.getDirection();
    offset.normalize();
    offset *= _maxDistance;
    Math::Point3 endpoint = _ray.getOrigin() + offset;

    //(m_pZone, _ray.getOrigin().m_array, endpoint.m_array, rayCastFilter, &rayCastQuery, rayCastPrefilter);

    btVector3 tquatFrom = btVector3(_ray.getOrigin().m_x,_ray.getOrigin().m_y,_ray.getOrigin().m_z);
    btVector3 tquatTo = btVector3(_ray.getOrigin().m_x,_ray.getOrigin().m_y,_ray.getOrigin().m_z);

	RayResultCallback	resultCallback(tquatFrom,tquatTo);
   
	m_pZone->rayTest(tquatFrom,tquatTo,resultCallback);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
float
PhysicsZone::RayResultCallback::addSingleResult(btCollisionWorld::LocalRayResult& _rayResult,bool _normalInWorldSpace)
{
    //caller already does the filter on the m_closestHitFraction
    btAssert(_rayResult.m_hitFraction <= m_closestHitFraction);
    
    m_closestHitFraction = _rayResult.m_hitFraction;
    m_collisionObject = _rayResult.m_collisionObject;
    if (_normalInWorldSpace)
    {
        m_hitNormalWorld = _rayResult.m_hitNormalLocal;
    } else
    {
        ///need to transform normal into worldspace
        m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*_rayResult.m_hitNormalLocal;
    }
    m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,_rayResult.m_hitFraction);

    PhysicsZone::RayCastResult* pCastquery;
    PhysicsActor* pRawPhysicsActor = static_cast<PhysicsActor*>(m_collisionObject->getUserPointer());

    pCastquery->m_distance = m_closestHitFraction;

    Math::Vector3 normal = Math::Vector3(m_hitNormalWorld.getX(),m_hitNormalWorld.getY(),m_hitNormalWorld.getZ());
    if (pCastquery->m_visitor.filter(pRawPhysicsActor->getSelfReference().lock()))
    {
        return (pCastquery->m_visitor.visit(
            pRawPhysicsActor->getSelfReference().lock(), 
            normal, 
            pCastquery->m_distance
        )) ? 1.0f : 0.0f;
    }

    return 1.f;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createNullShape()
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    //bit of a hacktastic workaround, but see http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=4461&hilit=btRigidBody+vs+btCollisionObject for intent
    pRawPointer->setShapePtr(new btSphereShape(btScalar(1.)));
    pRawPointer->setIsNUllShape(true);
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createBoxShape(float _dx, float _dy, float _dz)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    pRawPointer->setShapePtr(new btBoxShape(btVector3(_dx,_dy,_dz)));
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return pShape;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createOvoidShape(float _radiusX, float _radiusY, float _radiusZ)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    //seem to recall something about a multisphere shape being scalable in multiple directions... research further
    pRawPointer->setShapePtr(new btSphereShape(btScalar(_radiusX)));
    // TODO - set offsetMatrix (currently passing NULL, which centers the body at its origin)
    return pShape;
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createCapsuleShape(float _radius, float _height)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    pRawPointer->setShapePtr(new btCapsuleShape(_radius, _height));
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
ZBulletSerializeFile(void* serializeHandle, const void* buffer, size_t size)
{
	fwrite(buffer, size, 1, (FILE*)serializeHandle);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// GLOBAL
void
ZBulletDeSerializeFile(void* serializeHandle, void* buffer, size_t size)
{
	fread(buffer, size, 1, (FILE*)serializeHandle);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsZone::pCollisionShape_type
PhysicsZone::createHeightFieldShapeFromRaw(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    // TODO - verify the file is a RAW file
    FILE* file;
    file = fopen(_filename.c_str(), "rb");
    if( file== NULL )
    {
        std::cout << "Error: could not open file " << _filename.c_str() << "in CollisionShape::initHeightFieldShape()." << std::endl;
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsZone::createHeightFieldShapeFromRaw - Could not Open File!");
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
            pHeightFieldArray[column + (row * _size)] = height;
        }
    }
    fclose(file);

    //btHeightfieldTerrainShape::btHeightfieldTerrainShape(int heightStickWidth, int heightStickLength,void* heightfieldData,btScalar maxHeight,int upAxis,bool useFloatData,bool flipQuadEdges)
    dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(new btHeightfieldTerrainShape(_scaleXY,_scaleXY,&pHeightFieldArray,_maxHeight,0,true,false));
//    std::cout << "\nTreeCollision finished assigning vertices.\nNow beginning mesh optimization...\n\nPlease be patient, as this may take a few MINUTES depending on your computer." << std::endl;
    delete pHeightFieldArray;

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
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsZone::createHeightFieldShapeFromSerialization - Could not Open File!");
    }
    
    CollisionShape* pRawPointer = new CollisionShape();
    pCollisionShape_type pShape(pRawPointer, boost::bind(&PhysicsZone::destroyCollisionShape, this, _1));

    //dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateTreeCollision(m_pZone, NULL));

    //dynamic_cast<CollisionShape*>(pShape.get())->setShapePtr(NewtonCreateTreeCollisionFromSerialization(m_pZone, NULL, ZBulletDeSerializeFile, file));
	fclose (file);
    std::cout << "Finished loading TreeCollision from file." << std::endl;
    
    /* this is not description logic
    // Note: collision is deleted in attachBody() so do not use it after this function call.
    if (!attachBody((btCollisionShape *)pShape->getShapePtr()))
        return false;

    btRigidBodySetMatrix(pShape->getActorPtr(), _transform.m_array);

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
        throw Zen::Utility::runtime_exception("Zen::ZBullet::PhysicsZone::destroyCollisionShape() : _wpCollisionShape is an invalid CollisionShape.");
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
    getZonePtr()->stepSimulation((Zen::Math::Real)_elapsedTime);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
