//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// World Builder
//
// Copyright (C) 2001 - 2009 Tony Richards
//
// Licensed under the Games by Sarge Publishing License - See your licensing
// agreement for terms and conditions.
//
// Do not redistribute this source code.
//
// Tony Richards trichards@gamesbysarge.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef WORLDBUILDER_ZONE_FOLDER_CONTRIBUTOR_HPP_INCLUDED
#define WORLDBUILDER_ZONE_FOLDER_CONTRIBUTOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_ExplorerNodeContributor.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/indirect_explorer_node_action.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ContributorService;

class ZoneFolderContributor
:   public Zen::Studio::Workbench::I_ExplorerNodeContributor
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::indirect_explorer_node_action<ZoneFolderContributor> Action_type;
    /// @}

    /// @name I_ExplorerNodeContributor implementation
    /// @{
public:
    virtual void getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor);
    virtual void getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor);
    /// @}

    /// @name ZoneFolderContributor implementation
    /// @{
public:
    /// Create a new Zone that is a child of the selected node.
    /// The selected node must be a zone folder or another zone.
    void newZone(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    
    /// Remove the Zone aspect Folder from the project.
    void removeZoneFolder(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ZoneFolderContributor(ContributorService& _service);
    virtual ~ZoneFolderContributor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ContributorService&         m_service;
    Action_type                 m_newZone;
    Action_type                 m_removeZoneFolder;
    /// @}

};  // class ZoneFolderContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_ZONE_FOLDER_CONTRIBUTOR_HPP_INCLUDED
