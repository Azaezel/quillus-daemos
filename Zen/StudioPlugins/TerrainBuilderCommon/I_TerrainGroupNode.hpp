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
#ifndef TERRAIN_BUILDER_I_TERRAIN_GROUP_NODE_HPP_INCLUDED
#define TERRAIN_BUILDER_I_TERRAIN_GROUP_NODE_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeType;
            class I_Document;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

/// Terrain Group explorer node user data interface
class TERRAINBUILDERCOMMON_DLL_LINK I_TerrainGroupNode
:   public Zen::Studio::Workbench::I_ExplorerNode::UserData
{
    /// @name Types
    /// @{
public:
    typedef Zen::Memory::managed_ptr<Zen::Studio::Workbench::I_Document>            pDocument_type;
    /// @}

    /// @name I_TerrainGroupNode interface
    /// @{
public:
    /// Get the document associated with this Terrain Group.
    /// @todo MG - This should probably be a pFutureDocument_type instead of a
    ///     pDocument_type. If the document has not been loaded yet, it will
    ///     likely take a while to load.
    virtual pDocument_type getDocument() = 0;

    /// Create a new object that is a child of the selected node.
    virtual void addChild(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode) = 0;

    /// Get the database primary key for this terrain group.
    virtual boost::uint64_t getTerrainGroupId() const = 0;
    /// @}

    /// @name Static Methods
    /// @{
public:
    static Zen::Studio::Workbench::I_ExplorerNodeType& getNodeType();
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainGroupNode(const std::string& _name);
    virtual ~I_TerrainGroupNode();
    /// @}

};  // interface I_TerrainGroupNode

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_I_TERRAIN_GROUP_NODE_HPP_INCLUDED
