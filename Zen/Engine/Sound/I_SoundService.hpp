//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
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
#ifndef ZEN_ENGINE_SOUND_I_SOUND_SERVICE_HPP_INCLUDED
#define ZEN_ENGINE_SOUND_I_SOUND_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"
#include "I_SoundSource.hpp"
#include "I_SoundManager.hpp"

#include <Zen/Core/Math/Point3.hpp>
#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Event/Event.hpp>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Sound {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

   
class SOUND_DLL_LINK I_SoundService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                 index_type;
    typedef Memory::managed_ptr<I_SoundService>         pService_type;
    typedef Memory::managed_weak_ptr<I_SoundService>    wpService_type;
    typedef Event::Event<wpService_type>                serviceEvent_type;
    typedef Memory::managed_ptr<I_SoundSource>          pSource_type;
    /// @}

    /// @name I_SoundService interface
    /// @{
public:
	/// In case anything needs to be done each frame, put it in processEvents()
   	virtual void muteAll(bool bMute) = 0;
    virtual pSource_type createSource(I_SoundResource::pSoundResource_type _pResource) = 0;
    virtual pSource_type createSource(I_SoundResource::pSoundResource_type _pResource, Math::Point3 _pos) = 0;
    virtual pSource_type createSource(I_SoundResource::pSoundResource_type _pResource, Math::Real _x, Math::Real _y) = 0;

    virtual void setListenRadius(Math::Real _radius) = 0;
    virtual Math::Real getListenRadius() = 0;
    /// @}

    /// @name Event handlers
    /// @{
//protected:
    virtual void onFrame() = 0;
    /// @}

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name Events
    /// @{
public:
    serviceEvent_type   onDestroyEvent;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_SoundService();
    virtual ~I_SoundService();
    /// @}

};  // interface I_SoundService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Sound
}   // namespace Engine
namespace Memory {
    /// I_SoundService is managed by I_SoundServiceFactory
    template<>
    struct is_managed_by_factory<Engine::Sound::I_SoundService> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_SOUND_I_SOUND_SERVICE_HPP_INCLUDED
