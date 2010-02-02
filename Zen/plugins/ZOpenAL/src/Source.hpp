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
,   public Memory::managed_self_ref<Engine::Sound::I_SoundSource>
{
    /// @name 'Structors
    /// @{
public:
             Source();
    virtual ~Source();
    /// @}

    /// @name I_Source implementation
    /// @{
public:

    virtual void setResource(ResourceService::pResource_type _resource);
    virtual ResourceService::pResource_type getResource() const;
    //playback
    virtual void setLooping(const bool _loop);
    virtual bool getLooping() const;
	virtual bool play();
    virtual void mute(bool _mute);
    virtual void pause(bool _pause);
    virtual void stop();
    //3d coords
    virtual void setPosition(const Math::Point3& _pos);
    virtual const Math::Point3& getPosition() const;
    virtual void setVelocity(const Math::Vector3& _vel);
    virtual const Math::Vector3& getVelocity() const;
    //fine grain manipulation
    virtual void setVolume(const Math::Real _vol);
    virtual Math::Real getVolume() const;
    virtual void setPitch(const Math::Real _pitch);
    virtual Math::Real getPitch() const;
    virtual void setEmissionRadius(const Math::Real _radius);
    virtual Math::Real getEmissionRadius() const;
    /// @}

    /// @name SoundService class data-interfacing. For use with SoundService::onFrame() and SoundService::sortSounds()
    /// @{
public:

    virtual void queue();
    virtual void dequeue();
    virtual PLAYSTATE   getPlayState();
    virtual void    setPlayState(const PLAYSTATE _state);
    virtual Math::Real getVolDist(){return m_VolDist;};                 //these two are only placeholders
    virtual void setVolDist(const Math::Real _dist){ m_VolDist = _dist;};      //to cut down on sort-crunching
    /// @}

    /// @name Member Variables
    /// @{
private:
    PLAYSTATE           m_playState;
    Math::Real          m_VolDist;                              //volume*distance storage for sorting
    /// @}

    ///OpenAL specific:
public:
    virtual void setSourceID(unsigned int _sourceID);
    virtual unsigned int getSourceID();
private:
    /// @name Member Variables
    /// @{
    unsigned int    m_SourceID;
    /// @}

    /// Sound Source General:
    /// @name Member Variables
    /// @{
private:

    bool    m_looping;          //loops or a one off playthrough?
    ResourceService::pResource_type      m_SoundResource;    //the sound file struct (turn to vector<> for randomised list support?)
    Math::Point3        m_pos;              //3d coords of the emitter
    Math::Vector3       m_vec;              //3d velocity vector
    Math::Real          m_volume;           //volume from this source
    Math::Real          m_pitch;            //pitch from this source
    Math::Real          m_emissionRadius;
};  // class Source

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace OpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif // ZEN_ZOPENAL_SOURCE_HPP_INCLUDED
