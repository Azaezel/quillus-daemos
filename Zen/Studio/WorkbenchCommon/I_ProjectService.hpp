//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_SERVICE_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_SERVICE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <boost/filesystem/path.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_ProjectExplorerController;

/// @brief Project Service interface
class WORKBENCHCOMMON_DLL_LINK I_ProjectService
{
    /// @name Types
    /// @{
public:
    typedef std::string                                 index_type;
    typedef Memory::managed_ptr<I_ProjectService>       pService_type;
    typedef Memory::managed_weak_ptr<I_ProjectService>  wpService_type;
    typedef Event::Event<wpService_type>                ServiceEvent_type;

    /// @todo Should this be a reference instead managed_ptr since
    ///         it's explicitly closed?  (Or is it?)
    typedef Zen::Memory::managed_ptr<I_Project>         pProject_type;
    typedef Zen::Memory::managed_weak_ptr<I_Project>    wpProject_type;
    /// @}

    /// @name I_ProjectService interface
    /// @{
public:
    /// Create a project.
    /// @param _controller I_ProjectExplorerController that is being used to create this
    ///     I_Project.
    virtual pProject_type createProject(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, const std::string& _projectName) = 0;
    /// @}

    /// @name Events
    /// @{
public:
    ServiceEvent_type   onDestroyEvent;
    /// @}    

    /// @name Static methods
    /// @{
public:
    static const std::string& getNamespace();
    static const std::string& getExtensionPointName();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectService();
    virtual ~I_ProjectService();
    /// @}

};  // interface I_ProjectService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_ProjectService is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_ProjectService> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_SERVICE_HPP_INCLUDED
