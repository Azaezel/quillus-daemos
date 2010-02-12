//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Worlds by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef WORLDBUILDER_PROJECT_SERVICE_HPP_INCLUDED
#define WORLDBUILDER_PROJECT_SERVICE_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ProjectService.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class ProjectService
:   public Zen::Studio::Workbench::I_ProjectService
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ProjectService implementation
    /// @{
public:
    virtual pProject_type createProject(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, const std::string& _projectName);
    /// @}

    /// @name ProjectService implementation
    /// @{
public:
    virtual void destroyProject(wpProject_type _pProject);

protected:
    friend class ProjectServiceFactory;
    //void setWorkbench(wpWorkbench_type _pWorkbench);
    /// @}

    /// @name Static Methods
    /// @{
public:
    static ProjectService& getSingleton();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             ProjectService();
    virtual ~ProjectService();
    /// @}

    /// @name Member Variables
    /// @{
private:
    //wpWorkbench_type        m_pWorkbench;
    /// @}

};  // class ProjectService

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#endif  // WORLDBUILDER_PROJECT_SERVICE_HPP_INCLUDED

