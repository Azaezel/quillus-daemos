//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
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
#ifndef ZEN_ZPHYSX_PHYSICS_PROCESSOR_HPP_INCLUDED
#define ZEN_ZPHYSX_PHYSICS_PROCESSOR_HPP_INCLUDED

#include <Zen/Engine/Physics/I_PhysicsProcessor.hpp>
#include <Zen/Engine/Physics/I_PhysicsActor.hpp>
#include <Physics/include/NxPhysics.h>
#include <boost/shared_ptr.hpp>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPhysX {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsProcessor
:   public Physics::I_PhysicsProcessor
{
    /// @name Types
    /// @{
public:
    typedef Physics::I_PhysicsActor*                        actor_ptr_type;
    typedef std::map<scene_node_ptr_type, actor_ptr_type>   actor_map_type;
    typedef NxPhysicsSDK*                                   sdk_ptr_type;
    typedef NxScene*                                        scene_ptr_type;
    /// @}

    /// @name I_PhysicsProcessor implementation
    /// @{
public:
    virtual void addSceneNode(scene_node_ptr_type _pSceneNode);
    virtual void dropSceneNode(scene_node_ptr_type _pSceneNode);
    virtual void simulate(double _elapsedTime);
    virtual bool checkResults();
    virtual void fetchResults();
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsProcessor();
    virtual ~PhysicsProcessor();
    /// @}

    /// @name Member Variables
    /// @{
private:

    actor_map_type  m_actorMap;
    sdk_ptr_type    m_pPhysicsSDK;
    scene_ptr_type  m_pScene;

    /// @}

};  // class PhysicsProcessor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPhysX
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZPHYSX_PHYSICS_PROCESSOR_HPP_INCLUDED
