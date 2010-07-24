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
#ifndef ARTLIBRARY_ART_ASSET_CONTRIBUTOR_HPP_INCLUDED
#define ARTLIBRARY_ART_ASSET_CONTRIBUTOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_ExplorerNodeContributor.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>
#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetViewable.hpp>

#include <Zen/Studio/WorkbenchCommon/indirect_explorer_node_action.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ContributorService;

class ArtAssetContributor
:   public Zen::Studio::Workbench::I_ExplorerNodeContributor
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::indirect_explorer_node_action<ArtAssetContributor>          NodeAction_type;
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

    /// @name ArtAssetContributor implementation
    /// @{
public:
    /// SpreadSheet Viewable Action Handler
    void getActions(Zen::Studio::Workbench::I_SpreadSheetViewable::I_SpreadSheetViewEvent& _event);

    /// Edit a Art Asset.
    void open(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Rename a Art Asset.
    void rename(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ArtAssetContributor(ContributorService& _service);
    virtual ~ArtAssetContributor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ContributorService&         m_service;
    NodeAction_type             m_open;
    NodeAction_type             m_rename;
    /// @}

};  // class ArtAssetContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ARTLIBRARY_ART_ASSET_CONTRIBUTOR_HPP_INCLUDED
