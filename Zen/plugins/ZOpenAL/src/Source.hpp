//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ZOPENAL_SOURCE_HPP_INCLUDED
#define ZEN_ZOPENAL_SOURCE_HPP_INCLUDED

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Core/Math/Math.hpp>
#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Vector3.hpp>
#include "ResourceService.hpp"
#include <Zen/Engine/Sound/I_SoundSource.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOpenAL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class Source
:   public Engine::Sound::I_SoundSource
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_Audible implementation
    /// @{
public:
	virtual bool play();
    virtual void mute(bool _mute);
    virtual void pause(bool _pause);
    virtual void stop();
    /// @}

    /// @name I_Source implementation
    /// @{
public:
    virtual void setResource(ResourceService::pResource_type _pResource);
    virtual ResourceService::pResource_type getResource() const;
    virtual void setPosition(const Math::Point3& _pos);
    virtual const Math::Point3& getPosition() const;
    virtual void setVelocity(const Math::Vector3& _vel);
    virtual const Math::Vector3& getVelocity() const;
    virtual void setVolume(const Math::Real _vol);
    virtual Math::Real getVolume() const;
    virtual void setPitch(const Math::Real _pitch);
    virtual Math::Real getPitch() const;
    virtual void setEmissionRadius(const Math::Real _radius);
    virtual Math::Real getEmissionRadius() const;
    virtual void setLooping(const bool _loop);
    virtual bool getLooping() const;
    virtual void queue();
    virtual void dequeue();
    virtual PLAYSTATE getPlayState();
    virtual void setPlayState(const PLAYSTATE _state);
    virtual Math::Real getPriority() {return m_volDist;}
    virtual void setPriority(const Math::Real _dist);
    virtual Math::Real getTime() const;
    virtual void setTime(Math::Real _timeOffset);
    /// @}

    /// @name SoundService implementation
    /// @{
public:
    virtual void setSourceID(unsigned int _sourceID);
    virtual unsigned int getSourceID();
    /// @}

    /// @name 'Structors
    /// @{
public:
             Source();
    virtual ~Source();
    /// @}

    /// @name Member Variables
    /// @{
private:
    unsigned int                        m_sourceId;
    Math::Real                          m_timeOffset;

    PLAYSTATE                           m_playState;
    Math::Real                          m_volDist;

    bool                                m_looping;
    ResourceService::pResource_type     m_pSoundResource;
    Math::Point3                        m_pos;
    Math::Vector3                       m_vec;
    Math::Real                          m_volume;
    Math::Real                          m_pitch;
    Math::Real                          m_emissionRadius;

    ScriptObjectReference_type*         m_pScriptObject;
    pScriptModule_type                  m_pModule;
    /// @}

};  // class Source

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace OpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOPENAL_SOURCE_HPP_INCLUDED
