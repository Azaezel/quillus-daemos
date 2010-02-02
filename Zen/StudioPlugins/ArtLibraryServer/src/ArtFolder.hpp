//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
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
#ifndef ARTLIBRARY_ART_FOLDER_HPP_INCLUDED
#define ARTLIBRARY_ART_FOLDER_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Studio/WorkbenchCommon/I_Project.hpp>

#include <Zen/StudioPlugins/ArtLibraryCommon/I_Folder.hpp>

#include <Zen/StudioPlugins/ArtLibraryModel/I_ArtFolderDataMap.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Project;

class ArtFolder
:   public I_Folder
{
    /// @name Types
    /// @{
public:
    typedef I_ArtFolderDataMap::pArtFolderDataMap_type                  pArtFolderDataMap_type;
    typedef I_ArtFolderDataMap::pArtFolderDomainObject_type             pArtFolderDomainObject_type;
    /// @}

    /// @name UserData overrides
    /// @{
public:
    virtual void setName(const std::string& _name);
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name I_ArtFolder implementation
    /// @{
public:
    virtual void newFolder(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    virtual void newArtAsset(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    /// @}

    /// @name ArtFolder implementation
    /// @{
public:
    Zen::Studio::Workbench::I_Project& getProject() const;

    /// Called by load() to set the m_pFolderDO.
    /// This should be treated as private, but it needs to be public since
    /// it's called by a visitor struct.
    void setArtFolderDO(pArtFolderDomainObject_type _pArtFolderDO);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ExplorerNodeFactory;
    friend class Project;
    explicit ArtFolder(ArtFolder& _parentFolder);
    explicit ArtFolder(Zen::Studio::Workbench::I_Project& _parent, const std::string& _name);
    virtual ~ArtFolder();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_Project&  m_parent;
    static class ArtFolderType          sm_type;
    pArtFolderDomainObject_type         m_pArtFolderDO;

private:
    friend class ArtFolderType;
    static pExplorerNodeActions_type    sm_pActions;
    /// @}

};  // class ArtFolder

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // ARTLIBRARY_ART_FOLDER_HPP_INCLUDED
