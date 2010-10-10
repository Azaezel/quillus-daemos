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
#include "TerrainChunkNodeContributor.hpp"
#include "ContributorService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_Workbench.hpp>
#include <Zen/Studio/Workbench/I_ProjectExplorerWidget.hpp>

#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainChunkNode.hpp>
#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainChunkDocument.hpp>
#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainChunkController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkNodeContributor::TerrainChunkNodeContributor(ContributorService& _service)
:   m_service(_service)
,   m_open("Edit", "open", boost::bind(&TerrainChunkNodeContributor::open, this, _1))
,   m_rename("Rename", "rename", boost::bind(&TerrainChunkNodeContributor::rename, this, _1))
{
    I_ExplorerNode::onSelect.connect(boost::bind(&TerrainChunkNodeContributor::focus, this, _1));
    I_ExplorerNode::onDeselect.connect(boost::bind(&TerrainChunkNodeContributor::unfocus, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainChunkNodeContributor::~TerrainChunkNodeContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNodeContributor::getExplorerNodeActions(I_ExplorerNode& _explorerNode,
                                                    I_ExplorerNodeActionVisitor& _visitor)
{
    _visitor.begin();

    _visitor.visit(m_open);
    _visitor.visit(m_rename);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNodeContributor::getExplorerNodeDecorations(I_ExplorerNode& _explorerNode,
                                                        I_ExplorerNodeDecorationVisitor& _visitor)
{
    _visitor.begin();

    // TODO implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNodeContributor::open(I_ExplorerNode& _selectedNode)
{
    I_TerrainChunkNode* pTerrainChunkNode =
        dynamic_cast<I_TerrainChunkNode*>(_selectedNode.getUserData().get());

    if (pTerrainChunkNode != NULL)
    {
        // Create / get the view for this document.
        //m_service.getWorkbench().getView("ogrescene", pTerrainChunkNode->getDocument(), false);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNodeContributor::rename(I_ExplorerNode& _selectedNode)
{
    I_TerrainChunkNode* pTerrainChunkNode =
        dynamic_cast<I_TerrainChunkNode*>(_selectedNode.getUserData().get());

    if (pTerrainChunkNode != NULL)
    {
        m_service.getWorkbench().getProjectExplorerWidget().beginLabelEdit(_selectedNode);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNodeContributor::focus(I_ExplorerNode* _pNode)
{
    I_TerrainChunkNode* pTerrainChunkNode =
        dynamic_cast<I_TerrainChunkNode*>(_pNode->getUserData().get());

    if (pTerrainChunkNode != NULL)
    {
        // Create / get the view for this document.
        //m_service.getWorkbench().getView("ogrescene", pTerrainChunkNode->getDocument(), false);
        m_pPropertiesHandle = m_service.getWorkbench().registerProperties(
            _pNode->getUserData()->getName(),
            _pNode->getUserData()->getPropertiesPublisher()
        );

        m_pPropertiesHandle->show();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainChunkNodeContributor::unfocus(I_ExplorerNode* _pNode)
{
    I_TerrainChunkNode* pTerrainChunkNode =
        dynamic_cast<I_TerrainChunkNode*>(_pNode->getUserData().get());

    if (pTerrainChunkNode != NULL)
    {
        m_pPropertiesHandle.reset();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
