//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen FMOD Plugin
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

#ifndef ZEN_ZFMOD_FMOD_SERVICE_HPP_INCLUDED
#define ZEN_ZFMOD_FMOD_SERVICE_HPP_INCLUDED

#include <fmod.hpp>
#include <assert.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
    // inline Singleton class template, as per SgtFlame
    // singleton code borrowed from Ogre implementation, v1.4.9
    // TODO - abstract this for general use throughout the framework
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

class FMODService
:   public Singleton<FMODService>
{
    /// @name Static methods
    /// @{
public:
	static FMODService& getSingleton(void);
    static FMODService* getSingletonPtr(void);
    /// @}

    /// @name 'Structors
    /// @{
public:
             FMODService();
    virtual ~FMODService();
    /// @}

    /// @name Member Variables
    /// @{
private:
	FMOD::System* m_pFMODSystem;
    /// @}

    /// @name Member Methods
    /// @{
public:
    FMOD::System* getFMODSystemPtr(void);
    /// @}

};  // class FMODService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZFMOD_FMOD_SERVICE_HPP_INCLUDED
