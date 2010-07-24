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
#ifndef ZEN_ZOPENAL_SOUND_SERVICE_HPP_INCLUDED
#define ZEN_ZOPENAL_SOUND_SERVICE_HPP_INCLUDED

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
namespace ZOpenAL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SoundService
:   public Engine::Sound::I_SoundService
{
    /// @name Types
    /// @{
public:
    typedef std::map< std::string, std::string >                config_type;
    typedef std::set<wpSource_type>                             Sources_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Zen::Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_SoundService implementation
    /// @{
public:
	virtual void muteAll(bool bMute);
    virtual pSource_type createSource(pSoundResource_type _pResource);
    virtual pSource_type createSource(pSoundResource_type _pResource, Math::Point3 _pos);
    virtual pSource_type createSource(pSoundResource_type _pResource, Math::Real _x, Math::Real _y);
    virtual void setListenRadius(Math::Real _radius);
    virtual Math::Real getListenRadius();
    virtual void registerScriptModule(Zen::Scripting::script_module& _module);
    /// @}

    /// @name SoundService implementation
    /// @{
public:
    void destroySource(wpSource_type _wpSource);
    /// @}

    /// @name Static methods
    /// @{
public:
    /// @}

    /// @name Event handlers
    /// @{
//protected:
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
    Math::Real                      m_listenRadius;
    boost::uint8_t                  m_maxSources;
	bool                            m_isMuted;
    bool                            m_eaxEnabled;

    Zen::Threading::I_Mutex*        m_pSourcesMutex;
    Sources_type                    m_sources;

    Zen::Scripting::script_module*  m_pModule;
    ScriptObjectReference_type*     m_pScriptObject;
    /// @}

};  // class SoundService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOPENAL_SOUND_SERVICE_HPP_INCLUDED
