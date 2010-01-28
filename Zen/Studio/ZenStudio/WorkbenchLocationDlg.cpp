//---------------------------------------------------------------------------
//
// Name:        WorkbenchLocationDlg.cpp
// Author:      Tony Richards
// Created:     1/29/2009 12:06:08 PM
// Description: WorkbenchLocationDlg class implementation
//
//---------------------------------------------------------------------------

#include "WorkbenchLocationDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_WorkbenchLocationDlg_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// WorkbenchLocationDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(WorkbenchLocationDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(WorkbenchLocationDlg::OnClose)
	EVT_BUTTON(wxID_OK,WorkbenchLocationDlg::WxButtonOkClick)
	EVT_BUTTON(ID_WXBUTTON1,WorkbenchLocationDlg::WxButton1Click)
	EVT_TEXT_ENTER(ID_WXEDIT1,WorkbenchLocationDlg::WxButtonOkClick)
END_EVENT_TABLE()
////Event Table End

WorkbenchLocationDlg::WorkbenchLocationDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

WorkbenchLocationDlg::~WorkbenchLocationDlg()
{
} 

void WorkbenchLocationDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 0, wxALIGN_CENTER | wxALL, 5);

	WxStaticText1 = new wxStaticText(this, ID_WXSTATICTEXT1, wxT("Workspace Location"), wxPoint(5,9), wxDefaultSize, 0, wxT("WxStaticText1"));
	WxStaticText1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	WxBoxSizer2->Add(WxStaticText1,0,wxALIGN_CENTER | wxALL,5);

	WxEdit1 = new wxTextCtrl(this, ID_WXEDIT1, wxT(""), wxPoint(115,8), wxSize(121,19), 0, wxDefaultValidator, wxT("WxEdit1"));
	WxEdit1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	WxBoxSizer2->Add(WxEdit1,0,wxALIGN_CENTER | wxALL,5);

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("..."), wxPoint(246,5), wxSize(25,25), 0, wxDefaultValidator, wxT("WxButton1"));
	WxButton1->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	WxBoxSizer2->Add(WxButton1,0,wxALIGN_CENTER | wxALL,5);

	WxBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer3, 0, wxALIGN_RIGHT | wxALL, 5);

	WxButtonOk = new wxButton(this, wxID_OK, wxT("OK"), wxPoint(5,5), wxSize(75,25), 0, wxDefaultValidator, wxT("WxButtonOk"));
	WxButtonOk->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	WxBoxSizer3->Add(WxButtonOk,0,wxALIGN_CENTER | wxALL,5);
    WxButtonOk->SetDefault();

	WxButton2 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(90,5), wxSize(75,25), 0, wxDefaultValidator, wxT("WxButton2"));
	WxButton2->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false, wxT("Tahoma")));
	WxBoxSizer3->Add(WxButton2,0,wxALIGN_CENTER | wxALL,5);

	WxDirDialog1 =  new wxDirDialog(this, wxT("Choose a directory"), wxT(""));

	SetTitle(wxT("Select workspace location"));
	SetIcon(Self_WorkbenchLocationDlg_XPM);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
}

void WorkbenchLocationDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

void
WorkbenchLocationDlg::SetValue(wxString& _value) 
{
    return WxEdit1->SetValue(_value);
}

wxString
WorkbenchLocationDlg::GetValue() const
{
    return WxEdit1->GetValue();
}

/*
 * WxButton1Click
 */
void WorkbenchLocationDlg::WxButton1Click(wxCommandEvent& event)
{
    if (WxDirDialog1->ShowModal() == wxID_OK)
    {
        WxEdit1->SetValue(WxDirDialog1->GetPath());
    }
}

/*
 * WxButtonOkClick
 */
void WorkbenchLocationDlg::WxButtonOkClick(wxCommandEvent& event)
{
    EndDialog(wxID_OK);
}
