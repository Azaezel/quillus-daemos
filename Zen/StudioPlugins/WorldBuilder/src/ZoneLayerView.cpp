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
#include "ZoneLayerView.hpp"
#include "SimpleLayerPanel.hpp"
#include "TerrainLayerPanel.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BEGIN_EVENT_TABLE(ZoneLayerView,wxPanel)
    EVT_COLLAPSIBLEPANE_CHANGED(wxID_ANY, ZoneLayerView::onPaneChanged)
END_EVENT_TABLE()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZoneLayerView::ZoneLayerView(wxWindow* _pParent)
:   wxScrolledWindow(_pParent)
,   m_pActiveZone(NULL)
{
    createGUIControls();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ZoneLayerView::~ZoneLayerView()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneLayerView::createGUIControls()
{
    // Copied from ProjectExplorerView::CreateGUIControls().
    // See it for more references.

	m_pSizer = new wxFlexGridSizer(1, 1, 5, 5);
	SetSizer(m_pSizer);
    m_pSizer->SetFlexibleDirection(wxBOTH);
	SetAutoLayout(true);

    // TODO Check out using wxCollapsiblePane instead of wxFoldPanelBar
    // since wxFoldPanelBar does not support removing items.

    wxSize paneSize(400, 400);

    wxCollapsiblePane* const pDefaultPane = new wxCollapsiblePane(this, wxID_ANY, wxT("Default Layer"),
        wxDefaultPosition, paneSize, wxCP_NO_TLW_RESIZE|wxCP_DEFAULT_STYLE);
    m_pSizer->Add(pDefaultPane, 0, wxGROW | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    pDefaultPane->SetSizer(new wxBoxSizer(wxVERTICAL));
    pDefaultPane->GetSizer()->Add(new SimpleLayerPanel(pDefaultPane->GetPane()), 1, wxGROW|wxALL, 2);
    pDefaultPane->GetSizer()->SetSizeHints(pDefaultPane->GetPane());

    wxCollapsiblePane* const pSkyLayer = new wxCollapsiblePane(this, wxID_ANY, wxT("Sky Layer"),
        wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE|wxCP_DEFAULT_STYLE);
    m_pSizer->Add(pSkyLayer, 0, wxGROW | wxALIGN_CENTER | wxALL, 5);
    pSkyLayer->SetSizer(new wxBoxSizer(wxVERTICAL));
    pSkyLayer->GetSizer()->Add(new SimpleLayerPanel(pSkyLayer->GetPane()));
    pSkyLayer->GetSizer()->SetSizeHints(pSkyLayer->GetPane());

    wxCollapsiblePane* const pTerrainLayer = new wxCollapsiblePane(this, wxID_ANY, wxT("Terrain Layer"),
        wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE|wxCP_DEFAULT_STYLE);
    m_pSizer->Add(pTerrainLayer, 0, wxGROW | wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    pTerrainLayer->SetSizer(new wxBoxSizer(wxVERTICAL));
    pTerrainLayer->GetSizer()->Add(new TerrainLayerPanel(pTerrainLayer->GetPane()),
        1, wxEXPAND | wxALL);
    pTerrainLayer->GetSizer()->SetSizeHints(pTerrainLayer->GetPane());

    GetSizer()->AddStretchSpacer();

    FitInside();
    SetScrollbars(20, 20, 50, 50);
    GetSizer()->Layout();

	Center();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneLayerView::setActiveZone(I_Zone* _pZone)
{
    // TODO Insteaed of storing this zone (or in addition to)
    // get the zone's document and subscribe to it.
    m_pActiveZone = _pZone;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ZoneLayerView::onPaneChanged(wxCollapsiblePaneEvent& _event)
{
    FitInside();
    GetSizer()->Layout();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
