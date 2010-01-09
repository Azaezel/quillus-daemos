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
#include "TerrainLayerPanel.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Studio/Workbench/wxUtilities.hpp>

#include <wx/imaglist.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace WorldBuilder {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
BEGIN_EVENT_TABLE(TerrainLayerPanel,wxPanel)
END_EVENT_TABLE()
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainLayerPanel::TerrainLayerPanel(wxWindow* _pParent)
:   wxPanel(_pParent)
{
    createGUIControls();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
TerrainLayerPanel::~TerrainLayerPanel()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
TerrainLayerPanel::createGUIControls()
{
	wxFlexGridSizer* m_flexGridSizer;

	//Create(parent, wxID_ANY, wxDefaultPosition, wxSize(450,238), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	m_flexGridSizer = new wxFlexGridSizer(2, 3, 0, 0);
	m_staticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Heightmap File"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	m_flexGridSizer->Add(m_staticText1, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	m_fileNameTE = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(100,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	m_flexGridSizer->Add(m_fileNameTE, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	m_selectFileBTN = new wxButton(this, ID_BUTTON1, _("..."), wxDefaultPosition, wxSize(25,25), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	m_flexGridSizer->Add(m_selectFileBTN, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	m_staticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Bits per entry"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	m_flexGridSizer->Add(m_staticText2, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	m_bitsPerEntryTE = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	m_flexGridSizer->Add(m_bitsPerEntryTE, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(m_flexGridSizer);
	m_flexGridSizer->SetSizeHints(this);

	//GetSizer()->Layout();
	//GetSizer()->Fit(this);
	//Center();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace WorldBuilder
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

