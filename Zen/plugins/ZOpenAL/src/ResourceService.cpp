//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// IndieZen Game Engine Framework
//
// Copyright (C) 2001 - 2007 Tony Richards
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
//  portions Copyright (c) 2006, Creative Labs Inc.
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "ResourceService.hpp"
#include "Resource.hpp"

#include <Zen/Core/Threading/I_Mutex.hpp>
#include <Zen/Core/Threading/MutexFactory.hpp>
#include <Zen/Core/Threading/CriticalSection.hpp>

#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Engine/Sound/I_SoundManager.hpp>

#include <iostream>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#define OGG_ENDIAN 0 //as appropriate for your platform (0 for intel/little endian, 1 for ppc/big endian)

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZOpenAL {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
size_t 
ov_read_func(void* _ptr,size_t _size,size_t _nmemb,void* _datasource)
{
	return fread(_ptr, _size, _nmemb, (FILE*)_datasource);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
ov_seek_func(void* _datasource,ogg_int64_t _offset,int _whence)
{
	return fseek((FILE*)_datasource, (long)_offset, _whence);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
int 
ov_close_func(void* _datasource)
{
   return fclose((FILE*)_datasource);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
long
ov_tell_func(void* _datasource)
{
	return ftell((FILE*)_datasource);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::ResourceService()
:   m_pLocationListMutex(Zen::Threading::MutexFactory::create())
,   m_pGroupInitLock(Zen::Threading::MutexFactory::create())
,   m_bInitialized(false)
,   m_pScriptObject(NULL)
,   m_pModule(NULL)
{
	m_file_callbacks.read_func = ov_read_func;
	m_file_callbacks.seek_func = ov_seek_func;
	m_file_callbacks.close_func = ov_close_func;
	m_file_callbacks.tell_func = ov_tell_func;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::~ResourceService()
{
    Zen::Threading::MutexFactory::destroy(m_pLocationListMutex);
    Zen::Threading::MutexFactory::destroy(m_pGroupInitLock);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::addResourceLocation(const std::string& _path, const std::string& _type,
                                        const std::string& _group, bool _recursive)
{
    Zen::Threading::CriticalSection guard(m_pLocationListMutex);

	ResourceLocation* loc = new ResourceLocation();
	loc->path = _path;
	loc->recursive = _recursive;
    m_locationList.push_back(loc);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::initialiseAllResourceGroups()
{
    if (!m_bInitialized)
    {
        Zen::Threading::CriticalSection guard(m_pGroupInitLock);
        if (!m_bInitialized)
        {
            m_bInitialized = true;
        }
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::pResource_type 
ResourceService::loadResource(config_type& _config)
{
    initialiseAllResourceGroups();

	ALenum format;
	ALsizei size;
	ALvoid* data = NULL;
	ALfloat freq;
    bool internalMalloced;
    // TODO search paths based on string structure (i.e. "<Type>::<Group>::<ResourceName>")
    // and determine importer to use based on resource file extension and then,
    // of course, import the resource.

	if (m_locationList.empty())
	{
		// TODO - error: no resource locations set, use addResourceLocation() first.
		return pResource_type();
	}

    // TODO - don't assume non-recursive paths
    for (std::list<ResourceLocation*>::iterator iter = m_locationList.begin();
        iter != m_locationList.end(); iter++)
    {
        ResourceLocation* loc = *iter;
        boost::filesystem::path fullPath;

        if (loc->recursive)
        {
            if (!findFile(boost::filesystem::path(loc->path), _config["fileName"], fullPath))
                continue;
        }
        else
        {
	        fullPath = boost::filesystem::path(loc->path);
            fullPath = fullPath / _config["fileName"];
        }

        alGetError();
        ALuint pSound;
        alGenBuffers(1, &pSound);
        
        if (boost::filesystem::extension(fullPath) == ".ogg")
        {
            data = loadVorbisBuffer(fullPath.string().c_str(), &format, &size, &freq);
            internalMalloced = true;
        }
        else
        {
            data = alutLoadMemoryFromFile(fullPath.string().c_str(), &format, &size, &freq);
            internalMalloced = false;
        }
        if (data == NULL)
        {
            std::cout << std::endl << fullPath.string().c_str() << " NOT loaded" << std::endl;
            return pResource_type();
        }
	    alBufferData(pSound, format, data, size, freq);

        ALenum erno = alGetError();
        if (erno == AL_NO_ERROR)
        {
            // found the file
            std::cout << fullPath.string().c_str() << " loaded with format = " << format << " freq = " << freq << std::endl;
            
            SoundResource* pRawPointer = new SoundResource(pSound);
            pResource_type pSoundResource(pRawPointer, boost::bind(&ResourceService::destroyResource, this, _1));
            wpResource_type pWeakPtr(pSoundResource);
            pRawPointer->setSelfReference(pWeakPtr);

            pRawPointer->setIsInternallyMalloced(internalMalloced);
            pRawPointer->setFormat(format);
            pRawPointer->setData(data);
            pRawPointer->setSize(size);
            pRawPointer->setFreq(freq);
            return pSoundResource;
        }
        else
        {
            if (erno == AL_INVALID_NAME) printf("a bad name (ID) was passed to an OpenAL function"); //this is the one firing when out of voices
            if (erno == AL_INVALID_ENUM) printf("an invalid enum value was passed to an OpenAL function");
            if (erno == AL_INVALID_VALUE) printf("an invalid value was passed to an OpenAL function");
            if (erno == AL_INVALID_OPERATION) printf("the requested operation is not valid");
            if (erno == AL_OUT_OF_MEMORY) printf("the requested operation resulted in OpenAL running out of memory");

            // TODO error
            std::cout << std::endl << fullPath.string().c_str() << " NOT loaded" << std::endl;
        }
        alGetError();
    }
    // If we get to here we didn't find the file in any of our known resource locations, so return an uninitialized pResource_type.
    // TODO - error: couldn't load resource.
   	return pResource_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::destroyResource(wpResource_type _pResource)
{
    SoundResource* pResource =
        dynamic_cast<SoundResource*>(_pResource.get());

    if (pResource != NULL)
    {
        _pResource->onDestroyEvent(_pResource);
        delete pResource;
    }
    else
    {
        throw Zen::Utility::runtime_exception("ResourceService::destroyResource() : Invalid type.");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static std::string sm_scriptSingletonName("soundResourceService");
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
ResourceService::getScriptSingletonName() const
{
    return sm_scriptSingletonName;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Scripting::I_ObjectReference*
ResourceService::getScriptObject()
{
    // TODO Make thread safe?
    if (m_pScriptObject == NULL)
    {
        m_pScriptObject = new ScriptObjectReference_type(
            m_pModule->getScriptModule(), 
            m_pModule->getScriptModule()->getScriptType(getScriptTypeName()), 
            getSelfReference().lock()
        );
    }

    return m_pScriptObject;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::registerScriptModule(Zen::Scripting::script_module& _module)
{
    m_pModule = &_module;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ResourceService::findFile(const boost::filesystem::path& _dir_path, const std::string& _file_name, boost::filesystem::path& _path_found)
{
    if (!exists(_dir_path))
        return false;
    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
    for (boost::filesystem::directory_iterator itr(_dir_path); itr != end_itr; ++itr)
    {
        if (is_directory(itr->status()))
        {
            if (findFile(itr->path(), _file_name, _path_found))
                return true;
        }
        else if (itr->leaf() == _file_name) // see below
        {
            _path_found = itr->path();
            return true;
        }
    }
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Load a vorbis file into an OpenAL buffer
ALvoid*
ResourceService::loadVorbisBuffer(const char* _fileName, ALenum* _format, ALsizei* _size, ALfloat* _frequency)
{
    ALvoid* data = NULL;

    FILE *fh;
    vorbis_info *info;
    OggVorbis_File oggFile;
    fh = fopen(_fileName, "rb");
    if (fh)
    {
        if (ov_open_callbacks(fh, &oggFile, NULL, 0, m_file_callbacks) == 0)
        {
            info = ov_info(&oggFile, -1);
            DWORD len = ov_pcm_total(&oggFile, -1) * info->channels * 2;    // always 16 bit data
            BYTE *data = (BYTE *) malloc(len);
            if (data)
            {
                int bs = -1;
                DWORD todo = len;
                BYTE *bufpt = data;
                while (todo)
                {
                    int read = ov_read(&oggFile, (char *) bufpt, todo, OGG_ENDIAN, 2, 1, &bs);
                        todo -= read;
                        bufpt += read;
                }
                (info->channels == 1) ? *_format = AL_FORMAT_MONO16 : *_format = AL_FORMAT_STEREO16;
                *_size = len;
                *_frequency = info->rate;
                return data;
            }
            else
            printf("LoadVorbisBuffer() - couldn't allocate decode buffer");
            ov_clear(&oggFile);
        }
        else
        {
            fclose(fh);
            printf("LoadVorbisBuffer() - ov_open_callbacks() failed, file isn't vorbis?");
        }
    }
    else
        printf("LoadVorbisBuffer() - couldn't open file!");
    return data;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZOpenAL
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
