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

#include "../I_TerrainGroupNode.hpp"

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/generic_explorer_node_type.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_TerrainGroupNode::I_TerrainGroupNode(const std::string& _name)
:   Zen::Studio::Workbench::I_ExplorerNode::UserData(_name)
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
I_TerrainGroupNode::~I_TerrainGroupNode()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class TerrainGroupNodeType
:   public Zen::Studio::Workbench::generic_explorer_node_type<TerrainGroupNodeType>
{
    /// @name I_ExplorerNodeType implementation
    /// @{
public:
    virtual bool getNameMutability(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode) const
    {
        // TODO Possibly check to make sure this node isn't locked?
        return true;
    }

    virtual bool isDraggable(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode) const
    {
        return true;
    }

    virtual bool canDrop(Zen::Studio::Workbench::I_ExplorerNode& _sourceNode, Zen::Studio::Workbench::I_ExplorerNode& _destNode) const
    {
        // Return true if both nodes are of the same type.
        if (_sourceNode.getUserData()->getType().getName() == 
            _destNode.getUserData()->getType().getName())
        {
            // TODO Prune / graft.
            return true;
        }
        else
        {
            return false;
        }
    }
    /// @}

    /// @name 'Structors
    /// @{
public:
    TerrainGroupNodeType()
    :   Zen::Studio::Workbench::generic_explorer_node_type<TerrainGroupNodeType>::generic_explorer_node_type("TerrainBuilder::TerrainGroupNode", "./icons/page_green.png")
    {}

    virtual ~TerrainGroupNodeType()
    {}
    /// @}

};  // class TerrainGroupNodeType

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
static TerrainGroupNodeType sm_type;
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Studio::Workbench::I_ExplorerNodeType&
I_TerrainGroupNode::getNodeType()
{
    return sm_type;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
