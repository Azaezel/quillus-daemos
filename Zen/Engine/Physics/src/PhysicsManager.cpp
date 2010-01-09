//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
// Copyright (C) 2008 - 2009 Walt Collins
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
//  Matthew Alan Gray mgray@indiezen.org
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "PhysicsManager.hpp"

#include <Zen/Core/Math/Point3.hpp>

#include <Zen/Core/Scripting/I_ScriptEngine.hpp>
#include <Zen/Core/Scripting/I_ScriptModule.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>

#include <Zen/Engine/Physics/I_PhysicsShape.hpp>

#include <boost/any.hpp>

#include <vector>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Physics {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

PhysicsManager::PhysicsManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsManager::~PhysicsManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_PhysicsManager::pPhysicsService_type
PhysicsManager::create(const std::string& _type, config_type& _config)
{
    Threading::CriticalSection guard(m_physicsServiceCache.getLock());

    pPhysicsService_type pService(m_physicsServiceCache.getCachedService(_type));

    if (pService.isValid())
    {
        return pService;
    }

    I_PhysicsServiceFactory* pFactory = m_physicsServiceCache.getFactory(_type);

    if (pFactory == NULL)
    {
        // TODO Error
        return pService;
    }

#if 0 // do this to add scripting support
    if (m_pDefaultScriptEngine != NULL)
    {
        registerScriptEngine(m_pDefaultScriptEngine, pService);
    }
#endif

    // Connect PhysicsManager::onFrame() to the PhysicsService onFrameEvent
    //pService->onFrameEvent.connect(boost::bind(&PhysicsManager::onFrame, this, _1));

    return m_physicsServiceCache.cacheService(_type, pFactory->create(_type, _config));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::onFrame(FrameDelta_type frameDelta)
{
    // Fire the PhysicsManager::onFrameEvent event
    this->onFrameEvent(frameDelta);
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerDefaultScriptEngine(pScriptEngine_type _pEngine)
{
    registerScriptTypes(_pEngine);

    /// Register all of the existing services
    if(m_pDefaultScriptEngine == NULL)
    {
        Threading::CriticalSection guard(m_physicsServiceCache.getLock());

        for(PhysicsServiceCache_type::iterator iter = m_physicsServiceCache.begin(); iter != m_physicsServiceCache.end(); iter++)
        {
            registerScriptEngine(_pEngine, iter->second);
        }
    }

    m_pDefaultScriptEngine = _pEngine;
    m_scriptTypesInitialized = false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsManager::pScriptModule_type
PhysicsManager::getDefaultScriptModule()
{
    return m_pPhysicsModule;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerScriptEngine(pScriptEngine_type _pEngine, pPhysicsService_type _pService)
{
    // TODO - implement me
    //new Scripting::ObjectReference<I_PhysicsService>(m_pPhysicsModule, m_pPhysicsServiceType, _pService, "physicsService");
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static void 
setMass(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(float _mass)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Real mass = boost::any_cast<Math::Real>(_parms[0]);

    return pObject->getObject()->setMass(mass);
}

static void 
setScale(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(float _x, float _y, float _z)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Real dx = boost::any_cast<Math::Real>(_parms[0]);
    Math::Real dy = boost::any_cast<Math::Real>(_parms[1]);
    Math::Real dz = boost::any_cast<Math::Real>(_parms[2]);

    return pObject->getObject()->setScale(dx, dy, dz);
}

static void 
setPosition(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Vector3& _position)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    // TODO This should be a Point3 and not a Vector3
    Math::Vector3* pPosition = boost::any_cast<Math::Vector3*>(_parms[0]);

    Math::Point3 realPosition(pPosition->m_x, pPosition->m_y, pPosition->m_z);
    pObject->getObject()->setPosition(realPosition);
}

static void
getPosition(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(Math::Vector3& _position)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pPosition = boost::any_cast<Math::Vector3*>(_parms[0]);

    Math::Point3 position;

    pObject->getObject()->getPosition(position);

    pPosition->m_x = position.m_x;
    pPosition->m_y = position.m_y;
    pPosition->m_z = position.m_z;
}

static void 
setOrientation(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Quaternion4& _orient)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Quaternion4* pOrient = boost::any_cast<Math::Quaternion4*>(_parms[0]);

    pObject->getObject()->setOrientation(*pOrient);
}

static void 
setVelocity(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Vector3& _velocity)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pVelocity = boost::any_cast<Math::Vector3*>(_parms[0]);

    pObject->getObject()->setVelocity(*pVelocity);
}

static void 
getVelocity(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(Math::Vector3& _velocity)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pVelocity = boost::any_cast<Math::Vector3*>(_parms[0]);

    pObject->getObject()->getVelocity(*pVelocity);
}

static void 
setAngularMomentum(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Vector3& _omega)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pOmega = boost::any_cast<Math::Vector3*>(_parms[0]);

    pObject->getObject()->setAngularMomentum(*pOmega);
}

static void 
setAngularDamping(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(const Math::Vector3& _damping)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Vector3* pDamping = boost::any_cast<Math::Vector3*>(_parms[0]);

    pObject->getObject()->setAngularDamping(*pDamping);
}

static void 
setLinearDamping(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(float _damping)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    Math::Real damping = boost::any_cast<Math::Real>(_parms[0]);

    return pObject->getObject()->setLinearDamping(damping);
}

static void 
setAutoFreeze(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(bool _bFreeze)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    bool bFreeze = boost::any_cast<bool>(_parms[0]);

    return pObject->getObject()->setAutoFreeze(bFreeze);
}

static bool 
initNullShape(Scripting::I_ObjectReference* _pObject)
//()
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    return pObject->getObject()->initNullShape();
}

static bool 
initBoxShape(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(float _dx = 1.0f, float _dy = 1.0f, float _dz = 1.0f)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    if (_parms.size() == 0)
    {
        return pObject->getObject()->initBoxShape();
    }
    else
    {

        Math::Real dx = boost::any_cast<Math::Real>(_parms[0]);
        Math::Real dy = boost::any_cast<Math::Real>(_parms[1]);
        Math::Real dz = boost::any_cast<Math::Real>(_parms[2]);

        return pObject->getObject()->initBoxShape(dx, dy, dz);
    }
}

static bool 
initOvoidShape(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(float _radiusX = 1.0f, float _radiusY = 1.0f, float _radiusZ = 1.0f)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    if (_parms.size() == 0)
    {
        return pObject->getObject()->initOvoidShape();
    }
    else
    {

        Math::Real dx = boost::any_cast<Math::Real>(_parms[0]);
        Math::Real dy = boost::any_cast<Math::Real>(_parms[1]);
        Math::Real dz = boost::any_cast<Math::Real>(_parms[2]);

        return pObject->getObject()->initOvoidShape(dx, dy, dz);
    }
}

static bool 
initCapsuleShape(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(float _radius = 1.0f, float _height = 1.0f)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    if (_parms.size() == 0)
    {
        return pObject->getObject()->initCapsuleShape();
    }
    else
    {

        Math::Real radius = boost::any_cast<Math::Real>(_parms[0]);
        Math::Real height = boost::any_cast<Math::Real>(_parms[1]);

        return pObject->getObject()->initCapsuleShape(radius, height);
    }
    return false;
}

static bool 
initHeightFieldShapeFromRaw(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(std::string _filename, size_t _size, float _maxHeight, float _scaleXY, const Math::Matrix4& _transform, bool _bSerialize)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    std::string filename = boost::any_cast<std::string>(_parms[0]);
    size_t size = boost::any_cast<int>(_parms[1]);
    Math::Real maxHeight = boost::any_cast<Math::Real>(_parms[2]);
    Math::Real scaleXY = boost::any_cast<Math::Real>(_parms[3]);
    Math::Matrix4 transform = boost::any_cast<Math::Matrix4>(_parms[4]);
    bool bSerialize = boost::any_cast<bool>(_parms[5]);
    return pObject->getObject()->initHeightFieldShapeFromRaw(filename, size, (float)maxHeight, (float)scaleXY, transform, bSerialize);
}

static bool 
initHeightFieldShapeFromSerialization(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(std::string _filename, const Math::Matrix4& _transform)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    std::string filename = boost::any_cast<std::string>(_parms[0]);
    Math::Matrix4 transform = boost::any_cast<Math::Matrix4>(_parms[1]);

    return pObject->getObject()->initHeightFieldShapeFromSerialization(filename, transform);
}

static bool 
initConvexHullShape(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(std::string _filename)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    std::string filename = boost::any_cast<std::string>(_parms[0]);

    return pObject->getObject()->initConvexHullShape(filename);
}

static bool 
initTreeCollisionShape(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(std::string _filename)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    std::string filename = boost::any_cast<std::string>(_parms[0]);

    return pObject->getObject()->initTreeCollisionShape(filename);
}

static void 
setAdvancedCollisionPrediction(Scripting::I_ObjectReference* _pObject, std::vector<boost::any> _parms)
//(bool _mode)
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    bool mode = boost::any_cast<bool>(_parms[0]);

    pObject->getObject()->setAdvancedCollisionPrediction(mode);
}

static bool 
getAdvancedCollisionPrediction(Scripting::I_ObjectReference* _pObject)
//()
{
    I_CollisionShape::ScriptObjectReference_type* pObject = dynamic_cast<I_CollisionShape::ScriptObjectReference_type*>(_pObject);

    return pObject->getObject()->getAdvancedCollisionPrediction();
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
PhysicsManager::registerScriptTypes(pScriptEngine_type _pEngine)
{
    /// Don't bother if the types have already been initialized
    if (m_scriptTypesInitialized == true || _pEngine == NULL)
        return;

    // Create a Physics module
    m_pPhysicsModule = _pEngine->createScriptModule("Physics", "Zen Physics Module");

    // Expose I_PhysicsService to the Script Engine
    //m_pPhysicsServiceType = m_pPhysicsModule->createScriptType("PhysicsService", "Physics Service", 0);
    //m_pSceneServiceType->addMethod("createZone", "Create a Physics World", createChildNode);

    // Expose I_PhysicsWord to the Script Engine
    m_pPhysicsZoneType = m_pPhysicsModule->createScriptType("PhysicsZone", "Physics Zone", 0);
    //m_pPhysicsZoneType->addMethod("setZoneSize", "Set the size of the world.", setZoneSize);
    //m_pPhysicsZoneType->addMethod("createShape", "", createShape);

    // Expose I_CollisionShape to the Script Engine
    m_pCollisionShapeType = m_pPhysicsModule->createScriptType("CollisionShape", "Physics Shape", 0);
    m_pCollisionShapeType->addMethod("setMass", "", setMass);
    m_pCollisionShapeType->addMethod("setScale", "", setScale);
    m_pCollisionShapeType->addMethod("setPosition", "", setPosition);
    m_pCollisionShapeType->addMethod("getPosition", "", getPosition);
    m_pCollisionShapeType->addMethod("setOrientation", "", setOrientation);
    m_pCollisionShapeType->addMethod("setVelocity", "", setVelocity);
    m_pCollisionShapeType->addMethod("getVelocity", "", getVelocity);
    m_pCollisionShapeType->addMethod("setAngularMomentum", "", setAngularMomentum);
    m_pCollisionShapeType->addMethod("setAngularDamping", "", setAngularDamping);
    m_pCollisionShapeType->addMethod("setLinearDamping", "", setLinearDamping);
    m_pCollisionShapeType->addMethod("setAutoFreeze", "", setAutoFreeze);
    m_pCollisionShapeType->addMethod("initNullShape", "", initNullShape);
    m_pCollisionShapeType->addMethod("initBoxShape", "", initBoxShape);
    m_pCollisionShapeType->addMethod("initOvoidShape", "", initOvoidShape);
    m_pCollisionShapeType->addMethod("initCapsuleShape", "", initCapsuleShape);
    m_pCollisionShapeType->addMethod("initHeightFieldShapeFromRaw", "", initHeightFieldShapeFromRaw);
    m_pCollisionShapeType->addMethod("initHeightFieldShapeFromSerialization", "", initHeightFieldShapeFromSerialization);
    m_pCollisionShapeType->addMethod("initConvexHullShape", "", initConvexHullShape);
    m_pCollisionShapeType->addMethod("initTreeCollisionShape", "", initTreeCollisionShape);
    m_pCollisionShapeType->addMethod("setAdvancedCollisionPrediction", "", setAdvancedCollisionPrediction);
    m_pCollisionShapeType->addMethod("getAdvancedCollisionPrediction", "", getAdvancedCollisionPrediction);

    m_pPhysicsModule->activate();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Physics
}   // namespace Engine
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

