//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
#include "PhysicsProcessor.hpp"
#include "PhysicsActor.hpp"
#include "PhysicsActorFactory.hpp"
#include <Physics/include/NxPhysics.h>
#include <Foundation/include/NxVec3.h>
#include <Foundation/include/NxQuat.h>

#include <IndieZen/framework/Rendering/I_SceneNode.hpp>
#include <IndieZen/framework/Math/Quaternion4.hpp>


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace IndieZen {
namespace ZPhysX {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsProcessor::PhysicsProcessor()
{
    NxPhysicsSDKDesc desc;
    NxSDKCreateError errorCode = NXCE_NO_ERROR;

    NxPhysicsSDK* pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL, desc, &errorCode);
    if( pPhysicsSDK == NULL )
    {
        throw std::exception("Exception: Cannot create PhysX SDK Pointer.");
    }
    this->m_pPhysicsSDK = pPhysicsSDK;

    this->m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.025f);

    NxSceneDesc sceneDesc;
    sceneDesc.gravity = NxVec3(0.0f, -9.81f, 0.0f);

    NxScene* pScene = this->m_pPhysicsSDK->createScene(sceneDesc);
    if( pScene == NULL )
    {
        throw std::exception("Exception: Cannot create PhysX Scene Pointer.");
    }
    this->m_pScene = pScene;

    PhysicsActorFactory* pFactory = &PhysicsActorFactory::instance();

    pFactory->setScene(this->m_pScene);

    NxMaterial * defaultMaterial = this->m_pScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.0f);
	defaultMaterial->setStaticFriction(0.5f);
	defaultMaterial->setDynamicFriction(0.5f);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
PhysicsProcessor::~PhysicsProcessor()
{
    this->m_actorMap.clear();

    if(this->m_pPhysicsSDK != NULL)
	{
		if(this->m_pScene != NULL) 
        {
            this->m_pPhysicsSDK->releaseScene(*this->m_pScene);
        }
        this->m_pScene = NULL;

        NxReleasePhysicsSDK(this->m_pPhysicsSDK);
        this->m_pPhysicsSDK = NULL;
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsProcessor::addSceneNode(scene_node_ptr_type _pSceneNode)
{
    Physics::I_PhysicsActorFactory* pFactory = &PhysicsActorFactory::instance();

    Physics::I_PhysicsActor* pPhysicsActor = pFactory->create();

    pPhysicsActor->setSceneNode(_pSceneNode);

    _pSceneNode->attachPhysicsActor(*pPhysicsActor);

    this->m_actorMap[_pSceneNode] = pPhysicsActor;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsProcessor::dropSceneNode(scene_node_ptr_type _pSceneNode)
{
    actor_map_type::iterator found;

    found = this->m_actorMap.find(_pSceneNode);
    if( found != this->m_actorMap.end() )
    {
        this->m_actorMap.erase(found);
    }
    else
    {
        throw std::exception("Exception: Scene node actor does not exist.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsProcessor::simulate(double _elapsedTime)
{
    this->m_pScene->simulate((NxReal)_elapsedTime);
    this->m_pScene->flushStream();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
PhysicsProcessor::checkResults()
{
    if( this->m_pScene != NULL )
    {
        return this->m_pScene->checkResults(NX_RIGID_BODY_FINISHED,false);
    }
    else
    {
        throw std::exception("Exception: NULL PhysX Scene Pointer");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
PhysicsProcessor::fetchResults()
{
    if( this->m_pScene != NULL )
    {
        this->m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,true);

        long i;
        long numActors = this->m_pScene->getNbActors();
        NxActor** ppActors = this->m_pScene->getActors();

        for( i = 0 ; i < numActors ; i++ )
        {
            NxVec3 position = ppActors[i]->getGlobalPosition();
            NxQuat rotation = ppActors[i]->getGlobalOrientationQuat();
            Math::Quaternion4 quaternion(rotation.w,
                                         rotation.x,
                                         rotation.y,
                                         rotation.z);
            Rendering::I_SceneNode* pSceneNode = (Rendering::I_SceneNode *)(ppActors[i]->userData);
            pSceneNode->setPosition(position[0], position[1], position[2]);
            pSceneNode->setRotation(quaternion);
        }
    }
    else
    {
        throw std::exception("Exception: NULL PhysX Scene Pointer");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPhysX
}   // namespace IndieZen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
