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
#ifndef TERRAIN_BUILDER_I_TERRAIN_GROUP_VIEW_HPP_INCLUDED
#define TERRAIN_BUILDER_I_TERRAIN_GROUP_VIEW_HPP_INCLUDED

#include "Configuration.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class I_TerrainGroupDocument;

/// Terrain Group View.
/// Implement a view and use I_TerrainGroupDocument::subscribe() in
/// order to receive notifications of modifications to the document.
class TERRAINBUILDERCOMMON_DLL_LINK I_TerrainGroupView
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_TerrainGroupView interface
    /// @{
public:
    /// This method is called when a terrain group document is modified.
    /// Generally this modification is a property modification.
    virtual void onDocumentModified(I_TerrainGroupDocument& _terrainGroupDocument) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_TerrainGroupView();
    virtual ~I_TerrainGroupView();
    /// @}

};  // interface I_TerrainGroupView

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_I_TERRAIN_GROUP_VIEW_HPP_INCLUDED
