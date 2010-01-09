//---------------------------------------------------------------------------
//
// Name:        ProjectExplorerView.cpp
// Author:      Tony Richards
// Created:     1/30/2009 4:56:27 PM
// Description: ProjectExplorerView class implementation
//
//---------------------------------------------------------------------------

#include "ProjectExplorerView.h"

#include <wx/imaglist.h>

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <Zen/Studio/Workbench/I_ExplorerNodeContributor.hpp>

#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeAction.hpp>
#include <Zen/Studio/WorkbenchCommon/I_ExplorerNodeType.hpp>
#include <Zen/Studio/WorkbenchCommon/II_ExplorerNode.hpp>

#include <Zen/Spaces/ObjectModel/I_Filter.hpp>
#include <Zen/Spaces/ObjectModel/I_Subscription.hpp>

#include <boost/bind.hpp>

#include <list>
#include <map>

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// ProjectExplorerView
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(ProjectExplorerView,wxPanel)
	////Manual Code Start
	////Manual Code End

	EVT_CLOSE(ProjectExplorerView::OnClose)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_WXTREECTRL1,ProjectExplorerView::handleRightClick)
	EVT_TREE_BEGIN_LABEL_EDIT(ID_WXTREECTRL1,ProjectExplorerView::handleBeginLabelEdit)
	EVT_TREE_END_LABEL_EDIT(ID_WXTREECTRL1,ProjectExplorerView::handleEndLabelEdit)
    EVT_TREE_ITEM_ACTIVATED(ID_WXTREECTRL1,ProjectExplorerView::handleActivate)
    EVT_TREE_BEGIN_DRAG(ID_WXTREECTRL1,ProjectExplorerView::handleBeginDrag)
    EVT_TREE_END_DRAG(ID_WXTREECTRL1,ProjectExplorerView::handleEndDrag)
END_EVENT_TABLE()
////Event Table End

ProjectExplorerView::ProjectExplorerView(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
:   wxPanel(parent, id, position, size, style)
,   m_pModel()
{
	CreateGUIControls();

    m_parent = WxTreeCtrl1->AddRoot(wxT("Projects"));
}

ProjectExplorerView::~ProjectExplorerView()
{
}

void ProjectExplorerView::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxBoxSizer1 = new wxGridSizer(1, 1, 5, 5);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxTreeCtrl1 = new wxTreeCtrl(this, ID_WXTREECTRL1, wxPoint(5,5), wxSize(208,240),
        wxTR_EDIT_LABELS | wxTR_HAS_BUTTONS | wxTR_SINGLE | wxTR_HIDE_ROOT | wxTR_LINES_AT_ROOT);

    WxTreeCtrl1->AssignImageList(new wxImageList(16, 16, false, 2));
	//WxTreeCtrl1->SetForegroundColour(wxColour(*wxBLACK));
	//WxTreeCtrl1->SetBackgroundColour(*wxRED);
	WxTreeCtrl1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	WxBoxSizer1->Add(WxTreeCtrl1, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 5);

	//SetTitle(wxT("Projects"));
	//SetIcon(wxNullIcon);

	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();

	////GUI Items Creation End
}

void ProjectExplorerView::OnClose(wxCloseEvent& event)
{
	Destroy();
}

class InternalTreeItemData
:   public wxTreeItemData
,   public Zen::Studio::Workbench::Internal::II_ExplorerNode::InternalData
{
public:
    InternalTreeItemData(Zen::Studio::Workbench::I_ExplorerNode* _pNode)
    :   Zen::Studio::Workbench::Internal::II_ExplorerNode::InternalData(_pNode)
    {
    }

    virtual ~InternalTreeItemData()
    {
    }

private:
};

void ProjectExplorerView::setModel(pProjectExplorerModel_type _pModel)
{
    // TODO Make sure the model hasn't already been set?
    m_pModel = _pModel;

    m_pSubscription = m_pModel->subscribe(this);
}

void ProjectExplorerView::setImage(const std::string& _imageName, const wxTreeItemId& _id)
{
    ImageListMap_type::iterator iter = m_imageList.find(_imageName);

    if (iter == m_imageList.end())
    {

        const int i = WxTreeCtrl1->GetImageList()->Add(wxIcon(std2wx(_imageName), wxBITMAP_TYPE_PNG));

        m_imageList[_imageName] = i;
        WxTreeCtrl1->SetItemImage(_id, i);
    }
    else
    {
        WxTreeCtrl1->SetItemImage(_id, iter->second);
    }

}

void ProjectExplorerView::handleNewProject(Zen::Studio::Workbench::I_ExplorerNode* _pNode)
{
    InternalTreeItemData* pInternalData = new InternalTreeItemData(_pNode);

    WxTreeCtrl1->AppendItem(m_parent, std2wx(_pNode->getDisplayName()), -1, -1, pInternalData);

    // TODO Map from the name to an image name or something else.
    setImage(_pNode->getUserData()->getType().getIconName(), pInternalData->GetId());

    dynamic_cast<Zen::Studio::Workbench::Internal::II_ExplorerNode*>(_pNode)->setInternalData(pInternalData);

    Refresh();
}

void ProjectExplorerView::handleNewChildNode(Zen::Studio::Workbench::I_ExplorerNode* _pNode)
{
    const Zen::Studio::Workbench::Internal::II_ExplorerNode* pParent =
        dynamic_cast<Zen::Studio::Workbench::Internal::II_ExplorerNode*>(_pNode->getParent());

    const wxTreeItemId parentId = dynamic_cast<InternalTreeItemData*>(pParent->getInternalData())->GetId();

    InternalTreeItemData* pInternalData = new InternalTreeItemData(_pNode);

    WxTreeCtrl1->AppendItem(parentId, std2wx(_pNode->getDisplayName()), -1, -1, pInternalData);

    setImage(_pNode->getUserData()->getType().getIconName(), pInternalData->GetId());

    WxTreeCtrl1->Expand(parentId);

    dynamic_cast<Zen::Studio::Workbench::Internal::II_ExplorerNode*>(_pNode)->setInternalData(pInternalData);

    //Refresh();
}

class ActionVisitor
:   public Zen::Studio::Workbench::I_ExplorerNodeContributor::I_ExplorerNodeActionVisitor
{
public:
    virtual void begin()
    {
    }

    virtual void visit(Zen::Studio::Workbench::I_ExplorerNodeAction& _action)
    {
        m_actions.push_back(&_action);
    }


    virtual void end()
    {
    }


    ActionVisitor(std::list<Zen::Studio::Workbench::I_ExplorerNodeAction*>& _actions)
    :   m_actions(_actions)
    {
    }

    std::list<Zen::Studio::Workbench::I_ExplorerNodeAction*>&   m_actions;
};  // class ActionVisitor

/// Helper class to collect all of the actions for all of the contributors for a node and
/// then present it as a menu.
struct MenuContributorVisitor
:   public wxEvtHandler
,   public Zen::Studio::Workbench::I_ExplorerNodeType::I_ExplorerNodeContributorVisitor
{
    typedef std::list<Zen::Studio::Workbench::I_ExplorerNodeAction*>        ExplorerNodeActions_type;
    typedef std::map<int, Zen::Studio::Workbench::I_ExplorerNodeAction*>    IndexedExplorerNodeActions_type;

    virtual void begin()
    {
    }

    virtual void visit(Zen::Studio::Workbench::I_ExplorerNode& _node, Zen::Studio::Workbench::I_ExplorerNodeContributor& _contributor)
    {
        // Create an action visitor
        ActionVisitor visitor(m_actionList);

        // Get the actions.
        _contributor.getExplorerNodeActions(_node, visitor);
    }

    void handleEvent(wxCommandEvent& _event)
    {
        doAction(_event.GetId());
    }

    virtual void end()
    {
        // Iterate through the list of actions and create the menu
        for(ExplorerNodeActions_type::iterator iter = m_actionList.begin(); iter != m_actionList.end(); iter++)
        {
            // Create a new menu item.
            // TODO This should be done differently so that the menu can
            //  have sections and have a guaranteed consistent order.
            m_pMenu->Append(m_id, std2wx((*iter)->getName()));
            m_actions[m_id] = *iter;
            m_id++;
        }

        // ACK!  wxCommandEventHandler does static casts instead of dynamic casts... can't
        // get rid of the warning.  If you copy this code, make sure wxEvtHandler is the
        // first class in the inheritence chain, otherwise it'll get it wrong.
        Connect(m_startId, m_id, wxEVT_COMMAND_MENU_SELECTED,
            wxCommandEventHandler(MenuContributorVisitor::handleEvent));

    }

    void doAction(int _id)
    {
        IndexedExplorerNodeActions_type::iterator iter = m_actions.find(_id);
        if (iter != m_actions.end())
        {
            iter->second->invoke(m_node);
        }
    }

    MenuContributorVisitor(Zen::Studio::Workbench::I_ExplorerNode& _node, wxMenu* _pMenu, int _id)
    :   m_node(_node)
    ,   m_pMenu(_pMenu)
    ,   m_startId(_id)
    ,   m_id(_id)
    {
    }

private:
    Zen::Studio::Workbench::I_ExplorerNode&     m_node;
    wxMenu*                                     m_pMenu;
    int                                         m_startId;
    int                                         m_id;
    ExplorerNodeActions_type                    m_actionList;
    IndexedExplorerNodeActions_type             m_actions;
};  // class MenuContributorVisitor

/// Simple Contributor Visitor.
struct ContributorVisitor
:   public Zen::Studio::Workbench::I_ExplorerNodeType::I_ExplorerNodeContributorVisitor
{
    typedef std::list<Zen::Studio::Workbench::I_ExplorerNodeAction*>        ExplorerNodeActions_type;
    typedef std::map<int, Zen::Studio::Workbench::I_ExplorerNodeAction*>    IndexedExplorerNodeActions_type;

    virtual void begin()
    {
    }

    virtual void visit(Zen::Studio::Workbench::I_ExplorerNode& _node, Zen::Studio::Workbench::I_ExplorerNodeContributor& _contributor)
    {
        // Create an action visitor
        ActionVisitor visitor(m_actionList);

        // Get the actions.
        _contributor.getExplorerNodeActions(_node, visitor);
    }

    virtual void end()
    {

    }

    ContributorVisitor()
    {
    }

public:
    ExplorerNodeActions_type                    m_actionList;
};  // class ContributorVisitor

void ProjectExplorerView::handleRightClick(wxTreeEvent& event)
{
    InternalTreeItemData* pInternalData =
        dynamic_cast<InternalTreeItemData*>(WxTreeCtrl1->GetItemData(event.GetItem()));

    Zen::Studio::Workbench::I_ExplorerNode* pNode =
        pInternalData->getNode();

    Zen::Studio::Workbench::I_ExplorerNodeType& nodeType =
        pNode->getUserData()->getType();

    // Create a popup menu
    std::auto_ptr<wxMenu> pMenu(new wxMenu);

    // Probably shouldn't hard-code this number here.
    const int startId = wxID_HIGHEST + 1000;

    MenuContributorVisitor visitor(*pNode, pMenu.get(), startId);

    // If the node type has actions then use them
    // to populate the menu.
    nodeType.getContributors(*pNode, visitor);

    if (pMenu->GetMenuItemCount() > 0)
    {
        pMenu->SetEventHandler(&visitor);
        PopupMenu(pMenu.get());
        pMenu->SetEventHandler(NULL);
    }
}

void ProjectExplorerView::handleBeginLabelEdit(wxTreeEvent& event)
{
    InternalTreeItemData* const pInternalData =
        dynamic_cast<InternalTreeItemData*>(WxTreeCtrl1->GetItemData(event.GetItem()));

    Zen::Studio::Workbench::I_ExplorerNode* const pNode =
        pInternalData->getNode();

    Zen::Studio::Workbench::I_ExplorerNodeType& nodeType =
        pNode->getUserData()->getType();

    if (!nodeType.getNameMutability(*pNode))
    {
        event.Veto();
    }
}

void ProjectExplorerView::handleEndLabelEdit(wxTreeEvent& event)
{
    InternalTreeItemData* const pInternalData =
        dynamic_cast<InternalTreeItemData*>(WxTreeCtrl1->GetItemData(event.GetItem()));

    Zen::Studio::Workbench::I_ExplorerNode* const pNode =
        pInternalData->getNode();

    if (!event.IsEditCancelled())
    {
        pNode->setDisplayName(wx2std(event.GetLabel()));
    }
}

void ProjectExplorerView::handleActivate(wxTreeEvent& event)
{
    InternalTreeItemData* const pInternalData =
        dynamic_cast<InternalTreeItemData*>(WxTreeCtrl1->GetItemData(event.GetItem()));

    Zen::Studio::Workbench::I_ExplorerNode* const pNode =
        pInternalData->getNode();

    Zen::Studio::Workbench::I_ExplorerNodeType& nodeType =
        pNode->getUserData()->getType();

    ContributorVisitor visitor;
    nodeType.getContributors(*pNode, visitor);

    // TODO Handle the case where it's already open and handle the case where multiple open actions are valid.
    for(ContributorVisitor::ExplorerNodeActions_type::iterator iter = visitor.m_actionList.begin(); iter != visitor.m_actionList.end(); iter++)
    {
        if ((*iter)->getType() == "open")
        {
            (*iter)->invoke(*pNode);
            return;
        }
    }

    // TODO No default "open" for this node.  What do I do?
}

bool ProjectExplorerView::beginLabelEdit(Zen::Studio::Workbench::I_ExplorerNode& _node)
{
    Zen::Studio::Workbench::Internal::II_ExplorerNode* pNode =
        dynamic_cast<Zen::Studio::Workbench::Internal::II_ExplorerNode*>(&_node);

    Zen::Studio::Workbench::I_ExplorerNodeType& nodeType =
        pNode->getUserData()->getType();

    if (!nodeType.getNameMutability(*pNode))
    {
        return false;
    }

    const wxTreeItemId nodeId = dynamic_cast<InternalTreeItemData*>(pNode->getInternalData())->GetId();

    WxTreeCtrl1->EditLabel(nodeId);

    return true;
}

void ProjectExplorerView::handleBeginDrag(wxTreeEvent& event)
{
    InternalTreeItemData* const pInternalData =
        dynamic_cast<InternalTreeItemData*>(WxTreeCtrl1->GetItemData(event.GetItem()));

    Zen::Studio::Workbench::I_ExplorerNode* const pNode =
        pInternalData->getNode();

    Zen::Studio::Workbench::I_ExplorerNodeType& nodeType =
        pNode->getUserData()->getType();

    // If it's draggable, allow the event.  The event is vetoed by default.
    if (nodeType.isDraggable(*pNode))
    {
        m_pDraggingNode = pNode;
        event.Allow();
    }
    else
    {
        m_pDraggingNode = NULL;
    }
}

void ProjectExplorerView::handleEndDrag(wxTreeEvent& event)
{
    // Ignore if the node being dragged is invalid.
    if (m_pDraggingNode == NULL)
    {
        return;
    }

    InternalTreeItemData* const pDestInternalData =
        dynamic_cast<InternalTreeItemData*>(WxTreeCtrl1->GetItemData(event.GetItem()));

    Zen::Studio::Workbench::I_ExplorerNode* const pDestNode =
        pDestInternalData->getNode();

    Zen::Studio::Workbench::I_ExplorerNodeType& destNodeType =
        pDestNode->getUserData()->getType();

    Zen::Studio::Workbench::Internal::II_ExplorerNode* const pSourceNode = 
        dynamic_cast<Zen::Studio::Workbench::Internal::II_ExplorerNode*>(m_pDraggingNode);

    InternalTreeItemData* const pSourceInternalData = 
        dynamic_cast<InternalTreeItemData*>(pSourceNode->getInternalData());

    // If it's draggable, allow the event.  The event is vetoed by default.
    if (destNodeType.canDrop(*m_pDraggingNode, *pDestNode))
    {
        // Prune the source node and graft it onto the destination node.
        if (pSourceInternalData->GetId().IsOk())
        {
            WxTreeCtrl1->SetItemData(pSourceInternalData->GetId(), 0);
            WxTreeCtrl1->Delete(pSourceInternalData->GetId());

            WxTreeCtrl1->AppendItem(pDestInternalData->GetId(), std2wx(pSourceNode->getDisplayName()), -1, -1, pSourceInternalData);

            setImage(pSourceNode->getUserData()->getType().getIconName(), pSourceInternalData->GetId());

            WxTreeCtrl1->Expand(pDestInternalData->GetId());

            // TODO get to the I_ProjectExplorerController and use it to control
            // the model / notify it of the prune / graft.
            
            throw Zen::Utility::runtime_exception("ProjectExplorerView::handleEndDrag(): TODO Finish");

        }
    }

    m_pDraggingNode = NULL;
}

void ProjectExplorerView::onAddProject(pProject_type _pProject)
{
    handleNewProject(_pProject->getNode());
}

void ProjectExplorerView::onDeleteProject(pProject_type _pProject)
{
    // TODO Implement
    throw Zen::Utility::runtime_exception("ProjectExplorerView::onDeleteProject(): Error, not implemented.");
}

void ProjectExplorerView::onAddNode(Zen::Studio::Workbench::I_ExplorerNode& _node)
{
    handleNewChildNode(&_node);
}

void ProjectExplorerView::onDeleteNode(Zen::Studio::Workbench::I_ExplorerNode& _node)
{
    throw Zen::Utility::runtime_exception("ProjectExplorerView::onDeleteNode(): Error, not implemented.");
}
