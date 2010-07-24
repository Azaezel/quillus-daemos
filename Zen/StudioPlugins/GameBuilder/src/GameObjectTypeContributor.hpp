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
#ifndef GAMEBUILDER_GAME_OBJECT_TYPE_CONTRIBUTOR_HPP_INCLUDED
#define GAMEBUILDER_GAME_OBJECT_TYPE_CONTRIBUTOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_ExplorerNodeContributor.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetViewable.hpp>

#include <Zen/Studio/WorkbenchCommon/indirect_explorer_node_action.hpp>
#include <Zen/Studio/WorkbenchCommon/indirect_spread_sheet_action.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ContributorService;

class GameObjectTypeContributor
:   public Zen::Studio::Workbench::I_ExplorerNodeContributor
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::indirect_explorer_node_action<GameObjectTypeContributor>    NodeAction_type;
    typedef Zen::Studio::Workbench::indirect_spread_sheet_action<GameObjectTypeContributor>     DocumentAction_type;
    typedef Zen::Studio::Workbench::I_ExplorerNode                                              I_ExplorerNode;
    typedef Zen::Studio::Workbench::I_SpreadSheetView                                           I_SpreadSheetView;
    typedef Zen::Studio::Workbench::I_SpreadSheetCell                                           I_SpreadSheetCell;
    /// @}

    /// @name I_ExplorerNodeContributor implementation
    /// @{
public:
    virtual void getExplorerNodeActions(I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor);
    virtual void getExplorerNodeDecorations(I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor);
    /// @}

    /// @name GameObjectTypeContributor implementation
    /// @{
public:
    /// SpreadSheet Viewable Action Handler
    void getActions(Zen::Studio::Workbench::I_SpreadSheetViewable::I_SpreadSheetViewEvent& _event);

    /// Edit a Game Object Type.
    void open(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Rename a Game Object Type.
    void rename(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Add a child to a Game Object Type.
    void addChild(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Insert a row into the document associated with this view.
    void insertRow(I_SpreadSheetView& _view, int _column, int _row);

    /// @}

    /// @name 'Structors
    /// @{
public:
             GameObjectTypeContributor(ContributorService& _service);
    virtual ~GameObjectTypeContributor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ContributorService&         m_service;
    NodeAction_type             m_open;
    NodeAction_type             m_rename;
    NodeAction_type             m_addChild;
    DocumentAction_type         m_insertRow;
    /// @}

};  // class GameObjectTypeContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // GAMEBUILDER_GAME_OBJECT_TYPE_CONTRIBUTOR_HPP_INCLUDED
