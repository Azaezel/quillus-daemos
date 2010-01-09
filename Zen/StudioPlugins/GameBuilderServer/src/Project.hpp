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
#ifndef GAMEBUILDER_PROJECT_HPP_INCLUDED
#define GAMEBUILDER_PROJECT_HPP_INCLUDED

#include "GameObjectType.hpp"
#include "GameObjectTypeDocument.hpp"

#include <Zen/Core/Memory/managed_ptr.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_Project.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeView.hpp>

#include <set>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
    namespace Studio {
        namespace Workbench {
            class I_ExplorerNodeType;
        }   // namespace Workbench
    }   // namespace Studio
}   // namespace Zen
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
;

class Project
:   public I_Project
,   public I_GameObjectTypeView
{
    /// @name Types
    /// @{
public:
    typedef std::map<GameObjectTypeDocument*, GameObjectTypeDocument::pSubscription_type>   DocumentSubscriptionMap_type;
    /// @}

    /// @name Database::I_Persistable implementation
    /// @{
public:
    virtual void insert(pDatabaseConnection_type _pDBConn);
    virtual void load(pDatabaseConnection_type _pDBConn);
    virtual void save(pDatabaseConnection_type _pDBConn);
    virtual void remove(pDatabaseConnection_type _pDBConn);
    /// @}

    /// @name I_ExplorerNode::UserData implementation
    /// @{
public:
    virtual Zen::Studio::Workbench::I_ExplorerNodeType& getType() const;
    /// @}

    /// @name Workbench::I_Project implementation
    /// @{
public:
    virtual void onCreated();
    virtual boost::uint64_t getProjectId() const;
    virtual void setProjectId(boost::uint64_t _projectId);
    virtual Zen::Studio::Workbench::I_ProjectExplorerController& getController();
    virtual Zen::Studio::Workbench::I_WorkbenchService& getWorkbenchService();
    virtual pDatabaseConnection_type getDatabaseConnection();
    virtual const boost::filesystem::path& getControlPath();
    /// @}

    /// @name GameBuilder::I_Project implementation
    /// @{
public:
    virtual pSubscription_type subscribe(pProjectListener_type _pListener, pFilter_type _pFilter = pFilter_type());
    /// @}

    /// @name I_GameObjectTypeView implementation
    /// @{
public:
    virtual void onNewElement(I_GameObjectElement& _element, int _position);
    virtual void onElementRemoved(int _position);
    virtual void onElementModified(I_GameObjectElement& _element, int _row);
    /// @}

    /// @name Project implementation
    /// @{
public:
    /// This method is called by subscription<>.
    /// @note Internal Use Only
    void unSubscribe(pProjectListener_type _pListener);
private:
    /// Notify a listener of the current state of the project.
    void notifyView(pProjectListener_type _pListener);

    /// Called when a document is opened.
    void handleOpenedDocument(GameObjectTypeDocument* _pDocument);

    /// Called when a document is closed.
    void handleClosedDocument(GameObjectTypeDocument* _pDocument);

    void notifyViewsOfChange(I_GameObjectTypeDocument& _document);
    /// @}

    /// @name 'Structors
    /// @{
protected:
    friend class ProjectService;
    explicit Project(Zen::Studio::Workbench::I_ProjectExplorerController& _controller, const std::string& _name);
    virtual ~Project();
    /// @}

    /// @name Member Variables
    /// @{
private:
    Zen::Studio::Workbench::I_ProjectExplorerController&    m_controller;
    boost::uint64_t             m_projectId;

    static class ProjectType    sm_type;

    Zen::Threading::I_Mutex*                            m_pSubscriptionsGuard;

    typedef std::set<pProjectListener_type>             ProjectSubscribers_type;
    ProjectSubscribers_type     m_listeners;

    DocumentSubscriptionMap_type    m_documentSubscriptions;
    /// @}

};  // class Project

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif  // GAMEBUILDER_PROJECT_HPP_INCLUDED

