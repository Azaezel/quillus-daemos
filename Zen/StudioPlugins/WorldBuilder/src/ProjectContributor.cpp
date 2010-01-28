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

#include "ProjectContributor.hpp"
#include "ContributorService.hpp"

#include <Zen/Studio/Workbench/I_Workbench.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>

#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_WorkbenchService.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_Project.hpp>

#include <iostream>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectContributor::ProjectContributor(ContributorService& _service)
:   m_service(_service)
,   m_addZoneAspect("Add Zone Aspect", "aspect", boost::bind(&ProjectContributor::addZoneAspect, this, _1))
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ProjectContributor::~ProjectContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectContributor::getExplorerNodeActions(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor)
{
    class ExplorerNodeVisitor
    :   public Zen::Studio::Workbench::I_ExplorerNode::I_ExplorerNodeVisitor
    {
    public:
        virtual void begin()
        {
        }

        virtual void visit(Zen::Studio::Workbench::I_ExplorerNode& _node)
        {
            if( m_first && _node.getUserData()->getType().getName() == "WorldBuilder::ZoneFolder" )
            {
                m_first = false;
            }
        }

        virtual void end()
        {
            if( m_first )
            {
                m_actionVisitor.visit(m_nodeAction);
            }
        }

        ExplorerNodeVisitor(I_ExplorerNodeActionVisitor& _actionVisitor, NodeAction_type& _nodeAction)
        :   m_first(true)
        ,   m_actionVisitor(_actionVisitor)
        ,   m_nodeAction(_nodeAction)
        {
        }
        ~ExplorerNodeVisitor() {}

    private:
        bool                            m_first;
        I_ExplorerNodeActionVisitor&    m_actionVisitor;
        NodeAction_type&                m_nodeAction;
    };

    _visitor.begin();

    GameBuilder::I_Project* pProject = dynamic_cast<GameBuilder::I_Project*>(_explorerNode.getUserData().get());

    if( pProject != NULL )
    {
        // Visit the child nodes to see if a zone aspect already exists.
        // If not, visit the 'Add Zone Aspect' node action.
        ExplorerNodeVisitor visitor(_visitor, m_addZoneAspect);
        pProject->getNode()->getChildren(visitor);
    }

    //_visitor.visit(m_addZoneAspect);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectContributor::getExplorerNodeDecorations(Zen::Studio::Workbench::I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor)
{
    _visitor.begin();

    // TODO Implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ProjectContributor::addZoneAspect(Zen::Studio::Workbench::I_ExplorerNode& _selectedNode)
{
    GameBuilder::I_Project* pProject = dynamic_cast<GameBuilder::I_Project*>(_selectedNode.getUserData().get());

    if (pProject)
    {
        // TODO Make sure the project doesn't already have this aspect.
        // TODO What else to do to add an aspect to a project?

        // Create the Zones folder under the project folder.
        Zen::Studio::Workbench::I_Project* pProject = _selectedNode.getProject();
        if (pProject)
        {
            const boost::filesystem::path& projectPath = pProject->getProjectPath();
            boost::filesystem::path zonePath = projectPath / "Zones";
            if (!boost::filesystem::exists(zonePath))
            {
                boost::filesystem::create_directories(zonePath);
            }

            Zen::Studio::Workbench::I_ExplorerNode::pUserData_type
                pData = m_service.getWorkbench().getWorkbenchService().
                createNodeUserData(0, "WorldBuilder::ZoneFolder", _selectedNode);

            pProject->getController().createChildNode(*pProject->getNode(), pData);
        }
        else
        {
            throw Zen::Utility::runtime_exception("WorldBuilder::ProjectContributor::addZoneAspect(): Error, can't add a zone aspect to an explorer node that isn't associated with a project.");
        }
    }
    else
    {
        // Wrong project type, ignore it or throw an exception or what?
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
