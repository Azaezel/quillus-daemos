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

#ifndef ZEN_ZFMOD_RESOURCE_SERVICE_HPP_INCLUDED
#define ZEN_ZFMOD_RESOURCE_SERVICE_HPP_INCLUDED

#include "FMODService.hpp"

#include <Zen/Engine/Resource/I_ResourceService.hpp>
//#include <Zen/Engine/Resource/I_SoundResource.hpp>

#include <Zen/Engine/Sound/I_SoundResource.hpp>
#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <boost/filesystem.hpp>
#include "SoundResource.hpp"
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZFMOD {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ResourceService
:   public Engine::Resource::I_ResourceService
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_weak_ptr<Engine::Resource::I_ResourceService>   wpResourceService_type;
    typedef Zen::Memory::managed_ptr<Scripting::I_ScriptModule>             pScriptModule_type;
    /// @}

    /// @name I_ScriptableType implementation
    /// @{
public:
    virtual Scripting::I_ObjectReference* getScriptObject();
    /// @}

    /// @name I_ResourceService implementation
    /// @{
public:
    virtual void addResourceLocation(const std::string& _path, const std::string& _type, 
        const std::string& _group, bool _recursive = false);
    virtual void initialiseAllResourceGroups();
    virtual pResource_type loadResource(config_type& _config);
    virtual const std::string& getScriptSingletonName() const;
    virtual void registerScriptModule(Zen::Scripting::script_module& _module);
    
    virtual void removeResourceLocation(const std::string& _path, const std::string& _group);
    virtual void loadResourceGroup(const std::string& _group);
    virtual void unloadResourceGroup(const std::string& _group);
    virtual void getResourceNames(I_ResourceNameVisitor& _visitor, const std::string& _group, const std::string& _pattern) const;
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name ResourceService implementation
    /// @{
private:
    void destroyResource(wpResource_type);
    bool findFile(const boost::filesystem::path& _dir_path, const std::string& _file_name, boost::filesystem::path& _path_found);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ResourceService();
    virtual ~ResourceService();
    /// @}

    ///FMOD Specific
    /// @{
    void LoadFileIntoMemory(const char *name, void **buff, int *length);
    /// @}

    /// @name Member Variables
    /// @{
private:
    FMOD::System* m_pFMODSystem;
	struct ResourceLocation
	{
		std::string path;
		bool recursive;
	};
    /// True if the group manager has been initialized
    Threading::I_Mutex*             m_pGroupInitLock;
    volatile bool                   m_bInitialized;

    Threading::I_Mutex*             m_pLocationListMutex;
    std::list<ResourceLocation*>    m_locationList;

    ScriptObjectReference_type*     m_pScriptObject;
    Zen::Scripting::script_module*  m_pModule;
    /// @}

};  // class ResourceService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZFMOD
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZFMOD_RESOURCE_SERVICE_HPP_INCLUDED
