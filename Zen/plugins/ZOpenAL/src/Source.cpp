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

#include <Zen/Engine/Sound/I_SoundManager.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOpenAL {
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
,   m_timeOffset(0)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Source::~Source()
{
    if (m_sourceId!=NULL) alDeleteSources(1,&m_sourceId);
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
Source::setResource(ResourceService::pResource_type _pResource)
{
    m_pSoundResource = _pResource;
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
    m_looping = _loop;
    if (_loop)
    {
        //std::cout << "Sound looping mode enabled." << std::endl;
        alSourcei (m_sourceId, AL_LOOPING,  AL_TRUE  );
    }
    else
    {
        alSourcei (m_sourceId, AL_LOOPING,  AL_FALSE  );
        //std::cout << "Sound looping mode disabled." << std::endl;
    }
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
    //std::cout << "Source::play()" << std::endl;
    alGetError();
    ALuint sourceID;
    alGenSources(1, &sourceID);
    if(alGetError() == AL_NO_ERROR)
    {
        m_sourceId = sourceID;
        unsigned int buff = m_pSoundResource.as<Memory::managed_ptr<SoundResource>>()->getBufferID();
	    alSourcei (m_sourceId, AL_BUFFER, buff);    
        setPitch(m_pitch);
        setVolume(m_volume);
        setPosition(m_pos);
        setVelocity(m_vec);
        setLooping(m_looping);
        setEmissionRadius(m_emissionRadius);
        m_playState = PLAYING;
        alSourcePlay(m_sourceId);
        return true;
    }
    //std::cout << "Source::play() failed!\n";
    m_playState = QUEUED;
    return false;
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
    alGetError();
    m_playState = DEQUEUED;
    if (m_sourceId != NULL)
    {
        // Stop the Source and clear the Queue
        alSourceStop(m_sourceId);
        alDeleteSources(1, &m_sourceId);
        m_sourceId = NULL;
        if(alGetError() != AL_NO_ERROR) std::cout << "Source::dequeue() failed!";
        //else std::cout << "Sound stopped" << std::endl;
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::stop()
{
    alGetError();
    m_playState = STOPPED;
    if (m_sourceId != NULL)
    {
        // Stop the Source and clear the Queue
        alSourceStop(m_sourceId);
        alDeleteSources(1, &m_sourceId);
        m_sourceId = NULL;
        if(alGetError() != AL_NO_ERROR) std::cout << "Source::stop() failed!";
        //else std::cout << "Sound stopped" << std::endl;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Source::PLAYSTATE
Source::getPlayState()
{
    if (m_playState == PLAYING)
        if (m_sourceId != NULL)
        {
            alGetError();
            ALenum iState;
            alGetSourcei( m_sourceId, AL_SOURCE_STATE, &iState);
            if (iState != AL_PLAYING)
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
    if(alGetError() != AL_NO_ERROR) std::cout << "Source::getPlayState() failed!";
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
    alGetError();
    if (_mute)
    {
        //std::cout << "Sound muted" << std::endl;
        if (m_sourceId != NULL)
            alSourcef(m_sourceId, AL_GAIN, 0);
        else
            m_playState = DEQUEUED;
    }
    else
    {
        //std::cout << "Sound unmuted" << std::endl;
        if (m_sourceId != NULL)
            alSourcef(m_sourceId, AL_GAIN, m_volume);
        else
            m_playState = QUEUED;
    }
    if(alGetError() != AL_NO_ERROR) std::cout << "Source::mute(bool _mute) failed!";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::pause(const bool _paused)
{
    if (_paused)
    {
        //std::cout << "Sound paused" << std::endl;
        m_playState = PAUSED;
        alSourcePause(m_sourceId);
    }
    else
    {
        //std::cout << "Sound unpaused" << std::endl;
        m_playState = QUEUED;
    }
}
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void 
Source::setPosition(const Zen::Math::Point3& _pos)
{
    alGetError();
    m_pos = _pos;
    if (m_sourceId != NULL) alSourcefv(m_sourceId, AL_POSITION, m_pos.m_array);
    if(alGetError() != AL_NO_ERROR) std::cout << "Source::setPosition(Zen::Math::Point3 _pos) failed!";
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
    alGetError();
    m_vec = _vel;
    if (m_sourceId != NULL) alSourcefv(m_sourceId, AL_VELOCITY, m_vec.m_array);
    if(alGetError() != AL_NO_ERROR) std::cout << "Source::setPosition(Zen::Math::Point3 _pos) failed!";
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
    alGetError();
    m_volume = _vol;
    if (m_sourceId != NULL) alSourcef(m_sourceId, AL_GAIN, m_volume);
    if(alGetError() != AL_NO_ERROR) std::cout << "Source::setVolume(Zen::Math::Real _vol) failed!";
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
    alGetError();
    m_pitch = _pitch;
    if (m_sourceId != NULL) alSourcef(m_sourceId, AL_PITCH, m_pitch);
    if(alGetError() != AL_NO_ERROR) std::cout << "Source::setPitch(Zen::Math::Real _pitch) failed!";
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Math::Real
Source::getPitch() const
{
   return m_pitch;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ALuint
Source::getSourceID()
{
   return m_sourceId;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setSourceID(const unsigned int _sourceID)
{
    m_sourceId = _sourceID;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Source::setEmissionRadius(const Math::Real _radius)
{
    alGetError();
    m_emissionRadius = _radius;
    if (m_emissionRadius<1) m_emissionRadius = 1;
    if (m_sourceId != NULL)
    {
        // http://opensource.creative.com/pipermail/openal/2003-December/007048.html
        alSourcef(m_sourceId, AL_MAX_DISTANCE, m_emissionRadius);
        alSourcef(m_sourceId, AL_REFERENCE_DISTANCE, m_emissionRadius/2);
        alSourcef(m_sourceId, AL_ROLLOFF_FACTOR, 1/(m_emissionRadius/2));
        if(alGetError() != AL_NO_ERROR) std::cout << "Source::setEmissionRadius(Math::Real _radius) failed!";
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
        alGetError();
        alSourcef(m_sourceId, AL_SEC_OFFSET, m_timeOffset);
        ALenum erno = alGetError();
        //if we exceed the buffer range, consider us done.
        if (erno != AL_NO_ERROR)
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
}   // namespace ZOpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
