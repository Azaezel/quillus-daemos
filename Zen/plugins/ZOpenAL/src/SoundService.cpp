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

#include "SoundService.hpp"
#include "Source.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Engine/Sound/I_SoundManager.hpp>

#include <boost/bind.hpp>

#include <stdio.h>
#include <al.h>
#include <algorithm>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOpenAL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
struct SoundPriorityIsLess {
   bool operator()(const SoundService::wpSource_type& _lhs, const SoundService::wpSource_type& _rhs) const {
      return _lhs->getPriority() < _rhs->getPriority();
   }
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SoundService::SoundService(config_type& _config)
:   m_pModule(NULL)
,   m_pScriptObject(NULL)
,   m_pSourcesMutex(Zen::Threading::MutexFactory::create())
{
    alutInit(NULL,NULL); //initialize the ALUT library and create a default current context (from: The OpenAL Utility Toolkit)
    // Check for EAX 2.0 support
    m_eaxEnabled = static_cast<bool>(alIsExtensionPresent("EAX2.0"));
    alGetError(); // clear error code
    m_maxSources = 128;                     // verry verry unlikely to see a card that can handle this many sources
    m_listenRadius = 20.0f;
    m_listenMatrix.identity();
    alListener3f(AL_POSITION, 0, 0, 0);
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
    alSourcei(pRawPointer->getSourceID(),AL_SOURCE_RELATIVE,true);

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
    alSourcei(pRawPointer->getSourceID(),AL_SOURCE_RELATIVE,false);

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

    alSourcei(pRawPointer->getSourceID(),AL_SOURCE_RELATIVE,false);

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
SoundService::orderByPriority()
{
    std::set<wpSource_type,SoundPriorityIsLess> temp;

    Sources_type::iterator iter = m_sources.begin();
    while( iter != m_sources.end() )
    {
        temp.insert(*iter);
        ++iter;
    }

    std::set<wpSource_type,SoundPriorityIsLess>::iterator tempIter = temp.begin();
    while( tempIter != temp.end() )
    {
        m_sources.insert(*tempIter);
        ++tempIter;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SoundService::processEvents(Zen::Math::Real _deltaTime)
{
    Zen::Threading::CriticalSection guard(m_pSourcesMutex);

    alGetError(); // clear error code

    Sources_type::iterator iter = m_sources.begin();
    while( iter != m_sources.end() )
    {
        //increment timers for sounds that should have played but haven't
        Math::Real curSoundTime = (*iter)->getTime();
        (*iter)->setTime(curSoundTime+_deltaTime);
        //kill off finished, or should have finished if we were in range sounds
        if ((*iter)->getPlayState() == Source::STOPPED)
        {
            (*iter)->stop();
            iter = m_sources.erase( iter );
        }
        else
            ++iter;
    }

    orderByPriority();

    boost::uint8_t activeSourceCount = 0;
    boost::uint8_t sourceAddFails = 0;
	for(Sources_type::iterator iter = m_sources.begin(); iter != m_sources.end(); iter++)
    {
        //first order of buisiness: if we're exceeding the max sourcecount, skip it entirely.
        if (activeSourceCount>m_maxSources)
        {
            for(Sources_type::iterator iter2 = iter; iter2 != m_sources.end(); iter2++)
                (*iter2)->dequeue();
            continue;
        }

        Source::PLAYSTATE playState = (*iter)->getPlayState();
        //second, if we're marked as de-queued, free us up a voice
        if (playState == Source::DEQUEUED)
        {
            (*iter)->dequeue();
            continue;
        }

        activeSourceCount++;
        //third, if we're marked as playing, keep playing
        if (playState == Source::PLAYING) continue;
        //fourth, try just adding a new one
        if (playState == Source::QUEUED) 
        {
            if ((*iter)->play() == false)
            {
                sourceAddFails++;
            }
        }
    }
    if (sourceAddFails>0)
    {
        //we must have run out of possible voices. Update the maxSources to reflect this, and dequeue the rest
        //this way we avoid artificial hardware limitations
        m_maxSources = activeSourceCount-sourceAddFails;
        //std::cout << "alGenSources(1, &sourceID); failed! setting new voices cap to:" << m_maxSources <<"\n";
    }
}
	 	 
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~ 
void 
SoundService::setListenMatrix(Zen::Math::Matrix4& _listenMatrix) 
{ 
    alGetError(); 
    m_listenMatrix = _listenMatrix; 
    // http://www.gamedev.net/reference/articles/article1958.asp
    //set current listener position 
    Math::Point3 pos; 
    m_listenMatrix.getPosition(pos); 
    alListener3f(AL_POSITION,pos.m_x,pos.m_y,pos.m_z); 
    //set current listener orientation 
    Math::Real vec[6]; 
    Math::Vector3 fvec = m_listenMatrix.getForwardVector(); 
    vec[0] = fvec.m_x; //forward vector x value 
    vec[1] = fvec.m_y; //forward vector y value 
    vec[2] = fvec.m_z; //forward vector z value 
    vec[3] = m_listenMatrix.m_array[1]; //up vector x value 
    vec[4] = m_listenMatrix.m_array[5]; //up vector y value
    vec[5] = m_listenMatrix.m_array[9]; //up vector z value 
    alListenerfv(AL_ORIENTATION, vec); 
    //alListenerfv(AL_VELOCITY,    ListenerVel); 
    if(alGetError() != AL_NO_ERROR) std::cout << "SoundService::setListenMatrix(Zen::Math::Matrix4 _listenMatrix) failed!"; 
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
	m_isMuted = _isMuted;
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
}   // namespace ZOpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
