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
#ifndef ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_VIEW_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class I_ExplorerNode;

/// Project Explorer View.
/// Implement this interface and use I_ProjectExplorerModel::subscribe()
/// to be notified of updates to the Project Explorer Model.
class WORKBENCHCOMMON_DLL_LINK I_ProjectExplorerView
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Project>                              pProject_type;
    /// @}

    /// @name I_ProjectExplorerView interface
    /// @{
public:
    /// Notification when a project is added to the workbench.
    /// This could be a new project or a project that is
    /// being loaded.
    /// @note TR - Why is this a pProject_type instead of 
    ///     I_Project& like the onAddNode()?
    virtual void onAddProject(pProject_type _pProject) = 0;

    /// Notification when a project is removed from the workbench.
    virtual void onDeleteProject(pProject_type _pProject) = 0;

    /// Notification when a node is added to the workbench
    /// explorer view.
    virtual void onAddNode(I_ExplorerNode& _node) = 0;

    /// Notification when a node is removed from the workbench
    /// explorer view.
    virtual void onDeleteNode(I_ExplorerNode& _node) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectExplorerView();
    virtual ~I_ProjectExplorerView();
    /// @}

};  // interface I_ProjectExplorerView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_I_PROJECT_EXPLORER_VIEW_HPP_INCLUDED
