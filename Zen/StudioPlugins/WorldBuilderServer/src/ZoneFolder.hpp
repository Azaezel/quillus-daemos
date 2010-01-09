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
#ifndef WORLDBUILDER_ZONE_FOLDER_HPP_INCLUDED
#define WORLDBUILDER_ZONE_FOLDER_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>

#include <Zen/StudioPlugins/WorldBuilderCommon/I_ZoneFolder.hpp>

#include <Zen/StudioPlugins/WorldBuilderModel/I_ZoneFolderDataMap.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Project;

class ZoneFolder
:   public I_ZoneFolder
{
    /// @name Types
    /// @{
public:
    typedef I_ZoneFolderDataMap::pZoneFolderDataMap_type              pZoneFolderDataMap_type;
    typedef I_ZoneFolderDataMap::pZoneFolderDomainObject_type         pZoneFolderDomainObject_type;
    /// @}

    /// @name UserData overrides
    /// @{
public:
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name I_ZoneFolder implementation
    /// @{
public:
    virtual void newZone(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    /// @}

    /// @name ZoneFolder implementation
    /// @{
public:
    Zen::Studio::Workbench::I_Project& getProject() const;

    /// Called by load() to set the m_pFolderDO.
    /// This should be treated as private, but it needs to be public since
    /// it's called by a visitor struct.
    void setZoneFolderDO(pZoneFolderDomainObject_type _pZoneFolderDO);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ExplorerNodeFactory;
    friend class Project;
    explicit ZoneFolder(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name);
    virtual ~ZoneFolder();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Project&  m_parent;
    static class ZoneFolderType         sm_type;
    pZoneFolderDomainObject_type        m_pZoneFolderDO;

private:
    friend class ZoneFolderType;
    static pExplorerNodeActions_type    sm_pActions;
    /// @}

};  // class ZoneFolder

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // WORLDBUILDER_ZONE_FOLDER_HPP_INCLUDED
