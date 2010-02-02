//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Game Tutorial
//
// Copyright (C) 2001 - 2008 Tony Richards
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
// This project is part of the Zen Engine Tutorials
//
// For more details, click on the link below for the IndieZen.org documentation:
//
// http://www.indiezen.org/wiki/wiki/zoss/Engine/Tutorials
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "AvatarGameObject.hpp"
#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Zen/Engine/Physics/I_PhysicsMaterial.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Game {
namespace Client {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AvatarGameObject::AvatarGameObject(GameClient& _client, Zen::Engine::Core::I_GameGroup& _group, const std::string& _name)
:   GameObject(_client, _group, _name)
{
    // TODO Hook up to Core::I_GameObject::onDestroy event (doesn't exist yet)
    // When that event is fired, this object needs to be deleted.

    // TODO call m_baseObject.setUserData(this); or something like that so you can get back
    // to AvatarGameObject from a I_GameObject

    m_pActor = base().getPhysicsActor();
    //m_pActor->setName(base().getName());

    m_pZone = m_pActor->getPhysicsZone();

    m_pActor->setCollisionShape(m_pZone->createCapsuleShape(0.5f, 1.5f));

    // mass of 1.0f is inappropriate, but we need to set a non-zero value here
    m_pActor->setMass(75.0f);

    //m_pShape->setAngularDamping(Zen::Math::Vector3(1.0f, 1.0f, 1.0f));

    m_pUpJoint = m_pZone->createJoint();
    m_pUpJoint->attachActor(m_pActor);
    m_pUpJoint->initUpVectorJoint(Zen::Math::Vector3(0.0f, 1.0f, 0.0f));

	m_moveSpeed = 10.0f;	//too much lower than this, and you can't overcome your own inertia
	m_stepHeight = 85.0f;	//gravity counterforce for moevemt. Keep slightly below gravity.
							//too high, you start to float. To low, it's like youre dragging your feet.
							//Also by it's nature influences how steep a slope you can climb without going at an angle.
	m_turnSpeed = 94.2477f;	//30PI
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
AvatarGameObject::~AvatarGameObject()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Client
}   // namespace Game
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
