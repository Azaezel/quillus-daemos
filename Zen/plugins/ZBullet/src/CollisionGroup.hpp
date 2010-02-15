//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2009 Brian Roberts
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
//  Brian Roberts
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZBULLET_COLLISION_GROUP_HPP_INCLUDED
#define ZEN_ZBULLET_COLLISION_GROUP_HPP_INCLUDED

#include <vector>
#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Physics/I_CollisionGroup.hpp>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZBullet {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class CollisionGroupManager
:   public Engine::Physics::I_CollisionGroupManager
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_CollisionGroupManager>
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_CollisionGroupManager interface
    /// @{
public:
    virtual void setMaxGroups(int _groupCount);
    virtual int getMaxGroups();
    virtual void setGroupInteracts(int _lGroupID,int _rGroupID, bool _interacts);
    virtual bool getGroupInteracts(int _lGroupID,int _rGroupID);
    /// @}

    /// @name I_CollisionGroupManager implementation
    /// @{
public:

    virtual const std::string& getName();
    virtual void setName(const std::string& _name);
	virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name 'Structors
    /// @{
public:
    CollisionGroupManager();
    virtual ~CollisionGroupManager();
    /// @}
    
    /// @name Member Variables
    /// @{
private:
    std::vector<std::vector<bool>>  m_interactions;
    int                             m_maxGroups;
    /// @}
};  // interface I_CollisionService
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZBullet
}   // namespace Zen
#endif // ZEN_ZBULLET_COLLISION_GROUP_HPP_INCLUDED
