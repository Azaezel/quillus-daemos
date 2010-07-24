//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio Workbench
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
#ifndef WORKBENCHCOMMON_PROJECT_HPP_INCLUDED
#define WORKBENCHCOMMON_PROJECT_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_ExplorerNodeType;

/// Base Project implementation.
/// Feel free to derive your custom Project classes from this class
/// if you want.
/// @note It's not necessary to use this base class for Project classes;
///     just make sure you implement I_Project.
/// @see Zen::Studio::Workbench::I_Project
/// @note This virtually inherits I_Project because it's expected that
///     your implementation will have another interface and an implementation.
///     This will cause a diamond shape inheritance tree.  Your interface
///     should virtually derive from I_Project.  Your implementation should derive
///     your interface and this class.  If you do not derive from this class,
///     no real harm done (just an extra vtable is created).
class WORKBENCHCOMMON_DLL_LINK Project
:   public virtual I_Project
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

    /// @name Workbench::I_Project implementation
    /// @{
public:
    virtual void onCreated();
    virtual boost::uint64_t getProjectId() const;
    virtual void setProjectId(boost::uint64_t _projectId);
    virtual Zen::Studio::Workbench::I_ProjectExplorerController& getController();
    virtual Zen::Studio::Workbench::I_WorkbenchService& getWorkbenchService();
    virtual pDatabaseConnection_type getDatabaseConnection();
    virtual const boost::filesystem::path& getControlPath() const;
    virtual const boost::filesystem::path& getProjectPath() const;
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ProjectService;

    explicit Project(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, const std::string& _name);
    virtual ~Project();

private:
    /// Private constructor; this should not be used.
    Project();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_ProjectExplorerController&    m_controller;
    boost::filesystem::path                                 m_controlPath;
    boost::filesystem::path                                 m_projectPath;

    boost::uint64_t                                         m_projectId;

    /// @}

};  // class Project

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // WORKBENCHCOMMON_PROJECT_HPP_INCLUDED
