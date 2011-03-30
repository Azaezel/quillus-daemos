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
#include "Source.hpp"
#include <fmod_errors.h>
#include <Zen/Engine/Sound/I_SoundManager.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Source::Source()
:   m_pos(0.0f,0.0f,0.0f)
,   m_vec(0.0f,0.0f,0.0f)
,   m_sourceId(0)
,   m_volume(1.0)
,   m_pitch(1.0)
,   m_emissionRadius(10.0)
,   m_looping(false)
,   m_pModule(Zen::Engine::Sound::I_SoundManager::getSingleton().getDefaultScriptModule())
,   m_pScriptObject(NULL)
{
    m_pFMODSystem = FMODService::getSingletonPtr()->getFMODSystemPtr();
    m_pChannel = NULL;
    m_pSound = NULL;
    setEmissionRadius(m_emissionRadius);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Source::~Source()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Scripting::I_ObjectReference*
Source::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_pModule,
            m_pModule->getScriptType(getScriptTypeName()),
            getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setResource(ResourceService::pResource_type _resource)
{
    m_pSoundResource = _resource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::pResource_type
Source::getResource() const
{
   return m_pSoundResource;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Source::setLooping(const bool _loop)
{
    FMOD_RESULT result = FMOD_OK;
    m_looping = _loop;
    if (m_looping)
    {
        //std::cout << "Sound looping mode enabled." << std::endl;
        result = m_pChannel->setMode(FMOD_LOOP_NORMAL);
    }
    else
    {
        //std::cout << "Sound looping mode disabled." << std::endl;
        result = m_pChannel->setMode(FMOD_LOOP_OFF);
    }
    if (result != FMOD_OK) std::cout << "setLooping(bool _loop) error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool 
Source::getLooping() const
{
   return m_looping;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
Source::play()
{
    // TODO make robust
    FMOD_RESULT result = FMOD_OK;
    m_playState = QUEUED;
    //get buffer and exinfo data from the resource
    const char *buff = (const char *)m_pSoundResource.as<Memory::managed_ptr<SoundResource>>()->getBufferID();
    FMOD_CREATESOUNDEXINFO exinfo = m_pSoundResource.as<Memory::managed_ptr<SoundResource>>()->getExInfo();

    if (m_pSoundResource.as<Memory::managed_ptr<SoundResource>>()->getIs3d())
    {
        result = m_pFMODSystem->createSound(buff, FMOD_3D | FMOD_HARDWARE | FMOD_OPENMEMORY, &exinfo, &m_pSound);
        if (result != FMOD_OK)
        {
            std::cout << "play() error!" << result <<","<< FMOD_ErrorString(result) << std::endl;
            return false;
        }
    }
    else
    {
        result = m_pFMODSystem->createSound(buff, FMOD_HARDWARE | FMOD_OPENMEMORY | FMOD_2D, &exinfo, &m_pSound);
        if (result != FMOD_OK)
        {
            std::cout << "play() error!" << result <<","<< FMOD_ErrorString(result) << std::endl;
            return false;
        }
    }
    m_pSound->getDefaults(&m_frequency, NULL,NULL,NULL);

    setEmissionRadius(m_emissionRadius);
    result = m_pFMODSystem->playSound(FMOD_CHANNEL_FREE, m_pSound, true, &m_pChannel);
    if (result != FMOD_OK)
    {
        std::cout << "play() error!" << result <<","<< FMOD_ErrorString(result) << std::endl;
        return false;
    }
    setPitch(m_pitch);
    setVolume(m_volume);
    setLooping(m_looping);
    setPosition(m_pos);
    setVelocity(m_vec);
    m_pChannel->setPaused(false);

    m_playState=PLAYING;
    return true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::queue()
{
    m_playState = getPlayState();
    if ((m_playState!=PLAYING)&&(m_playState!=STOPPED))
        m_playState = QUEUED;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::dequeue()
{
    FMOD_RESULT result = FMOD_OK;
    m_playState = DEQUEUED;
    if (m_pChannel != NULL)
    {
        result = m_pChannel->stop();
        m_pChannel = NULL;
        if (result != FMOD_OK) std::cout << "dequeue() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
        //else std::cout << "Sound stopped" << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::stop()
{
    FMOD_RESULT result = FMOD_OK;
    //std::cout << "Sound stopped" << std::endl;
    // Stop the Source and clear the Queue
    if (m_pChannel != NULL)
    {
        result = m_pChannel->stop();
        if (result != FMOD_OK) std::cout << "stop() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
    }
    if (m_pSound != NULL)
    {
        result = m_pSound->release();
        if (result != FMOD_OK) std::cout << "stop() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Source::PLAYSTATE
Source::getPlayState()
{
    FMOD_RESULT result = FMOD_OK;
    if (m_playState == PLAYING)
        if (m_pChannel != NULL)
        {
            bool isPlaying;
            unsigned int lenms = 0;
            result = m_pChannel->isPlaying(&isPlaying);
            if (result == FMOD_ERR_INVALID_HANDLE)
            {
                m_pChannel = NULL;
                isPlaying = false;
                m_playState = STOPPED;
            }
            if (!isPlaying)
            {
                if (m_looping == true)
                {
                    m_playState = QUEUED;
                }
                else
                    m_playState = STOPPED;
            }
        }
        else
            m_playState = QUEUED;
    if ((result != FMOD_OK)&&(m_playState != STOPPED)) std::cout << "getPlayState() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
    return m_playState;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setPlayState(const PLAYSTATE _state)
{
    m_playState = _state;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::mute(const bool _mute)
{
    FMOD_RESULT result = FMOD_OK;
    if (_mute)
    {
        //std::cout << "Sound muted" << std::endl;
    }
    else
    {
        //std::cout << "Sound unmuted" << std::endl;
    }

    result = m_pChannel->setMute(_mute);
    if (result != FMOD_OK) std::cout << "mute() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::pause(const bool _paused)
{
    FMOD_RESULT result = FMOD_OK;
    if (_paused)
    {
        //std::cout << "Sound paused" << std::endl;
        m_playState = PAUSED;
    }
    else
    {
        //std::cout << "Sound unpaused" << std::endl;
        m_playState = QUEUED;
    }
    result = m_pChannel->setPaused(_paused);
    if (result != FMOD_OK) std::cout << "pause() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Source::setPosition(const Zen::Math::Point3& _pos)
{
    if (dynamic_cast<SoundResource*>(m_pSoundResource.get())->getIs3d())
    {
        m_pos = _pos;
        if (m_pChannel != NULL)
        {
            FMOD_RESULT result = FMOD_OK;
            result = m_pChannel->set3DAttributes((FMOD_VECTOR *)m_pos.m_array, (FMOD_VECTOR *)m_vec.m_array);
            if (result != FMOD_OK) std::cout << "setPosition() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
        }
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Point3&
Source::getPosition() const
{
   return m_pos;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setVelocity(const Zen::Math::Vector3& _vel)
{
    if (dynamic_cast<SoundResource*>(m_pSoundResource.get())->getIs3d())
    {
        FMOD_RESULT result = FMOD_OK;
        m_vec = _vel;
        result = m_pChannel->set3DAttributes((FMOD_VECTOR *)m_pos.m_array,(FMOD_VECTOR *)m_vec.m_array);
        if (result != FMOD_OK) std::cout << "setVelocity() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const Zen::Math::Vector3&
Source::getVelocity() const
{
   return m_vec;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Source::setVolume(const Zen::Math::Real _vol)
{
    FMOD_RESULT result = FMOD_OK;
    m_volume = _vol;
    result = m_pChannel->setVolume(m_volume);
    if (result != FMOD_OK) std::cout << "setVolume() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
Source::getVolume() const
{
   return m_volume;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setPitch(const Zen::Math::Real _pitch)
{
    FMOD_RESULT result = FMOD_OK;
    m_pitch = _pitch;
    result = m_pChannel->setFrequency(m_pitch * m_frequency);
    if (result != FMOD_OK) std::cout << "setPitch() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
Source::getPitch() const
{
   return m_pitch;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
FMOD::Channel*
Source::getSourceID()
{
   return m_pChannel;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setSourceID(FMOD::Channel* _sourceID)
{
    m_pChannel = _sourceID;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setEmissionRadius(const Math::Real _radius)
{
    FMOD_RESULT result = FMOD_OK;
    m_emissionRadius = _radius;
    if (m_emissionRadius<1) m_emissionRadius = 1;
    if (m_pSound != NULL)
    {
        result = m_pSound->set3DMinMaxDistance(_radius/2,_radius);
        if (result != FMOD_OK) std::cout << "setEmissionRadius() error: " << result <<","<< FMOD_ErrorString(result) << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real
Source::getEmissionRadius() const
{
   return m_emissionRadius;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setPriority(const Math::Real _dist)
{
    m_volDist = _dist;
    m_pChannel->setPriority(m_volDist);
};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Math::Real
Source::getTime() const
{
   return m_timeOffset;
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setTime(Math::Real _timeOffset)
{
    m_timeOffset += _timeOffset;
    if (m_sourceId!= NULL)
    {
        FMOD_RESULT erno = FMOD_OK;
        erno = m_pChannel->setPosition(m_timeOffset,FMOD_TIMEUNIT_MS);
        //if we exceed the buffer range, consider us done.
        if (erno != FMOD_OK)
        {
            if (m_looping == true)
            {
                m_timeOffset = 0;
            }
            else
            {
                stop();
            }
        }
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
