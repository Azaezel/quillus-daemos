//---------------------------------------------------------------------------
//
// Name:        WorkbenchLocationDlg.h
// Author:      Tony Richards
// Created:     1/29/2009 12:06:08 PM
// Description: WorkbenchLocationDlg class declaration
//
//---------------------------------------------------------------------------

#ifndef __WORKBENCHLOCATIONDLG_h__
#define __WORKBENCHLOCATIONDLG_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/dirdlg.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
////Header Include End

////Dialog Style Start
#undef WorkbenchLocationDlg_STYLE
#define WorkbenchLocationDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxCLOSE_BOX
////Dialog Style End

class WorkbenchLocationDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		WorkbenchLocationDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Select Workbench"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = WorkbenchLocationDlg_STYLE);
		virtual ~WorkbenchLocationDlg();
		void WxButton1Click(wxCommandEvent& event);

        void SetValue(wxString& _value);
        wxString GetValue() const;
		void WxButtonOkClick(wxCommandEvent& event);
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxDirDialog *WxDirDialog1;
		wxButton *WxButton2;
		wxButton *WxButtonOk;
		wxBoxSizer *WxBoxSizer3;
		wxButton *WxButton1;
		wxTextCtrl *WxEdit1;
		wxStaticText *WxStaticText1;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXBUTTON1 = 1013,
			ID_WXEDIT1 = 1012,
			ID_WXSTATICTEXT1 = 1011,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
