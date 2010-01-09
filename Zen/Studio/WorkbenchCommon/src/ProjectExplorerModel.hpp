//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
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
#ifndef ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_MODEL_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_MODEL_HPP_INCLUDED

#include "../I_ProjectExplorerModel.hpp"
#include "../I_ExplorerNode.hpp"

#include <Zen/Core/Event/Event.hpp>

#include <string>
#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_Project;
class ProjectExplorerNode;

class ProjectExplorerModel
:   public I_ProjectExplorerModel
{
    /// @name Types
    /// @{
public:
    typedef Memory::managed_ptr<I_Project>          pProject_type;
    typedef Event::Event<ProjectExplorerNode*>      ProjectEvent_type;

    typedef std::map<std::string, pProject_type>    Projects_type;
    /// @}

    /// @name I_ProjectExplorerModel implementation
    /// @{
public:
    virtual void createNewProject(pProject_type _pProject);
    virtual I_ExplorerNode& createChildNode(I_ExplorerNode& _parent, I_ExplorerNode::pUserData_type _pData);
    /// @}

    /// @name I_ExplorerModel implementation
    /// @{
    virtual I_ExplorerNode& getRootNode();
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class I_ProjectExplorerModel;
    explicit ProjectExplorerModel();
    virtual ~ProjectExplorerModel();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Projects_type       m_projects;
    /// @}
};  // class ProjectExplorerModel

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_PROJECT_EXPLORER_MODEL_HPP_INCLUDED
