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
//  portions Copyright (c), Firelight Technologies Pty, Ltd 2004-2009.
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "ResourceService.hpp"
#include "SoundResource.hpp"

#include <Zen/Core/Scripting/I_ScriptType.hpp>
#include <Zen/Engine/Sound/I_SoundManager.hpp>

#include <iostream>

#include <boost/bind.hpp>
#include <boost/function.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::ResourceService()
:   m_pLocationListMutex(Zen::Threading::MutexFactory::create())
,   m_pGroupInitLock(Zen::Threading::MutexFactory::create())
,   m_bInitialized(false)
,   m_pScriptObject(NULL)
,   m_pModule(NULL)
{
    m_pFMODSystem = NULL;
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

    void *buff = 0;
    int length = 0;
    FMOD_CREATESOUNDEXINFO exinfo;

    if (m_pFMODSystem == NULL)
	{
        // FMOD not yet initialized, so start it up now:
        m_pFMODSystem = FMODService::getSingletonPtr()->getFMODSystemPtr();
        if (m_pFMODSystem == NULL)
	    {
		    // TODO - error: FMOD system failed to initialize.
		    return pResource_type();
	    }
    }

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
        //std::string fullPath;
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

        LoadFileIntoMemory(fullPath.string().c_str(), &buff, &length);
        memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        exinfo.length = length;

        if (length > 0)
        {
            // found the file
            std::cout << fullPath.string().c_str() << " loaded" << std::endl;
            
            SoundResource* pRawPointer = new SoundResource(static_cast<const char *>(buff),exinfo);
            pResource_type pSoundResource(pRawPointer, boost::bind(&ResourceService::destroyResource, this, _1));
            wpResource_type pWeakPtr(pSoundResource);
            pRawPointer->setSelfReference(pWeakPtr);

            return pSoundResource;
        }
        else
        {
            // TODO error
            std::cout << fullPath.string().c_str() << " NOT loaded" << std::endl;
        }
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

void ResourceService::LoadFileIntoMemory(const char *name, void **buff, int *length)
{
    FILE *fp = fopen(name, "rb");
    
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    *buff = malloc(*length);
    fread(*buff, *length, 1, fp);
    
    fclose(fp);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::removeResourceLocation(const std::string& _path, const std::string& _group)
{
    // TODO Implement?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::loadResourceGroup(const std::string& _group)
{
    // TODO Implement?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::unloadResourceGroup(const std::string& _group)
{
    // TODO Implement?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::getResourceNames(I_ResourceNameVisitor& _visitor,
                                         const std::string& _group,
                                         const std::string& _pattern) const
{
    // TODO Implement?
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
