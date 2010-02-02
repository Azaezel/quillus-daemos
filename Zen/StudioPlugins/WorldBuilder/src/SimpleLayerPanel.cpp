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
#include "SimpleLayerPanel.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <wx/imaglist.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BEGIN_EVENT_TABLE(SimpleLayerPanel,wxPanel)
END_EVENT_TABLE()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SimpleLayerPanel::SimpleLayerPanel(wxWindow* _pParent)
:   wxPanel(_pParent)
{
    createGUIControls();

    m_parentItemId = m_pTreeCtrl->AddRoot(wxT("Root Group"));
    setImage("./icons/folder.png", m_parentItemId);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
SimpleLayerPanel::~SimpleLayerPanel()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SimpleLayerPanel::createGUIControls()
{
    // Copied from ProjectExplorerView::CreateGUIControls().
    // See it for more references.

	m_pBoxSizer = new wxGridSizer(1, 1, 5, 5);
	SetSizer(m_pBoxSizer);
	SetAutoLayout(true);

#if 1
    // TODO 450 should be calculated based on the size of the window.
    int treeHeight = 350;
    int treeWidth = 500;
	m_pTreeCtrl = new wxTreeCtrl(this, ID_WXTREECTRL1, wxPoint(5,5), wxSize(treeWidth, treeHeight),
        wxTR_EDIT_LABELS | wxTR_HAS_BUTTONS | wxTR_SINGLE);

    m_pTreeCtrl->AssignImageList(new wxImageList(16, 16, false, 2));
	m_pTreeCtrl->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));

	m_pBoxSizer->Add(m_pTreeCtrl, 1, wxEXPAND | wxALL, 5);
#endif
    m_pTreeCtrl->FitInside();
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	Center();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
SimpleLayerPanel::setImage(const std::string& _imageName, const wxTreeItemId& _id)
{
    ImageListMap_type::iterator iter = m_imageList.find(_imageName);

    if (iter == m_imageList.end())
    {

        const int i = m_pTreeCtrl->GetImageList()->Add(wxIcon(std2wx(_imageName), wxBITMAP_TYPE_PNG));

        m_imageList[_imageName] = i;
        m_pTreeCtrl->SetItemImage(_id, i);
    }
    else
    {
        m_pTreeCtrl->SetItemImage(_id, iter->second);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
