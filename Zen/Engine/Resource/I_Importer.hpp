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
#ifndef ZEN_ENGINE_RESOURCE_I_IMPORTER_HPP_INCLUDED
#define ZEN_ENGINE_RESOURCE_I_IMPORTER_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>
#include <Zen/Core/Event/Event.hpp>

#include <boost/filesystem/path.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Engine {
namespace Resource {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_CompoundResource;
class I_ResourceStream;

/// Base resource importer interface
class RESOURCE_DLL_LINK I_Importer
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_CompoundResource>         pCompoundResource_type;
    typedef Zen::Event::Event<pCompoundResource_type>       compoundResourceEvent_type;

    typedef Memory::managed_ptr<I_Importer>                 pImporter_type;
    typedef Memory::managed_weak_ptr<I_Importer>            wpImporter_type;
    typedef Event::Event<wpImporter_type>                   importerEvent_type;
    /// @}

    /// @name I_Importer interface
    /// @{
public:
    /// Get the type of resource this importer imports.
    virtual const std::string& getType() const = 0;

    /// Import the resource from a file.
	/// This defaults to an asynchronous call.
    /// @note The compound resource might contain just a single resource
    virtual void import(const boost::filesystem::path& _path, bool _async = true) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    compoundResourceEvent_type  onImportStarted;
    compoundResourceEvent_type  onImportComplete;
    importerEvent_type          onDestroyEvent;
	/// @}

    /// @name 'Structors
    /// @{
public:
             I_Importer();
    virtual ~I_Importer();
    /// @}

};  // interface I_Importer

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Resource
}   // namespace Engine
namespace Memory {
    // I_Importer is managed by I_ImporterFactory
    template<>
    struct is_managed_by_factory<Engine::Resource::I_Importer> : public boost::true_type{};
}   // namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZEN_ENGINE_RESOURCE_I_IMPORTER_HPP_INCLUDED
