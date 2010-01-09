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
#ifndef WORLDBUILDER_PROJECT_HPP_INCLUDED
#define WORLDBUILDER_PROJECT_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/StudioPlugins/WorldBuilderCommon/I_Project.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeType;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class Project
:   public I_Project
{
    /// @name Types
    /// @{
public:
    /// @}


    /// @name Database::I_Persistable implementation
    /// @{
public:
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name I_ExplorerNode::UserData implementation
    /// @{
public:
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    /// @}

    /// @name Workbench::I_Project implementation
    /// @{
public:
    virtual void onCreated();
    virtual boost::uint64_t getProjectId() const;
    virtual void setProjectId(boost::uint64_t _projectId);
    virtual Zen::Studio::Workbench::I_ProjectExplorerController& getController();
    virtual Zen::Studio::Workbench::I_WorkbenchService& getWorkbenchService();
    virtual pDatabaseConnection_type getDatabaseConnection();
    virtual const boost::filesystem::path& getControlPath();
    /// @}

    /// @name WorldBuilder::I_Project implementation
    /// @{
public:
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ProjectService;
    explicit Project(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, const std::string& _name);
    virtual ~Project();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_ProjectExplorerController&    m_controller;

    boost::uint64_t                                         m_projectId;

    static class ProjectType                                sm_type;
    /// @}

};  // class Project

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // WORLDBUILDER_PROJECT_HPP_INCLUDED

