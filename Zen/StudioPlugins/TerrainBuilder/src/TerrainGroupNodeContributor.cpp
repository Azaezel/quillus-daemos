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
#include "TerrainGroupNodeContributor.hpp"
#include "ContributorService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_Workbench.hpp>
#include <Zen/Studio/Workbench/I_ProjectExplorerWidget.hpp>

#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>
#include <Zen/Studio/Workbench/I_SceneView.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>
#include <Zen/Studio/WorkbenchCommon/I_PropertiesPublisher.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Property.hpp>

#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainGroupNode.hpp>
#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainGroupDocument.hpp>
#include <Zen/StudioPlugins/TerrainBuilderCommon/I_TerrainGroupController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace TerrainBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupNodeContributor::TerrainGroupNodeContributor(ContributorService& _service)
:   m_service(_service)
,   m_open("Edit", "open", boost::bind(&TerrainGroupNodeContributor::open, this, _1))
,   m_rename("Rename", "rename", boost::bind(&TerrainGroupNodeContributor::rename, this, _1))
,   m_addTerrainChunk("Add Terrain Chunk", "new", boost::bind(&TerrainGroupNodeContributor::addTerrainChunk, this, _1))
{
    I_ExplorerNode::onSelect.connect(boost::bind(&TerrainGroupNodeContributor::focus, this, _1));
    I_ExplorerNode::onDeselect.connect(boost::bind(&TerrainGroupNodeContributor::unfocus, this, _1));
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainGroupNodeContributor::~TerrainGroupNodeContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNodeContributor::getExplorerNodeActions(I_ExplorerNode& _explorerNode,
                                                    I_ExplorerNodeActionVisitor& _visitor)
{
    _visitor.begin();

    _visitor.visit(m_open);
    _visitor.visit(m_rename);
    _visitor.visit(m_addTerrainChunk);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNodeContributor::getExplorerNodeDecorations(I_ExplorerNode& _explorerNode,
                                                        I_ExplorerNodeDecorationVisitor& _visitor)
{
    _visitor.begin();

    // TODO implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNodeContributor::open(I_ExplorerNode& _selectedNode)
{
    I_TerrainGroupNode* pTerrainGroupNode =
        dynamic_cast<I_TerrainGroupNode*>(_selectedNode.getUserData().get());

    /// TODO Would it make sense to pass an I_Properties object to the view?
    /// Or rather, would it make more sense to have the view act as a subscriber
    /// to an I_PropertiesPublisher object?
    if (pTerrainGroupNode != NULL)
    {
        // Create / get the view for this document.
        Zen::Studio::Workbench::I_View& view = 
            m_service.getWorkbench().getView(
                "ogrescene", 
                pTerrainGroupNode->getDocument(), 
                true
            );

        Zen::Studio::Workbench::I_SceneView* pView =
            dynamic_cast<Zen::Studio::Workbench::I_SceneView*>(&view);
        assert(pView);

        std::string skyboxType = "";
        const Zen::Studio::Workbench::I_Property* pProperty;

        typedef std::map<std::string, std::string>      config_type;
        config_type resourceConfig;
        config_type skyboxConfig;

        pProperty = pTerrainGroupNode->getPropertiesPublisher()
            .getPropertyByName("Resource Group\\Name");
        if (pProperty != NULL)
        {
            resourceConfig["groupName"] = pProperty->getValue();
        }

        pProperty = pTerrainGroupNode->getPropertiesPublisher()
            .getPropertyByName("Resource Group\\Path");
        if (pProperty != NULL)
        {
            resourceConfig["path"] = pProperty->getValue();
        }

        pView->addResourceLocation(
            resourceConfig["path"],
            "FileSystem",
            resourceConfig["groupName"],
            true
        );

        pProperty = pTerrainGroupNode->getPropertiesPublisher()
            .getPropertyByName("Skybox Properties\\Distance");
        if (pProperty != NULL)
        {
            skyboxConfig["distance"] = pProperty->getValue();
        }

        pProperty = pTerrainGroupNode->getPropertiesPublisher()
            .getPropertyByName("Skybox Properties\\Resource Name");
        if (pProperty != NULL)
        {
            skyboxConfig["resourceName"] = pProperty->getValue();
        }

        pProperty = pTerrainGroupNode->getPropertiesPublisher()
            .getPropertyByName("Skybox Properties\\Scene Manager");
        if (pProperty != NULL)
        {
            skyboxConfig["scene"] = pProperty->getValue();
        }

        pProperty = pTerrainGroupNode->getPropertiesPublisher()
            .getPropertyByName("Skybox Properties\\Skybox Type");
        if (pProperty != NULL)
        {
            skyboxType = pProperty->getValue();
            skyboxConfig["type"] = "skybox";
        }

        pView->setSkyboxProperties(skyboxType, skyboxConfig);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNodeContributor::rename(I_ExplorerNode& _selectedNode)
{
    I_TerrainGroupNode* pTerrainGroupNode =
        dynamic_cast<I_TerrainGroupNode*>(_selectedNode.getUserData().get());

    if (pTerrainGroupNode != NULL)
    {
        m_service.getWorkbench().getProjectExplorerWidget().beginLabelEdit(_selectedNode);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNodeContributor::addTerrainChunk(I_ExplorerNode& _selectedNode)
{
    I_TerrainGroupNode* pTerrainGroupNode =
        dynamic_cast<I_TerrainGroupNode*>(_selectedNode.getUserData().get());

    if (pTerrainGroupNode != NULL)
    {
        pTerrainGroupNode->addChild(_selectedNode);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNodeContributor::focus(I_ExplorerNode* _pNode)
{
    I_TerrainGroupNode* pTerrainGroupNode =
        dynamic_cast<I_TerrainGroupNode*>(_pNode->getUserData().get());

    if (pTerrainGroupNode != NULL)
    {
        // Create / get the view for this document.
        //m_service.getWorkbench().getView("ogrescene", pTerrainGroupNode->getDocument(), false);
        m_pPropertiesHandle = m_service.getWorkbench().registerProperties(
            _pNode->getUserData()->getName(),
            _pNode->getUserData()->getPropertiesPublisher()
        );

        m_pPropertiesHandle->show();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainGroupNodeContributor::unfocus(I_ExplorerNode* _pNode)
{
    I_TerrainGroupNode* pTerrainGroupNode =
        dynamic_cast<I_TerrainGroupNode*>(_pNode->getUserData().get());

    if (pTerrainGroupNode != NULL)
    {
        m_pPropertiesHandle.reset();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace TerrainBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
