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

#ifndef ZEN_ZFMOD_SOUND_SERVICE_HPP_INCLUDED
#define ZEN_ZFMOD_SOUND_SERVICE_HPP_INCLUDED

#include "FMODService.hpp"

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <Zen/Engine/Sound/I_SoundService.hpp>
#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Math/Matrix4.hpp>
#include <vector>

#include "ResourceService.hpp"
#include "Source.hpp"

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SoundService
:   public Zen::Engine::Sound::I_SoundService
,   public Memory::managed_self_ref<Zen::Engine::Sound::I_SoundService>
{
    /// @name Types
    /// @{
public:
    typedef std::map< std::string, std::string >                config_type;
    typedef ResourceService                                     pResourceService;
    typedef Engine::Sound::I_SoundResource::pSoundResource_type pSoundResource_type;
    /// @}

    /// @name I_SoundService implementation
    /// @{
public:
    virtual pSource_type createSource(pSoundResource_type _pResource);
    virtual pSource_type createSource(pSoundResource_type _pResource, Math::Point3 _pos);
    virtual pSource_type createSource(pSoundResource_type _pResource, Math::Real _x, Math::Real _y);
public:
	virtual void muteAll(bool bMute);
    virtual void setListenMatrix(Math::Matrix4 _listenMatrix);
    virtual Math::Matrix4 getListenMatrix();
    void    sortVectorbyLooping(std::vector<pSource_type> * _vector);
    virtual void sortSounds();
    virtual void setListenRadius(Math::Real _radius);
    virtual Math::Real getListenRadius();
    /// @}

    /// @name Event handlers
    /// @{
protected:
	virtual void onFrame();
    /// @}

    /// @name 'Structors
    /// @{
public:
             SoundService(config_type& _config);
    virtual ~SoundService();
    /// @}

    /// @name Member Variables
    /// @{
private:
	FMOD::System* m_pFMODSystem;
	FMOD::ChannelGroup* m_channelGroupMain;
	FMOD::Channel* m_channelBGMusic;
	bool m_bMainMuted;
    Math::Matrix4 m_ListenMatrix;
    Math::Real m_listenRadius;
	std::vector<pSource_type> m_SoundSources;
    Math::Real m_maxSources;
    /// @}

};  // class SoundService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZFMOD_SOUND_SERVICE_HPP_INCLUDED
