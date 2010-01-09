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

#ifndef ZEN_ZCLOUD_RESOURCE_SERVICE_HPP_INCLUDED
#define ZEN_ZCLOUD_RESOURCE_SERVICE_HPP_INCLUDED

#include "CloudService.hpp"

#include <Zen/Engine/Resource/I_ResourceService.hpp>

#include <Zen/Core/Memory/managed_self_ref.hpp>

#include <boost/filesystem.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace ZCloud {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ResourceService
:   public Zen::Engine::Resource::I_ResourceService
,   public Zen::Memory::managed_self_ref<Zen::Engine::Resource::I_ResourceService>
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_weak_ptr<Zen::Engine::Resource::I_ResourceService> wpResourceService_type;
    /// @}

    /// @name I_ResourceService implementation
    /// @{
public:
    virtual void addResourceLocation(const std::string& _path, const std::string& _type, 
        const std::string& _group, bool _recursive = false);
    //virtual void removeResourceLocation(const std::string& _path, const std::string& _type, 
    //    const std::string& _group, bool _recursive = false);
    virtual pResource_type loadResource(config_type& _config);
    /// @}

    /// @name Event handlers
    /// @{
protected:
    /// @}

    /// @name ResourceService implementation
    /// @{
private:
    void destroyResource(wpResource_type);
    bool findFile(const boost::filesystem::path& dir_path, const std::string& file_name, boost::filesystem::path& path_found);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ResourceService();
    virtual ~ResourceService();
    /// @}

    /// @name Member Variables
    /// @{
private:
	struct ResourceLocation
	{
		std::string path;
		bool recursive;
	};
	std::list<ResourceLocation*> m_locationList;
    /// @}

};  // class ResourceService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ZCloud
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ZCLOUD_RESOURCE_SERVICE_HPP_INCLUDED
