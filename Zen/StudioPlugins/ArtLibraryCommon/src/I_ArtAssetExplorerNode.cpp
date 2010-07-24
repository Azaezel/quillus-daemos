//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Art Library
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

#include "../I_ArtAssetExplorerNode.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/generic_explorer_node_type.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace ArtLibrary {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ArtAssetExplorerNode::I_ArtAssetExplorerNode(const std::string& _name)
:   Zen::Studio::Workbench::I_ExplorerNode::UserData(_name)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_ArtAssetExplorerNode::~I_ArtAssetExplorerNode()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ArtAssetNodeType
:   public Zen::Studio::Workbench::generic_explorer_node_type<ArtAssetNodeType>
{
    /// @name I_ExplorerNodeType implemenetation
    /// @{
public:
    virtual bool getNameMutability(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode) const
    {
        // TODO Possibly check to make sure this node isn't locked?
        return true;
    }

    virtual bool isDraggable(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode) const
    {
        return false;
    }

    virtual bool canDrop(Zen::Studio::Workbench::I_ExplorerNode& _sourceNode, Zen::Studio::Workbench::I_ExplorerNode& _destNode) const
    {
        return false;
    }
    /// @}

public:
    ArtAssetNodeType()
    :   Zen::Studio::Workbench::generic_explorer_node_type<ArtAssetNodeType>::generic_explorer_node_type("ArtLibrary::ArtAssetExplorerNode", "./icons/page_green.png")
    {
    }

    virtual ~ArtAssetNodeType()
    {
    }

};

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static ArtAssetNodeType sm_type;

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
I_ArtAssetExplorerNode::getNodeType()
{
    return sm_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace ArtLibrary
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
