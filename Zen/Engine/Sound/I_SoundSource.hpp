//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Matthew Alan Gray
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
//  Matthew Alan Gray mgray@indiezen.org
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZEN_ENGINE_RESOURCE_I_SOUND_SOURCE_HPP_INCLUDED
#define ZEN_ENGINE_RESOURCE_I_SOUND_SOURCE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Scripting.hpp>

#include <Zen/Engine/Sound/I_Audible.hpp>
#include <Zen/Engine/Sound/I_SoundResource.hpp>

#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Math/Vector3.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Sound {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SOUND_DLL_LINK I_SoundSource
:   public virtual Zen::Scripting::I_ScriptableType
,   public Zen::Memory::managed_self_ref<I_SoundSource>
,   public I_Audible
{
    /// @name Types
    /// @{
public:
    enum PLAYSTATE
    {
        QUEUED,                             //for letting the service know to start it next onframe
        PLAYING,                            //for letting the service know it's already playing during the next onframe
        DEQUEUED,                           //for letting the service know to stop playing it next onframe
        PAUSED,
        STOPPED
    };

    typedef Zen::Memory::managed_ptr<I_SoundSource>         pScriptObject_type;
    typedef Zen::Scripting::ObjectReference<I_SoundSource>  ScriptObjectReference_type;

    typedef Memory::managed_ptr<I_SoundSource>              pSoundSource_type;
    typedef Memory::managed_weak_ptr<I_SoundSource>         wpSoundSource_type;
    typedef Event::Event<wpSoundSource_type>                SoundSourceEvent_type;

    typedef Memory::managed_ptr<Resource::I_Resource>       pResource_type;
    /// @}

    /// @name I_SoundSource interface
    /// @{
public:
    virtual void setResource(pResource_type _resource) = 0;
    virtual pResource_type getResource() const = 0;
    //3d coords
    virtual void setPosition(const Math::Point3& _pos) = 0;
    virtual const Math::Point3& getPosition() const = 0;
    virtual void setVelocity(const Math::Vector3& _vel) = 0;
    virtual const Math::Vector3& getVelocity() const = 0;
    //fine grain manipulation
    virtual void setVolume(const Math::Real _volume) = 0;
    virtual Math::Real getVolume() const = 0;
    virtual void setPitch(const Math::Real _pitch) = 0;
    virtual Math::Real getPitch() const = 0;
    virtual void setEmissionRadius(const Math::Real _radius) = 0;
    virtual Math::Real getEmissionRadius() const = 0;
    //queue status
    virtual void setLooping(const bool bLoop) = 0;
    virtual bool getLooping() const = 0;
    virtual void queue() = 0;
    virtual void dequeue() = 0;
    virtual PLAYSTATE getPlayState()= 0;
    virtual void setPlayState(const PLAYSTATE _state)= 0;
    virtual Math::Real getPriority() = 0;
    virtual void setPriority(const Math::Real _dist) = 0;

    virtual Math::Real getTime() const = 0;
    virtual void setTime(Math::Real _timeOffset) = 0;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    /// This is implemented to return "SoundSource"
    /// Override this method if you create a derived type
    const std::string& getScriptTypeName();
    /// @}

    /// @name Events
    /// @{
public:
    /// This event is fired immediately before this object is destroyed.
    SoundSourceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
             I_SoundSource();
    virtual ~I_SoundSource();
    /// @}

};  // interface I_SoundSource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
namespace Memory 
{
    /// I_SoundResource is managed by a factory
    template<>
    struct is_managed_by_factory<Zen::Engine::Sound::I_SoundSource> 
    :   public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RESOURCE_I_SOUND_SOURCE_HPP_INCLUDED
