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
#ifndef WORLDBUILDER_ZONE_CONTRIBUTOR_HPP_INCLUDED
#define WORLDBUILDER_ZONE_CONTRIBUTOR_HPP_INCLUDED

#include <Zen/Core/Memory/managed_weak_ptr.hpp>

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_ExplorerNodeContributor.hpp>
#include <Zen/Studio/Workbench/I_SceneContributor.hpp>
#include <Zen/Studio/Workbench/I_Notebook.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/indirect_explorer_node_action.hpp>

#include <OgreConfig.h>

#if OGRE_MEMORY_ALLOCATOR == OGRE_MEMORY_ALLOCATOR_NED
#error Oops
#endif

#include <map>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class ContributorService;
class Creator;
class ZoneLayerView;

/// Zone Contributor.
/// This contributes to the Zone Explorer Node as well as the Scene View
class ZoneContributor
:   public Zen::Studio::Workbench::I_ExplorerNodeContributor
,   public Zen::Studio::Workbench::I_SceneContributor
{
    /// @name Types
    /// @{
public:
    typedef Zen::Studio::Workbench::indirect_explorer_node_action<ZoneContributor>  NodeAction_type;
    typedef std::map<Zen::Studio::Workbench::I_View*, Creator*>                     Views_type;
    /// @}

    /// @name I_ExplorerNodeContributor implementation
    /// @{
public:
    virtual void getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor);
    virtual void getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor);
    /// @}

    /// @name I_SceneContributor interface
    /// @{
public:
    /// Get the scene actions associated with this model.
    /// @param _pSceneNode possibly NULL if the action is requested of the model and not of a specific scene node.
    virtual void getActions(Zen::Studio::Workbench::I_SceneModel& _model, Zen::Engine::Rendering::I_SceneNode* _pSceneNode, I_SceneActionVisitor& _visitor);
    /// @}

    /// @name ZoneContributor implementation
    /// @{
public:
    /// Enable this contributor.
    void enableActions();

    /// Edit a Game Object Type.
    void open(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Set a zone as the one being actively edited.
    void setActiveZone(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode);

    /// Handle workspace notebook onActivate event.
    void activate(Zen::Studio::Workbench::I_Notebook::ViewEvent& _event);
    
    /// Create the ZoneLayerView.
    void createZoneLayerView();

    /// Initialize Ogitor
    void initializeOgitor(Zen::Studio::Workbench::I_View& _view);
    /// @}

    /// @name 'Structors
    /// @{
public:
             ZoneContributor(ContributorService& _service);
    virtual ~ZoneContributor();
    /// @}

    /// @name Member Variables
    /// @{
private:
    ContributorService&         m_service;
    NodeAction_type             m_open;
    NodeAction_type             m_setActiveZone;

    Views_type                  m_views;
    Creator*                    m_pCurrentCreator;

    /// Only one of these?  Probably should be one
    /// per project like the Creator views.
    ZoneLayerView*              m_pZoneLayerView;

    /// True if this contributor has been initialized.
    bool                        m_initialized;
    /// @}

};  // class ZoneContributor

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // WORLDBUILDER_ZONE_CONTRIBUTOR_HPP_INCLUDED
