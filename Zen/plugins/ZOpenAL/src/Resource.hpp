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
#ifndef ZEN_ZOPENAL_RESOURCE_HPP_INCLUDED
#define ZEN_ZOPENAL_RESOURCE_HPP_INCLUDED

#include <AL/alut.h>

#include <boost/filesystem/path.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>
#include <Zen/Engine/Sound/I_SoundResource.hpp>
#include <Zen/Core/Scripting/I_ScriptType.hpp>
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOpenAL {
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

    /// @name ZOpenAL::Resource interface
    /// @{
public:
    int getFormat();
    void setFormat(int _format);
    int getSize();
    void setSize(int _size);
    int getFreq();
    void setFreq(int _freq);
    char getLoop();
    void* getData();
    void setData(void* _data);
    /// @}

    /// @name 'Structors
    /// @{
public:
             SoundResource(ALuint _bufferID);
    virtual ~SoundResource();
    /// @}

    ///OpenAL specific:
public:
    virtual void setBufferID(unsigned int _bufferID);
    virtual unsigned int getBufferID();
    bool isInternallyMalloced();
    void setIsInternallyMalloced(bool _malloced);
private:
    bool m_internalMalloc;
    /// @}

    /// @name Member Variables
    /// @{
private:
    //FMOD::Sound* m_pSound;
    ALuint m_bufferID;
    char*   m_fileName;
    int     m_format;
    int     m_size;
    int     m_freq;
    void*   m_data;
    bool    m_is3d;             //are we a 3d sound, or music/gui?
    /// @}

    pScriptModule_type          m_pScriptModule;
    ScriptObjectReference_type* m_pScriptObject;
};  // class Resource

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZOPENAL_RESOURCE_HPP_INCLUDED