//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Game Engine Framework
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

#ifndef ZEN_ZFMOD_SOUND_RESOURCE_HPP_INCLUDED
#define ZEN_ZFMOD_SOUND_RESOURCE_HPP_INCLUDED

#include "FMODService.hpp"

//#include <Zen/Core/Scripting/I_ScriptableType.hpp>
//#include <boost/filesystem/path.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Sound/I_SoundResource.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class SoundResource
:   virtual public Engine::Sound::I_SoundResource
,   public Memory::managed_self_ref<Zen::Engine::Resource::I_Resource>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule> pScriptModule_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_SoundResource implementation
    /// @{
public:
    virtual void setIs3d(bool _is3d);
    virtual bool getIs3d();
    /// @}

    /// @name ZFMOD::SoundResource interface
    /// @{
public:
    int getFormat();
    int getSize();
    int getFreq();
    char getLoop();
    void* getData();
    /// @name 'Structors
    /// @{
public:
             SoundResource(const char * _buff, FMOD_CREATESOUNDEXINFO _exinfo);
    virtual ~SoundResource();
    /// @}

    ///FMOD specific:
public:
    virtual const char * getBufferID();
    virtual void setBufferID(const char * _bufferID);
    virtual FMOD_CREATESOUNDEXINFO getExInfo();
    virtual void setExInfo(FMOD_CREATESOUNDEXINFO _exInfo);
    /// @}

    /// @name Member Variables
    /// @{
private:
    FMOD::System* m_pFMODSystem;
    const char *   m_bufferID;
    FMOD_CREATESOUNDEXINFO m_exInfo;
    int     m_format;
    int     m_size;
    int     m_freq;
    char    m_loop;
    void*   m_data;
    bool    m_is3d;
    pScriptModule_type          m_pScriptModule;
    ScriptObjectReference_type* m_pScriptObject;
    /// @}

};  // class SoundResource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZFMOD_SOUND_RESOURCE_HPP_INCLUDED