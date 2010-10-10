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
#ifndef TERRAIN_BUILDER_I_PROJECT_LISTENER_HPP_INCLUDED
#define TERRAIN_BUILDER_I_PROJECT_LISTENER_HPP_INCLUDED

#include "Configuration.hpp"

#include <boost/noncopyable.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

class I_Project;
class I_TerrainGroupNode;
class I_TerrainGroupDocument;

/// A project listener.
/// A project listener is notified of all changes made to a project.
/// Implement this interface and subscribe to a I_Project in order to
/// receive notifications of changes to a TerrainBuilder::I_Project and
/// all of its child elements.
/// @see TerrainBuilder::I_Project::subscribe()
class TERRAINBUILDERCOMMON_DLL_LINK I_ProjectListener
:   public boost::noncopyable
{
    /// @name Types
    /// @{
public:
    /// @}

    /// @name I_ProjectListener interface
    /// @{
public:
    /// Called when a terrain group is created.
    /// Don't retain a reference to this element. Instead, use getNode()->getNodeId()
    /// as a unique key.
    virtual void terrainGroupCreated(I_TerrainGroupNode& _terrainGroupNode) = 0;

    /// Called when a terrain group document is modified.
    virtual void terrainGroupDocumentModified(I_TerrainGroupDocument& _document) = 0;

    /// Called when a terrain group is deleted.
    virtual void terrainGroupDeleted(I_TerrainGroupNode& _terrainGroupNode) = 0;
    /// @}

    /// @name 'Structors
    /// @{
protected:
             I_ProjectListener();
    virtual ~I_ProjectListener();
    /// @}

};  // interface I_ProjectListener

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // TERRAIN_BUILDER_I_PROJECT_LISTENER_HPP_INCLUDED
