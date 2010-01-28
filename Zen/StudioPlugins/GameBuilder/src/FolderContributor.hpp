//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
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
#ifndef GAMEBUILDER_FOLDER_CONTRIBUTOR_HPP_INCLUDED
#define GAMEBUILDER_FOLDER_CONTRIBUTOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_ExplorerNodeContributor.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//#include <Zen/Studio/WorkbenchCommon/generic_explorer_node_type.hpp>

#include <Zen/Studio/WorkbenchCommon/indirect_explorer_node_action.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ContributorService;

class FolderContributor
:   public Zen::Studio::Workbench::I_ExplorerNodeContributor
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::indirect_explorer_node_action<FolderContributor> Action_type;
    /// @}

    /// @name I_ExplorerNodeContributor implementation
    /// @{
public:
    virtual void getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor);
    virtual void getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor);
    /// @}

    /// @name FolderContributor implementation
    /// @{
public:
    /// Create a new object that is a child of the selected node.
    void newObjectType(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
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
    Action_type                 m_newObjectType;
    /// @}

};  // class FolderContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMEBUILDER_FOLDER_CONTRIBUTOR_HPP_INCLUDED
