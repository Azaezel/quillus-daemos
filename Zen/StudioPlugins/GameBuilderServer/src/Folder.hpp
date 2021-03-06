//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef GAMEBUILDER_FOLDER_HPP_INCLUDED
#define GAMEBUILDER_FOLDER_HPP_INCLUDED

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_Folder.hpp>

#include <Zen/StudioPlugins/GameBuilderModel/I_GameObjectFolderDataMap.hpp>

#include <string>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ProjectExplorerController;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class Project;

class Folder
:   public I_Folder
{
    /// @name Types
    /// @{
public:
    typedef I_GameObjectFolderDataMap::pGameObjectFolderDataMap_type        pGameObjectFolderDataMap_type;
    typedef I_GameObjectFolderDataMap::pGameObjectFolderDomainObject_type   pGameObjectFolderDomainObject_type;
    /// @}

    /// @name UserData overrides
    /// @{
public:
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    virtual void registerProperties(Zen::Studio::Workbench::I_PropertiesPublisher& _properties);
    virtual Zen::Studio::Workbench::I_PropertiesPublisher& getPropertiesPublisher();
    /// @}

    /// @name I_Folder implementation
    /// @{
public:
    virtual void newObjectType(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    /// @}

    /// @name Folder implementation
    /// @{
public:
    Project& getProject() const;

    /// Called by load() to set the m_pFolderDO.
    /// This should be treated as private, but it needs to be public since
    /// it's called by a visitor struct.
    void setFolderDO(pGameObjectFolderDomainObject_type _pFolderDO);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class Project;
    friend class ExplorerNodeFactory;
    explicit Folder(Project& _parent, const std::string& _name);
    virtual ~Folder();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Project&                            m_parent;
    static class FolderType             sm_type;
    pGameObjectFolderDomainObject_type  m_pFolderDO;

private:
    friend class FolderType;
    static pExplorerNodeActions_type    sm_pActions;
    /// @}

};  // class Folder

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // GAMEBUILDER_FOLDER_HPP_INCLUDED
