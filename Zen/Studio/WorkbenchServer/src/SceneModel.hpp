//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Studio
//
// Copyright (C) 2001 - 2009 Tony Richards
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  Tony Richards trichards@indiezen.com
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifndef ZENSTUDIO_WORKBENCH_SCENE_MODEL_HPP_INCLUDED
#define ZENSTUDIO_WORKBENCH_SCENE_MODEL_HPP_INCLUDED

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNode.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SceneModel.hpp>

#include "SceneController.hpp"

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Studio {
namespace Workbench {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
class WorkbenchService;

/// Scene Model.
/// @todo TR - Do I need to have an implemeentation for a Document class?
class SceneModel
:   public I_SceneModel
{
    /// @name Types
    /// @{
public:
    /// Set of weak pointers to views.
    typedef std::set<pSceneView_type>                                   SceneViews_type;

    typedef I_ExplorerNode*                                             pExplorerNode_type;
    typedef I_ExplorerNode::ExplorerNodeEvent_type::pConnection_type    pNodeEventConnection_type;
    /// @}

    /// @name I_Document implementation
    /// @todo Should this be moved to a Document class?
    /// @{
public:
    virtual const std::string& getShortName() const;
    virtual const std::string& getLongName() const;
    virtual I_Project* getProject() const;
    /// @}

    /// @name I_SceneModel implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pSceneView_type _pView, pFilter_type _pFilter = pFilter_type());
    virtual I_SceneController& getController();
    virtual pUserData_type getUserData() const;
    /// @}

    /// @name SceneModel implementation.
    ///     These methods should only be used by SceneController.
    /// @{
protected:
    friend class SceneController;

protected:
    friend class WorkbenchService;
    /// Load (or create) a SceneModel document using the node's
    /// nodeId as a unique identifier for the document.
    void load(pExplorerNode_type _pNode);

    /// Save this document.
    /// @note If we add a mutex to lock the document then assume that
    ///     it is locked before save() is called.
    void save();

    /// @name Internal implementation.
    /// Don't call these methods directly.  They're called by helper
    /// classes / functions in SpreadSheetDocument.cpp
    /// @{
public:
    /// Set the internal document id.
    void setSceneModelId(boost::uint64_t _modelId);

    /// Get the internal document Id.
    boost::uint64_t getSceneModelId() const;

    /// Unsubscribe a view from this document.
    void unSubscribe(pSceneView_type _pView);
    /// @}

    /// @name Event Handlers
    /// @{
public:
    void handleExplorerNodeModified(I_ExplorerNode* _pNode);
    /// @}

    /// @name 'Structors
    /// @{
public:
    explicit SceneModel(WorkbenchService& _workbenchService, I_Project* _pProject, pUserData_type _pUserData);
    virtual ~SceneModel();
    /// @}

    /// @name Member Variables
    /// @{
private:
    WorkbenchService&           m_workbenchService;
    /// The containing project, or NULL if this model is not contained
    /// by a project.
    I_Project*                  m_pProject;
    /// The I_SceneModel::UserData associated with this model.
    pUserData_type              m_pUserData;
    /// The SceneController that controls this model.
    /// i.e. m_controller is the public write-access to this model.
    SceneController             m_controller;
    pExplorerNode_type          m_pNode;
    pNodeEventConnection_type   m_pNodeConnection;

    boost::uint64_t             m_sceneModelId;
    /// true if this document has been loaded from the database.
    bool                        m_loaded;

    /// Views subscribed to this model.
    SceneViews_type             m_views;

    /// Guard for m_views.
    Threading::I_Mutex*         m_pViewsGuard;
    /// @}

};  // class SpreadSheetDocument

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Workbench
}   // namespace Studio
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // ZENSTUDIO_WORKBENCH_SCENE_MODEL_HPP_INCLUDED
