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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_CONTROLLER_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_CONTROLLER_HPP_INCLUDED

#include "Configuration.hpp"

#include "I_ExplorerNode.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>
#include <Zen/Core/Event/future_return_value.hpp>

#include <Zen/Studio/WorkbenchModel/I_ExplorerNodeDataMap.hpp>

#include <boost/noncopyable.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_ProjectExplorerModel;
class I_WorkbenchService;

class WORKBENCHCOMMON_DLL_LINK I_ProjectExplorerController
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Project>          pProject_type;

    /// Not a managed pointer since it's really just used as a reference.
    typedef Event::future_return_value<I_ExplorerNode*>     FutureExplorerNode_type;
    typedef Memory::managed_ptr<FutureExplorerNode_type>    pFutureExplorerNode_type;
    /// @}

    /// @name I_ProjectExplorerController interface
    /// @{
public:
    /// Get the root workbench service.
    virtual I_WorkbenchService& getWorkbenchService() const = 0;

    /// Create a new project.
    virtual void createNewProject(pProject_type _pProject) = 0;

    /// Load an existing project.
    /// @param _pProject Project to load.  All of the I_Project values
    ///     must be set before calling this method, including projectId, name, 
    ///     and explorerNodeId.  Normally this is not called directly and only
    ///     the WorkbenchService calls this method.
    virtual void loadExistingProject(pProject_type _pProject, I_ExplorerNodeDataMap::pExplorerNodeDomainObject_type _pExplorerNodeDO) = 0;

    /// @todo Pass a string that is the extension for this node's user data 
    ///     instead of I_ExplorerNode::pUserData_type.
    virtual pFutureExplorerNode_type createChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData) = 0;

    /// Graft an existing node onto a new parent.
    virtual void graft(I_ExplorerNode& _node, I_ExplorerNode& _parent) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectExplorerController();
    virtual ~I_ProjectExplorerController();
    /// @}

};  // interface I_ProjectExplorerController

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
namespace Memory {
	/// I_ProjectExplorerController is managed by factory
	template<>
	struct is_managed_by_factory<Studio::Workbench::I_ProjectExplorerController> : public boost::true_type{};
}	// namespace Memory
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_CONTROLLER_HPP_INCLUDED