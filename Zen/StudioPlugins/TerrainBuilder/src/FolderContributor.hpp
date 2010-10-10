//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Terrain Builder
//
// Copyright (C) 2001 - 2010 Tony Richards
// Copyright (C) 2008 - 2010 Matthew Alan Gray
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
// Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef TERRAINBUILDER_FOLDER_CONTRIBUTOR_HPP_INCLUDED
#define TERRAINBUILDER_FOLDER_CONTRIBUTOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_ExplorerNodeContributor.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/indirect_explorer_node_action.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class ContributorService;

class FolderContributor
:   public Zen::Studio::Workbench::I_ExplorerNodeContributor
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::indirect_explorer_node_action<FolderContributor>    Action_type;
    /// @}

    /// @name I_ExplorerNodeContributor implementation
    /// @{
public:
    virtual void getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, Zen::Studio::Workbench::I_ExplorerNodeContributor::I_ExplorerNodeActionVisitor& _visitor);
    virtual void getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, Zen::Studio::Workbench::I_ExplorerNodeContributor::I_ExplorerNodeDecorationVisitor& _visitor);
    /// @}

    /// @name FolderContributor implementation
    /// @{
public:
    /// Create a new terrain group
    void newTerrainGroup(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Open the terrain
    void open(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Rename folder
    void rename(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    /// @}

    /// @name 'Structors
    /// @{
public:
             FolderContributor(ContributorService& _service);
    virtual ~FolderContributor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ContributorService&         m_service;
    Action_type                 m_newTerrainGroup;
    /// @}

};  // class FolderContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAINBUILDER_FOLDER_CONTRIBUTOR_HPP_INCLUDED
