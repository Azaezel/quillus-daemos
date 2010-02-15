//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Game Builder
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

#include "GameObjectTypeContributor.hpp"
#include "ContributorService.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/I_Workbench.hpp>
#include <Zen/Studio/Workbench/I_ProjectExplorerWidget.hpp>

#include <Zen/Studio/WorkbenchCommon/direct_explorer_node_action.hpp>

#include <Zen/Studio/WorkbenchCommon/I_View.hpp>
#include <Zen/Studio/WorkbenchCommon/I_Document.hpp>

#include <Zen/Studio/WorkbenchCommon/I_SpreadSheetView.hpp>

#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectType.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeDocument.hpp>
#include <Zen/StudioPlugins/GameBuilderCommon/I_GameObjectTypeController.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace GameBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeContributor::GameObjectTypeContributor(ContributorService& _service)
:   m_service(_service)
,   m_open("Edit", "open", boost::bind(&GameObjectTypeContributor::open, this, _1))
,   m_rename("Rename", "rename", boost::bind(&GameObjectTypeContributor::rename, this, _1))
,   m_addChild("Add Child", "new", boost::bind(&GameObjectTypeContributor::addChild, this, _1))
,   m_insertRow("Insert Element", "insertrow", boost::bind(&GameObjectTypeContributor::insertRow, this, _1, _2, _3))
{

}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
GameObjectTypeContributor::~GameObjectTypeContributor()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeContributor::getExplorerNodeActions(I_ExplorerNode& _explorerNode, I_ExplorerNodeActionVisitor& _visitor)
{
    _visitor.begin();

    _visitor.visit(m_open);
    _visitor.visit(m_rename);
    _visitor.visit(m_addChild);

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeContributor::getExplorerNodeDecorations(I_ExplorerNode& _explorerNode, I_ExplorerNodeDecorationVisitor& _visitor)
{
    _visitor.begin();

    // TODO Implement

    _visitor.end();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeContributor::getActions(Zen::Studio::Workbench::I_SpreadSheetViewable::I_SpreadSheetViewEvent& _event)
{
    _event.addAction(m_insertRow);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeContributor::open(I_ExplorerNode& _selectedNode)
{
    I_GameObjectType* pGameObjectType = dynamic_cast<I_GameObjectType*>(_selectedNode.getUserData().get());

    if (pGameObjectType)
    {
        // Create / get the view for this document.
        m_service.getWorkbench().getView("spreadsheet", pGameObjectType->getDocument(), false);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeContributor::rename(I_ExplorerNode& _selectedNode)
{
    I_GameObjectType* pGameObjectType = dynamic_cast<I_GameObjectType*>(_selectedNode.getUserData().get());

    if (pGameObjectType)
    {
        m_service.getWorkbench().getProjectExplorerWidget().beginLabelEdit(_selectedNode);
    }
}


//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeContributor::addChild(I_ExplorerNode& _selectedNode)
{
    I_GameObjectType* pGameObjectType = dynamic_cast<I_GameObjectType*>(_selectedNode.getUserData().get());

    if (pGameObjectType)
    {
        pGameObjectType->addChild(_selectedNode);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
GameObjectTypeContributor::insertRow(I_SpreadSheetView& _view, int _column, int _row)
{
    // TODO Get the document from the view, typecast it to I_GameObjectTypeDocument, 
    // get the controller from the document and then add a new element.
    
    I_GameObjectTypeDocument* pDoc = dynamic_cast<I_GameObjectTypeDocument*>(_view.getViewable());

    // It should be a I_GameObjectTypeDocument, but double-check.
    if (pDoc)
    {
        pDoc->getGameObjectTypeController().insertElement(-1, "New Element");
    }
    else
    {
        throw Zen::Utility::runtime_exception("GameObjectTypeContributor::insertRow(): Error, not implemented");
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace GameBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
