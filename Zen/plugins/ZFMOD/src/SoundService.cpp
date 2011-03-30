//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
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

#include "SoundService.hpp"

#include <iostream>
#include <fmod_errors.h>
#include <boost/bind.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::SoundService(config_type& _config)
:   m_pModule(NULL)
,   m_pScriptObject(NULL)
,   m_pSourcesMutex(Zen::Threading::MutexFactory::create())
{
    m_pFMODSystem = FMODService::getSingletonPtr()->getFMODSystemPtr();
    m_listenMatrix.identity();
    setListenMatrix(m_listenMatrix);
    m_listenRadius = 2000.0f;
    m_maxSources = 128;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::~SoundService()
{
    for(Sources_type::iterator iter = m_sources.begin(); iter != m_sources.end(); iter++)
        m_sources.erase(iter);
    Zen::Threading::MutexFactory::destroy(m_pSourcesMutex);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
SoundService::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_pModule->getScriptModule(), 
            m_pModule->getScriptModule()->getScriptType(getScriptTypeName()), 
            getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::pSource_type
SoundService::createSource(Engine::Sound::I_SoundResource::pSoundResource_type _pResource)
{
    Source* pRawPointer = new Source();

    pSource_type pSource(
        pRawPointer,
        boost::bind(&SoundService::destroySource, this, _1)
    );

    pSource->setResource(_pResource);

    _pResource->setIs3d(false);

    pSource->play(); 

    {
        Zen::Threading::CriticalSection guard(m_pSourcesMutex);
        m_sources.insert(pSource.getWeak());
    }
    return pSource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::pSource_type
SoundService::createSource(Engine::Sound::I_SoundResource::pSoundResource_type _pResource, Math::Point3 _pos)
{
    Source* pRawPointer = new Source();

    pSource_type pSource(
        pRawPointer,
        boost::bind(&SoundService::destroySource, this, _1)
    );

    pSource->setResource(_pResource);
    _pResource->setIs3d(true);

    pSource->setPosition(_pos);

    pSource->play();

    {
        Zen::Threading::CriticalSection guard(m_pSourcesMutex);
        m_sources.insert(pSource.getWeak());
    }

    return pSource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::pSource_type
SoundService::createSource(Engine::Sound::I_SoundResource::pSoundResource_type _pResource, Math::Real _x, Math::Real _y)
{
    Source* pRawPointer = new Source();

    pSource_type pSource(
        pRawPointer,
        boost::bind(&SoundService::destroySource, this, _1)
    );

    pSource->setResource(_pResource);

    Math::Point3 pos = Math::Point3(_x,_y,0);

    pSource->setPosition(pos);

    pSource->play();

    {
        Zen::Threading::CriticalSection guard(m_pSourcesMutex);
        m_sources.insert(pSource.getWeak());
    }

    return pSource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::processEvents(Zen::Math::Real _deltaTime)
{
    Zen::Threading::CriticalSection guard(m_pSourcesMutex);

    Sources_type::iterator iter = m_sources.begin();
    while( iter != m_sources.end() )
    { 
        //increment timers for sounds that should have played but haven't
        Math::Real curSoundTime = (*iter)->getTime();
        (*iter)->setTime(curSoundTime+_deltaTime);
        //kill off finished, or should have finished if we were in range sounds
        if ((*iter)->getPlayState() == Source::STOPPED)
        {
            iter = m_sources.erase( iter );
        }
        else
            ++iter;
    }
    
    boost::uint8_t activeSourceCount = 0;
    boost::uint8_t sourceAddFails = 0;
    for(Sources_type::iterator iter = m_sources.begin(); iter != m_sources.end(); iter++)
    {
        //first, note that fmod specificly internally allocates virtual voices,
        //so here there is no max to account for hardware wise, however there is still a ceiling
        // see FMODService::FMODService() for details
        if (activeSourceCount>m_maxSources)
        {
            for(Sources_type::iterator iter2 = iter; iter2 != m_sources.end(); iter2++)
                (*iter2)->dequeue();
                continue;
        }
        
        Source::PLAYSTATE playState = (*iter)->getPlayState();
        //second, if we're marked as de-queued, free us up a software voice (check for redundency later)
        if (playState == Source::DEQUEUED)
        {
            (*iter)->dequeue();
            continue;
        }

        activeSourceCount++;
        //third, if we're marked as playing, keep playing
        if (playState == Source::PLAYING) continue;
        //fourth, try just adding a new one
        if (playState == Source::QUEUED) (*iter)->play();
    }
	// this should fire every frame...
	static FMOD_RESULT result = FMOD_OK;
	result = m_pFMODSystem->update();
	if (result != FMOD_OK)
	{
		// TODO - throw an error here: couldn't update FMOD system during processEvents()
        //printf("onFrame() error! (%d) %s\n", result, FMOD_ErrorString(result));
        return;
	}
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::setListenMatrix(Zen::Math::Matrix4& _listenMatrix)
{
    FMOD_RESULT result = FMOD_OK;
    m_listenMatrix = _listenMatrix;
    // http://jerome.jouvie.free.fr/Fmod/Tutorials/Tutorial03.php

    //set current listener position
    Math::Point3 pos;
    m_listenMatrix.getPosition(pos);

    //set current listener orientation
    Math::Vector3 forward = m_listenMatrix.getForwardVector();
    Math::Vector3 up;
    up.m_x = m_listenMatrix.m_array[1]; //up vector x value
    up.m_y = m_listenMatrix.m_array[5]; //up vector y value
    up.m_z = m_listenMatrix.m_array[9]; //up vector z value
    Math::Vector3 vel(0,0,0); //We'll assume (for now) that this'll primarily be unnecessary since dopler-shifting while youre moving towards something
    //static is far less noticeable than when somethings moving towards you, wich is more the job of sources than listeners 

    result = m_pFMODSystem->set3DListenerAttributes(0, (const FMOD_VECTOR *)pos.m_array, (const FMOD_VECTOR *)vel.m_array, (const FMOD_VECTOR *)forward.m_array, (const FMOD_VECTOR *)up.m_array);
    if (result != FMOD_OK) printf("setListenMatrix(Zen::Math::Matrix4 _listenMatrix) error! (%d) %s\n", result, FMOD_ErrorString(result));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Math::Matrix4&
SoundService::getListenMatrix()
{
    return m_listenMatrix;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::muteAll(bool _isMuted)
{
    //std::cout << "muteAll() called" << std::endl;
	m_isMuted = _isMuted;
	m_channelGroupMain->setMute(m_isMuted);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::setListenRadius(Math::Real _radius)
{
    m_listenRadius = _radius;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real
SoundService::getListenRadius()
{
    return m_listenRadius;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::destroySource(wpSource_type _wpSource)
{
    {
        Zen::Threading::CriticalSection guard(m_pSourcesMutex);
        m_sources.erase(_wpSource);
    }

    /// Fire the source's onDestroyEvent
    _wpSource->onDestroyEvent(_wpSource);

    Source* pSource = 
        dynamic_cast<Source*>(_wpSource.get());

    if (pSource != NULL)
    {
        delete pSource;
    }
    else
    {
        throw Zen::Utility::runtime_exception("SoundService::destroySource() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::registerScriptModule(Zen::Scripting::script_module& _module)
{
    m_pModule = &_module;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
