//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Engine Framework
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
#ifndef ZEN_ZODE_PHYSICS_SERVICE_HPP_INCLUDED
#define ZEN_ZODE_PHYSICS_SERVICE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Engine/Physics/I_PhysicsService.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZODE {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsService
:   public Engine::Physics::I_PhysicsService
{
    /// @name Types
    /// @{
public:
    typedef std::set<wpPhysicsZone_type>            ZoneCollection_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_PhysicsService implementation
    /// @{
public:
   	virtual pPhysicsZone_type createZone();
	virtual void stepSimulation(double _elapsedTime);
    virtual void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name PhysicsService implementation
    /// @{
public:
    void onDestroyPhysicsZone(wpPhysicsZone_type _wpPhysicsZone);
    /// @}

    /// @name 'Structors
    /// @{
public:
             PhysicsService();
    virtual ~PhysicsService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ZoneCollection_type         m_zones;

    Zen::Scripting::script_module*  m_pScriptModule;
    ScriptObjectReference_type*     m_pScriptObject;
    /// @}

};  // class PhysicsService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZODE
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZODE_PHYSICS_SERVICE_HPP_INCLUDED
