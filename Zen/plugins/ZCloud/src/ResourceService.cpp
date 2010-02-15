//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Enterprise Framework
//
// Copyright (C) 2001 - 2008 Tony Richards
// Copyright (C)        2008 Walt Collins
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
//  Walt Collins (Arcanor) - wcollins@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#include "ResourceService.hpp"

#include <Zen/Engine/Resource/I_Resource.hpp>

#include <iostream>

#include <boost/bind.hpp>
#include <boost/function.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCloud {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::ResourceService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::~ResourceService()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ResourceService::findFile(const boost::filesystem::path& dir_path, const std::string& file_name, boost::filesystem::path& path_found)
{
    if (!exists(dir_path))
        return false;
    boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
    for (boost::filesystem::directory_iterator itr(dir_path); itr != end_itr; ++itr)
    {
        if (is_directory(itr->status()))
        {
            if (findFile(itr->path(), file_name, path_found))
                return true;
        }
        else if (itr->leaf() == file_name) // see below
        {
            path_found = itr->path();
            return true;
        }
    }
    return false;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ResourceService::pResource_type 
ResourceService::loadResource(config_type& _config)
{
    // TODO search paths based on string structure (i.e. "<Type>::<Group>::<ResourceName>")
    // and determine importer to use based on resource file extension and then,
    // of course, import the resource.

    //if (m_pFMODSystem == NULL)
    //{
    //    // FMOD not yet initialized, so start it up now:
    //    m_pFMODSystem = FMODService::getSingletonPtr()->getFMODSystemPtr();
    //    if (m_pFMODSystem == NULL)
    //    {
    //	    // TODO - error: FMOD system failed to initialize.
    //	    return pResource_type();
    //    }
    //}

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
            //fullPath.append("/");
            //fullPath.append(_fileName);
        }

        //FMOD_RESULT result = FMOD_OK;
        //FMOD::Sound* pSound;
        //result = m_pFMODSystem->createSound(fullPath.string().c_str(), FMOD_3D | FMOD_SOFTWARE | FMOD_CREATESTREAM, 0, &pSound);
        //if (result == FMOD_OK)
        //{
        //    // found the file
        //    std::cout << fullPath.string().c_str() << " loaded" << std::endl;
        //    
        //    // set some default values for the new sound:
        //    //pSound->setMode(FMOD_LOOP_NORMAL);
	    //    pSound->setMode(FMOD_LOOP_OFF);
        //
        //    SoundResource* pRawPointer = new SoundResource(pSound);
        //    pResource_type pSoundResource(pRawPointer, boost::bind(&ResourceService::destroyResource, this, _1));
        //    wpResource_type pWeakPtr(pSoundResource);
        //    pRawPointer->setSelfReference(pWeakPtr);
        //
        //    return pSoundResource;
        //}
        //else
        //{
        //    // TODO error
        //    std::cout << fullPath.string().c_str() << " NOT loaded" << std::endl;
        //}
    }

    // If we get to here we didn't find the file in any of our known resource locations, so return an uninitialized pResource_type.
    // TODO - error: couldn't load resource.
   	return pResource_type();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#if 0
void
ResourceService::removeResourceLocation(const std::string& _path, const std::string& _type,
                                     const std::string& _group, bool _recursive)
{
	// Note: parameters _type, _group and _recursive are ignored
    // TODO - don't assume non-recursive paths
    for (std::list<ResourceLocation*>::iterator iter = m_locationList.begin();
        iter != m_locationList.end(); iter++)
    {
        ResourceLocation* loc = *iter;
	    std::string filename = loc->path;
        if (filename.compare(_path) == 0)
        {
            m_locationList.remove(*iter);
            // TODO - is this a memory leak??
            //delete loc;
            break;
        }
    }
}
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::addResourceLocation(const std::string& _path, const std::string& _type,
                                        const std::string& _group, bool _recursive)
{
    std::cout << "addResourceLocation() called" << std::endl;
	// Note: parameters _type, _group and _recursive are ignored
	ResourceLocation* loc = new ResourceLocation();
	loc->path = _path;
	loc->recursive = _recursive;
    m_locationList.push_back(loc);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ResourceService::destroyResource(wpResource_type)
{
    // TODO Call onDestroy and delete
    
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
