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
#ifndef ZEN_ZPHYSX_PHYSICS_SERVICE_HPP_INCLUDED
#define ZEN_ZPHYSX_PHYSICS_SERVICE_HPP_INCLUDED

#include <set>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>
#include <Zen/Engine/Physics/I_PhysicsService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZPhysX {

    // inline Singleton class template, as per SgtFlame
    // singleton code borrowed from Ogre implementation, v1.4.9
    // TODO - abstract this for general use throughout the framework (copypaste from FMOD implementation)
    template<typename T> class Singleton
    {
    protected:
        static T* ms_Singleton;
    public:
        Singleton()
        {
            assert(!ms_Singleton);
#if defined(_MSC_VER) && _MSC_VER < 1200	 
            int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
            ms_Singleton = (T*)((int)this + offset);
#else
	        ms_Singleton = static_cast<T*>(this);
#endif
        }
        ~Singleton()
        {
            assert(ms_Singleton);
            ms_Singleton = 0;
        }
        static T& getSingleton()
		{	
            assert(ms_Singleton);
            return (*ms_Singleton);
        }
        static T* getSingletonPtr()
		{
            return ms_Singleton;
        }
    };
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class PhysicsService
:   public Engine::Physics::I_PhysicsService
,   public Memory::managed_self_ref<Zen::Engine::Physics::I_PhysicsService>
{
    /// @name Types
    /// @{
public:
    //typedef Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsService> wpPhysicsService_type;
    //typedef Zen::Memory::managed_ptr<Zen::Engine::Physics::I_PhysicsZone> pPhysicsZone_type;
    //typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Physics::I_PhysicsZone> wpPhysicsZone_type;
    /// @}

    /// @name I_PhysicsService implementation
    /// @{
public:
    virtual pPhysicsZone_type createZone(void);
	virtual void stepSimulation(double _elapsedTime);
    /// @}

    /// @name PhysicsService implementation
    /// @{
public:
    void onDestroyPhysicsZone(wpPhysicsZone_type _wpPhysicsZone);
    /// @}

    /// @name Event handlers
    /// @{
protected:
	virtual void onFrame();
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
    std::set<pPhysicsZone_type> m_zoneSet;
    NxPhysicsSDK*               m_pPhysicsSDK;
    /// @}

    /// @name Member Methods
    /// @{
public:
    NxPhysicsSDK* getPhysicsSDKPtr(void);
    /// @}

};  // class PhysicsService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZPhysX
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZPHYSX_PHYSICS_SERVICE_HPP_INCLUDED
